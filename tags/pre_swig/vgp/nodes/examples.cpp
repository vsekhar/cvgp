/*
 * examples.cpp
 *
 *  Created on: Sep 29, 2008
 *      Author: vsekhar
 */

#include <vgp/nodes/examples.hpp>

namespace vgp {
namespace example {

int myfunc(int i) {return i;}
boost::function<int(int)> make_function() {return myfunc;}
int take_function(boost::function<int(int)> func) { return func(5); }
int test_int() {
	return *vgp::getsourcedata<int>("test_int");
}

} // namespace example
} // namespace vgp
