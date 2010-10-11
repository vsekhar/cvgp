/*
 * organism.cpp
 *
 *  Created on: 2010-01-31
 */

#include <cvgp/organism.hpp>
#include <cvgp/detail/generate.hpp>
#include <cvgp/detail/adf.hpp>

namespace vgp {

Organism::Organism(util::TypeInfo t) {
	// create first tree
	detail::NodeBase* n = detail::generate(t, trees, trees.end(), 0);
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
	detail::make_random_adf(trees);
}

void Organism::collapse_adf() {

}

std::ostream& operator<<(std::ostream& o, const Organism& org) {
	return o << org.trees;
}

} // namespace vgp
