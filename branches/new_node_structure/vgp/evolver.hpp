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
	/** Create an evolver
	 * @param pomap parameter map from program_options containing parsed cmdline
	 * @param f fitness functor
	 * @param t TypeInfo of evolution's result_type
	 */
	Evolver(po::variables_map pomap, FitnessFunctor f, util::TypeInfo t);

	std::size_t updatefitness() {
		return vgp::updatefitness(pop, fitnessfunctor);
	}

	/// Advance evolution by one generation
	void advance() {advance(1);}

	/// Advance evolution by 'n' generations
	void advance(std::size_t);

	/** Do we need to checkpoint?
	 */
	bool needcheckpoint() const {
		return checkpointinterval && ((generation-lastcheckpoint)>=checkpointinterval);
	}

	bool needtosave() const {return save;}

	/** Perform checkpoint
	 */
	void checkpoint() const;

	/** Output some stats
	 * @pre population must be sorted for the best/worst stats to be correct
	 */
	std::string stats() const;
	std::string best() const;
	std::string worst() const;

	void loadpopulation(std::string);
	void savepopulation(std::string) const;

	/// The population
	Population pop;

private:
	unsigned int checkpointinterval;
	mutable unsigned int lastcheckpoint;
	std::string checkpointfilename;
	std::string savefilename;
	unsigned int generation;
	double pc, pr, pm;
	std::size_t crossovers, reproductions, mutations;
	FitnessFunctor fitnessfunctor;
	util::TypeInfo result_type;
	bool save;

}; // class Evolver

} // namespace vgp


#endif /* VGP_EVOLVER_HPP_ */
