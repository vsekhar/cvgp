/*
 * runloop.hpp
 *
 *  Created on: Dec 21, 2008
 *      Author: vsekhar
 */

#ifndef VGP_RUN_HPP_
#define VGP_RUN_HPP_

#include <vgp/evolutionops.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {

unsigned int run(int argc, char** argv, FitnessFunctor, util::TypeInfo);

} // namespace vgp

#endif /* VGP_RUN_HPP_ */
