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
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace vgp {
namespace python {

template <class RESULT>
RESULT pyget(std::string name, boost::python::dict d) {
	using namespace boost::python;
	return extract<std::string>(d[name]);
}

} // namespace python
} // namespace vgp

#endif // VGP_UTIL_PYTHON_HPP_

