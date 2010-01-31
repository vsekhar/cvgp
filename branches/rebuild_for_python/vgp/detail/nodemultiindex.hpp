#ifndef VGP_NODEMULTIINDEX_HPP_
#define VGP_NODEMULTIINDEX_HPP_

#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>

#include <vgp/detail/nodeentry.hpp>

namespace vgp {

namespace detail {

struct bySequence {};
struct byName {};
struct byResultType {};

using namespace ::boost::multi_index;

typedef multi_index_container<
	NodeEntry,
	indexed_by<
		sequenced<tag<bySequence> >
		, ordered_unique<
			tag<byName>,
			member<NodeEntry, const std::string, &NodeEntry::name>
		>
		, ordered_non_unique<
			tag<byResultType>,
			composite_key<
				NodeEntry,
				member<NodeEntry, const util::TypeInfo, &NodeEntry::result_type>,
				member<NodeEntry, const unsigned int, &NodeEntry::arity>
			>
		>
	>
> NodeMultiIndex;

typedef NodeMultiIndex::index<bySequence>::type NodesBySequence;
typedef NodeMultiIndex::index<byName>::type NodesByName;
typedef NodeMultiIndex::index<byResultType>::type NodesByResultType;

} // namespace detail
} // namespace vgp


#endif /*VGP_NODEMULTIINDEX_HPP_*/
