/*
 * organism.cpp
 *
 *  Created on: 2010-01-31
 */

#include <vgp/organism.hpp>
#include <vgp/detail/generate.hpp>

namespace vgp {

Organism::Organism(util::TypeInfo t) {
	// create first tree
	detail::NodeBase* n = detail::generate(t, trees, trees.begin(), 0);
	trees.push_back(detail::tree(n));
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

} // namespace vgp
