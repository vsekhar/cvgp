/*
 * register_node.hpp
 *
 *  Created on: 2010-01-27
 */

#ifndef REGISTER_HPP_
#define REGISTER_HPP_

#include <string>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <cvgp/detail/nodestorage.hpp>
#include <cvgp/detail/make_node.hpp>

#define VGP_NAME(x) x,BOOST_PP_STRINGIZE(x)

namespace vgp {

namespace mpl = ::boost::mpl;
namespace ft = ::boost::function_types;

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

#endif /* REGISTER_HPP_ */
