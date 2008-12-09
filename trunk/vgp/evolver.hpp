/*
 * evolver.hpp
 *
 *  Created on: Dec 8, 2008
 *      Author: vsekhar
 */

#ifndef VGP_EVOLVER_HPP_
#define VGP_EVOLVER_HPP_

#include <sstream>
#include <iostream>
#include <queue>

#include <vgp/population.hpp>
#include <vgp/evolutionops.hpp>
#include <vgp/program_options.hpp>
#include <vgp/exception.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace po = ::boost::program_options;

class Evolver {
public:
	Evolver(po::variables_map pomap, FitnessFunctor f, util::TypeInfo);
	std::size_t updatefitness(FitnessFunctor fitness) {
		return vgp::updatefitness(pop, fitness);
	}

	void advance();
	bool done() {return generation >= generations;}

	Population pop;

private:
	unsigned int checkpointinterval;
	std::string checkpointfilename;
	unsigned int generation;
	unsigned int generations;
	double pc, pr, pm;
	std::size_t crossovers, reproductions, mutations;
	FitnessFunctor fitnessfunctor;
	util::TypeInfo result_type;

}; // class Evolver

} // namespace vgp


#endif /* VGP_EVOLVER_HPP_ */
