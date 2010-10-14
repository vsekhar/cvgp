/*
 * python.cpp
 *
 *  Created on: 2010-01-27
 */

// has to be first b/c it includes Python.h
#include <cvgp/python/gil_wrap.hpp>

#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <iostream>
#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <cvgp/vgp.hpp>
#include <cvgp/usrcode.hpp>
#include <cvgp/detail/run.hpp>
#include <cvgp/python/helpers.hpp>

namespace vgp {
namespace python {

std::string greet() {
	return "hello from libvgp";
}

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

boost::python::list send(int count) {
	boost::python::list ret;
	static int counter = 1;
	for(int i=0; i < count; i++) {
		std::stringstream ss;
		ss << "Msg " << counter;
		ret.append(ss.str());
		counter++;
	}
	return ret;
}

void receive(boost::python::list) {}
void advance(int) {}

BOOST_PYTHON_MODULE(libvgp)
{
	using namespace boost::python;

	/*
	 * Module setup
	 * register nodes on import, since python may ask for node information
	 * before any other code
	*/
	vgp::usr::register_nodes();

	// Startup and diagnostics
	def("initialize", usr::initialize, "initialize module and user code");
	def("greet", greet, "greeting");
	def("memtest", memtest, "memory test");
	def("memtest_mt", GIL_wrapped(memtest), "memory test (multi-threaded)");

	// Query functions
	def("listnodes", detail::listnodes);
	
	// Message-passing functions
	def("send", send);
	def("receive", receive);
	
	// Evolution control functions
	def("advance", advance);
	
	// Testing functions
	def("make_int_org", make_org_returning<int>);
	def("run_as_int", detail::run_as<int>);
	def("pcurve", detail::probability_curve);

	// VGP data types
	class_<Organism>("Organism", no_init)
		.def(self_ns::str(self)) // gcc hiccups without the namespace here
		;

	// Utility, error and other data types
	class_<detail::GenerateError>("GenerateError", no_init);

	class_<util::TypeInfo>("TypeInfo", no_init)
		.def(self_ns::str(self)) // gcc hiccups without the namespace here
		.def(self == self)
		.def(self < self)
		;

	class_<util::TypeInfoVector>("TypeInfoVector")
			.def(vector_indexing_suite<util::TypeInfoVector>())
			;

	typedef std::vector<std::string> VecOfStr;
	class_<VecOfStr>("VectorOfStrings")
		.def(vector_indexing_suite<VecOfStr>());

} // BOOST_PYTHON_MODULE(libvgp)

} // namespace python
} // namespace vgp

