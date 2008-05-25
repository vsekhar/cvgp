#ifndef RANDOMSTATE_HPP_
#define RANDOMSTATE_HPP_

#include <vgp/util/random.hpp>

#include <boost/ref.hpp>

namespace vgp {
namespace util {

template <class OUTPUT, class GENERATOR, class DIST>
struct RandomState_impl {
	RandomState_impl() :
		generator(singleton0<GENERATOR>::instance()),
		distribution(),
		var(generator, distribution),
		state(0) {}
	RandomState_impl(OUTPUT low, OUTPUT high) :
		generator(singleton0<GENERATOR>::instance()),
		distribution(low, high),
		var(generator, distribution),
		state(0) {}
	OUTPUT get() const {return state;}
	OUTPUT getnew() {return state=var();}
	GENERATOR & generator;	// refers to the global singleton for performance
	DIST distribution;
	boost::variate_generator<GENERATOR&, DIST> var;
	OUTPUT state;
};

template <class OUTPUT, class GENERATOR, class DIST, int low, int high>
struct RandomRangedInt_impl : RandomState_impl <OUTPUT, GENERATOR, DIST> {
	RandomRangedInt_impl() :
		RandomState_impl<OUTPUT, GENERATOR, DIST>(low, high) {}
};

struct RandomIntState : 
RandomState_impl <int, time_seeded_mersenne, boost::uniform_int<int> > {};

struct RandomSmallIntState :
RandomState_impl <int, time_seeded_mersenne, boost::uniform_smallint<int> > {};

template <int low, int high>
struct RandomRangedIntState :
RandomRangedInt_impl <int, time_seeded_mersenne, boost::uniform_int<int>, low, high> {};

template <int low, int high>
struct RandomRangedSmallIntState :
RandomRangedInt_impl <int, time_seeded_mersenne, boost::uniform_smallint<int>, low, high> {};

struct RandomDouble01State :
RandomState_impl <double, time_seeded_mersenne, boost::uniform_real<> > {
	RandomDouble01State() :
		RandomState_impl<double, time_seeded_mersenne, boost::uniform_real<> >(0,1) {}
};
	
} // namespace util
} // namespace vgp

#endif /*RANDOMSTATE_HPP_*/
