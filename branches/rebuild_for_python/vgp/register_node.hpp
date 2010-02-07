/*
 * register_node.hpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#ifndef REGISTER_NODE_HPP_
#define REGISTER_NODE_HPP_

#include <string>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/mpl/for_each.hpp>
#include <vgp/detail/node.hpp>
#include <vgp/detail/nodestorage.hpp>

namespace vgp {

namespace mpl = ::boost::mpl;
namespace ft = ::boost::function_types;

namespace detail {

template <class FPTR>
NodeBase* make_node(const FPTR f) {
	return new Node<FPTR>(f);
}

template <typename FROM, typename TO>
TO adapter(FROM a) {return a;}

template <typename FROM, typename TO>
NodeBase* make_adapter() {
	typedef Node<TO(*)(FROM)> node_t;
	return new node_t(adapter<FROM,TO>);
}

template <class FPTR>
NodeBase* make_terminal(const FPTR f) {
	return new Terminal<FPTR>(f);
}

template <class FPTR, class IPTR, class MPTR>
NodeBase* make_terminal(const FPTR f, const IPTR i, const MPTR m) {
	return new Terminal_w_state<FPTR>(f,i,m);
}

template <class FPTR, class STATE, class IPTR, class MPTR>
NodeBase* make_terminal(const FPTR f, STATE s, const IPTR i, const MPTR m) {
	return new Terminal_w_state<FPTR>(f,s,i,m);
}

} // namespace detail

namespace register_ {

template <typename FPTR>
void node_common(detail::NodeBase* node, std::string name) {
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	util::TypeInfoVector types;
	mpl::for_each<parameter_types> (util::TypeInfoInserter(types));
	detail::NodeEntry ne(node, typeid(result_type), types, name);
	detail::nodesbysequence.push_back(ne);
}

template <typename FPTR>
void node(const FPTR f, std::string name) {
	node_common<FPTR>(detail::make_node(f), name);
}

template <typename FROM, typename TO>
void adapter() {
	typedef TO(*FPTR)(FROM);
	std::string name = std::string("vgp_adapter_")
		+ typeid(FROM).name() + "_" + typeid(TO).name();
	node_common<FPTR>(detail::make_adapter<FROM,TO>(), name);
}

template <typename FPTR>
void terminal_common(detail::NodeBase* node, std::string name) {
	typedef typename ft::result_type<FPTR>::type result_type;
	detail::NodeEntry ne(node, typeid(result_type), util::TypeInfoVector(), name);
	detail::nodesbysequence.push_back(ne);
}

template <typename FPTR>
void terminal(const FPTR f, std::string name) {
	terminal_common<FPTR>(detail::make_terminal(f), name);
}

template <typename FPTR, typename PTR>
void terminal(const FPTR f, const PTR i, const PTR m, std::string name) {
	terminal_common<FPTR>(detail::make_terminal(f, i, m), name);

}

template <typename FPTR, typename PTR, typename STATE>
void terminal(const FPTR f, const STATE s, const PTR i, const PTR m, std::string name) {
	terminal_common<FPTR>(detail::make_terminal(f, s, i, m), name);
}

} // namespace register
} // namespace vgp

#endif /* REGISTER_NODE_HPP_ */
