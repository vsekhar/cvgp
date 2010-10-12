/*
 * gil_handling.hpp
 *
 *  Created on: 2010-01-31
 */

#ifndef GIL_HANDLING_HPP_
#define GIL_HANDLING_HPP_

#include <Python.h>	// must be first
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>

namespace vgp {
namespace python {

struct ScopeGIL
{
    inline ScopeGIL() : gstate(PyEval_SaveThread()) {}

    inline ~ScopeGIL() {
    	PyEval_RestoreThread(gstate);
	gstate = NULL;
    }
    PyThreadState* gstate;

};

template <class FuncType_>
struct GILFuncWrapper;

// You can code-generate specializations for other arities...

template <class R_>
struct GILFuncWrapper<R_ ()>
{
    typedef R_ (*func_type)();

    func_type m_wrapped_func;

    GILFuncWrapper(func_type p_wrapped_func) : m_wrapped_func(p_wrapped_func) {}

    R_ operator()() {
        ScopeGIL gil_guard;
        return this->m_wrapped_func();
    }
};

template <class R_, class A0_>
struct GILFuncWrapper<R_ (A0_)>
{
    typedef R_ (*func_type)(A0_);
    typedef typename boost::add_const<typename boost::add_reference<A0_>::type>::type AC0_;

    func_type m_wrapped_func;

    GILFuncWrapper(func_type p_wrapped_func) : m_wrapped_func(p_wrapped_func) {}

    R_ operator()(AC0_ p0) {
        ScopeGIL gil_guard;
        return this->m_wrapped_func(p0);
    }
};

template <class R_, class A0_, class A1_>
struct GILFuncWrapper<R_ (A0_, A1_)>
{
	typedef R_ (*func_type)(A0_, A1_);
	typedef typename boost::add_const<typename boost::add_reference<A0_>::type>::type AC0_;
	typedef typename boost::add_const<typename boost::add_reference<A1_>::type>::type AC1_;

	func_type m_wrapped_func;

	GILFuncWrapper(func_type p_wrapped_func) : m_wrapped_func(p_wrapped_func) {}

	R_ operator()(AC0_ p0, AC1_ p1) {
		ScopeGIL gil_guard;
		return this->m_wrapped_func(p0, p1);
	}
};

template <
	class R_,
	class C_,
	class A0_=void,
	class A1_=void,
	class A2_=void
	// ...
>
struct GILMemberFuncWrapper;

template <class R_, class C_, class A0_>
struct GILMemberFuncWrapper<R_, C_, A0_>
{
	typedef R_ (C_::*member_func_type)(A0_);
	typedef typename boost::add_const<typename boost::add_reference<A0_>::type>::type AC0_;

	member_func_type m_wrapped_method;

	GILMemberFuncWrapper(member_func_type p_wrapped_method) : m_wrapped_method(p_wrapped_method) {}

	R_ operator()(C_* p_self, AC0_ p0) {
		ScopeGIL gil_guard;
		return (p_self->*(this->m_wrapped_method))(p0);
	}
};



template <class FuncPtr_>
void GIL_wrapped(FuncPtr_);

// You can code-generate specialisations for other arities...

template <class R_>
GILFuncWrapper<R_ ()> GIL_wrapped(R_ (*p_wrapped_func)()) {
	return GILFuncWrapper<R_ ()>(p_wrapped_func);
}

template <class R_, class A0_>
GILFuncWrapper<R_ (A0_)> GIL_wrapped(R_ (*p_wrapped_func)(A0_)) {
	return GILFuncWrapper<R_ (A0_)>(p_wrapped_func);
}

template <class R_, class A0_, class A1_>
GILFuncWrapper<R_ (A0_, A1_)> GIL_wrapped(R_ (*p_wrapped_func)(A0_, A1_)) {
	return GILFuncWrapper<R_ (A0_, A1_)>(p_wrapped_func);
}

template <class R_, class C_, class A0_>
GILMemberFuncWrapper<R_, C_, A0_> GIL_wrapped(R_ (C_::*p_wrapped_method)(A0_)) {
	return GILMemberFuncWrapper<R_, C_, A0_>(p_wrapped_method);
}

template <class R_, class C_, class A0_, class A1_>
GILMemberFuncWrapper<R_, C_, A0_, A1_> GIL_wrapped(R_ (C_::*p_wrapped_method)(A0_, A1_)) {
	return GILMemberFuncWrapper<R_, C_, A0_, A1_>(p_wrapped_method);
}

} // namespace python
} // namespace vgp

// insert into boost namespace
namespace boost { namespace python { namespace detail {
	// You can code-generate specializations for other arities...

	template <class R_>
	inline boost::mpl::vector<R_>
	get_signature(::vgp::python::GILFuncWrapper<R_ ()>, void* = 0) {
		return boost::mpl::vector<R_>();
	}

	template <class R_, class P0_>
	inline boost::mpl::vector<R_, P0_>
	get_signature(::vgp::python::GILFuncWrapper<R_ (P0_)>, void* = 0) {
		return boost::mpl::vector<R_, P0_>();
	}

	template <class R_, class P0_, class P1_>
	inline boost::mpl::vector<R_, P0_, P1_>
	get_signature(::vgp::python::GILFuncWrapper<R_ (P0_, P1_)>, void* = 0) {
	return boost::mpl::vector<R_, P0_, P1_>();
	}

	template <class R_, class C_, class P0_>
	inline boost::mpl::vector<R_, C_*, P0_>
	get_signature(::vgp::python::GILMemberFuncWrapper<R_, C_, P0_>, void* = 0) {
	return boost::mpl::vector<R_, C_*, P0_>();
	}

	template <class R_, class C_, class P0_, class P1_>
	inline boost::mpl::vector<R_, C_*, P0_, P1_>
	get_signature(::vgp::python::GILMemberFuncWrapper<R_, C_, P0_, P1_>, void* = 0) {
	return boost::mpl::vector<R_, C_*, P0_, P1_>();
	}

} } } // namespaces boost::python::detail

#endif /* GIL_HANDLING_HPP_ */
