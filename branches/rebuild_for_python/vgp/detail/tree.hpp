/*
 * tree.hpp
 *
 *  Created on: 2010-01-31
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <ostream>
#include <boost/foreach.hpp>
#include <vgp/detail/node_intermediate.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct tree {
	tree(const tree&);
	tree(NodeBase*);
	~tree();

	void init();
	void mutate();

	NodeBase* root;
};

util::TypeInfo result_type(const tree&);
std::ostream& operator<<(std::ostream&, const NodeBase&);
std::ostream& operator<<(std::ostream&, const tree&);

template <typename CALLABLE>
void walk_tree(NodeBase* n, CALLABLE& c) {
	std::list<NodeBase*> stack;
	stack.push_back(n);
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
void walk_tree(tree& t, CALLABLE& c) {
	walk_tree(t.root, c);
}

} // namespace detail
} // namespace vgp


#endif /* TREE_HPP_ */
