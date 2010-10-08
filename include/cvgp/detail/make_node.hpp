/*
 * make_node.hpp
 *
 *  Created on: 2010-02-07
 */

#ifndef MAKE_NODE_HPP_
#define MAKE_NODE_HPP_

#include <cvgp/detail/node_concrete.hpp>
#include <cvgp/detail/adf.hpp>
#include <cvgp/detail/tree.hpp>

namespace vgp {
namespace detail {

template <class FPTR>
NodeBase* make_node(const FPTR f) {
	return new Node<FPTR>(f);
}

template <typename FROM, typename TO>
TO adapter(FROM a) {return a;}

template <typename FROM, typename TO>
NodeBase* make_adapter() {
	typedef Node<TO(*)(FROM)> node_t;
	return new node_t(adapter<FROM,TO>);
}

template <class FPTR>
NodeBase* make_terminal(const FPTR f) {
	return new Terminal<FPTR>(f);
}

template <class FPTR, class IPTR, class MPTR>
NodeBase* make_terminal(const FPTR f, const IPTR i, const MPTR m) {
	return new Terminal_w_state<FPTR>(f,i,m);
}

template <class FPTR, class STATE, class IPTR, class MPTR>
NodeBase* make_terminal(const FPTR f, STATE s, const IPTR i, const MPTR m) {
	return new Terminal_w_state<FPTR>(f,s,i,m);
}

template <class result_type>
NodeBase* make_adf() {
	return new ADF<result_type>();
}

} // namespace detail
} // namespace vgp

#endif /* MAKE_NODE_HPP_ */
