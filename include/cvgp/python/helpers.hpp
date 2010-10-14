/*
 * util/python.hpp
 *
 *  Created on: 2010-10-10
 */

#ifndef VGP_UTIL_PYTHON_HPP_
#define VGP_UTIL_PYTHON_HPP_

#include <string>

#include <boost/python/extract.hpp>
#include <boost/python/dict.hpp>

namespace vgp {
namespace usr {

template <class RESULT>
RESULT getarg(std::string name, boost::python::dict d) {
	return boost::python::extract<RESULT>(d[name]);
}

} // namespace usr
} // namespace vgp

#endif // VGP_UTIL_PYTHON_HPP_

