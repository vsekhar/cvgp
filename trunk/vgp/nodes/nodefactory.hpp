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

#include <vgp/nodes/node.hpp>
#include <vgp/nodes/terminal.hpp>

/** \file
 * Factory functions to generate nodes from user-provided code
 */

namespace vgp {
namespace ft = ::boost::function_types;
namespace mpl = ::boost::mpl;

template <typename FPTR>
detail::NodeBase* makenode(FPTR fptr, std::string name)
{
	return new detail::Node<FPTR>(fptr, name);
}

// Terminals have state and mutate
template <typename FPTR, typename MUTATEPTR, typename INITPTR>
detail::NodeBase* maketerminal(FPTR fptr, MUTATEPTR mutateptr, INITPTR initptr, std::string name)
{
	return new detail::Terminal_mi<FPTR, MUTATEPTR, INITPTR>(fptr, mutateptr, initptr, name);
}

template <typename FPTR, typename MUTATEPTR>
detail::NodeBase* maketerminal_m(FPTR fptr, MUTATEPTR mutateptr, std::string name)
{
	return new detail::Terminal_m<FPTR, MUTATEPTR>(fptr, mutateptr, name);
}

template <typename FPTR, typename INITPTR>
detail::NodeBase* maketerminal_i(FPTR fptr, INITPTR initptr, std::string name)
{
	return new detail::Terminal_i<FPTR, INITPTR>(fptr, initptr, name);
}

template <typename FPTR>
detail::NodeBase* maketerminal(FPTR fptr, std::string name)
{
	return new detail::Terminal_simple<FPTR>(fptr, name);
}

namespace detail {
template <typename SOURCE, typename TARGET>
TARGET adapter(SOURCE s) {return s;}
} // end namespace detail

template <typename FPTR_SOURCE, typename FPTR_TARGET, typename PARAM_INDEX>
detail::NodeBase* makeadapter(FPTR_SOURCE fptrsrc, FPTR_TARGET fptrtarget) {
	typedef typename ft::result_type<FPTR_SOURCE>::type source_type;
	typedef typename ft::parameter_types<FPTR_TARGET>::type target_types;
	typedef typename mpl::at<target_types, PARAM_INDEX>::type target_type;
	return new detail::Node<target_type(*)(source_type)>
		(detail::adapter<source_type, target_type>);
}

} // end namespace vgp

#endif /*NODEFACTORY_HPP_*/
