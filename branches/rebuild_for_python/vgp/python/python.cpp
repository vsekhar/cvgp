/*
 * python.cpp
 *
 *  Created on: 2010-01-27
 */

// has to be first b/c it includes Python.h
#include <vgp/python/gil_wrap.hpp>

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

void memtest(long i, long j) {
	static vgp::detail::NodeList nodes;
	using vgp::detail::nodesbyname;
	vgp::detail::NodeEntry n = *nodesbyname.find("f1");
	for(long count = 0; count < i; count++)
	//while(nodes.size() < 1000000)
		nodes.push_back(n.prototype->clone());
	for(long count = 0; count < j; count++) {
		nodes.pop_front();
		nodes.push_back(n.prototype->clone());
	}
}

BOOST_PYTHON_MODULE(vgp)
{
	using namespace boost::python;

	// initialization code here


	// random stuff
	def("myinit", myinit, "this is the myinit docstring");
	def("greet", vgp::python::GIL_wrapped(greet), "greeting");
	def("memtest", memtest, "memory test");

	// Register nodes
	{
		load_testnodes();
		//vgp::registration_done();
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
