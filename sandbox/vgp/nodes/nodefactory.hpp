#ifndef NODEFACTORY_HPP_
#define NODEFACTORY_HPP_

#include <string>

#include <boost/static_assert.hpp>

#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

#include <boost/mpl/front.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <vgp/nodes/node.hpp>
#include <vgp/nodes/terminal.hpp>

/** \file
 * Factory functions to generate nodes, terminals and adapters from user-provided code
 * 
 * Nodes carry functions that accept arguments and return a result. They carry
 * no node-specific internal state (though they may have global state in the form
 * of static variables defined in the user's function).
 * 
 * Terminals take no node inputs. They may have node-specific state (managed by vgp)
 * or global state (static variables within the user functions). They may also have
 * functions that initialize and mutate their node-specific state. 
 */

namespace vgp {
namespace ft = ::boost::function_types;
namespace mpl = ::boost::mpl;

/// Make a node (no internal state)
template <typename FPTR>
detail::NodeBase* makenode(FPTR fptr, std::string name)
{
	return new detail::Node<FPTR>(fptr, name);
}

/** Make a terminal, with the specified mutate and initialization functions
 * @pre All three function pointers must be unary and must accept a reference of the
 * same state type. State type must be default constructible.
 * @param functionptr unary function that accepts a reference to a state variable
 * @param mutateptr function called when node's internal state must be mutated
 * @param initptr function called before evolution to initialize node's internal state
 */
template <typename FPTR, typename MUTATEPTR, typename INITPTR>
detail::NodeBase* maketerminal(FPTR functionptr, MUTATEPTR mutateptr, INITPTR initptr, std::string name)
{
	return new detail::Terminal_mi<FPTR, MUTATEPTR, INITPTR>(functionptr, mutateptr, initptr, name);
}

/** Make a terminal with a mutate function only (no initialize function, only
 * state type's default constructor)
 */
template <typename FPTR, typename MUTATEPTR>
detail::NodeBase* maketerminal_m(FPTR fptr, MUTATEPTR mutateptr, std::string name)
{
	return new detail::Terminal_m<FPTR, MUTATEPTR>(fptr, mutateptr, name);
}

/** Make a terminal with an initialization function only (terminal's state is
 * not mutatable)
 */
template <typename FPTR, typename INITPTR>
detail::NodeBase* maketerminal_i(FPTR fptr, INITPTR initptr, std::string name)
{
	return new detail::Terminal_i<FPTR, INITPTR>(fptr, initptr, name);
}

/** Make a simple terminal with no state at all
 */
template <typename FPTR>
detail::NodeBase* maketerminal(FPTR fptr, std::string name)
{
	return new detail::Terminal_simple<FPTR>(fptr, name);
}

namespace detail {
template <typename SOURCE, typename TARGET>
TARGET adapter(SOURCE s) {return s;}
}// end namespace detail

/** Make an adapter that provides conversion such that the result of
 * function SOURCE can be provided as the INDEX'th argument of function
 * TARGET (INDEX counts from zero)
 * 
 * e.g.
 * @code
 * int targetfunction(char i, double d) {return i + d}
 * int sourcefunction() {return 7;}
 * @endcode
 * @code
 * makeadapter<0>(sourcefunction, targetfunction)
 * @endcode
 * creates an adapter that accepts int and returns char
 * @code
 * makeadapter<1>(sourcefunction, targetfunction)
 * @endcode
 * creates an adapter that accepts int and returns double
 */
template <unsigned int INDEX, typename FPTR_SOURCE, typename FPTR_TARGET>
detail::NodeBase* makeadapter(FPTR_SOURCE, FPTR_TARGET) {
	typedef typename ft::result_type<FPTR_SOURCE>::type source_type;
	typedef typename ft::parameter_types<FPTR_TARGET>::type target_types;
	typedef typename mpl::at<target_types, mpl::int_<INDEX> >::type target_type;
	BOOST_STATIC_ASSERT((mpl::not_<boost::is_same<source_type, target_type> >::value));
	return makenode(detail::adapter<source_type, target_type>, "adapter");
}

/** Makes an adapter directly using SOURCE and TARGET
 * e.g.
 * @code
 * makeadapter<int, double>()	// adapter: int to double
 * makeadapter<char, int>()		// adapter: char to int
 * @endcode
 */ 
template <typename SOURCE, typename TARGET>
detail::NodeBase* makeadapter() {
	return makenode(detail::adapter<SOURCE, TARGET>, "adapter");
}

} // end namespace vgp

#endif /*NODEFACTORY_HPP_*/
