/*
 * nodestorage.hpp
 *
 *  Created on: 2010-01-27
 */

#ifndef NODESTORAGE_HPP_
#define NODESTORAGE_HPP_

#include <string>
#include <vector>

#include <cvgp/detail/nodemultiindex.hpp>
#include <cvgp/util/typeinfo.hpp>
#include <cvgp/detail/nodebase_fwd.hpp>

namespace vgp {
namespace detail {

extern NodeMultiIndex nodes;
extern NodesBySequence &nodesbysequence;
extern NodesById &nodesbyid;
extern NodesByResultType &nodesbyresulttype;
extern NodesByFptr &nodesbyfptr;

const NodeEntry& node_entry(void_fptr_t);
const NodeEntry& node_entry(const NodeBase*);
util::TypeInfo result_type(void_fptr_t);
util::TypeInfo result_type(const NodeBase*);

std::vector<std::string> listnodes();

} // namespace detail
} // namespace vgp


#endif /* NODESTORAGE_HPP_ */
