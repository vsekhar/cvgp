/*
 * adf_terminal.hpp
 *
 *  Created on: 2010-02-07
 *      Author: vsekhar
 */

#ifndef ADF_TERMINAL_HPP_
#define ADF_TERMINAL_HPP_

#include <vgp/detail/trees.hpp>
#include <vgp/detail/run.hpp>

namespace vgp {
namespace detail {
namespace adf {

typedef Trees::const_iterator state_t;

template <typename result_type>
result_type func(const state_t& s) {
	return run_as<result_type>(*s);
}

template <typename result_type>
struct fptr_t {
	typedef result_type (*type)(const state_t&);
};

} // namespace adf
} // namespace detail
} // namespace vgp

#endif /* ADF_TERMINAL_HPP_ */
