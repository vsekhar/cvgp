/*
 * exception.hpp
 *
 *  Created on: Jul 28, 2008
 *      Author: vsekhar
 */

/** @file exception.hpp
 * Exceptions related to running an evolution process. These exceptions are
 * distinct from standard exceptions in that they represent errors contained
 * in a single organism. These error conditions are recoverable by either
 * ignoring or eliminating the offending organism and do not affect the stability
 * of other organisms or the VGP system.
 *
 * Any errors that do not meet the above criteria should not use the vgp exception
 * hierarchy, but should instead use std::exception's or an application-specific
 * exception hierarchy.
 */

#include <stdexcept>

#ifndef VGP_EXCEPTION_HPP_
#define VGP_EXCEPTION_HPP_

namespace vgp {

// Base for all vgp runtime exceptions
struct runtime_exception : std::runtime_error {};

// Exceptions thrown by the library
struct library_exception : runtime_exception {};
struct library_bad_type : library_exception {};

// Exceptions thrown by user code (i.e. thrown in node functions)
struct user_exception : runtime_exception {};
struct user_bad_param : user_exception {};
struct user_bad_type : user_exception {};
struct user_bad_range : user_exception {};

} // end namespace vgp


#endif /* VGP_EXCEPTION_HPP_ */
