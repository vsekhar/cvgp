#include <numeric>

#include "population.hpp"

#include <boost/numeric/conversion/converter.hpp>
#include <boost/foreach.hpp>

#include <vgp/util/rounder.hpp>

namespace vgp {

Subpopulation Population::grow(std::size_t s, const std::type_info& t) {
	if(s < size())
		throw std::invalid_argument("Population: tried to shrink population using grow (use poach instead)");
	iterator cur, last;
	cur = end();	// point just past end
	cur--; // point to last existing element
	organisms.resize(s);
	cur++; // point to first newly created element
	last = end();	// point to past end (after newly created elements)
	Subpopulation ret;
	ret.get<1>() = cur;
	ret.get<2>() = last;
	ret.get<0>() = generaterange(cur, last, t);
	return ret;
}

std::size_t Population::generaterange(iterator itr, const_iterator end, const std::type_info& t) {
	std::size_t count = 0;
	for( ; itr != end; itr++, count++)
		itr->generate(t);
	return count;
}

std::size_t Population::nodecount() const {
	std::size_t ret = 0;
	for(const_iterator i = begin(); i != end(); i++)
		ret += i->nodecount();
	return ret;
}

std::size_t Population::poach(double proportion)  {
	iterator i = begin();
	std::size_t topoach = util::Rounder::convert(proportion * organisms.size());
	for(std::size_t count = 0; count < topoach; count++) i = organisms.erase(i);
	return size();
}

Subpopulation Population::poachandreplace(double proportion, const std::type_info& t) {
	std::size_t cursize = size();
	poach(proportion);
	return grow(cursize, t);
}

double Population::avgfitness() const {
	double fitness = 0;
	BOOST_FOREACH(const Organism& o, *this) {
		fitness += o.getfitness();
	}
	return fitness / size();
}

std::string Population::summary() const {
	std::stringstream ret;
	ret << "Population: " << size() << " organisms, " << nodecount() << " nodes, " << avgfitness() << " average fitness";
	return ret.str();
}

std::string Population::best(std::size_t n) const {
	std::stringstream ret;
	ret << "Best " << n << " organisms:" << std::endl;
	const_reverse_iterator i = rbegin();
	std::size_t limit = std::min(n, size());
	for(std::size_t count = 0; count < limit; count++) {
		ret << *i++ << std::endl;
	}
	return ret.str();
}

std::string Population::worst(std::size_t n) const {
	std::stringstream ret;
	ret << "Worst " << n << " organisms:" << std::endl;
	const_iterator i = begin();
	std::size_t limit = std::min(n, size());
	for(std::size_t count = 0; count < limit; count++) {
		ret << *i++ << std::endl;
	}
	return ret.str();
}

} // namespace vgp
