/*
 * usrcode.hpp
 *
 *  Created on: 2010-10-10
 */

#ifndef VGP_USRCODE_HPP_
#define VGP_USRCODE_HPP_

#include <boost/python/dict.hpp>

// Include these here so user code doesn't have to
#include <cvgp/register.hpp>
#include <cvgp/python/helpers.hpp>
#include <cvgp/library.hpp>

namespace vgp {
namespace usr {

// Pull in some definitions for user code to use
typedef ::boost::python::dict kwargs_t;

// Users supply these functinos
extern bool initialize(kwargs_t);
extern void register_nodes();

} // namespace usr
} // namespace vgp


#endif // VGP_USRCODE_HPP_
