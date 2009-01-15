#ifndef VGP_NODE_HPP_
#define VGP_NODE_HPP_

#include <string>
#include <stdexcept>

#include <boost/static_assert.hpp>

#include <boost/mpl/for_each.hpp>

#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>

#include <vgp/util/typeinfo.hpp>
#include <vgp/nodes/nodebase.hpp>
#include <vgp/detail/functionbinder.hpp>

namespace vgp {
namespace detail {

namespace mpl = ::boost::mpl;
namespace ft = ::boost::function_types;

template <typename FPTR>
struct Node : NodeBase
{
	BOOST_STATIC_ASSERT(ft::is_function_pointer<FPTR>::value);
	BOOST_STATIC_ASSERT(ft::function_arity<FPTR>::value);
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	static const unsigned int arity = ft::function_arity<FPTR>::value;

	Node(FPTR fptr, std::string name) :
		NodeBase(name, util::TypeInfo(typeid(result_type)), arity),
		function(fptr)
	{
		mpl::for_each<parameter_types> (util::TypeInfoInserter(NodeBase::param_types));
	}
	Node(const Node& n) : NodeBase(n), function(n.function) {}

	// Overloads of virtual functions from NodeBase
	boost::any getfunc() const {return Binder::getfunc(function, children);}
	bool ismutatable() const {return false;}
	bool isinitiable() const {return false;}
	void do_mutate() {
		throw std::invalid_argument("ERROR: tried to mutate a non-terminal");
	}
	void do_init() {
		throw std::invalid_argument("ERROR: tried to init a non-terminal");
	}
	NodeBase* clone() const {return new Node<FPTR>(*this);}

	typedef detail::FunctionBinder<FPTR> Binder;
	FPTR function;
};

} // namespace detail
} // namespace vgp

#endif /*VGP_NODE_HPP_*/
