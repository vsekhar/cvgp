/*
 * evolver.cpp
 *
 *  Created on: Dec 9, 2008
 *      Author: vsekhar
 */

#include <sstream>
#include <fstream>
#include <vgp/detail/text_archives.hpp>

#include "evolver.hpp"
#include "sourcedata.hpp"

namespace vgp {

using std::size_t;

Evolver::Evolver(po::variables_map pomap, FitnessFunctor f, util::TypeInfo t) :
	checkpointinterval(0), lastcheckpoint(0), generation(0),
	pc(0), pr(0), pm(0), crossovers(0), reproductions(0), mutations(0),
	fitnessfunctor(f), result_type(t), save(false)
	{
	vgp::addsourcedata("vgp_population_cref", new boost::reference_wrapper<Population>(pop));
	vgp::Nodes.setdepthpenalty(pomap["depth-penalty"].as<double>());

	// Setup checkpoints if requested
	if(pomap.count("checkpoint"))
		checkpointinterval = pomap["checkpoint"].as<unsigned int>();
	else
		checkpointinterval = 0;

	if(checkpointinterval) {
		if(pomap.count("checkpoint-file"))
			checkpointfilename = pomap["checkpoint-file"].as<std::string>();
		else if(pomap.count("save-population"))
			checkpointfilename = pomap["save-population"].as<std::string>();
		else
			throw std::invalid_argument("No filename specified for checkpoint");
	}

	if(pomap.count("save-population")) {
		savefilename = pomap["save-population"].as<std::string>();
		save = true;
	}

	// Create or load initial population
	if(pomap.count("load-population"))
		loadpopulation(pomap["load-population"].as<std::string>());
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

void Evolver::advance(size_t n) {
	for(size_t i = 0; i < n; i++) {
		Population newpop;
		vgp::evolve(pop, newpop, reproductions, crossovers, mutations);
		vgp::updatefitness(newpop, fitnessfunctor);
		generation++;
		pop.swap(newpop);
	}
}

void Evolver::checkpoint() const {
	savepopulation(checkpointfilename);
	std::cerr << "done saving" << std::endl;
	lastcheckpoint = generation;
}

std::string Evolver::stats() const {
	std::stringstream ret;
	ret << "Generation = " << generation << " of " << generations
		<< ", " << pop.size() << " organisms with " << pop.avgnodecount()
		<< " nodes each = " << pop.nodecount() << " total nodes" << std::endl;

	ret << "(MIN fitness = " << pop.front().getfitness()
		<< ", MAX fitness = " << pop.back().getfitness()
		<< ", nodecount = " << pop.nodecount() << ")";
	return ret.str();
}

std::string Evolver::best() const {
	const Organism &best = *pop.rbegin();
	std::stringstream ret;
	ret << best.getfitness() << " = " << best << std::endl;
	return ret.str();
}

std::string Evolver::worst() const {
	const Organism &worst = *pop.begin();
	std::stringstream ret;
	ret << worst.getfitness() << " = " << worst << std::endl;
	return ret.str();
}

void Evolver::loadpopulation(std::string filename) {
	std::ifstream inputfile(filename.c_str());
	vgp::text_archive_types::iarchive_type ia(inputfile);
	ia >> pop;
}

void Evolver::savepopulation(std::string filename) const {
	std::ofstream outputfile(filename.c_str(), std::ios_base::trunc);
	BOOST_ASSERT(outputfile.good());
	vgp::text_archive_types::oarchive_type oa(outputfile);
	oa << pop;
}

} // namespace vgp
