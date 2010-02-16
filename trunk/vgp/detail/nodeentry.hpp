/*
 * nodeentry.hpp
 *
 *  Created on: 2010-01-28
 */

#ifndef NODEENTRY_HPP_
#define NODEENTRY_HPP_

#include <string>
#include <ostream>
#include <vgp/detail/nodebase_fwd.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct NodeEntry {
	NodeEntry(const NodeBase*, const NodeBase*, const util::TypeInfo,
			const util::TypeInfoVector, const std::string&);
	NodeEntry(const NodeEntry&);

	bool terminal() const { return arity==0;}

	const NodeBase* prototype;
	// const NodeBase* prototype_adf;
	const void_fptr_t fptr;
	const util::TypeInfo result_type;
	const util::TypeInfoVector parameter_types;
	const std::string name;
	const unsigned int arity;
};

std::ostream& operator<<(std::ostream&, const NodeEntry&);

} // namespace detail
} // namespace vgp

#endif /* NODEENTRY_HPP_ */
