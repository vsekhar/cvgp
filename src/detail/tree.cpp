/*
 * tree.cpp
 *
 *  Created on: 2010-01-31
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/python.hpp>
#include <boost/ref.hpp>

#include <cvgp/detail/tree.hpp>
#include <cvgp/detail/node_intermediate.hpp>
#include <cvgp/detail/nodestorage.hpp>
#include <cvgp/util/random.hpp>
#include <cvgp/defaults.hpp>

using std::cout;
using std::endl;

namespace vgp {
namespace detail {

tree::tree(const tree& t) : root(t.root->clone()) {}
tree::tree(NodeBase* r) : root(r)  {}
tree::~tree() {delete root;}

void do_init(NodeBase& n) {n.init();}
void tree::init() {walk_tree(*this, do_init);}

struct get_mutatables {
	std::vector<NodeBase*> result;
	void operator()(NodeBase& n) {if(n.mutatable()) result.push_back(&n);}
};

void tree::mutate() {
	get_mutatables g;
	walk_tree(*this, g);
	if(!g.result.empty()) {
		const std::size_t randint
			= boost::uniform_int<>(0,g.result.size())(util::default_generator);
		g.result[randint]->mutate();
	}
}

struct counter {
	std::size_t count;
	counter() : count(0) {}
	void operator()(NodeBase&) {++count;}
};

std::size_t tree::count() const {
	counter c;
	walk_tree(*this, c);
	return c.count;
}

util::TypeInfo result_type(const tree& t) {
	return result_type(t.root);
}

std::ostream& operator<<(std::ostream& o, const NodeBase& n) {
	// can't use walk_tree because of the comma handling
	const Node_w_ptr* node = static_cast<const Node_w_ptr*>(&n);
	void_fptr_t fptr = node->fptr;
	NodesByFptr::const_iterator itr = nodesbyfptr.find(fptr);
	if(itr == nodesbyfptr.end()) return o;
	o << itr->name << "[" << itr->result_type << "](";
	bool first = true;
	if(itr->arity) {
		BOOST_FOREACH(const NodeBase& n, node->getchildren()) {
			if(first) first = false;
			else o << ", ";
			o << n;
		}
	}
	o << ")";
	return o;
}

std::ostream& operator<<(std::ostream& o, const tree& t) {
	return o << *t.root;
}

} // namespace detail
} // namespace vgp
