#ifndef MULTIFOLD_HPP_
#define MULTIFOLD_HPP_

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/placeholders.hpp>

///////////////////
// Multifold
//
// A combiner along the lines of boost::mpl::fold<> but supporting
// 1, 2, or 3 containers of types through which all combinations
// will be instantiated.
//
// TODO: Make this variadic?
// Application note: prob. not needed until VGP accepts any-arity nodes...
// (should be easy though with a recursive application using mpl::bind<>
//  combined with some preprocessor-generated templates, as mpl does)
///////////////////

namespace vgp {
namespace util {
namespace mpl = ::boost::mpl;

// Multifold 1 (a pseudonym for the regular mpl::fold)
template <class TYPES1,
	class STATE, class INSERTER >
struct multifold1 : mpl::fold<
	TYPES1,
	STATE,
	mpl::apply_wrap2<INSERTER, mpl::_1, mpl::_2>
> {};

// Multifold 2
namespace detail {
template <
	class TYPES2,
	class INSERTER
>
struct multifold2inner {
	template <class CURSTATE, class CURTYPE1>
	struct apply : public mpl::fold<
			TYPES2,
			CURSTATE,
			mpl::apply_wrap3<INSERTER, mpl::_1, CURTYPE1, mpl::_2> >
	{};
};
} // namespace detail

template <
	class TYPES1,
	class TYPES2,
	class STATE,
	class INSERTER >
struct multifold2 : public mpl::fold<
		TYPES1,
		STATE,
		detail::multifold2inner<TYPES2, INSERTER>
	>
{};

// Multifold 3
namespace detail {
template <class CURTYPE1, class TYPES3, class INSERTER>
struct multifold3inner2 {
	template <class CURSTATE, class CURTYPE2>
	struct apply : public mpl::fold<
			TYPES3,
			CURSTATE,
			mpl::apply_wrap4<INSERTER, mpl::_1, CURTYPE1, CURTYPE2, mpl::_2> >
	{};
};

template <class TYPES2, class TYPES3, class INSERTER>
struct multifold3inner {
	template <class CURSTATE, class CURTYPE1>
	struct apply : public mpl::fold<
			TYPES2,
			CURSTATE,
			multifold3inner2<CURTYPE1, TYPES3, INSERTER> >
	{};
};
} // namespace detail

template <
	class TYPES1,
	class TYPES2,
	class TYPES3,
	class STATE,
	class INSERTER>
struct multifold3 {
	typedef typename mpl::fold<
		TYPES1,
		STATE,
		detail::multifold3inner<TYPES2, TYPES3, INSERTER>
	>::type type;
};


} // namespace util
} // namespace vgp

#endif /*MULTIFOLD_HPP_*/
