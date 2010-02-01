/*
 * organism.cpp
 *
 *  Created on: 2010-01-31
 */

#include <boost/foreach.hpp>
#include <vgp/organism.hpp>

namespace vgp {

std::vector<std::size_t> Organism::adfs(const util::TypeInfo& t) const {
	std::vector<bool> ret(adf.size());
	//for(std::size_t i = 0; i < adf.size(); i++)
		// todo: replace with result_type test code
		//if(n) ret[i] = true;
	return std::vector<std::size_t>();
}

} // namespace vgp
