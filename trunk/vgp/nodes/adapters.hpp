#ifndef ADAPTERS_HPP_
#define ADAPTERS_HPP_

#include <boost/mpl/insert.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <vgp/util/multifold.hpp>

#include <vgp/nodes/node.hpp>
#include <vgp/nodehelpers.hpp>

namespace vgp {
namespace detail {
namespace mpl = ::boost::mpl;

/** \brief Type conversion adapter
 * 
 * Adapters are just unary nodes that do nothing but return their param
 * in a different type. Boost::function takes care of the type conversion
 * (does it? aren't we just c-style casting? unsafe?)
 * TODO: modify to use boost::function conversion more directly
 * 
 * Adapters can be created manually:
 * 
 * 	\code
 * typedef mpl::vector<
 * 		vgp::vgp_adapter<double, int>,
 *	 	vgp::vgp_adapter<int, double>
 * 	> ADAPTERNODES;
 * \endcode
 * 
 * Adapters can also be generated from nodes:
 * 
 * 	\code
 * typedef vgp::generateadapters<
 * 		OUTPUTTYPES, INPUTTYPES
 * 	>::type ADAPTERNODES;
 * \endcode
 * 
 * WARNING: automatic generation DOES NOT work with nodes that take or
 * emit boost::function. These conversions must be manually generated when
 * needed (care should be taken to determine if such a conversion is needed)
 */

template <class IN, class OUT>
VGP_NODE(vgp_adapter, (OUT)(IN), VGP_NO_STATE, VGP_NOT_MUTATABLE)
VGP_NODE_CODE((OUT)(IN)) {return OUT(vgp_param0);}
VGP_NODE_END

/// Internal use
struct insert_if_convertible {
	template <class CONTAINER, class OUT, class IN>
	struct apply : mpl::if_<
		boost::is_convertible<OUT, IN>,
		mpl::push_back<CONTAINER, vgp_adapter<OUT, IN> >,
		mpl::identity<CONTAINER>
	>::type {};
	// Specialization to skip adapters that don't convert
	template <class CONTAINER, class T>
	struct apply<CONTAINER, T, T> : CONTAINER {};
};

/** \brief Generate adapters from two lists of types
 * 
 * An MPL function that generates adapter types from a list of input and output types
 * 
 * Usage:
 * \code
 * typedef vgp::getoutputtypes<NODES>::type OUTPUTTYPES;
 * typedef vgp::getparamtypes<NODES>::type PARAMTYPES;
 * typedef vgp::generateadapters<OUTPUTTYPES, PARAMTYPES>::type ADAPTERNODES;
 * \endcode
 */
template <class OUTPUTTYPES, class PARAMTYPES>
struct generateadapters : util::multifold2<
	OUTPUTTYPES,
	PARAMTYPES,
	mpl::vector<>,
	insert_if_convertible
> {};

} // namespace detail
using detail::vgp_adapter;
using detail::generateadapters;
} // namespace vgp

#endif /*ADAPTERS_HPP_*/
