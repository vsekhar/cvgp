#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <ctime>	// for std::time

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <vgp/util/singleton.hpp>

namespace vgp {
namespace util {

template <class GENERATOR, unsigned int seed>
struct SeededGenerator : GENERATOR {
	SeededGenerator() : GENERATOR(seed) {};
};

template <class GENERATOR>
struct TimeSeededGenerator : GENERATOR {
	TimeSeededGenerator() : GENERATOR(std::time(0)) {};
};

typedef TimeSeededGenerator<boost::mt19937> time_seeded_mersenne;
typedef singleton0<time_seeded_mersenne> default_generator;

struct RandomRangedInt :
boost::variate_generator<default_generator::object_type&, boost::uniform_int<int> > {
	RandomRangedInt(int low, int high) : 
		boost::variate_generator<
			default_generator::object_type&,
			boost::uniform_int<int>
		>
	(default_generator::instance(), boost::uniform_int<int>(low, high)) {}
};

typedef boost::uniform_01<default_generator::object_type> RandomBool_base;
struct RandomBool : RandomBool_base {
	RandomBool() : RandomBool_base(default_generator::instance()) {}
	result_type operator()(double probability_of_true) {
		return RandomBool_base::operator()() < probability_of_true;
	}
};

} // namespace util
} // namespace vgp

#endif /*RANDOM_HPP_*/
