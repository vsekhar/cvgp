/*
 * program_options.hpp
 *
 *  Created on: Dec 8, 2008
 *      Author: vsekhar
 */

#ifndef VGP_PROGRAM_OPTIONS_HPP_
#define VGP_PROGRAM_OPTIONS_HPP_

#include <boost/program_options.hpp>

namespace vgp {
namespace program_options {

boost::program_options::options_description getdescription();
boost::program_options::variables_map parsecmdline(unsigned int, char **);

} // namespace program_options
} // namespace vgp

#endif /* VGP_PROGRAM_OPTIONS_HPP_ */
