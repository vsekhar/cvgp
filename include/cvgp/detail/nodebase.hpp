/*
 * nodebase.hpp
 *
 *  Created on: 2010-01-27
 */

#ifndef NODEBASE_HPP_
#define NODEBASE_HPP_

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <cvgp/detail/nodebase_fwd.hpp>

namespace vgp {
namespace detail {

typedef boost::ptr_vector<NodeBase> children_t;

// interface (pure virtual and templates)
struct NodeBase {
	virtual ~NodeBase() {}
	virtual void init() = 0;
	virtual void mutate() = 0;
	virtual bool mutatable() const = 0;
	virtual void_fptr_t getpointer() const = 0;
	virtual children_t & getchildren() = 0;
	virtual const children_t& getchildren() const = 0;
	virtual NodeBase* clone() const = 0;
};

inline NodeBase* new_clone(const NodeBase& n) {
	return n.clone();
}

} // namespace detail
} // namespace vgp


#endif /* NODEBASE_HPP_ */
