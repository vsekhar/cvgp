/*
 * nodebase.cpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#include <boost/python.hpp>
#include <vgp/detail/nodebase.hpp>

namespace vgp {
namespace detail {

using namespace boost::python;

void pyexport_nodebase() {
	class_<NodeVector>("NodeVector");
	class_<NodeBase, boost::noncopyable>("NodeBase", no_init)
			.def("init", pure_virtual(&NodeBase::init))
			.def("mutate", pure_virtual(&NodeBase::mutate))
			.def("getchildren", pure_virtual(&NodeBase::getchildren),
					return_value_policy<copy_non_const_reference>())
			.def("clone", pure_virtual(&NodeBase::clone),
					return_value_policy<manage_new_object>());
}

} // namespace detail
} // namespace vgp
