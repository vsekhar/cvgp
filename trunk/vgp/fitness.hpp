#ifndef FITNESS_HPP_
#define FITNESS_HPP_

#include <boost/function.hpp>
#include <vgp/population.hpp>

namespace vgp {
template <class RESULT>
struct FitnessFunctor {
	typedef boost::function<double(
			vgp::Population::const_iterator org,
			result_container_type::const_iterator result,
			const vgp::Population &pop,
			const result_container_type &results) >	fitnessfunctor_t;	
};
}

#endif /*FITNESS_HPP_*/
