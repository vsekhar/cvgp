#ifndef INSTANTIATENODES_HPP_
#define INSTANTIATENODES_HPP_

#include <vgp/nodes/nodecontainer.hpp>

namespace vgp {

template <class NODES>
unsigned int instantiatenodes() {
	return util::types_to_objs<
		NODES,
		util::baseptr_wrapped_push_back<
			NodeContainer,
			detail::NodeBase,
			detail::Node
		> 
	>(Nodes);
}

} // namespace vgp

#endif /*INSTANTIATENODES_HPP_*/
