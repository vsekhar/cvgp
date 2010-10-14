/*
 * initialize.cpp
 *
 *  Created on: 2010-10-10
 *
 * Code here is responsible for loading any usr data and setting it up for user
 * code.
 *
 * It is advisable that python handle any loading of files and parsing of
 * data (it's just easier and less error-prone), and then python presents the
 * parsed data (as lists, vectors, dicts, whatever) to the usr::initialize()
 * for use in the C++ code.
 *
 */

#include <string>

#include <cvgp/usrcode.hpp>

namespace vgp {
namespace usr {

bool initialize(kwargs_t kwargs) {
	// access and use kwargs
	return getarg<std::string>("initparam", kwargs) == "Ok";
}

} // namespace usr
} // namespace vgp

