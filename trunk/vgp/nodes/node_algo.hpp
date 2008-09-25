/*
 * node_algo.hpp
 *
 *  Created on: Sep 25, 2008
 *      Author: vsekhar
 */

#ifndef NODE_ALGO_HPP_
#define NODE_ALGO_HPP_

#include <vgp/nodes/nodebase.hpp>

namespace vgp {

template <class Container>
std::size_t gathertypes(detail::NodeBase*, Container &c) {
	c.insert(std::make_pair(curnode->getresulttypeinfo(), curnode));
	detail::NodeBase::ChildrenContainer::const_iterator i = curnode->children.begin();
	for( ; i != curnode->children.end(); i++)
		gathertypes_recursive(c, &(*i));
}

} // namespace vgp


#endif /* NODE_ALGO_HPP_ */
