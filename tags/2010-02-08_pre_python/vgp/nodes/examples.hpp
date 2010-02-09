/*
 * examples.hpp
 *
 *  Created on: Sep 29, 2008
 *      Author: vsekhar
 */

#ifndef EXAMPLES_HPP_
#define EXAMPLES_HPP_

#include <iostream>
#include <limits>

#include <vgp/defnode.hpp>
#include <vgp/detail/adapter.hpp>
#include <vgp/sourcedata.hpp>
#include <boost/function.hpp>

namespace vgp {
namespace example {

int myfunc(int i);
boost::function<int(int)> make_function();
int take_function(boost::function<int(int)> func);
int test_int();

template <class Loader>
void examplenodes(Loader loader) {
	// NB: this adapter should be implemented as a rounder
	loader.template makeadapter<int, double>();

	// NB: the above adapter is dangerous (range of double > range of int)
	// causes hardware integer overflow errors
	// Need to implement this with range checking
	//loader.makeadapter<double, int>();

	loader.maketerminal(make_function, "vgp_make_function");
	loader.makenode(take_function, "vgp_take_function");
	loader.maketerminal(test_int, "vgp_test_int");
}

} // namespace example
} // namespace vgp

#endif /* EXAMPLES_HPP_ */
