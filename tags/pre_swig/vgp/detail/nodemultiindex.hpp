#ifndef VGP_NODEMULTIINDEX_HPP_
#define VGP_NODEMULTIINDEX_HPP_

#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>

#include <vgp/nodes/nodebase.hpp>

namespace vgp {

/** \name Index tags
 * Empty types used as tags to retrieve specific indexes
 */
//@{
struct bySequence {};
struct byID {};
struct byResultType {};
//@}

namespace detail {
using namespace ::boost::multi_index;

/** Multi Index Container for node storage and multi-index lookup
 */
typedef multi_index_container<
	const NodeBase*,
	indexed_by<
		sequenced<tag<bySequence> >
		, ordered_unique<
			tag<byID>,
			const_mem_fun<NodeBase, std::string, &NodeBase::getID>
		>
		, ordered_non_unique<
			tag<byResultType>,
			member<NodeBase, const util::TypeInfo, &NodeBase::result_type>
//			const_mem_fun<NodeBase, util::TypeInfo, &NodeBase::getresulttypeinfo>
		>
	>
> NodeMultiIndex;
} // namespace detail
} // namespace vgp


#endif /*VGP_NODEMULTIINDEX_HPP_*/