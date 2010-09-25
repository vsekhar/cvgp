/*
 * trees.hpp
 *
 *  Created on: 2010-02-06
 */

#ifndef TREES_HPP_
#define TREES_HPP_

#include <ostream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/global_fun.hpp>

#include <vgp/detail/tree.hpp>
#include <vgp/detail/nodestorage.hpp>
#include <vgp/util/typeinfo.hpp>

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

struct Trees : Trees_base {
	Trees();
	Trees(const Trees&);

	TreesBySequence& bysequence;
	TreesByRandomAccess& byrandomaccess;
	TreesByResultType& byresulttype;
};

std::ostream& operator<<(std::ostream& o, const Trees& t);

} // namespace detail
} // namespace vgp

#endif /* TREES_HPP_ */
