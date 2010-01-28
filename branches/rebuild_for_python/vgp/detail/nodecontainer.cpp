/*
 * nodecontainer.cpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#include <boost/python.hpp>
#include <vgp/detail/nodecontainer.hpp>

namespace vgp {
namespace detail {

void pyexport_nodecontainer() {
	using namespace boost::python;
	//class_<NodeContainer::NodesByName_t>("NodesByName_t");
	//class_<NodeContainer::NodesByResultType_t>("NodesByResultType_t");
	class_<NodeContainer>("NodeContainer");
}

} // namespace detail
} // namespace vgp
