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

#include <iostream>
#include <string>

#include <boost/python/dict.hpp>
#include <boost/python/extract.hpp>

#include <cvgp/usrcode.hpp>
#include <cvgp/python/helpers.hpp>

namespace vgp {
namespace usr {

bool initialize(boost::python::dict kwargs) {
	// access and use kwargs
	using vgp::python::pyget;
	std::cout << pyget<std::string>("hello", kwargs) << std::endl;
	return true;
}

} // namespace usr
} // namespace vgp

