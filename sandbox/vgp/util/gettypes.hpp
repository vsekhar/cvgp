#ifndef GETTYPES_HPP_
#define GETTYPES_HPP_

#include <boost/mpl/set.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/clear.hpp>
#include <boost/mpl/fold.hpp>

namespace vgp {
namespace detail {
namespace mpl = ::boost::mpl;

struct outputtype_inserter {
	template <class CONTAINER, class NODE>
	struct apply : mpl::insert<CONTAINER, typename NODE::result_type> {};
};

struct paramtypes_inserter {
	template <class CONTAINER, class NODE>
	struct apply : mpl::fold<
		typename NODE::param_types,
		CONTAINER,
		mpl::insert<mpl::_1, mpl::_2>
	> {};
};

} // namespace detail


// We use here a funny way of creating an empty set:
// 	mpl::clear<mpl::set<int> >::type
// Not sure why mpl::set<> doesn't work (mpl::vector<> does)

/** \brief Collects all the result_type typedefs in the types supplied
 * 
 * Usage:
 * \code
 * typedef getoutputtypes<NODES>::type OUTPUTTYPES;
 * \endcode
 * 
 * \pre NODES is an MPL container of types, each with a statement of the kind:
 * \code
 * typedef <unspecified type> result_type;
 * \endcode
 * \post OUTPUTTYPES is an MPL container containing all the result_types in NODES 
 */
template <class NODES>
struct getoutputtypes : public mpl::fold<
	NODES,
	mpl::clear<mpl::set<int> >::type,
	detail::outputtype_inserter
> {};

/** \brief Collects all types in the param_types container typedefs in the types supplied
 * 
 * Usage:
 * \code
 * typedef getparamtypes<NODES>::type PARAMTYPES;
 * \endcode
 * 
 * \pre NODES is an MPL container of types, each with an MPL container of the kind:
 * \code
 * typedef mpl::vector< <type1>, <type2>, ... > param_types;
 * \endcode
 * \post PARAMTYPES is an MPL container containing all <type1>, <type2>, etc. from all NODES
 */
template <class NODES>
struct getparamtypes : public mpl::fold<
	NODES,
	mpl::clear<mpl::set<int> >::type,
	detail::paramtypes_inserter
> {};

} // namespace vgp
#endif /*GETTYPES_HPP_*/
