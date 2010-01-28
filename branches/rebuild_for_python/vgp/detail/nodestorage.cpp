/*
 * nodestorage.cpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#include <boost/python.hpp>
#include <vgp/detail/nodestorage.hpp>

namespace vgp {
namespace detail {

NodeContainer nodes;
const NodeContainer& getnodes() {return nodes;}

void pyexport_nodestorage() {
	using namespace boost::python;
	def("getnodes", getnodes, return_value_policy<copy_const_reference>());
}

} // namespace detail
} // namespace vgp
