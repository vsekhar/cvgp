/*
 * nodestorage.cpp
 *
 *  Created on: 2010-01-27
 */

#include <string>
#include <vector>
#include <sstream>
#include <boost/foreach.hpp>
#include <cvgp/detail/nodestorage.hpp>
#include <cvgp/detail/node_intermediate.hpp>

namespace vgp {
namespace detail {

NodeMultiIndex nodes;
NodesBySequence &nodesbysequence = nodes.get<bySequence>();
NodesByName &nodesbyname = nodes.get<byName>();
NodesByResultType &nodesbyresulttype = nodes.get<byResultType>();
NodesByFptr &nodesbyfptr = nodes.get<byFptr>();

NodeMultiIndex internal_nodes;
NodesBySequence &internal_nodesbysequence = internal_nodes.get<bySequence>();
NodesByName &internal_nodesbyname = internal_nodes.get<byName>();
NodesByResultType &internal_nodesbyresulttype = internal_nodes.get<byResultType>();
NodesByFptr &internal_nodesbyfptr = internal_nodes.get<byFptr>();

const NodeEntry& node_entry(void_fptr_t p) {return *nodesbyfptr.find(p);}
const NodeEntry& node_entry(const NodeBase* n) {
	return node_entry(static_cast<const Node_w_ptr*>(n)->fptr);
}

util::TypeInfo result_type(void_fptr_t p) {return node_entry(p).result_type;}
util::TypeInfo result_type(const NodeBase* n) {return node_entry(n).result_type;}

std::vector<std::string> listnodes() {
	std::vector<std::string> ret;
	ret.reserve(nodes.size());
	BOOST_FOREACH(const NodeEntry& n, nodes) {
		std::stringstream ss;
		ss << n;
		ret.push_back(ss.str());
	}
	return ret;
}

} // namespace detail
} // namespace vgp
