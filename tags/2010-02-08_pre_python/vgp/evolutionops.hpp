/*
 * evolutionops.hpp
 *
 *  Created on: Sep 26, 2008
 *      Author: vsekhar
 */

#ifndef EVOLUTIONOPS_HPP_
#define EVOLUTIONOPS_HPP_

#include <boost/function.hpp>

#include <vgp/population.hpp>

namespace vgp {

typedef boost::function<double(const Organism&)> FitnessFunctor;

/// Updates the fitness values stored in each organism in a population
std::size_t updatefitness(Population &pop, FitnessFunctor f);

/** Performs one generation of evolution (reproduction, crossover, and mutation)
 *
 * @pre each organism in pop must have a valid fitness value, and pop must be sorted
 * against those values (smallest fitness first)
 *
 * @post newpop contains the population for the next generation
 */
void evolve(
		const Population& pop,
		Population &newpop,
		const std::size_t reproductions,
		const std::size_t crossovers,
		const std::size_t mutations);

} // namespace vgp


#endif /* EVOLUTIONOPS_HPP_ */
