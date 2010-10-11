/*
 * register.cpp
 *
 *  Created on: 2010-10-10
 *
 * This file demonstrates how custom usr code can be added to the
 * framework. Nodes from both the vgp library and the user's own code can
 * be registered here. vgp will call vgp::usr::register_usrcode() once
 * when the module is initialized.
 */

#include <cvgp/usrcode.hpp>
#include <cvgp/library.hpp>
#include <cvgp/register.hpp>

namespace mynamespace {

double custom_usrcode() {return 3.141592685;}
int overloaded(int i) {return i;}
double overloaded(double d) {return d;}

} // namespace mynamespace

namespace vgp {
namespace usr {

void register_nodes() {
	// register vgp pre-defined nodes
	vgp::library::int_arithmetic();
	vgp::library::double_arithmetic();
	vgp::library::helloworld();
	vgp::library::test_nodes();

	// register user's own nodes
	using namespace vgp::register_;
	terminal(mynamespace::custom_usrcode, "custom_usrcode");
	
	// register overloaded functions (ok as long as signatures are unique)
	int (*n1)(int) = mynamespace::overloaded;
	double (*n2)(double) = mynamespace::overloaded;
	node(n1, "overloaded");
	node(n2, "overloaded");
}

} // namespace usr
} // namespace vgp
