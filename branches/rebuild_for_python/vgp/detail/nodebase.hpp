/*
 * nodebase.hpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#ifndef NODEBASE_HPP_
#define NODEBASE_HPP_

#include <boost/any.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace vgp {
namespace detail {

struct NodeBase;
typedef boost::ptr_vector<NodeBase> children_t;
typedef void(*void_fptr_t)();

// interface (pure virtual and templates)
struct NodeBase {
	virtual void init() = 0;
	virtual void mutate() = 0;
	virtual children_t& getchildren() = 0;
	virtual boost::any run_node() const = 0;
	virtual NodeBase* clone() const = 0;
};

inline NodeBase* new_clone(const NodeBase& n) {
	return n.clone();
}

void pyexport_nodebase();

} // namespace detail

template <typename result_type>
result_type runner(const detail::NodeBase& n) {
	return boost::any_cast<result_type>(n.run_node());
}

} // namespace vgp


#endif /* NODEBASE_HPP_ */
