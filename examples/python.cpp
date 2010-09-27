/*
 * python.cpp
 *
 *  Created on: 2010-01-27
 */

// has to be first b/c it includes Python.h
#include <cvgp/gil_wrap.hpp>

#include <string>
#include <list>
#include <boost/python.hpp>
#include <cvgp/vgp.hpp>
#include <cvgp/detail/run.hpp>
#include <cvgp/examples/testnodes.hpp>

int myinit() {
	return 6;
}

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

void make_adf(vgp::Organism& o) {o.make_adf();}

int run_as_int(const vgp::Organism& o) {
	return vgp::detail::run_as<int>(o);
}

BOOST_PYTHON_MODULE(vgp)
{
	using namespace boost::python;

	// initialization code here


	// random stuff
	def("myinit", myinit, "this is the myinit docstring");
	def("greet", vgp::python::GIL_wrapped(greet), "greeting");
	def("memtest", memtest, "memory test");
	def("make_int_org", make_int_org);
	def("run_as_int", run_as_int);
	def("make_adf", make_adf);

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
			pyexport_nodestorage();
			pyexport_generate();
		}
		{
			using namespace util;
			pyexport_typeinfo();
		}
		pyexport_organism();
	}
}
