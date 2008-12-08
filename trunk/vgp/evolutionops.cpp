/*
 * evolutionops.cpp
 *
 *  Created on: Sep 26, 2008
 *      Author: vsekhar
 */

#include <utility>
#include <exception>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <vgp/evolutionops.hpp>
#include <vgp/util/random.hpp>

namespace vgp {

using std::size_t;
using std::pair;
using boost::tuple;
using boost::make_tuple;
using boost::tie;

namespace detail {

typedef pair<double, const Organism*> fitnessentry_t;
typedef std::vector<fitnessentry_t> fitnessvector_t;

/// Perform reproduction on the population
size_t reproduce(
		const Population& pop,
		Population& newpop,
		const size_t reproductions,
		const fitnessvector_t& fitnessvector,
		const double fitnesssum)
{
	const double minfitness = pop.front().getfitness();
	boost::uniform_real<double> rand_gen(minfitness, fitnesssum);
	size_t count = 0;
	for(size_t reproductioncount = 0; reproductioncount < reproductions; reproductioncount++) {
		const double fitnessdelta = rand_gen(util::default_generator);
		size_t j;
		double accumulator = 0;
		for(j = 0; j < fitnessvector.size(); j++) {
			accumulator += fitnessvector[j].first;
			if(accumulator >= fitnessdelta) break;
		}
		newpop.push_back(new Organism(*fitnessvector[j].second));
		count++;
	}
	return count;
}

/// Perform crossover on the population
size_t crossover(
		const Population &pop,
		Population &newpop,
		const size_t crossovers,
		const fitnessvector_t& fitnessvector,
		const double fitnesssum)
{
	const double minfitness = pop.front().getfitness();
	boost::uniform_real<double> rand_gen(minfitness, fitnesssum);
	size_t count = 0;
	for(size_t crossovercount = 0; crossovercount < crossovers; crossovercount++) {
		const double fitnessdelta1 = rand_gen(util::default_generator);
		const double fitnessdelta2 = rand_gen(util::default_generator);

		// choose the two orgs to crossover
		size_t index1 = 0;
		size_t index2 = 0;
		bool org1found = false;
		bool org2found = false;
		double accumulator = minfitness;
		for(size_t j = 0; j < fitnessvector.size(); j++) {
			accumulator += fitnessvector[j].first;
			if(!org1found && accumulator >= fitnessdelta1) {
				index1 = j;
				org1found = true;
			}
			if(!org2found && accumulator >= fitnessdelta2) {
				index2 = j;
				org2found = true;
			}
			if(org1found && org2found) break;
		}

		// copy orgs, perform x-over and insert
		Organism *org1 = new Organism(*fitnessvector[index1].second);
		Organism *org2 = new Organism(*fitnessvector[index2].second);
		org1->crossover(*org2);
		newpop.push_back(org1);
		newpop.push_back(org2);
		count++;
	}
	return count;
}

/// Perform mutation on the population (not implemented)
size_t mutate(Population& pop, const size_t mutations) {
	if(mutations) throw std::runtime_error("Mutation not implemented");
	return 0;
}

} // namespace detail

using std::cerr;
using std::endl;

using detail::reproduce;
using detail::crossover;
using detail::mutate;

std::size_t updatefitness(Population &pop, FitnessFunctor f) {
	std::size_t newfitness = 0;
	BOOST_FOREACH(Organism &o, pop) {
		try {
			if(!o.validfitness()) {
				o.setfitness(f(o));
				++newfitness;
			}
		}
		catch(boost::bad_any_cast &e){
			cerr << "Org failed (bad_any_cast): " << o << endl;
			throw;
		}
		catch(...) {
			cerr << "Org failed (unknown exception): " << o << endl;
			throw;
		}
	}
	return newfitness;
}

void evolve(
		const Population& pop,
		Population &newpop,
		const size_t reproductions,
		const size_t crossovers,
		const size_t mutations)
{
	// Make fitness vector for fitness-proportional evolution ops
	detail::fitnessvector_t fitnessvector;
	double fitnesssum = 0;
	fitnessvector.reserve(pop.size());
	for(Population::const_iterator i = pop.begin(); i!=pop.end(); i++) {
		fitnesssum += i->getfitness();
		fitnessvector.push_back(std::make_pair(i->getfitness(), &(*i)));
	}

	// Perform evolution ops
	reproduce(pop, newpop, reproductions, fitnessvector, fitnesssum);
	crossover(pop, newpop, crossovers, fitnessvector, fitnesssum);
	mutate(newpop, mutations);
}

} // namespace vgp
