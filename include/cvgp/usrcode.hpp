/*
 * usrcode.hpp
 *
 *  Created on: 2010-10-10
 */

#ifndef VGP_USRCODE_HPP_
#define VGP_USRCODE_HPP_

#include <boost/python/dict.hpp>

namespace vgp {
namespace usr {

extern bool initialize(boost::python::dict);
extern void register_nodes();

} // namespace usr
} // namespace vgp

#endif // VGP_USRCODE_HPP_
