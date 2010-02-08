/*
 * organism.cpp
 *
 *  Created on: 2010-01-31
 */

#include <boost/python.hpp>

#include <vgp/organism.hpp>
#include <vgp/detail/generate.hpp>

namespace vgp {

Organism::Organism(util::TypeInfo t) {
	// create first tree
	detail::NodeBase* n = detail::generate(t, trees, trees.begin(), 0);
	trees.push_back(detail::tree(n));
	init();
}

struct init_functor {
	void operator()(detail::tree& t) {t.init();}
};

void Organism::init() {
	detail::Trees::iterator itr = trees.begin();
	for(; itr != trees.end(); itr++)
		trees.modify(itr, init_functor());
}

void Organism::make_adf() {

}

void Organism::collapse_adf() {

}

std::ostream& operator<<(std::ostream& o, const Organism& org) {
	return o << org.trees;
}

void pyexport_organism() {
	using namespace boost::python;
	class_<Organism>("Organism", no_init)
			.def(self_ns::str(self)) // gcc hiccups without the namespace here
			;
}

} // namespace vgp
