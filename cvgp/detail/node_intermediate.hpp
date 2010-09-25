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

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/front.hpp>

#include <vgp/detail/nodebase.hpp>

namespace vgp {

namespace mpl = ::boost::mpl;
namespace ft = ::boost::function_types;

namespace detail {

template <typename T>
struct getfirstparam_wo_reference :
	boost::remove_reference<
		typename mpl::front<
			typename ft::parameter_types<T>::type
		>::type
	>
{};

template <typename T>
struct fptr_to_imptr {
	typedef void (*type)(typename getfirstparam_wo_reference<T>::type &);
};

template <typename T>
struct fptr_to_state_type
	: boost::remove_const<
		  typename getfirstparam_wo_reference<T>::type
	> {};

struct NoChildren : virtual std::exception {};

// Adds pointer, construct code, and failure to get children
struct Node_w_ptr : NodeBase {
	Node_w_ptr(void_fptr_t p) : fptr(p) {}
	virtual void init() {}
	virtual void mutate() {}
	virtual bool mutatable() const {return false;}
	virtual bool isADF() const {return false;}
	virtual void_fptr_t getpointer() const {return fptr;}
	virtual NodeVector& getchildren() {throw NoChildren();}
	virtual const NodeVector& getchildren() const {throw NoChildren();}
	void_fptr_t const fptr;
};

template <typename result_type>
struct Node_returning : Node_w_ptr {
	Node_returning(void_fptr_t p) : Node_w_ptr(p) {}
	virtual result_type run_node() const = 0;
};

typedef NodeVector children_t;

// Adds children handling
template <typename FPTR>
struct Node_w_children : Node_returning<typename ft::result_type<FPTR>::type> {
	Node_w_children(const FPTR p)
		: Node_returning<typename ft::result_type<FPTR>::type>(
				reinterpret_cast<void_fptr_t>(p)) {}
	virtual children_t& getchildren() {return children;}
	virtual const NodeVector& getchildren() const {return children;}
	children_t children;
};



} // namespace detail
} // namespace vgp

#endif /* NODE_INTERMEDIATE_HPP_ */