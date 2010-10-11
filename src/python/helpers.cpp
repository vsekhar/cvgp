/*
 * initialize.cpp
 *
 *  Created on: 2010-10-10
 */

#include <boost/foreach.hpp>

#include <cvgp/python/helpers.hpp>

namespace std {

std::ostream& operator<<(std::ostream& os, const vgp::python::VecOfStr &v) {
	os << "[";
	bool first = true;
	BOOST_FOREACH(const std::string& s, v) {
		if(first) {
			first = false;
			os << s;
		}
		else
			os << ", " << s;
	}
	return os;
}

} // namespace std

