/*
 * register.cpp
 *
 *  Created on: 2010-10-10
 */

/**********************************************************************

This file demonstrates how custom usr code can be added to the
framework. Nodes from both the vgp library and the user's own code can
be registered here. vgp will call vgp::usr::register_usrcode() once
when the module is initialized.

**********************************************************************/

#include <cvgp/usrcode.hpp>
#include <cvgp/library.hpp>
#include <cvgp/register.hpp>

namespace mynamespace {

double custom_usrcode() {return 3.141592685;}

} // namespace mynamespace

namespace vgp {
namespace usr {

size_t register_nodes() {
	int count = 0;
	// register vgp pre-defined nodes
	count += vgp::library::int_arithmetic();
	count += vgp::library::double_arithmetic();
	count += vgp::library::helloworld();
	count += vgp::library::test_nodes();

	// register user's own nodes
	using namespace vgp::register_;
	terminal(mynamespace::custom_usrcode, "custom_usrcode");

	return count + 1; // number of nodes registered
}

} // namespace usr
} // namespace vgp
