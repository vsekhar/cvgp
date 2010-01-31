/*
 * nodeentry.cpp
 *
 *  Created on: 2010-01-30
 *      Author: vsekhar
 */

#include <boost/foreach.hpp>
#include <boost/python.hpp>
#include <vgp/detail/nodeentry.hpp>

namespace vgp {
namespace detail {

std::ostream& operator<<(std::ostream& o, const NodeEntry& n) {
	o << n.name << "[" << n.result_type << "](";
	bool first = true;
	BOOST_FOREACH(const util::TypeInfo& t, n.parameter_types) {
		if(first) first = false;
		else o << ",";
		o << t;
	}
	o << ")";
	return o;
}

void pyexport_nodeentry() {
	using namespace boost::python;
	class_<NodeEntry>("NodeEntry", no_init)
			.def_readonly("prototype", &NodeEntry::prototype)
			.def_readonly("result_type", &NodeEntry::result_type)
			.def_readonly("parameter_types", &NodeEntry::parameter_types)
			.def_readonly("name", &NodeEntry::name)
			.def_readonly("arity", &NodeEntry::arity)
			;
}

} // namespace detail
} // namespace vgp
