#ifndef VGP_NODE_HPP_
#define VGP_NODE_HPP_

#include <string>
#include <ostream>
#include <stdexcept>

#include <boost/ref.hpp>
#include <boost/static_assert.hpp>

#include <boost/mpl/for_each.hpp>

#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>

#include <boost/serialization/base_object.hpp>

#include <vgp/util/typeinfo.hpp>

#include <vgp/nodes/nodebase.hpp>
#include <vgp/detail/nodecontainer.hpp>
#include <vgp/detail/functionbinder.hpp>

namespace vgp {
namespace detail {

namespace mpl = ::boost::mpl;
namespace ft = ::boost::function_types;

template <typename FPTR>
struct Node : NodeBase
{
	// Check we have a non-nullary function pointer, expose its
	// result and parameter types, and arity
	BOOST_STATIC_ASSERT(ft::is_function_pointer<FPTR>::value);
	BOOST_STATIC_ASSERT(ft::function_arity<FPTR>::value);
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	static const unsigned int arity = ft::function_arity<FPTR>::value;

	// Construct from a pointer and name, pass name, result typeinfo
	// and arity to NodeBase (nodebase has no compile-time info about FPTR)
	Node(FPTR fptr, std::string name) :
		NodeBase(name, util::TypeInfo(typeid(result_type)), arity),
		function(fptr)
	{
		mpl::for_each<parameter_types> (TypeInfoInserter(NodeBase::param_types));
	}
	Node(const Node& n) : NodeBase(n), function(n.function) {}

	boost::any getfunc() const {return Binder::getfunc(function, children);}
	bool ismutatable() const {return false;}
	bool isinitiable() const {return false;}
	void mutate() {
		throw std::invalid_argument("ERROR: tried to mutate a non-terminal");
	}
	void init() {
		throw std::invalid_argument("ERROR: tried to init a non-terminal");
	}
	NodeBase* clone() const {return new Node<FPTR>(*this);}
private:
	struct TypeInfoInserter {
		TypeInfoInserter(util::TypeInfoVector &v) : vector(v) {}
		template <class TOINSERT>
		void operator()(TOINSERT) {
			vector.push_back(util::TypeInfo(typeid(TOINSERT)));
		}
		util::TypeInfoVector& vector;
	};

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int /* version */) {
		ar & boost::serialization::base_object<NodeBase>(*this);
		ar & function;
	}

	typedef detail::FunctionBinder<FPTR> Binder;
	FPTR function;
};

} // namespace detail
} // namespace vgp

#endif /*VGP_NODE_HPP_*/
