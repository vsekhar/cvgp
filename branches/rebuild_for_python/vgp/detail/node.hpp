/*
 * node.hpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#ifndef NODE_HPP_
#define NODE_HPP_

#include <string>

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/front.hpp>

#include <vgp/detail/nodebase.hpp>
#include <vgp/detail/nodeentry.hpp>

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

// Adds pointer, construct code, and failure to get children
struct Node_w_ptr : NodeBase {
	template <class FPTR>
	Node_w_ptr(const FPTR p) : fptr(reinterpret_cast<void_fptr_t>(p)) {}
	virtual void init() {}
	virtual void mutate() {}
	virtual children_t& getchildren() {BOOST_ASSERT(0);}
	void_fptr_t const fptr;
};

template <typename result_type>
struct Node_returning : Node_w_ptr {
	Node_returning(void_fptr_t p) : Node_w_ptr(p) {}
	virtual result_type run_node() const = 0;
};

// Adds children handling
template <typename FPTR>
struct Node_w_children : Node_returning<typename ft::result_type<FPTR>::type> {
	Node_w_children(const FPTR p)
		: Node_returning<typename ft::result_type<FPTR>::type>(p) {}
	virtual children_t& getchildren() {return children;}
	children_t children;
};

template <class, unsigned int> struct Node_concrete_n;

template <class FPTR>
struct Node_concrete_n<FPTR, 1>	: Node_w_children<FPTR> {
	typedef Node_w_children<FPTR> base;
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	Node_concrete_n(const FPTR p) : Node_w_children<FPTR>(p) {}
	Node_concrete_n(const void_fptr_t& p) : Node_w_children<FPTR>(p) {}
	virtual result_type run_node() const {
		FPTR f = reinterpret_cast<FPTR>(base::fptr);
		typedef typename mpl::at<parameter_types,mpl::int_< 0 > >::type arg0;
		Node_returning<arg0>* child0
			= reinterpret_cast<Node_returning<arg0> >(&base::children[0]);
		return f(child0->run_node());
	}
};

template <class FPTR>
struct Node_concrete_n<FPTR, 2> : Node_w_children<FPTR> {
	typedef Node_w_children<FPTR> base;
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	Node_concrete_n(const FPTR p) : Node_w_children<FPTR>(p) {}
	Node_concrete_n(const void_fptr_t& p) : Node_w_children<FPTR>(p) {}
	virtual result_type run_node() const {
		FPTR f = reinterpret_cast<FPTR>(base::fptr);
		typedef typename mpl::at<parameter_types,mpl::int_< 0 > >::type arg0;
		typedef typename mpl::at<parameter_types,mpl::int_< 1 > >::type arg1;
		Node_returning<arg0>* child0
			= reinterpret_cast<Node_returning<arg0> >(&base::children[0]);
		Node_returning<arg1>* child1
			= reinterpret_cast<Node_returning<arg1> >(&base::children[1]);
		return f(child0->run_node(), child1->run_node());
	}
};

template <class FPTR>
struct Node : Node_concrete_n<FPTR, ft::function_arity<FPTR>::value> {
	Node(const FPTR p) : Node_concrete_n<FPTR, ft::function_arity<FPTR>::value>(p) {}
	Node(const Node<FPTR>& n) : Node_concrete_n<FPTR, ft::function_arity<FPTR>::value>(n.fptr) {}
	virtual NodeBase* clone() const {return new Node<FPTR>(*this);}
};

template <class FPTR>
struct Terminal : Node_returning<typename ft::result_type<FPTR>::type> {
	typedef Node_returning<typename ft::result_type<FPTR>::type> base;
	typedef typename ft::result_type<FPTR>::type result_type;
	Terminal(const FPTR p) : base(p) {}
	Terminal(const Terminal& t) : base(t.fptr) {}
	virtual result_type run_node() const {
		return reinterpret_cast<FPTR>(base::fptr)();
	}
	virtual NodeBase* clone() const {return new Terminal<FPTR>(*this);}
};

template <class FPTR>
struct Terminal_w_state : Node_returning<typename ft::result_type<FPTR>::type> {
	typedef Node_returning<typename ft::result_type<FPTR>::type> base;
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename fptr_to_state_type<FPTR>::type state_type;
	typedef void (*im_ptr)(state_type&);
	Terminal_w_state(const FPTR p, const state_type& s, im_ptr i, im_ptr m)
		: base(p), state(s), iptr(i), mptr(m) {}
	Terminal_w_state(const FPTR p, im_ptr i, im_ptr m)
		: base(p), iptr(i), mptr(m) {}
	Terminal_w_state(const Terminal_w_state& t)
		: base(t.fptr), state(t.state), iptr(t.iptr), mptr(t.mptr) {}
	virtual void init() {if(iptr) iptr(state);}
	virtual void mutate() {if(mptr) mptr(state);}
	virtual result_type run_node() const {
		return reinterpret_cast<FPTR>(base::fptr)(state);
	}
	virtual NodeBase* clone() const {return new Terminal_w_state<FPTR>(*this);}
	state_type state;
	im_ptr iptr;
	im_ptr mptr;
};

} // namespace detail

// during testing (actual code should refer properly to namespace detail
using detail::NodeBase;
using detail::children_t;

} // namespace vgp

#endif /* NODE_HPP_ */
