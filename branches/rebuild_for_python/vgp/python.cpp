/*
 * python.cpp
 *
 *  Created on: 2010-01-27
 */

#include <string>

#include <boost/python.hpp>
#include <vgp/make_node.hpp>
#include <vgp/register_node.hpp>

#include <vgp/detail/nodecontainer.hpp>
#include <vgp/detail/nodestorage.hpp>

#include <vgp/util/typeinfo.hpp>

using namespace boost::python;

int myinit() {
	return 6;
}

std::string greet() {
	return "hello from libvgp";
}

BOOST_PYTHON_MODULE(vgp)
{
	// initialization code here

	// random stuff
	def("myinit", myinit, "this is the myinit docstring");
	def("greet", greet, "greeting");

	// Grab python declarations from elsewhere
	{
		using namespace vgp;
		{
			using namespace vgp::detail;
			pyexport_nodebase();
			pyexport_nodecontainer();
			pyexport_nodestorage();
		}
		{
			using namespace util;
			//pyexport_typeinfo();
		}
	}
}
