/*
 * organism.hpp
 *
 *  Created on: 2010-01-31
 */

#ifndef ORGANISM_HPP_
#define ORGANISM_HPP_

#include <ostream>
#include <stdexcept>
#include <vector>
#include <cvgp/detail/trees.hpp>
#include <cvgp/util/typeinfo.hpp>

namespace vgp {

struct OrganismRunError : std::exception {};
struct OrganismBadRunType : OrganismRunError {};

struct Organism {
	Organism(util::TypeInfo);

	void init();
	void mutate();

	detail::Trees trees;
};

template <class R_>
Organism make_org_returning() {return Organism(typeid(R_));}

std::ostream& operator<<(std::ostream&, const Organism&);

} // namespace vgp


#endif /* ORGANISM_HPP_ */
