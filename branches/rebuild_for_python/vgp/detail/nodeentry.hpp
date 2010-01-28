/*
 * nodeentry.hpp
 *
 *  Created on: 2010-01-28
 *      Author: vsekhar
 */

#ifndef NODEENTRY_HPP_
#define NODEENTRY_HPP_

#include <string>
#include <vgp/detail/nodebase_fwd.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct NodeEntry {
	NodeEntry(const NodeBase *nb, const util::TypeInfo t, const util::TypeInfoList p, const std::string& n)
		: nodeptr(nb), result_type(t), parameter_types(p), name(n), arity(p.size())
	{}
	NodeEntry(const NodeEntry& n)
		: nodeptr(n.nodeptr),
		  result_type(n.result_type),
		  parameter_types(n.parameter_types),
		  name(n.name),
		  arity(n.parameter_types.size())
	{}
	bool terminal() const { return arity==0;}
	const NodeBase* nodeptr;
	const util::TypeInfo result_type;
	const util::TypeInfoList parameter_types;
	const std::string name;
	const unsigned int arity;
};

} // namespace detail
} // namespace vgp

#endif /* NODEENTRY_HPP_ */
