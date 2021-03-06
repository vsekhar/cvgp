/*
 * nodeentry.hpp
 *
 *  Created on: 2010-01-28
 */

#ifndef NODEENTRY_HPP_
#define NODEENTRY_HPP_

#include <string>
#include <ostream>
#include <cvgp/detail/nodebase_fwd.hpp>
#include <cvgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct NodeEntry {
	NodeEntry(const NodeBase*, const util::TypeInfo,
			const util::TypeInfoVector, const std::string&);
	NodeEntry(const NodeEntry&);

	bool terminal() const { return arity==0;}
	std::string id() const;

	const NodeBase* prototype;
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
