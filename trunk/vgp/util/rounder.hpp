#ifndef ROUNDER_HPP_
#define ROUNDER_HPP_

#include <boost/numeric/conversion/converter.hpp>

namespace vgp {
namespace util {

/** \brief A simple double -> int rounder
 * Rounds doubles to ints
 * \par
 * Behaves like a function object that accepts doubles and returns the nearest int
 */
typedef ::boost::numeric::converter<
	int,
	double,
	::boost::numeric::conversion_traits<int, double>,
	::boost::numeric::def_overflow_handler,
	::boost::numeric::RoundEven<double>
> Rounder;

} // namespace util
} // namespace vgp

#endif /*ROUNDER_HPP_*/
