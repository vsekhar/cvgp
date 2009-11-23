/*
 * nodetypeindex.hpp
 *
 *  Created on: Sep 25, 2008
 *      Author: vsekhar
 */

#ifndef NODETYPEINDEX_HPP_
#define NODETYPEINDEX_HPP_

#include <utility>
#include <map>

#include <vgp/nodes/nodebase.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {

typedef std::pair<detail::NodeBase*, std::size_t> NodeEntry;
typedef std::pair<util::TypeInfo, NodeEntry> NodeTypeEntry;
typedef std::multimap<util::TypeInfo, NodeEntry> NodeTypeMap;

} // namespace vgp

#endif /* NODETYPEINDEX_HPP_ */
