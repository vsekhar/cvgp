/*
 * fptr.hpp
 *
 *  Created on: 2010-10-08
 *
 */

#ifndef FPTR_HPP_
#define FPTR_HPP_

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/front.hpp>

namespace vgp {
namespace detail {

namespace mpl = ::boost::mpl;
namespace ft = ::boost::function_types;

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

} // end namespace detail
} // end namespace vgp


#endif
