#ifndef TERMINAL_HPP_
#define TERMINAL_HPP_

#include <boost/static_assert.hpp>

#include <boost/mpl/front.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/bind.hpp>
#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>

#include <vgp/util/typestoobjs.hpp>
#include <vgp/util/typeinfo.hpp>

#include <vgp/nodes/nodebase.hpp>
#include <vgp/nodes/nodecontainer.hpp>
#include <vgp/nodes/functionbinder.hpp>


namespace vgp {
namespace detail {

template <typename T>
struct getfirstparam_wo_reference :
	boost::remove_reference<
		typename mpl::front<
			typename ft::parameter_types<T>::type
		>::type
	>
{};

template <typename FPTR, typename MUTATEPTR, typename INITPTR>
struct Terminal_mi : NodeBase
{
	BOOST_STATIC_ASSERT(ft::is_function_pointer<FPTR>::value);
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	typedef typename getfirstparam_wo_reference<FPTR>::type state_type;

	static const unsigned int arity = ft::function_arity<FPTR>::value;
	BOOST_STATIC_ASSERT((arity == 1));
	BOOST_STATIC_ASSERT((ft::function_arity<MUTATEPTR>::value == 1));
	BOOST_STATIC_ASSERT((ft::function_arity<INITPTR>::value == 1));
	
	typedef typename getfirstparam_wo_reference<MUTATEPTR>::type mutate_type;
	typedef typename getfirstparam_wo_reference<INITPTR>::type init_type;
	BOOST_STATIC_ASSERT((boost::is_same<mutate_type, state_type>::value));
	BOOST_STATIC_ASSERT((boost::is_same<init_type, state_type>::value));
	
	Terminal_mi(FPTR fptr, MUTATEPTR mutateptr, INITPTR initptr, std::string name) :
		NodeBase(name, util::TypeInfo(typeid(result_type)), arity),
		function(fptr), mutatefunction(mutateptr), initfunction(initptr) {
		boundfunction = boost::function<result_type()>(boost::bind(fptr, boost::ref(state)));
	}
	
	Terminal_mi(const Terminal_mi& t) :
		NodeBase(t), function(t.function), boundfunction(t.boundfunction),
		mutatefunction(t.mutatefunction), initfunction(t.initfunction), state(t.state) {
		boundfunction = boost::function<result_type()>(boost::bind(function, boost::ref(state)));
	}
	
	NodeBase* clone() const {
		return new Terminal_mi<FPTR, MUTATEPTR, INITPTR>(*this);
	}
	
	boost::any inline getfunc() const {return boundfunction;}
	bool inline ismutatable() const {return true;}
	bool inline isinitiable() const {return true;}
	void inline mutate() {mutatefunction(state);}
	void inline init() {initfunction(state);}

	FPTR function;
	boost::any boundfunction;
	MUTATEPTR mutatefunction;
	INITPTR initfunction;
	state_type state;
};

template <typename FPTR, typename MUTATEPTR>
struct Terminal_m : NodeBase
{
	BOOST_STATIC_ASSERT(ft::is_function_pointer<FPTR>::value);
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	typedef typename getfirstparam_wo_reference<FPTR>::type state_type;

	static const unsigned int arity = ft::function_arity<FPTR>::value;
	BOOST_STATIC_ASSERT((arity == 1));
	BOOST_STATIC_ASSERT((ft::function_arity<MUTATEPTR>::value == 1));
	
	typedef typename getfirstparam_wo_reference<MUTATEPTR>::type mutate_type;
	BOOST_STATIC_ASSERT((boost::is_same<mutate_type, state_type>::value));
	
	Terminal_m(FPTR fptr, MUTATEPTR mutateptr, std::string name) :
		NodeBase(name, util::TypeInfo(typeid(result_type)), arity),
		function(fptr), mutatefunction(mutateptr) {
		boundfunction = boost::function<result_type()>(boost::bind(fptr, boost::ref(state)));
	}
	
	Terminal_m(const Terminal_m& t) :
		NodeBase(t), function(t.function), boundfunction(t.boundfunction),
		mutatefunction(t.mutatefunction), state(t.state) {
		boundfunction = boost::function<result_type()>(boost::bind(function, boost::ref(state)));
	}
	
	NodeBase* clone() const {
		return new Terminal_m<FPTR, MUTATEPTR>(*this);
	}
	
	boost::any inline getfunc() const {return boundfunction;}
	bool inline ismutatable() const {return true;}
	bool inline isinitiable() const {return false;}
	void inline mutate() {mutatefunction(state);}
	void inline init() {}

	FPTR function;
	boost::any boundfunction;
	MUTATEPTR mutatefunction;
	state_type state;
};

template <typename FPTR, typename INITPTR>
struct Terminal_i : NodeBase
{
	BOOST_STATIC_ASSERT(ft::is_function_pointer<FPTR>::value);
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	typedef typename getfirstparam_wo_reference<FPTR>::type state_type;

	static const unsigned int arity = ft::function_arity<FPTR>::value;
	BOOST_STATIC_ASSERT((arity == 1));
	BOOST_STATIC_ASSERT((ft::function_arity<INITPTR>::value == 1));
	
	typedef typename getfirstparam_wo_reference<INITPTR>::type init_type;
	BOOST_STATIC_ASSERT((boost::is_same<init_type, state_type>::value));
	
	Terminal_i(FPTR fptr, INITPTR initptr, std::string name) :
		NodeBase(name, util::TypeInfo(typeid(result_type)), arity),
		function(fptr), initfunction(initptr) {
		boundfunction = boost::function<result_type()>(boost::bind(fptr, boost::ref(state)));
	}
	
	Terminal_i(const Terminal_i& t) :
		NodeBase(t), function(t.function), boundfunction(t.boundfunction),
		initfunction(t.initfunction), state(t.state) {
		boundfunction = boost::function<result_type()>(boost::bind(function, boost::ref(state)));
	}
	
	NodeBase* clone() const {
		return new Terminal_i<FPTR, INITPTR>(*this);
	}
	
	boost::any inline getfunc() const {return boundfunction;}
	bool inline ismutatable() const {return false;}
	bool inline isinitiable() const {return true;}
	void inline mutate() {
		std::cerr << "ERROR: tried to mutate a non-mutatable terminal (no-op)" << std::endl;
	}
	void inline init() {initfunction(state);}

	FPTR function;
	boost::any boundfunction;
	INITPTR initfunction;
	state_type state;
};

template <typename FPTR>
struct Terminal_simple : NodeBase
{
	BOOST_STATIC_ASSERT(ft::is_function_pointer<FPTR>::value);
	typedef typename ft::result_type<FPTR>::type result_type;
	BOOST_STATIC_ASSERT((ft::function_arity<FPTR>::value == 0));
	static const unsigned int arity = ft::function_arity<FPTR>::value;
	
	Terminal_simple(FPTR fptr, std::string name) :
		NodeBase(name, util::TypeInfo(typeid(result_type)), arity),
		function(fptr) {}
	
	Terminal_simple(const Terminal_simple& t) :	NodeBase(t), function(t.function) {}
	
	NodeBase* clone() const {
		return new Terminal_simple<FPTR>(*this);
	}
	
	boost::any getfunc() const {return boost::function<result_type()>(boost::bind(function));}
	bool inline ismutatable() const {return false;}
	bool inline isinitiable() const {return false;}
	void inline mutate() {
		std::cerr << "ERROR: tried to mutate a non-mutatable terminal (no-op)" << std::endl;
	}
	void inline init() {}

	FPTR function;
};

} // end namespace detail
} // end namespace vgp


#endif /*TERMINAL_HPP_*/
