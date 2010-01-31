/*
 * python.cpp
 *
 *  Created on: 2010-01-27
 */

#include <string>

#include <boost/python.hpp>
#include <vgp/vgp.hpp>
#include <vgp/testnodes/testnodes.hpp>

int myinit() {
	return 6;
}

std::string greet() {
	return "hello from libvgp";
}

BOOST_PYTHON_MODULE(vgp)
{
	using namespace boost::python;

	// initialization code here


	// random stuff
	def("myinit", myinit, "this is the myinit docstring");
	def("greet", greet, "greeting");

	// Register nodes
	{
		load_testnodes();
	}

	// Grab python declarations from elsewhere
	{
		using namespace vgp;
		{
			using namespace vgp::detail;
			pyexport_nodebase();
			pyexport_nodeentry();
			pyexport_nodestorage();
		}
		{
			using namespace util;
			pyexport_typeinfo();
		}
	}
}
