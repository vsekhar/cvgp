/*
 * trees.hpp
 *
 *  Created on: 2010-02-06
 */

#ifndef TREES_HPP_
#define TREES_HPP_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/global_fun.hpp>

#include <vgp/detail/nodestorage.hpp>
#include <vgp/detail/trees_base.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct Trees : Trees_base {
	Trees()
		: bysequence(get<bySequence>()),
		  byrandomaccess(get<byRandomAccess>()),
		  byresulttype(get<byResultType>()) {}

	TreesBySequence& bysequence;
	TreesByRandomAccess& byrandomaccess;
	TreesByResultType& byresulttype;
};

} // namespace detail
} // namespace vgp

#endif /* TREES_HPP_ */
