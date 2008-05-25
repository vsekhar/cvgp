#ifndef NODEFACTORY_HPP_
#define NODEFACTORY_HPP_

#include <string>

#include <boost/static_assert.hpp>

#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

#include <boost/mpl/front.hpp>

#include <vgp/nodes/node.hpp>
#include <vgp/nodes/terminal.hpp>

/** \file
 * Factory functions to generate nodes from user-provided code
 */

namespace vgp {
namespace ft = ::boost::function_types;

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

template <typename FPTR>
detail::NodeBase* maketerminal(FPTR fptr, std::string name)
{
	return new detail::Terminal_simple<FPTR>(fptr, name);
}
#if 0
template <typename FPTR, typename MUTATEPTR>
detail::NodeBase* maketerminal_m(FPTR fptr, MUTATEPTR mutateptr, std::string name)
{
	return new detail::Terminal<FPTR, MUTATEPTR, NULL_PTR_T>(fptr, mutateptr, NULL, name);
}

template <typename FPTR, typename INITPTR>
detail::NodeBase* maketerminal_i(FPTR fptr, INITPTR initptr, std::string name)
{
	return new detail::Terminal<FPTR, NULL_PTR_T, INITPTR>(fptr, NULL, initptr, name);
}
#endif

} // end namespace vgp

#endif /*NODEFACTORY_HPP_*/
