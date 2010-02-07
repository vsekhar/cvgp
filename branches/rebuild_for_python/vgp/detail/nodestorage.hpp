/*
 * nodestorage.hpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#ifndef NODESTORAGE_HPP_
#define NODESTORAGE_HPP_

#include <vgp/detail/nodemultiindex.hpp>
#include <vgp/util/typeinfo.hpp>
#include <vgp/detail/nodebase_fwd.hpp>

namespace vgp {
namespace detail {

extern NodeMultiIndex nodes;
extern NodesBySequence &nodesbysequence;
extern NodesByName &nodesbyname;
extern NodesByResultType &nodesbyresulttype;
extern NodesByFptr &nodesbyfptr;

const NodeEntry& node_entry(void_fptr_t);
const NodeEntry& node_entry(const NodeBase*);
util::TypeInfo result_type(void_fptr_t);
util::TypeInfo result_type(const NodeBase*);

void pyexport_nodestorage();

} // namespace detail
} // namespace vgp


#endif /* NODESTORAGE_HPP_ */
