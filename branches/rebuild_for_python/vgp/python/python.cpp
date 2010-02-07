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
#include <vgp/examples/testnodes.hpp>

int myinit() {
	return 6;
}

std::string greet() {
	return "hello from libvgp";
}

// re-implement memtest() to use lists of organisms, etc.
void memtest(long i, long j) {
#if 0
	static vgp::detail::NodeList nodes;
	using vgp::detail::nodesbyname;
	vgp::detail::NodeEntry n = *nodesbyname.find("f1");
	for(long count = 0; count < i; count++)
		nodes.push_back(n.prototype->clone());
	for(long count = 0; count < j; count++) {
		nodes.pop_front();
		nodes.push_back(n.prototype->clone());
	}
#endif
}

//using vgp::detail::tree;
vgp::detail::tree make_int_org() {
	using vgp::Organism;
	using vgp::detail::Trees;
	using vgp::detail::tree;
	//using vgp::detail::generate;
	Trees t;
	Trees::const_iterator i = t.begin();
	return tree(vgp::detail::generate(typeid(int), t, i, 0));
}

int run_as_int(const vgp::detail::tree& t) {
	return vgp::detail::run_as<int>(t);
}

BOOST_PYTHON_MODULE(vgp)
{
	using namespace boost::python;

	// initialization code here


	// random stuff
	def("myinit", myinit, "this is the myinit docstring");
	def("greet", vgp::python::GIL_wrapped(greet), "greeting");
	def("memtest", memtest, "memory test");
	def("make_int_tree", make_int_tree);
	def("run_as_int", run_as_int);

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
			pyexport_tree();
		}
		{
			using namespace util;
			pyexport_typeinfo();
		}
	}
}
