/*
 * node_intermediate.hpp
 *
 *  Created on: 2010-01-27
 *
 *  Derives from nodebase to add handling of children, access to result_type
 *  and the ability to 'run' a tree of nodes
 *
 *  This header is used by code that needs the ability to use existing nodes.
 */

#ifndef NODE_INTERMEDIATE_HPP_
#define NODE_INTERMEDIATE_HPP_

#include <string>
#include <stdexcept>

#include <cvgp/detail/nodebase.hpp>
#include <cvgp/detail/fptr.hpp>

namespace vgp {
namespace detail {

struct NoChildren : virtual std::exception {};

// Adds pointer, construct code, and failure to get children
struct Node_w_ptr : NodeBase {
	Node_w_ptr(void_fptr_t p) : fptr(p) {}
	virtual void init() {}
	virtual void mutate() {}
	virtual bool mutatable() const {return false;}
	virtual void_fptr_t getpointer() const {return fptr;}
	virtual children_t& getchildren() {throw NoChildren();}
	virtual const children_t& getchildren() const {throw NoChildren();}
	void_fptr_t const fptr;
};

/*
 * Adds a result_type for the contained function
 *
 * Separating this from Node_w_ptr allows code to refer to a node
 * via Node_w_ptr, and even execute that node without knowing the
 * function's return type
 *
 * Leaf/nullary functions use this as a base
 */
template <typename RESULT>
struct Node_returning : Node_w_ptr {
	typedef RESULT result_type;
	Node_returning(void_fptr_t p) : Node_w_ptr(p) {}
	virtual result_type run_node() const = 0;
};

// Adds children handling for non-leaf functions
template <typename FPTR>
struct Node_w_children : Node_returning<typename ft::result_type<FPTR>::type> {
	Node_w_children(const FPTR p)
		: Node_returning<typename ft::result_type<FPTR>::type>(
				reinterpret_cast<void_fptr_t>(p)) {}
	virtual children_t& getchildren() {return children;}
	virtual const children_t& getchildren() const {return children;}
	children_t children;
};

} // namespace detail
} // namespace vgp

#endif /* NODE_INTERMEDIATE_HPP_ */
