/*
 * run_tree.hpp
 *
 *  Created on: 2010-02-07
 */

#ifndef RUN_HPP_
#define RUN_HPP_

#include <cvgp/organism.hpp>
#include <cvgp/detail/tree.hpp>
#include <cvgp/detail/node_intermediate.hpp>

namespace vgp {
namespace detail {

template <typename R_>
R_ run_as(const detail::tree& t) {
	using namespace detail;
#ifdef _DEBUG
	BOOST_ASSERT(typeid(R_)==result_type(t.root));
#endif
	return static_cast<const Node_returning<R_>*>(t.root)->run_node();
}

template <typename T>
T run_as(const Organism& o) {
	using namespace detail;
	// Find first tree with desired result_type T, and run
	TreesByResultType::const_iterator itr
		= o.trees.byresulttype.lower_bound(typeid(T));
	if(itr == o.trees.byresulttype.end())
		throw OrganismBadRunType();
	return run_as<T>(*itr);
}

} // namespace detail
} // namespace vgp

#endif /* RUN_HPP_ */
