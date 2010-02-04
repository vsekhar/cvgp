/*
 * nodestorage.cpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#include <sstream>
#include <boost/foreach.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <vgp/detail/nodestorage.hpp>

namespace vgp {
namespace detail {

NodeMultiIndex nodes;
NodesBySequence &nodesbysequence = nodes.get<bySequence>();
NodesByName &nodesbyname = nodes.get<byName>();
NodesByResultType &nodesbyresulttype = nodes.get<byResultType>();
NodesByFptr &nodesbyfptr = nodes.get<byFptr>();

std::string printnodes() {
	std::stringstream ss;
	bool first = true;
	BOOST_FOREACH(const NodeEntry& n, nodes) {
		if(first) first = false;
		else ss << ", ";
		ss << n;
	}
	return ss.str();
}

void pyexport_nodestorage() {
	using namespace boost::python;
	def("printnodes", printnodes);
}

} // namespace detail
} // namespace vgp
