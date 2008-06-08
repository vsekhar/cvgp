#ifndef NODEFRAME_HPP_
#define NODEFRAME_HPP_

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/scoped_ptr.hpp>

#include <vgp/nodes/nodebase.hpp>

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

#endif /*NODEFRAME_HPP_*/
