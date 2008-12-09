/*
 * evolver.cpp
 *
 *  Created on: Dec 9, 2008
 *      Author: vsekhar
 */

#include "evolver.hpp"
#include "sourcedata.hpp"

namespace vgp {

Evolver::Evolver(po::variables_map pomap, FitnessFunctor f, util::TypeInfo t) :
	generation(0), fitnessfunctor(f), result_type(t)
	{
	vgp::addsourcedata("vgp_population_cref", new boost::reference_wrapper<Population>(pop));
	vgp::Nodes.setdepthpenalty(pomap["depth-penalty"].as<double>());

	// Setup checkpoints if requested
	if(pomap.count("checkpoint"))
		checkpointinterval = pomap["checkpoint"].as<unsigned int>();
	else
		checkpointinterval = 0;
	std::string checkpointfilename;
	if(checkpointinterval) {
		if(pomap.count("checkpoint-file"))
			checkpointfilename = pomap["checkpoint-file"].as<std::string>();
		else if(pomap.count("save-population"))
			checkpointfilename = pomap["save-population"].as<std::string>();
		else
			throw std::invalid_argument("No filename specified for checkpoint");
	}

	// Create or load initial population
	if(pomap.count("load-population"))
		loadpopulation(pop, pomap["load-population"].as<std::string>());
	else
		pop.add(pomap["population"].as<unsigned int>(), result_type);
	generations = pomap["generations"].as<unsigned int>();

	// Set number of crossovers
	pc = pomap["pc"].as<double>();
	crossovers = (pop.size() * pc) / 2;	// each crossover carries two organisms

	// Set number of reproductions
	pr = 1-pc;
	std::size_t reproductions_tmp = pop.size() * pr;
	// Adjust for whole numbers to preserve population size
	if(reproductions_tmp + (crossovers*2) < pop.size())
		reproductions_tmp += pop.size() - reproductions_tmp - (crossovers*2);
	reproductions = reproductions_tmp;	// make const

	// Check that reproduction & crossover will together preserve population size

	// Set number of mutations
	pm = pomap["pm"].as<double>();
	mutations = pm * pop.size();

	// Emit warnings if things aren't happening that should happen
	if(reproductions == 0 && pc != 1)
		std::cerr << "Warning: population too small for reproduction to take place: pop.size() == "
			<< pop.size() << ", pr == (1-pc) == " << pr << std::endl;
	if(crossovers == 0 && pc != 0)
		std::cerr << "Warning: population too small for crossover to take place: pop.size() == "
			<< pop.size() << ", pc == " << pc << std::endl;
	if(reproductions + (crossovers*2) < pop.size())
		std::cerr << "Warning: population size cannot be maintained: "
			<< "pop.size() == " << pop.size()
			<< ", reproductions == " << reproductions
			<< ", crossovers == " << crossovers
			<< std::endl;
}

void Evolver::advance() {
	Population newpop;
	vgp::evolve(pop, newpop, reproductions, crossovers, mutations);
	vgp::updatefitness(newpop, fitnessfunctor);
	generation++;

	// checkpoint
	if(checkpointinterval && !(generation % checkpointinterval)) {
		savepopulation(newpop, checkpointfilename);
		std::cout << "Checkpoint: Generation " << generation << ": " << newpop.avgfitness() << ", " <<
			newpop.avgnodecount() << std::endl;
	}

	pop.swap(newpop);
}

} // namespace vgp
