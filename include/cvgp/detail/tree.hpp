/*
 * tree.hpp
 *
 *  Created on: 2010-01-31
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <ostream>
#include <boost/foreach.hpp>
#include <cvgp/detail/node_intermediate.hpp>
#include <cvgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct tree {
	tree(const tree&);
	tree(NodeBase*);
	~tree();

	void init();
	void mutate();
	std::size_t count() const;

	NodeBase* root;
};

util::TypeInfo result_type(const tree&);
std::ostream& operator<<(std::ostream&, const NodeBase&);
std::ostream& operator<<(std::ostream&, const tree&);

template <typename CALLABLE>
void walk_tree(NodeBase& n, CALLABLE& c) {
	std::list<NodeBase*> stack;
	stack.push_back(&n);
	while(!stack.empty()) {
		NodeBase* cur_node = stack.back();
		stack.pop_back();
		c(*cur_node);
		try {
			children_t& children = cur_node->getchildren();
			BOOST_FOREACH(NodeBase& n, children)
				stack.push_back(&n);
		}
		catch (NoChildren) {}
	}
}

template <typename CALLABLE>
void walk_tree(const NodeBase& n, CALLABLE& c) {
	std::list<NodeBase*> stack;
	stack.push_back(&n);
	while(!stack.empty()) {
		NodeBase* cur_node = stack.back();
		stack.pop_back();
		c(*cur_node);
		try {
			children_t& children = cur_node->getchildren();
			BOOST_FOREACH(NodeBase& n, children)
				stack.push_back(&n);
		}
		catch (NoChildren) {}
	}
}

template <typename CALLABLE>
void walk_tree(tree& t, CALLABLE& c) {walk_tree(*t.root, c);}

template <typename CALLABLE>
void walk_tree(const tree& t, CALLABLE& c) {walk_tree(*t.root, c);}

template <typename CALLABLE>
void walk_tree_children(NodeBase& n, CALLABLE& c) {
	std::list<NodeBase*> stack;
	stack.push_back(&n);
	while(!stack.empty()) {
		NodeBase* cur_node = stack.back();
		stack.pop_back();
		try {
			children_t& children = cur_node->getchildren();
			children_t::iterator itr = children.begin();
			for(; itr != children.end(); itr++) {
				c(*cur_node, itr);
				stack.push_back(&(*itr));
			}
		}
		catch(NoChildren) {}
	}
}

template <typename CALLABLE>
void walk_tree_children(const NodeBase& n, CALLABLE& c) {
	std::list<NodeBase*> stack;
	stack.push_back(&n);
	while(!stack.empty()) {
		NodeBase* cur_node = stack.back();
		stack.pop_back();
		try {
			children_t& children = cur_node->getchildren();
			children_t::iterator itr = children.begin();
			for(; itr != children.end(); itr++) {
				c(*cur_node, itr);
				stack.push_back(&(*itr));
			}
		}
		catch(NoChildren) {}
	}
}

template <typename CALLABLE>
void walk_tree_children(tree& t, CALLABLE& c) {walk_tree_children(*t.root, c);}

template <typename CALLABLE>
void walk_tree_children(const tree& t, CALLABLE& c) {walk_tree_children(*t.root, c);}

} // namespace detail
} // namespace vgp


#endif /* TREE_HPP_ */
