#ifndef VGP_CROSSOVER_HPP_
#define VGP_CROSSOVER_HPP_

#include <boost/function.hpp>
#include <vgp/organism.hpp>

namespace vgp {
namespace crossover {

struct CrossoverResult : boost::function<vgp::organism*(const vgp::organism&)> {};

} // end namespace crossover
} // end namespace vgp

#endif /*VGP_CROSSOVER_HPP_*/
