/*
 * adf_terminal.hpp
 *
 *  Created on: 2010-02-07
 */

#ifndef ADF_TERMINAL_HPP_
#define ADF_TERMINAL_HPP_

#include <vgp/detail/trees.hpp>
#include <vgp/detail/run.hpp>
#include <vgp/detail/node_concrete.hpp>

namespace vgp {
namespace detail {
namespace adf {

typedef Trees::const_iterator state_t;

template <typename result_type>
result_type func(const state_t& s) {
	return run_as<result_type>(*s);
}

template <typename result_type>
struct types {
	typedef result_type(*fptr_type)(const state_t&);
	typedef void(*imptr_type)(state_t&);
	typedef Terminal_w_state<fptr_type> node_type;
};

struct ADF_base {
	virtual void set(const state_t&) = 0;
	virtual state_t get() const = 0;
};

} // namespace adf

template <typename result_type>
struct ADF : Terminal_w_state<typename adf::types<result_type>::fptr_type>, adf::ADF_base {
	typedef Terminal_w_state<typename adf::types<result_type>::fptr_type> base;
	ADF() : base(adf::func<result_type>) {}
	virtual void set(const adf::state_t& s) {base::state = s;}
	virtual adf::state_t get() const {return base::state;}
	virtual bool isADF() const {return true;}
};

NodeBase* expand_adf(const NodeBase&);
void expand_adfs(NodeBase&);
void expand_adfs(tree&);

} // namespace detail
} // namespace vgp

#endif /* ADF_TERMINAL_HPP_ */
