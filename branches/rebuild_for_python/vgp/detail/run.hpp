/*
 * run_tree.hpp
 *
 *  Created on: 2010-02-07
 */

#ifndef RUN_TREE_HPP_
#define RUN_TREE_HPP_

// Don't include node.hpp here, or it becomes circular with adf.hpp
//#include <vgp/detail/node.hpp>

#include <vgp/detail/tree.hpp>
#include <vgp/organism.hpp>
#include <vgp/detail/nodebase.hpp>

namespace vgp {

namespace detail {
template <typename T> struct Node_returning;

template <typename R_>
R_ run_as(const detail::tree& t) {
	using namespace detail;
#ifdef _DEBUG
	BOOST_ASSERT(typeid(R_)==result_type(t.root->getpointer()));
#endif
	return static_cast<const Node_returning<R_>*>(t.root)->run_node();
}

template <typename T>
T run_as(const Organism& o) {
	using namespace detail;
	// Find first tree with desired result_type T, and run
	TreesByResultType::const_iterator itr
		= o.trees.byresulttype.lower_bound(typeid(T));
	if(itr = o.trees.byresulttype.end())
		throw OrganismBadRunType();
	return run_as<T>(*itr);
}

} // namespace detail
} // namespace vgp

#endif /* RUN_TREE_HPP_ */
