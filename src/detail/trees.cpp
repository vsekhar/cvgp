/*
 * trees.cpp
 *
 *  Created on: 2010-02-06
 */

#include <boost/foreach.hpp>

#include <cvgp/detail/trees.hpp>
#include <cvgp/detail/nodebase.hpp>

namespace vgp {
namespace detail {

Trees::Trees()
	: bysequence(get<bySequence>()),
	  byrandomaccess(get<byRandomAccess>()),
	  byresulttype(get<byResultType>())
{}

Trees::Trees(const Trees& t)
	: Trees_base(t),
	  bysequence(get<bySequence>()),
	  byrandomaccess(get<byRandomAccess>()),
	  byresulttype(get<byResultType>())
{}

std::ostream& operator<<(std::ostream& o, const Trees& trees) {
	// need to convert tree::iterator's into indexes, then convert
	// iterators in ADF nodes into indexes
	o << "[";
	bool first = true;
	BOOST_FOREACH(const tree& cur_tree, trees) {
		if(first) first = false;
		else o << ", ";
		o << cur_tree;
	}
	o << "]";
	return o;
}

} // namespace detail
} // namespace vgp
