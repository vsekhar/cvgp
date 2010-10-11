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

size_t helloworld() {
	using namespace vgp::register_;
	terminal(VGP_NAME(hw));
	return 1;
}

} // namespace library
} // namespace vgp
