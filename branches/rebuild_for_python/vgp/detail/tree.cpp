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
#include <vgp/detail/tree.hpp>
#include <vgp/detail/node.hpp>
#include <vgp/detail/nodestorage.hpp>
#include <vgp/util/random.hpp>
#include <vgp/defaults.hpp>

using std::cout;
using std::endl;

namespace vgp {
namespace detail {

tree::tree(const tree& t) : root(t.root->clone()) {}
tree::tree(NodeBase* r) : root(r)  {}
tree::~tree() {delete root;}

void init_tree(NodeBase& n) {
	n.init();
	try {
		NodeVector& children = n.getchildren();
		BOOST_FOREACH(detail::NodeBase& child, children)
			init_tree(child);
	}
	catch(NoChildren) {}
}

void tree::init() {init_tree(*root);}

void tree::mutate() {
	std::vector<NodeBase*> mutatables;
	std::list<NodeBase*> stack;
	stack.push_back(root);
	while(!stack.empty()) {
		NodeBase* cur = stack.back();
		stack.pop_back();
		if(cur->mutatable())
			mutatables.push_back(cur);
		try {
			children_t& c = cur->getchildren();
			BOOST_FOREACH(NodeBase& n, c)
				stack.push_back(&n);
		}
		catch(NoChildren) {}
	}
	if(!mutatables.empty()) {
		const std::size_t randint
			= boost::uniform_int<>(0,mutatables.size())(util::default_generator);
		mutatables[randint]->mutate();
	}
}

util::TypeInfo result_type(const tree& t) {
	return result_type(t.root);
}

std::ostream& operator<<(std::ostream& o, const NodeBase& n) {
	const Node_w_ptr* node = static_cast<const Node_w_ptr*>(&n);
	void_fptr_t fptr = node->fptr;
	NodesByFptr::const_iterator itr = nodesbyfptr.find(fptr);
	if(itr == nodesbyfptr.end()) return o;
	o << itr->name << "[" << itr->result_type << "](";
	if(itr->arity) {
		BOOST_FOREACH(const NodeBase& n, node->getchildren())
			o << n;
	}
	o << ")";
	return o;
}

std::ostream& operator<<(std::ostream& o, const tree& t) {
	return o << *t.root;
}

void pyexport_tree() {
	using namespace boost::python;
	class_<tree>("tree", no_init)
			.def(self_ns::str(self)) // gcc hiccups without the namespace here
			;
}

} // namespace detail
} // namespace vgp
