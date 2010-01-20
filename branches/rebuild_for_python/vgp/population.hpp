#ifndef VGP_POPULATION_HPP_
#define VGP_POPULATION_HPP_

#include <list>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ptr_container/serialize_ptr_list.hpp>

#include <vgp/organism.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {

typedef boost::ptr_list<Organism> Organisms;

struct Population : Organisms {
	/// An empty population
	Population() {}

	/// A population of size \c i with organisms returning type \c t
	Population(std::size_t i, util::TypeInfo t) {add(i, t);}

	/** Add organisms to the population whose root nodes produce result_type
	 *
	 * Organisms are populated with randomly selected nodes
	 */
	void add(std::size_t, const std::type_info&);

	/** the total number of nodes in all organisms in the population
	 */
	std::size_t nodecount() const;

	/** the average number of nodes in the organisms in the population
	 */
	double avgnodecount() const {return (double)nodecount() / size();}

	/** the average fitness value of all organisms in the population
	 */
	double avgfitness() const;

private:
	friend std::ostream& operator<<(std::ostream& o, const Population& p);
	friend class boost::serialization::access;
	template <class Archive>
	void serialize (Archive &ar, const unsigned int /* version */) {
		ar & boost::serialization::base_object<Organisms>(*this);
	}
};

} // namespace vgp

#endif /*VGP_POPULATION_HPP_*/
