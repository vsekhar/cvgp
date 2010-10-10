/*
 * initialize.cpp
 *
 *  Created on: 2010-10-10
 */

#include <string>
#include <vector>
#include <ostream>

#include <boost/foreach.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <cvgp/python/helpers.hpp>

namespace std {

typedef std::vector<std::string> VecOfStr;

std::ostream& operator<<(std::ostream& os, const VecOfStr &v) {
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

namespace vgp {
namespace python {

void register_helpers() {
	using namespace boost::python;
	
	class_<std::VecOfStr>("VectorOfStrings")
		.def(vector_indexing_suite<std::VecOfStr>())
		.def(self_ns::str(self));
}

} // namespace python
} // namespace vgp
