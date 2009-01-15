#ifndef VGP_NODEBASE_HPP_
#define VGP_NODEBASE_HPP_

#include <cstdlib>
#include <vector>
#include <string>
#include <ostream>
#include <typeinfo>
#include <memory>

#include <boost/function.hpp>
#include <boost/foreach.hpp>
#include <boost/any.hpp>
#include <boost/utility.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>
#include <boost/serialization/access.hpp>

#include <vgp/organism/organism_fwd.hpp>

#include <vgp/util/typeinfo.hpp>
#include <vgp/util/treeserializer_fwd.hpp>

namespace vgp {

namespace detail {

struct NodeBase;

std::ostream& operator<<(std::ostream&, const NodeBase&);

/** Base class for all nodes
 *
 * NodeBase is a non-templated base class for all nodes available to organisms.
 * It provides most functionality related to manipulating self-similar trees.
 */

struct NodeBase : boost::noncopyable {
	typedef boost::ptr_vector<NodeBase> ChildrenContainer;
	struct Undefined {};

	NodeBase() : result_type(typeid(Undefined)), _arity(0), _name() {}
	NodeBase(std::string n, const util::TypeInfo& t, std::size_t a)
	: result_type(t), _arity(a), _name(n) {}

	NodeBase (const NodeBase& nb)
		: children(nb.children),
		param_types(nb.param_types),
		result_type(nb.result_type),
		_arity(nb._arity),
		_name(nb._name) {}

	virtual ~NodeBase() {}

	template <class OUTPUT_TYPE>
	boost::function<OUTPUT_TYPE()> getfunc() const {
		try {
		return boost::any_cast<boost::function<OUTPUT_TYPE()> >(getfunc());
		}
		catch(boost::bad_any_cast &e) {
			std::cerr << "NodeBase any_cast ERROR: " << *this;
			throw;
		}
	}
	virtual boost::any getfunc() const = 0;

	inline std::size_t count() const {
		std::size_t ret = 1;
		BOOST_FOREACH(const NodeBase &child, children)
			ret += child.count();
		return ret;
	}

	// Queries
	std::string getID() const;
	std::type_info const & getresulttype() const {return result_type.get();}
	std::string getresulttypename() const {return result_type.get().name();}
	util::TypeInfo getresulttypeinfo() const {return result_type;}
	util::TypeInfoVector getparamtypes() const {return param_types;}
	std::size_t arity() const {return _arity;}
	bool complete() const;
	bool isterminal() const {return arity() == 0;}
	inline bool hasstate() const {return isterminal() && (ismutatable() || isinitiable());}
	std::string name() const {return _name;}

	// Operations
	virtual void init() {
		BOOST_FOREACH(NodeBase &child, children)
			child.init();
	}

	// Virtual functions (defined in Node<> or Terminal<>)
	virtual bool ismutatable() const = 0;
	virtual bool isinitiable() const = 0;
	virtual void mutate() = 0;
	virtual NodeBase* clone() const = 0;

	ChildrenContainer children;

	util::TypeInfoVector param_types;
	util::TypeInfo result_type;
	std::size_t _arity;
	std::string _name;
};

inline NodeBase* new_clone(const NodeBase &n) {return n.clone();}
void delete_clone(const NodeBase *);
typedef std::auto_ptr<NodeBase> NodePtr;

} // namespace detail
} // namespace vgp

#endif /*VGP_NODEBASE_HPP_*/
