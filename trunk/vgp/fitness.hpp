#ifndef VGP_FITNESS_HPP_
#define VGP_FITNESS_HPP_

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

#endif /*VGP_FITNESS_HPP_*/
