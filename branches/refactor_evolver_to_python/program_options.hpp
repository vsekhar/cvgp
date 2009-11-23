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

using boost::program_options::variables_map;

boost::program_options::options_description getdescription();
variables_map parsecmdline(unsigned int, char **);


} // namespace vgp

#endif /* VGP_PROGRAM_OPTIONS_HPP_ */
