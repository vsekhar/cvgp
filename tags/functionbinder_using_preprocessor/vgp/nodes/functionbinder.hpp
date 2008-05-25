#if !BOOST_PP_IS_ITERATING

#ifndef FUNCTIONBINDER_HPP_
#define FUNCTIONBINDER_HPP_

#ifndef VGP_MAX_NODE_ARITY
#define VGP_MAX_NODE_ARITY 4
#endif

// Includes (when put here, they are included only once)
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/assert.hpp>

// primary/default template (compile error if we try to instantiate this)
namespace vgp {
namespace detail {
namespace mpl = ::boost::mpl;

template <class T, class RESULT_TYPE, unsigned int arity>
struct FunctionBinder {
private:
	virtual ~FunctionBinder() {}
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
			typename mpl::at<typename T::param_types,mpl::int_< n > >::type>()))

// specialization pattern
namespace vgp {
namespace detail {

template <class T, class RESULT_TYPE>
struct FunctionBinder<T, RESULT_TYPE, n> {
	static boost::function<RESULT_TYPE()>
	getfunc(
		const typename T::state_type &state, 
		const NodeBase::ptr_vector &children) {
		NodeBase::ptr_vector::const_iterator itr = children.begin();
#ifdef __DEBUG__
		BOOST_ASSERT(children.size() > n);
#endif
		BOOST_PP_REPEAT(n, VGP_FUNC_GET_PARAM, node_)
		typedef RESULT_TYPE result_type;
		return boost::bind(
			T::function,
			state 
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
