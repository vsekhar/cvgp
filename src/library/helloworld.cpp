/*
 * helloworld.cpp
 *
 *  Created on: 2010-10-08
 */

#include <string>

#include <cvgp/library.hpp>
#include <cvgp/register.hpp>

namespace vgp {
namespace library {

///////////////
// Hello World
///////////////

std::string hw() {return "Hello World\n";}

void helloworld() {
	using namespace vgp::register_;
	terminal(VGP_NAME(hw));
}

} // namespace library
} // namespace vgp
