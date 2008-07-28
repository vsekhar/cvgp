#ifndef VGP_NODEHELPERS_HPP_
#define VGP_NODEHELPERS_HPP_

/** \file
 * Macros to aid writing code for nodes
 *
 * Basic usage:
 * \code
 * VGP_NODE(<nodename>, (<return_type>)(<param1_type>)..., <state_type>, <mutatable_flag>)
 * VGP_NODE_CODE((<return type>)(<param1_type>)...) { // code with return statement }
 * VGP_NODE_END
 * \endcode
 * \arg \c return_type the data type returned by the node
 * \arg \c param1_type zero or more parameter types, each in its own parentheses
 * \arg \c state_type the type that stores the node's state
 * \arg \c mutatable_flag indicates whether a node is internally mutatable (if true,
 * a mutate function must be provided using VGP_MUTATE in addition to the node's own code)
 *
 * Examples:
 * \code
 * VGP_NODE(zero, (int), VGP_NO_STATE, VGP_NOT_MUTATABLE)
 * VGP_NODE_CODE((int)) {return 0;}
 * VGP_NODE_END
 * \endcode
 * \code
 * VGP_NODE(double_01, (double), double, VGP_MUTATABLE)
 * VGP_NODE_CODE((double)) {return vgp_state; }
 * VGP_MUTATE{vgp_state = getnewnumber();}
 * VGP_NODE_END
 * \endcode
 *
 * Nodes can also be templated:
 * \code
 * template <class T>
 * VGP_NODE(add, (T)(T)(T), VGP_NO_STATE, VGP_NOT_MUTATABLE)
 * VGP_NODE_CODE((T)(T)(T)) {return vgp_param0 + vgp_param1;}
 * VGP_NODE_END
 * typedef mpl::vector<add<int>, add<double>, add<std::string> > arithmetic;
 * \endcode
 *
 * Nodes can access sourcedata using the VGP_SOURCE_DATA macros. Nodes need to know
 * the name and type of the sourcedata they access:
 * \code
 * VGP_NODE(sourcedata_demo, (int), VGP_NO_STATE, VGP_NOT_MUTATABLE)
 * VGP_NODE_CODE((int)) {return 4 * VGP_SOURCE_DATA(demodata, int);}
 * VGP_END_NODE
 * \endcode
 *
 * The user must ensure the appropriate data is loaded/initialized using:
 * \code
 * vgp::addsourcedata(<name_string>, <value>);
 * \endcode
 *
 * Nodes that simply return a specific kind of sourcedata can be created in shortform:
 * \code
 * VGP_SOURCE_DATA_NODE(<name>, <type>)
 * \endcode
 * This macro will create a node with the provided name that accesses sourcedata of the same
 * name and outputs it. Note that using this node causes an extra copy of the sourcedata.
 *
 * Coding nodes manually:
 * Nodes need not use the macros provided above. The macros expand to a node declaration
 * similar to the following:
 * \code
 * struct example_node {
 * 	typedef int result_type;
 * 	typedef ::boost::mpl::vector<int, double> param_types;
 * 	typedef int state_type;
 * 	static const unsigned int arity = 2;
 * 	static const bool mutatable = false;
 *
 * 	static ::std::string name() {return "example node name";}
 * 	static int function(int, double);
 * 	static void mutate(state_type &);
 * };
 * \endcode
 *
 */

#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/comparison.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/size.hpp>
#include <boost/any.hpp>

#include <boost/function.hpp>

namespace mpl = ::boost::mpl;

#define VGP_NO_STATE ::boost::mpl::empty_base
#define VGP_NOT_MUTATABLE 0
#define VGP_MUTATABLE 1
#define VGP_MUTATE static void mutate(state_type &vgp_state)
#define VGP_NODE_END };

// for creating nodes that themselves return (non-nullary) functions
// (which are then wrapped inside VGP's function mechanism)
// TODO: generate these? using the same arity macro as FunctionBinder
#define VGP_FUNC0(ret) boost::function<ret()>
#define VGP_FUNC1(ret, p1) boost::function<ret(p1)>
#define VGP_FUNC2(ret, p1, p2) boost::function<ret(p1, p2)>
#define VGP_FUNC3(ret, p1, p2, p3) boost::function<ret(p1, p2, p3)>

// used to clean up template types
// e.g. VGP_DECOMMA(1, ( std::pair<int, int> ) )
//  (parentheses around 2nd argument are required)
#define VGP_DECOMMA(n, x) BOOST_PP_TUPLE_REM(BOOST_PP_ADD(n,1))x

// need this for sequence handling below
#define BOOST_PP_EMPTY_1(x)

///////////////////////////////////
// Node struct declaration
///////////////////////////////////
#define VGP_SEQ_TAIL_ENUM(T) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(T))

#define VGP_NODE(nodename, types, stype, m)									\
	struct nodename {														\
		typedef BOOST_PP_SEQ_HEAD(types) result_type;						\
		typedef ::boost::mpl::vector<										\
			BOOST_PP_IF(BOOST_PP_LESS(BOOST_PP_SEQ_SIZE(types), 2),			\
			BOOST_PP_EMPTY_1,												\
			VGP_SEQ_TAIL_ENUM)(types)										\
		> param_types;														\
		typedef stype state_type;											\
		static ::std::string name()											\
		{ return BOOST_PP_STRINGIZE(nodename); }							\
		static const unsigned int arity =									\
			::boost::mpl::size<param_types>::value;							\
		static const bool mutatable = m;									\
		BOOST_PP_IF(m, BOOST_PP_EMPTY(), VGP_MUTATE {})


///////////////////////////////////
// Node function declaration
///////////////////////////////////
#define VGP_PARAM_DECL(r, ar, n, t)											\
	const t BOOST_PP_CAT(vgp_param, n) BOOST_PP_COMMA_IF(BOOST_PP_LESS(n, BOOST_PP_SUB(ar, 1)))

#define VGP_NODE_FUNCTION_INPUTS(intypes)									\
	BOOST_PP_SEQ_FOR_EACH_I(												\
		VGP_PARAM_DECL,														\
		BOOST_PP_SEQ_SIZE(intypes),											\
		intypes																\
	)

#define VGP_NODE_FUNCTION_ARGS(types)										\
	const state_type &vgp_state												\
	BOOST_PP_COMMA_IF(BOOST_PP_GREATER(BOOST_PP_SEQ_SIZE(types),1))			\
	BOOST_PP_IF(															\
		BOOST_PP_LESS(BOOST_PP_SEQ_SIZE(types),2),							\
		BOOST_PP_EMPTY_1,													\
		VGP_NODE_FUNCTION_INPUTS											\
	)(BOOST_PP_SEQ_TAIL(types))

#define VGP_NODE_CODE(types)												\
	static BOOST_PP_SEQ_HEAD(types) function(VGP_NODE_FUNCTION_ARGS(types))

#define VGP_NODE_CODE_MODULE(name, types)									\
	BOOST_PP_SEQ_HEAD(types) name::function(VGP_NODE_FUNCTION_ARGS(types))

#define VGP_SOURCE_DATA(name, type)											\
	(boost::any_cast<type>((vgp::getsourcedata(BOOST_PP_STRINGIZE(name)))->second))
#define VGP_SOURCE_DATA_NODE(name, type)									\
	VGP_NODE(name, (type), VGP_NO_STATE, VGP_NOT_MUTATABLE)					\
	VGP_NODE_CODE((type)) {return VGP_SOURCE_DATA(name, type);}				\
	VGP_NODE_END

#endif /*VGP_NODEHELPERS_HPP_*/
