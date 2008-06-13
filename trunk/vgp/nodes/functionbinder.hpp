#if !BOOST_PP_IS_ITERATING

#ifndef FUNCTIONBINDER_HPP_
#define FUNCTIONBINDER_HPP_

#ifndef VGP_MAX_NODE_ARITY
#define VGP_MAX_NODE_ARITY 3
#endif

// Includes (when put here, they are included only once)
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/function.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>

namespace vgp {
namespace detail {
namespace mpl = ::boost::mpl;
namespace ft = ::boost::function_types;

// Fwd declare for wrapper
template <class, unsigned int> struct FunctionBinder_impl;

// Wrapper to extract function arity
template <class T> struct FunctionBinder :
	FunctionBinder_impl<T, ft::function_arity<T>::value> {};

// primary/default template (compile error if we try to instantiate this,
// usually if we try to use a function with arity > VGP_MAX_NODE_ARITY)
template <class T, unsigned int arity>
struct FunctionBinder_impl {
private:
	virtual ~FunctionBinder_impl() {}
	virtual void instatiable() const = 0;
};

} // namespace detail
} // namespace vgp

// specializations
#define BOOST_PP_ITERATION_LIMITS (0, VGP_MAX_NODE_ARITY)
#define BOOST_PP_FILENAME_1			<vgp/nodes/functionbinder.hpp>
#include BOOST_PP_ITERATE()

#endif // FUNCTIONBINDER_HPP_ include guard

#else // BOOST_PP_IS_ITERATING

#define n BOOST_PP_ITERATION()

// helper macro
#define VGP_FUNC_GET_PARAM(z, n, prefix) const NodeBase& BOOST_PP_CAT(prefix,n) = *itr++;
#define VGP_FUNC_BINDER_PARAM(z, n, prefix) 									\
	(boost::bind(BOOST_PP_CAT(prefix,n).template getfunc<						\
			typename mpl::at<parameter_types,mpl::int_< n > >::type>()))

// specialization pattern
namespace vgp {
namespace detail {

template <class T>
struct FunctionBinder_impl<T, n> {
	typedef typename ft::result_type<T>::type result_type;
	typedef typename ft::parameter_types<T>::type parameter_types;
	static boost::function<result_type()>
	getfunc(
		T function,
//		const typename T::state_type &state, 
		const NodeBase::ptr_vector &children) {
		NodeBase::ptr_vector::const_iterator itr = children.begin();
#ifdef __DEBUG__
		BOOST_ASSERT(children.size() == n);
#endif
		BOOST_PP_REPEAT(n, VGP_FUNC_GET_PARAM, node_)
		return boost::bind(
			function
//			state 
			BOOST_PP_COMMA_IF(n)
			BOOST_PP_ENUM(n, VGP_FUNC_BINDER_PARAM, node_)
		);
	}
private:
	void instantiable() const {}
};

} // namespace detail
} // namespace vgp

#undef VGP_FUNC_BINDER_PARAM
#undef n

#endif // BOOST_PP_IS_ITERATING
