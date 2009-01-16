#ifndef VGP_NODEBASE_HPP_
#define VGP_NODEBASE_HPP_

#include <cstdlib>
#include <vector>
#include <string>
#include <ostream>
#include <typeinfo>
#include <memory>

#include <boost/function.hpp>
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

	NodeBase(std::string n, const util::TypeInfo& t, std::size_t a)
	: result_type(t), arity(a), name(n) {}

	NodeBase (const NodeBase& nb)
		: children(nb.children),
		param_types(nb.param_types),
		result_type(nb.result_type),
		arity(nb.arity),
		name(nb.name) {}

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


	// Queries
	std::size_t count() const;
	std::string getID() const;
	bool complete() const;
	bool isterminal() const {return arity == 0;}
	inline bool hasstate() const {return isterminal() && (ismutatable() || isinitiable());}

	// Operations
	void init();	// calls do_init() recursively down the tree

	// Virtual functions (defined in Node<> or Terminal<>)
	virtual bool ismutatable() const = 0;
	virtual bool isinitiable() const = 0;
	virtual void mutate() = 0;
	virtual void do_init() = 0;
	virtual NodeBase* clone() const = 0;

	typedef boost::ptr_vector<NodeBase> ChildrenContainer;
	ChildrenContainer children;

	const util::TypeInfoVector param_types;
	const util::TypeInfo result_type;
	const std::size_t arity;
	const std::string name;
};

inline NodeBase* new_clone(const NodeBase &n) {return n.clone();}
void delete_clone(const NodeBase *);
typedef std::auto_ptr<NodeBase> NodePtr;

} // namespace detail
} // namespace vgp

#endif /*VGP_NODEBASE_HPP_*/
