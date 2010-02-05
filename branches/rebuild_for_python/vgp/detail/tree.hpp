/*
 * tree.hpp
 *
 *  Created on: 2010-01-31
 *      Author: vsekhar
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <stdexcept>
#include <ostream>
#include <string>
#include <vgp/detail/node.hpp>
#include <vgp/util/typeinfo.hpp>
#include <boost/assert.hpp>

namespace vgp {
namespace detail {

struct TreeException : virtual std::exception {};
struct TreeGenerateFailed : virtual TreeException {};
struct MaxDepthReached : virtual TreeException {};

NodeBase* generate(const util::TypeInfo&);

struct tree {
	tree(const tree& t) : root(t.root->clone()), result_type(t.result_type) {}
	tree(NodeBase* r, const util::TypeInfo& t) : root(r), result_type(t) {}
	tree(const util::TypeInfo& t) : root(generate(t)), result_type(t) {}
	~tree() {delete root;}

	template <typename R_>
	R_ run_as() const {
#		ifdef _DEBUG
		BOOST_ASSERT(typeid(R_)==result_type);
#		endif
		return reinterpret_cast<const Node_returning<R_>*>(root)->run_node();
	}

	NodeBase* root;
	util::TypeInfo result_type;
};

void init_tree(tree&);
std::ostream& operator<<(std::ostream&, const NodeBase&);
std::ostream& operator<<(std::ostream&, const tree&);

void pyexport_tree();

} // namespace detail
} // namespace vgp


#endif /* TREE_HPP_ */
