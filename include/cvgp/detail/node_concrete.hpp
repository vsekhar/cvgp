/*
 * node_concrete.hpp
 *
 *  Created on: 2010-01-27
 *
 *  Derives from node_intermediate.hpp to add actual concrete implementations.
 *
 *  This header is used by code that needs the ability to create concrete nodes.
 */

#ifndef NODE_CONCRETE_HPP_
#define NODE_CONCRETE_HPP_

#include <string>
#include <stdexcept>

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>

#include <cvgp/detail/node_intermediate.hpp>

namespace vgp {

namespace mpl = ::boost::mpl;
namespace ft = ::boost::function_types;

namespace detail {

template <class FPTR>
struct Terminal : Node_returning<typename ft::result_type<FPTR>::type> {

	typedef Node_returning<typename ft::result_type<FPTR>::type> base;

	Terminal(const FPTR p) : base(reinterpret_cast<void_fptr_t>(p)) {}

	virtual typename base::result_type run_node() const {
		return reinterpret_cast<FPTR>(base::fptr)();
	}
	virtual NodeBase* clone() const {return new Terminal<FPTR>(*this);}
};

// state_type must be copy-constructible or default-constructible
template <class FPTR>
struct Terminal_w_state : Node_returning<typename ft::result_type<FPTR>::type> {

	typedef Node_returning<typename ft::result_type<FPTR>::type> base;
	typedef typename fptr_to_state_type<FPTR>::type state_type;
	typedef void (*im_ptr)(state_type&);

	Terminal_w_state(const FPTR p, const state_type& s, im_ptr i, im_ptr m)
		: base(reinterpret_cast<void_fptr_t>(p)), state(s), iptr(i), mptr(m) {}
	Terminal_w_state(const FPTR p, im_ptr i, im_ptr m)
		: base(reinterpret_cast<void_fptr_t>(p)), state(), iptr(i), mptr(m) {}
	Terminal_w_state(const FPTR p)
		: base(reinterpret_cast<void_fptr_t>(p)), state(), iptr(0), mptr(0) {}

	virtual void init() {if(iptr) iptr(state);}
	virtual void mutate() {if(mptr) mptr(state);}
	virtual bool mutatable() const {return mptr!=0;}
	virtual typename base::result_type run_node() const {
		return reinterpret_cast<FPTR>(base::fptr)(state);
	}
	virtual NodeBase* clone() const {return new Terminal_w_state<FPTR>(*this);}

	state_type state;
	im_ptr iptr;
	im_ptr mptr;
};

template <class, unsigned int> struct Node_concrete_n;

template <class FPTR>
struct Node_concrete_n<FPTR, 1>	: Node_w_children<FPTR> {

	typedef Node_w_children<FPTR> base;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;

	Node_concrete_n(const FPTR p) : Node_w_children<FPTR>(p) {}

	virtual typename base::result_type run_node() const {
		FPTR f = reinterpret_cast<FPTR>(base::fptr);
		typedef typename mpl::at<parameter_types,mpl::int_< 0 > >::type arg0;
		const Node_returning<arg0>* child0
			= static_cast<const Node_returning<arg0>*>(&base::children[0]);
		return f(child0->run_node());
	}
};

template <class FPTR>
struct Node_concrete_n<FPTR, 2> : Node_w_children<FPTR> {

	typedef Node_w_children<FPTR> base;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;

	Node_concrete_n(const FPTR p) : Node_w_children<FPTR>(p) {}

	virtual typename base::result_type run_node() const {
		FPTR f = reinterpret_cast<FPTR>(base::fptr);
		typedef typename mpl::at<parameter_types,mpl::int_< 0 > >::type arg0;
		typedef typename mpl::at<parameter_types,mpl::int_< 1 > >::type arg1;
		const Node_returning<arg0>* child0
			= static_cast<const Node_returning<arg0>*>(&base::children[0]);
		const Node_returning<arg1>* child1
			= static_cast<const Node_returning<arg1>*>(&base::children[1]);
		return f(child0->run_node(), child1->run_node());
	}
};

template <class FPTR>
struct Node : Node_concrete_n<FPTR, ft::function_arity<FPTR>::value> {
	Node(const FPTR p) : Node_concrete_n<FPTR, ft::function_arity<FPTR>::value>(p) {}

	virtual NodeBase* clone() const {return new Node<FPTR>(*this);}
};

} // namespace detail
} // namespace vgp

#endif /* NODE_CONCRETE_HPP_ */
