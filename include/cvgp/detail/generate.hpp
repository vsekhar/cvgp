/*
 * generate.hpp
 *
 *  Created on: 2010-02-07
 */

#ifndef GENERATE_HPP_
#define GENERATE_HPP_

#include <stdexcept>
#include <string>
#include <cvgp/detail/trees.hpp>
#include <cvgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct GenerateError: virtual std::out_of_range {
	explicit GenerateError(const std::string& s) : std::out_of_range(s) {}
};
struct MaxDepthReached : virtual std::length_error {
	explicit MaxDepthReached(const std::string& s) : std::length_error(s) {}
};

NodeBase* generate(util::TypeInfo, const Trees&, const Trees::iterator&, std::size_t);

void pyexport_generate();

} // namespace detail
} // namespace vgp

#endif /* GENERATE_HPP_ */
