/*
 * constants.hpp
 *
 *  Created on: Sep 29, 2008
 *      Author: vsekhar
 */

#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <vgp/defnode.hpp>
#include <vgp/util/random.hpp>

#include <string>
#include <list>
#include <boost/ref.hpp>

namespace vgp {
namespace nodes {

namespace util = ::vgp::util;

template <class T> T passthrough(T t) { return t; }
template <class T> T zero() { return 0; }
template <class T> T one() { return 1; }
template <class T> T five() { return 5; }
double five_point_four() {return 5.4;}

std::string helloworld() {return "Hello World";}

int smallint(int& state) {
	return state;
}
void smallint_mutate(int& state) {
	boost::uniform_int<int> dist(0, 10);
	state = dist(vgp::util::default_generator);
}

double double_01(double& state) {return state;}
void double_01_mutate(double& state) {
	boost::uniform_01<vgp::util::default_generator_t> dist(boost::ref(vgp::util::default_generator));
	state = dist();
}

template <class Loader>
void constantnodes(Loader &loader) {
	loader.makenode(passthrough<int>, "vgp_passthrough");
	loader.makenode(passthrough<double>, "vgp_passthrough");

	loader.maketerminal(zero<int>, "vgp_zero");
	loader.maketerminal(zero<double>, "vgp_zero");
	loader.maketerminal(one<int>, "vgp_one");
	loader.maketerminal(one<double>, "vgp_one");
	loader.maketerminal(five<int>, "vgp_five");
	loader.maketerminal(five<double>, "vgp_five");
	loader.maketerminal(five_point_four, "vgp_five_point_four");

	loader.maketerminal(helloworld, "vgp_helloworld");

	loader.maketerminal_m(smallint, smallint_mutate, "vgp_smallint");
	loader.maketerminal_m(double_01, double_01_mutate, "vgp_double_01");
}

} // namespace nodes
} // namespace vgp

#endif /* CONSTANTS_HPP_ */
