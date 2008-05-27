#ifndef NODEBASE_HPP_
#define NODEBASE_HPP_

#include <cstdlib>
#include <vector>
#include <string>
#include <ostream>
#include <typeinfo>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/utility.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/scoped_ptr.hpp>

#include <vgp/organism/organism_fwd.hpp>

#include <vgp/util/typeinfo.hpp>
#include <vgp/util/accesscontrol.hpp>

namespace vgp {

namespace detail {

/// Base class for all nodes.

/** NodeBase is a non-templated base class for all nodes available to organisms.
 * It provides most functionality related to manipulating self-similar trees.
 */

struct NodeBase : boost::noncopyable {
	typedef boost::ptr_vector<NodeBase> ptr_vector;

	/// Construct and set result_type
	NodeBase(std::string n, const util::TypeInfo& t, std::size_t a)
	: result_type(t), _arity(a), _name(n) {}
	/// Copy constructor deep copies children
	NodeBase (const NodeBase& nb)
		: param_types(nb.param_types),
		result_type(nb.result_type),
		_arity(nb._arity),
		_name(nb._name) {
		deepcopychildren(nb);
	}
	/// Destructor deletes children
	virtual ~NodeBase() {
		clearchildren();
	}
	
	/// A unique identifier string for this node
	/** Node identifiers are usually of the format "name[result_type](p1_type, ...)"
	 */
	std::string getID() const;
	
	/// 
	template <class OUTPUT_TYPE>
	boost::function<OUTPUT_TYPE()> getfunc() const {
		try {
		return boost::any_cast<boost::function<OUTPUT_TYPE()> >(getfunc());			
		}
		catch(boost::bad_any_cast &e) {
			std::cerr << *this;
			throw;
		}
	}
	virtual boost::any getfunc() const = 0;

	/// Returns the number of nodes in this [sub-]tree
	/** Counts recursively down the tree, starting from the 
	 * current node.
	 * E.g. for a node with two terminal children, node->count() == 3
	 */
	std::size_t count() const {
		std::size_t ret = 1;
		ptr_vector::const_iterator i = children.begin();
		for( ; i != children.end(); i++)
			ret += i->count();
		return ret;
	}

	/// Count how many immediate children the current has (does not traverse further)
	std::size_t childrencount() const { return children.size(); }
	/// Delete all children of the current node (and all of their children)
	void clearchildren() {children.clear();}
	/// Copy children (and their subsequent children) from another node
	void deepcopychildren(const NodeBase&);
	
	/// The type of data returned by this node
	std::type_info const & getresulttype() const {return result_type.get();}
	/// Compiler-generated type name string for the node's result type
	std::string getresulttypename() const {return result_type.get().name();}
	/// The type if data returned by this node (reference wrapped)
	util::TypeInfo getresulttypeinfo() const {return result_type;}
	/// The types of data required as parameters of this node [0,VGP_MAX_NODE_ARITY]
	util::TypeInfoVector getparamtypes() const {return param_types;}
	/// Number of children required by this node
	std::size_t arity() const {return _arity;}

	/// True if this node requires no children
	bool isterminal() const {return arity() == 0;}
	virtual bool ismutatable() const = 0;
	virtual bool isinitiable() const = 0;
	virtual void init() {
		ptr_vector::iterator i = children.begin();
		for( ; i != children.end(); i++) i->init();
	}

	/// Name of node
	/** Node names must be globally unique based on functionality (not types).
	 * E.g. add[i](i,i) and add[d](d,d) can co-exist, but zero[i] and zero[i] cannot
	 */
	std::string name() const {return _name;}
	
	/////////////////////////
	// Virtual functions
	// To be defined once we know the node type (i.e. in Node<>)
	/////////////////////////
	
	/** @brief Mutate the current node (as defined by the user-supplied mutate function)
	 * Node-defined behaviour (may also be no-op)
	 */
	virtual void mutate() = 0;

	/** @brief Clone the current node
	 * Returns a pointer to a new node that is identical to (but independent of)
	 * the current node.
	 */
	virtual NodeBase* clone() const = 0;

protected:
	/** Internal use only.
	 * @bug should be protected, but for some reason Node<> can't access it.
	 * gcc bug 2617? http://gcc.gnu.org/ml/gcc-prs/2001-04/msg00434.html
	 */
	ptr_vector children;

	friend struct ::vgp::Organism;
	//template <class T1, class T2> friend struct ::vgp::detail::Node<T1,T2>;
	friend std::ostream& operator<<(std::ostream&, const NodeBase&);
	util::TypeInfoVector param_types;
private:
	util::TypeInfo result_type;
	std::size_t _arity;
	std::string _name;
};

std::ostream& operator<<(std::ostream&, const NodeBase&);
typedef boost::scoped_ptr<NodeBase> NodePtr;
NodeBase* new_clone(const NodeBase &);

} // namespace detail
} // namespace vgp

#endif /*NODEBASE_HPP_*/
