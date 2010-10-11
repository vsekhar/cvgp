/*
 * util/python.hpp
 *
 *  Created on: 2010-10-10
 */

#ifndef VGP_UTIL_PYTHON_HPP_
#define VGP_UTIL_PYTHON_HPP_

#include <string>
#include <vector>
#include <ostream>

#include <boost/python/dict.hpp>

namespace vgp {
namespace python {

template <class RESULT>
RESULT pyget(std::string name, boost::python::dict d) {
	using namespace boost::python;
	return extract<std::string>(d[name]);
}

typedef std::vector<std::string> VecOfStr;

} // namespace python
} // namespace vgp

namespace std {

ostream& operator<<(ostream&, const vgp::python::VecOfStr&);

} // namespace std

#endif // VGP_UTIL_PYTHON_HPP_

