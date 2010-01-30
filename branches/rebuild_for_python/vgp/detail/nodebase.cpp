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

struct NodeBaseWrap : NodeBase, wrapper<NodeBase>
{
	void init() {this->get_override("init")();};
	void mutate() {this->get_override("mutate")();}
	children_t& getchildren() {return this->get_override("getchildren")();}
	NodeBase* clone() const {return this->get_override("clone")();}
};

void pyexport_nodebase() {
	class_<children_t>("children_t");
	class_<NodeBaseWrap, boost::noncopyable>("NodeBase")
			.def("init", pure_virtual(&NodeBase::init));
	class_<NodeBaseWrap, boost::noncopyable>("NodeBase")
			.def("mutate", pure_virtual(&NodeBase::mutate));
	class_<NodeBaseWrap, boost::noncopyable>("NodeBase")
			.def("getchildren", pure_virtual(&NodeBase::getchildren),
					return_value_policy<copy_non_const_reference>());
	class_<NodeBaseWrap, boost::noncopyable>("NodeBase")
			.def("clone", pure_virtual(&NodeBase::clone),
					return_value_policy<manage_new_object>());
}
} // namespace detail
} // namespace vgp
