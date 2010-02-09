/*
 * generate.hpp
 *
 *  Created on: 2010-02-07
 */

#ifndef GENERATE_HPP_
#define GENERATE_HPP_

#include <stdexcept>
#include <vgp/detail/trees.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct GenerateError: virtual std::exception {};
struct MaxDepthReached : virtual GenerateError {};

NodeBase* generate(util::TypeInfo, const Trees&, const Trees::iterator&, std::size_t);

} // namespace detail
} // namespace vgp

#endif /* GENERATE_HPP_ */
