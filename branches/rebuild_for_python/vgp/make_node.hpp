/*
 * make_node.hpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#ifndef MAKE_NODE_HPP_
#define MAKE_NODE_HPP_

#include <vgp/detail/node.hpp>

namespace vgp {

template <class FPTR>
detail::NodeBase* make_node(const FPTR f, std::string) {
	return new detail::Node<FPTR>(f);
}

template <class FPTR>
detail::NodeBase* make_terminal(const FPTR f, std::string) {
	return new detail::Terminal<FPTR>(f);
}

template <class FPTR, class IPTR, class MPTR>
detail::NodeBase* make_terminal(
		const FPTR f,
		const IPTR i,
		const MPTR m,
		std::string)
{
	return new detail::Terminal_w_state<FPTR>(f,i,m);
}

template <class FPTR, class STATE, class IPTR, class MPTR>
detail::NodeBase* make_terminal(
		const FPTR f,
		STATE s,
		const IPTR i,
		const MPTR m,
		std::string)
{
	return new detail::Terminal_w_state<FPTR>(f,s,i,m);
}

} // namespace vgp

#endif /* MAKE_NODE_HPP_ */
