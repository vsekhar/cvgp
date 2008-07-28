#ifndef VGP_NODEFRAME_HPP_
#define VGP_NODEFRAME_HPP_

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/scoped_ptr.hpp>

#include <vgp/nodes/nodebase.hpp>

// TODO: what's up with this? necessary? what about for tree manipulation?

namespace vgp {

struct NodeFrame; // fwd declare
typedef boost::scoped_ptr<NodeFrame> NodeFramePtr;

struct NodeFrame {
	NodeFrame() {}
	NodeFrame(NodeBase* p) : nodeptr_(p) {
		children.reserve(p->getarity());
	}

	NodeBase* getnode() {return ptr_.get();}
	inline bool empty() const {return ptr_.empty();}
	inline void reset() {ptr_.reset();}

private:
	friend class boost::serialization::access;
	template <class Archive>
	void save(Archive &ar, const unsigned int /* version */) {

	}
	NodePtr nodeptr_
	std::vector<NodeFramePtr> children;
};

} // end namespace vgp

#endif /*VGP_NODEFRAME_HPP_*/
