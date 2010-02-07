/*
 * trees_base.hpp
 *
 *  Created on: 2010-02-07
 *      Author: vsekhar
 */

#ifndef TREES_BASE_HPP_
#define TREES_BASE_HPP_

#include <vgp/detail/tree.hpp>

namespace vgp {
namespace detail {

struct bySequence;
struct byRandomAccess;
struct byResultType;

using namespace boost::multi_index;

typedef multi_index_container <
	tree,
	indexed_by<
		sequenced<tag<bySequence> >
		, random_access<tag<byRandomAccess> >
		, ordered_non_unique<
			tag<byResultType>,
			global_fun<const tree&, util::TypeInfo, &result_type>
		>
	>
> Trees_base;

typedef Trees_base::index<bySequence>::type TreesBySequence;
typedef Trees_base::index<byRandomAccess>::type TreesByRandomAccess;
typedef Trees_base::index<byResultType>::type TreesByResultType;

} // namespace detail
} // namespace vgp

#endif /* TREES_BASE_HPP_ */
