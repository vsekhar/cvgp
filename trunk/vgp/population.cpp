#include <numeric>

#include "population.hpp"

#include <boost/assert.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/foreach.hpp>

#include <vgp/util/rounder.hpp>
#include <vgp/organism.hpp>

namespace vgp {

Subpopulation Population::add(std::size_t s, const std::type_info& t) {
	iterator cur, last;
	cur = end();	// point just past end
	cur--; // point to last existing element
	for(std::size_t i = 0; i < s; i++) {
		Organism* neworg = new Organism();
		neworg->generate(t);
		push_back(neworg);
	}
	cur++; // point to first newly created element
	last = end();	// point to past end (after newly created elements)
	Subpopulation ret;
	ret.get<1>() = cur;
	ret.get<2>() = last;
	ret.get<0>() = s;
	return ret;
}

std::size_t Population::nodecount() const {
	std::size_t ret = 0;
	for(const_iterator i = begin(); i != end(); i++)
		ret += i->nodecount();
	return ret;
}

double Population::avgfitness() const {
	double fitness = 0;
	BOOST_FOREACH(const Organism& o, *this)
		fitness += o.getfitness();
	return fitness / size();
}

std::string Population::summary() const {
	std::stringstream ret;
	ret << "Population: " << size() << " organisms, " << nodecount() << " nodes, " << avgfitness() << " average fitness";
	return ret.str();
}

std::ostream& operator<<(std::ostream& o, const Population& p) {
	Population::const_iterator i = p.begin();
	for( ; i != p.end(); i++)
		o << *i << std::endl;
	return o;
}

} // namespace vgp
