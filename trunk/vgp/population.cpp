#include <numeric>
#include <fstream>

#include "population.hpp"

#include <boost/assert.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/foreach.hpp>

#include <vgp/organism.hpp>
#include <vgp/detail/text_archives.hpp>

namespace vgp {

void Population::add(std::size_t s, const std::type_info& t) {
	for(std::size_t i = 0; i < s; i++) {
		Organism* neworg = new Organism();
		neworg->generate(t);
		push_back(neworg);
	}
}

std::size_t Population::nodecount() const {
	std::size_t ret = 0;
	for(const_iterator i = begin(); i != end(); i++)
		ret += i->nodecount();
	return ret;
}

double Population::avgfitness() const {
	double fitness = 0;
	std::size_t count = 0;
	BOOST_FOREACH(const Organism& o, *this) {
		try{
			fitness += o.getfitness();
			count++;
		}

		// Skip organisms without valid fitnesses (avg among the others)
		catch(const Organism::InvalidFitness &e) {}
	}
	if(count == 0) return 0;
	else return fitness / count;
}

std::ostream& operator<<(std::ostream& o, const Population& p) {
	Population::const_iterator i = p.begin();
	for( ; i != p.end(); i++)
		o << *i << std::endl;
	return o;
}

} // namespace vgp
