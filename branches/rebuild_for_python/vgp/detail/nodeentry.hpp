/*
 * nodeentry.hpp
 *
 *  Created on: 2010-01-28
 *      Author: vsekhar
 */

#ifndef NODEENTRY_HPP_
#define NODEENTRY_HPP_

#include <string>
#include <ostream>
#include <vgp/detail/node.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct NodeEntry {
	NodeEntry(const NodeBase *nb, const util::TypeInfo t, const util::TypeInfoVector p,
			const std::string& n)
		: prototype(nb), fptr(reinterpret_cast<const Node_w_ptr*>(nb)->fptr),
		  result_type(t), parameter_types(p), name(n), arity(p.size())
	{}
	NodeEntry(const NodeEntry& n)
		: prototype(n.prototype),
		  fptr(n.fptr),
		  result_type(n.result_type),
		  parameter_types(n.parameter_types),
		  name(n.name),
		  arity(n.arity)
	{}
	bool terminal() const { return arity==0;}

	const NodeBase* prototype;
	const void_fptr_t fptr;
	const util::TypeInfo result_type;
	const util::TypeInfoVector parameter_types;
	const std::string name;
	const unsigned int arity;
};

std::ostream& operator<<(std::ostream&, const NodeEntry&);

extern void pyexport_nodeentry();

} // namespace detail
} // namespace vgp

#endif /* NODEENTRY_HPP_ */
