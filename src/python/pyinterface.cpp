/*
 * python.cpp
 *
 *  Created on: 2010-01-27
 */

// has to be first b/c it includes Python.h
#include <cvgp/gil_wrap.hpp>

#include <string>
#include <list>
#include <iostream>
#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <cvgp/vgp.hpp>
#include <cvgp/usrcode.hpp>
#include <cvgp/detail/run.hpp>
#include <cvgp/python/helpers.hpp>

namespace vgp {
namespace python {

std::string greet() {
	return "hello from libvgp";
}

// re-implement memtest() to use lists of organisms, etc.
void memtest(long i, long j) {
	using vgp::Organism;
	static std::list<Organism> orgs;
	for(long count = 0; count < i; count++)
		orgs.push_back(Organism(typeid(int)));
	for(long count = 0; count < j; count++) {
		orgs.pop_front();
		orgs.push_back(Organism(typeid(int)));
	}
}

//using vgp::detail::tree;
vgp::Organism make_int_org() {
	return vgp::Organism(typeid(int));
}

int run_as_int(const vgp::Organism& o) {
	return vgp::detail::run_as<int>(o);
}

bool init(boost::python::dict kwargs) {
	return vgp::usr::initialize(kwargs);
}

BOOST_PYTHON_MODULE(libvgp)
{
	using namespace boost::python;

	// initialization code here
	// (load data files?)
	size_t count = vgp::usr::register_nodes();
	std::cout << count << " nodes" << std::endl;

	// python access functions
	vgp::python::register_helpers();
	def("init", init, "initialize module and user code");
	def("greet", greet, "greeting");
	def("memtest", memtest, "memory test");
	def("memtest_mt", vgp::python::GIL_wrapped(memtest), "memory test (multi-threaded)");
	def("make_int_org", make_int_org);
	def("run_as_int", run_as_int);
	

	// Grab python declarations from elsewhere
	{
		using namespace vgp;
		{
			using namespace vgp::detail;
			pyexport_nodestorage();
			pyexport_generate();
		}
		{
			using namespace util;
			pyexport_typeinfo();
		}
		pyexport_organism();
	}

} // BOOST_PYTHON_MODULE(libvgp)

} // namespace python
} // namespace vgp

