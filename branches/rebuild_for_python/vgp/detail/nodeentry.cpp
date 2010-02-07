/*
 * nodeentry.cpp
 *
 *  Created on: 2010-01-30
 *      Author: vsekhar
 */

#include <boost/foreach.hpp>
#include <boost/python.hpp>
#include <vgp/detail/nodeentry.hpp>
#include <vgp/detail/nodebase.hpp>

namespace vgp {
namespace detail {

NodeEntry::NodeEntry(const NodeBase *nb, const util::TypeInfo t, const util::TypeInfoVector p,
		const std::string& n)
	: prototype(nb), fptr(nb->getpointer()),
	  result_type(t), parameter_types(p), name(n), arity(p.size())
{}
NodeEntry::NodeEntry(const NodeEntry& n)
	: prototype(n.prototype),
	  fptr(n.fptr),
	  result_type(n.result_type),
	  parameter_types(n.parameter_types),
	  name(n.name),
	  arity(n.arity)
{}

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
