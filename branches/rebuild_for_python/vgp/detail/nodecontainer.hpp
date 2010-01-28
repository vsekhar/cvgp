/*
 * nodecontainer.hpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#ifndef NODECONTAINER_HPP_
#define NODECONTAINER_HPP_

#include <vgp/detail/nodemultiindex.hpp>

namespace vgp {
namespace detail {

struct NodeContainer {
	NodeContainer()
		: nodesbyname(_nodes.get<byName>()),
		  nodesbyresulttype(_nodes.get<byResultType>())
	{}

	typedef NodeMultiIndex::index<byName>::type NodesByName_t;
	typedef NodeMultiIndex::index<byResultType>::type NodesByResultType_t;
private:
	NodeMultiIndex _nodes;
	NodesByName_t& nodesbyname;
	NodesByResultType_t& nodesbyresulttype;
};

void pyexport_nodecontainer();

} // namespace detail
} // namespace vgp


#endif /* NODECONTAINER_HPP_ */
