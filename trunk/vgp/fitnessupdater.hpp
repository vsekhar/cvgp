#ifndef VGP_FITNESSUPDATER_HPP_
#define VGP_FITNESSUPDATER_HPP_

#include <boost/function.hpp>
#include <boost/foreach.hpp>
#include <boost/assert.hpp>

#include <vgp/organism.hpp>
#include <vgp/population.hpp>

#ifndef VGP_FITNESS_CONVERGE_LIMIT
#define VGP_FITNESS_CONVERGE_LIMIT 100
#endif

namespace vgp {

template <class RESULT_TYPE>
struct ResultContainer : std::list<RESULT_TYPE> {};

template <class RESULT_TYPE>
struct FitnessUpdater {
	typedef RESULT_TYPE result_type;
	typedef ResultContainer<RESULT_TYPE> result_container_type;
	typedef double fitness_type;
	typedef std::list<fitness_type> fitness_container_type;
	typedef boost::function<double(
			vgp::Population::const_iterator org,
			result_container_type::const_iterator result,
			const vgp::Population &pop,
			const result_container_type &results) >	fitnessfunctor_t;
	static std::list<double> computefitnesses(const Population &pop, fitnessfunctor_t fitnessfunc) {
		result_container_type results;
		BOOST_FOREACH(const Organism &o, pop) {
			results.push_back(o.run<result_type>());
		}
		BOOST_ASSERT(results.size() == pop.size());
		Population::const_iterator pop_itr = pop.begin();
		result_container_type::const_iterator res_itr = results.begin();
		std::list<double> ret;
		for( ; pop_itr != pop.end(); pop_itr++, res_itr++)
			ret.push_back(fitnessfunc(pop_itr, res_itr, pop, results));
	}

	static std::list<double> computefitnesses_converge(Population &pop, fitnessfunctor_t fitnessfunc, double convergencethreshold) {
		fitness_container_type fitnessvalues1;
		fitness_container_type fitnessvalues2;
		unsigned int i = 0;
		for( ; i < VGP_FITNESS_CONVERGE_LIMIT; i++) {
			if(i%2)
				fitnessvalues1 = computefitness(pop, fitnessfunc);
			else
				fitnessvalues2 = computefitness(pop, fitnessfunc);
			BOOST_ASSERT(fitnessvalues1.size() == fitnessvalues2.size());
			fitness_container_type::const_iterator itr1 = fitnessvalues1.begin();
			fitness_container_type::const_iterator itr2 = fitnessvalues2.begin();
			bool runagain = false;
			for( ; itr1 != fitnessvalues1.end(); itr1++, itr2++) {
				double pcnt1 = (*itr1 - *itr2)/ *itr1;
				double pcnt2 = (*itr1 - *itr2)/ *itr2;
				double pcnt = (pcnt1 + pcnt2) / 2;
				if(pcnt > convergencethreshold) {
					runagain = true;
					break;
				}
			}
			if(!runagain) break;
		}
		if(i%2) return fitnessvalues1;
		else return fitnessvalues2;
	}

	static void loadfitnessvalues(const fitness_container_type &fitnessvalues, Population &pop) {
		BOOST_ASSERT(fitnessvalues.size() == pop.size());
		fitness_container_type::const_iterator fitnessitr = fitnessvalues.begin();
		Population::interator popitr = pop.begin();
		for( ; popitr != pop.end(); popitr++, fitnessitr++)
			popitr->setfitness(*fitnessitr);
		pop.sort();
	}

	static void setfitness(Population &pop, fitnessfunctor_t fitnessfunc) {
		loadfitnessvalues(computefitness(pop, fitnessfunc), pop);
	}
	static void setfitness_converge(Population &pop, fitnessfunctor_t, fitnessfunc, double convergencethreshold) {
		loadfitnessvalues(computefitness_converge(pop, fitnessfunc, convergencethreshold));
	}

	double divergence
};

} // end namespace vgp

#endif /*VGP_FITNESSUPDATER_HPP_*/
