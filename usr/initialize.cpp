/*
 * initialize.cpp
 *
 *  Created on: 2010-10-10
 */
 
 #include <iostream>

#include <boost/python/dict.hpp>
#include <boost/python/extract.hpp>

#include <cvgp/usrcode.hpp>
#include <cvgp/util/pyhelp.hpp>

namespace vgp {
namespace usr {

bool initialize(boost::python::dict kwargs) {
	// access and use kwargs
	using vgp::util::pyget;
	std::cout << pyget<std::string>("testarg", kwargs) << std::endl;
	return true;
}

} // namespace usr
} // namespace vgp

