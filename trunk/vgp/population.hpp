#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <list>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>

#include <vgp/organism.hpp>

namespace vgp {

/// A std::list of Organisms
typedef std::list<Organism> Organisms;
/// A boost::tuple containing an element count and beginning and end iterators
typedef boost::tuple<std::size_t, Organisms::iterator, Organisms::iterator>
	Subpopulation;

struct Population {
	/// An empty population
	Population() {}
	
	/// A population of size \c i with organisms returning type \c t
	Population(std::size_t i, const std::type_info& t) : organisms(i) {
		generate(t);
	}
	
	/** generate organisms in the population whose root nodes produce a result_type
	 * matching the type_info passed.
	 * 
	 * Has no effect on organisms who are already generated
	 */
	std::size_t generate(const std::type_info& t) {return generaterange(begin(), end(), t);}
	
	/** expand the population and create organisms producing the given result type
	 */
	Subpopulation grow(std::size_t, const std::type_info&);
	
	/** delete a proportion of the population from the beginning of the container.
	 * Normally, the user would call \c sort() before calling poach to move the worst
	 * organisms (those with the lowest fitness) to the beginning for poaching
	 */
	std::size_t poach(double);
	
	/** Poach a proportion of populatoin from the beginning of the container and
	 * replace the deleted organisms with new random organisms of the given result type
	 */
	Subpopulation poachandreplace(double, const std::type_info&);
	
	/** sort the organisms based on \c operator<(Organism, Organism)
	 */
	void sort() {organisms.sort();}
	
	/** the total number of nodes in all organisms in the population
	 */
	std::size_t nodecount() const;
	
	/** the average number of nodes in the organisms in the population
	 */
	double avgnodecount() const {return (double)nodecount() / size();}
	
	/** the average fitness value of all organisms in the population
	 */
	double avgfitness() const;
	
	/** a string summarizing some key stats of the population
	 */
	std::string summary() const;
	
	/** a string summarizing the best n organisms of the population.
	 * \note the population must be sorted before calling best() for best
	 * to work properly
	 */
	std::string best(std::size_t n) const;
	std::string worst(std::size_t n) const;

	std::size_t size() const { return organisms.size(); }
	Organisms::iterator begin() { return organisms.begin(); }
	Organisms::const_iterator begin() const { return organisms.begin(); }
	Organisms::reverse_iterator rbegin() { return organisms.rbegin(); }
	Organisms::const_reverse_iterator rbegin() const { return organisms.rbegin(); }
	Organisms::iterator end() { return organisms.end(); }
	Organisms::const_iterator end() const { return organisms.end(); }
	Organisms::reverse_iterator rend() { return organisms.rend(); }
	Organisms::const_reverse_iterator rend() const { return organisms.rend(); }
	typedef Organisms::iterator iterator;
	typedef Organisms::reverse_iterator reverse_iterator;
	typedef Organisms::const_iterator const_iterator;
	typedef Organisms::const_reverse_iterator const_reverse_iterator;
	
private:
	Organisms organisms;
	std::size_t generaterange(iterator beg, const_iterator end, const std::type_info& t);
	friend std::ostream& operator<<(std::ostream& o, const Population& p) {
		Organisms::const_iterator i = p.organisms.begin();
		for( ; i != p.organisms.end(); i++)
			o << *i << std::endl;
		return o;
	}
};

} // namespace vgp

#endif /*POPULATION_HPP_*/
