/*
 * tree.hpp
 *
 *  Created on: 2010-01-31
 *      Author: vsekhar
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <vgp/detail/node.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

NodeBase* generate(const util::TypeInfo&);

struct tree {
	tree(const tree& t) : root(t.root->clone()), result_type(t.result_type) {}
	tree(NodeBase* r, const util::TypeInfo& t) : root(r), result_type(t) {}
	tree(const util::TypeInfo& t) : root(generate(t)), result_type(t) {}
	~tree() {delete root;}

	NodeBase* root;
	util::TypeInfo result_type;
};

void init_tree(tree&);

} // namespace detail
} // namespace vgp


#endif /* TREE_HPP_ */
