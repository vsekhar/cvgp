/*
 * util/python.hpp
 *
 *  Created on: 2010-10-10
 */

#ifndef VGP_UTIL_PYTHON_HPP_
#define VGP_UTIL_PYTHON_HPP_

#include <string>

#include <boost/python.hpp>
#include <boost/python/dict.hpp>

namespace vgp {
namespace python {

template <class RESULT>
RESULT pyget(std::string name, boost::python::dict d) {
	return boost::python::extract<RESULT>(d[name]);
}

} // namespace python
} // namespace vgp

#endif // VGP_UTIL_PYTHON_HPP_

