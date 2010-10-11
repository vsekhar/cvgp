/*
 * nodeentry.cpp
 *
 *  Created on: 2010-01-30
 */

#include <sstream>

#include <boost/foreach.hpp>

#include <cvgp/detail/nodeentry.hpp>
#include <cvgp/detail/nodebase.hpp>

namespace vgp {
namespace detail {

NodeEntry::NodeEntry(const NodeBase *nb, const util::TypeInfo t,
		const util::TypeInfoVector p, const std::string& n)
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

std::string NodeEntry::id() const {
	std::stringstream ret;
	ret << name << "[" << result_type << "](";
	bool first = true;
	BOOST_FOREACH(const util::TypeInfo& t, parameter_types) {
		if(first) first = false;
		else ret << ",";
		ret << t;
	}
	ret << ")";
	return ret.str();
}

std::ostream& operator<<(std::ostream& o, const NodeEntry& n) {
	return o<<n.id();
}

} // namespace detail
} // namespace vgp
