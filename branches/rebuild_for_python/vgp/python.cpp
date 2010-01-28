/*
 * python.cpp
 *
 *  Created on: 2010-01-27
 */

#include <string>

#include <boost/python.hpp>
#include <vgp/make_node.hpp>
#include <vgp/register_node.hpp>

using namespace boost::python;

int myinit() {
	return 6;
}

std::string greet() {
	return "hello from libvgp";
}

BOOST_PYTHON_MODULE(vgp)
{
	def("myinit", myinit, "this is the myinit docstring");
	def("greet", greet, "greeting");
}
