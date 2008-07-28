#ifndef VGP_NODECONTAINER_HPP_
#define VGP_NODECONTAINER_HPP_

#include <vector>
#include <map>
#include <ostream>
#include <string>
#include <cmath>

#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/utility.hpp>

#include <vgp/nodes/nodebase.hpp>
#include <vgp/nodes/nodemultiindex.hpp>

#include <vgp/util/typeinfo.hpp>

#ifndef VGP_DEFAULT_DEPTH_PENALTY
#define VGP_DEFAULT_DEPTH_PENALTY (0.99)
#endif

namespace vgp {

/** A class for storing and retrieving nodes at run-time.
 *
 * NodeContainer provides facilities to add nodes to the vgp system and retrieves
 * nodes as needed by vgp.
 *
 * \par Insertion
 * At run-time, the user calls insert(node) with a pointer to each node to be added.
 * NodeContainer will index the node and make it available to the vgp library.
 * \par
 * The pointer passed to NodeContainer becomes owned by NodeContainer and should never
 * be deleted in user code.
 *
 * \par Retrieval
 * Nodes are retrieved via the getnode() and getrandomnode() members. These functions are
 * provided with some characteristic of the node (the ID, name or result_type) and lookup nodes
 * that match the provided characteristic. In the case of non-unique lookup, a random node
 * is selected.
 * \par
 * Retrieval functions return clones of original nodes. Nodes inside the NodeContainer are
 * used only as prototypes for spawning other nodes.
 *
 */
struct NodeContainer : boost::noncopyable {
	/** Default constructor (necessary because the library puts a NodeContainer in a util::singleton0)
	 * @issue Is this still necessary? we don't use singleton0 anymore...
	 */
	NodeContainer() :
		nodesbysequence(nodes.get<bySequence>()),
		terminalsbysequence(terminals.get<bySequence>()),
		nodesbyID(nodes.get<byID>()),
		terminalsbyID(terminals.get<byID>()),
		nodesbyresulttype(nodes.get<byResultType>()),
		terminalsbyresulttype(terminals.get<byResultType>()),
		nodesbyresulttypename(nodes.get<byResultTypeName>()),
		terminalsbyresulttypename(terminals.get<byResultTypeName>()),
		nodesbyname(nodes.get<byName>()),
		terminalsbyname(terminals.get<byName>()),
		depthpenalty(VGP_DEFAULT_DEPTH_PENALTY)
		{}

	/** \brief Adds a node to the container
	 * \param n pointer to node to be added (this pointer should never be freed by user code)
	 * \sa insert(n)
	 */
	void push_back(const detail::NodeBase* n);

	/** \brief Adds a node to the container
	 * \param n pointer to node to be added (this pointer should never be freed by user code)
	 * \sa push_back(n)
	 */
	void insert(const detail::NodeBase* n) {push_back(n);}

	/// Number of nodes in the container
	std::size_t nodecount() const {return nodes.size();}
	/// Number of terminals in the container
	std::size_t terminalcount() const {return terminals.size();}
	/// Returns the number of nodes and terminals in the container
	std::size_t size() const {return nodes.size() + terminals.size();}

	/// Get a specific node by its globally unique ID string
	detail::NodeBase* getnode(const std::string&) const;

	/// Select a random node returning the given type
	detail::NodeBase* getrandomnode(const std::type_info&, std::size_t depth) const;

	/// Select a random node with the given name
	detail::NodeBase* getrandomnode(const std::string&, std::size_t depth) const;

	/** Returns the type information associated with a specific compiler-generated name
	 *
	 * \throw std::invalid_argument if no node in the container has a result_type whose
	 * name matches the string provided
	 */
	const std::type_info &gettypeinfo(const std::string &name) const;

	/** \brief Set the depth penalty
	 *
	 * The depth penalty is by default 0.99. Higher values result in deeper trees,
	 * lower values in shallower ones.
	 * \param d the new depth penalty (0 < d < 1, runtime error otherwise)
	 * \warning Use this with great caution. Lowering the depth penalty results in
	 * super-exponential growth in the number of nodes in a given population and in the
	 * memory requirements for storing that population. Adjustments should be very minor
	 * and should be monitored experimentally to see the impact on memory requirements
	 * and evolutionary characteristics.
	 */
	void setdepthpenalty(double d) {
		BOOST_ASSERT(d > 0 && d <= 1);
		depthpenalty = d;
	}

	/// Get the current depth penalty factor
	double getdepthpenalty() const {return depthpenalty;}

private:
	/** \name Index types
	 * Types used to declare indices into the NodeContainer, permitting lookup
	 * by various attributes of the nodes.
	 */
	//@{
	/// Index by insertion sequence
	typedef detail::NodeMultiIndex::index<bySequence>::type NodesBySequence_t;
	/// Index by unique ID string
	typedef detail::NodeMultiIndex::index<byID>::type NodesByID_t;
	/// Index by type_info of result type (non-unique)
	typedef detail::NodeMultiIndex::index<byResultType>::type NodesByResultType_t;
	/// Index by compiler-generated type name of the node's result type (non-unique)
	typedef detail::NodeMultiIndex::index<byResultTypeName>::type NodesByResultTypeName_t;
	/// Index by name (non-unique, nodes may have same name but use different types)
	typedef detail::NodeMultiIndex::index<byName>::type NodesByName_t;
	//@}

	/** \name Node and terminal Storage
	 * Nodes and terminals are stored in separate multi-indexes
	 */
	//@{
	detail::NodeMultiIndex nodes;
	detail::NodeMultiIndex terminals;
	//@}

	/// Randomly selects a node from two pools (nodes and terminals), with a given depth penalty
	boost::tuple<bool, std::size_t> weightedpick
		(std::size_t nodecount, std::size_t terminalcount, std::size_t depth) const;

	/** \name Index references
	 * References to the indices of various types in the node and terminal storage containers
	 */
	//@{
	NodesBySequence_t& nodesbysequence;
	NodesBySequence_t& terminalsbysequence;
	NodesByID_t& nodesbyID;
	NodesByID_t& terminalsbyID;
	NodesByResultType_t& nodesbyresulttype;
	NodesByResultType_t& terminalsbyresulttype;
	NodesByResultTypeName_t& nodesbyresulttypename;
	NodesByResultTypeName_t& terminalsbyresulttypename;
	NodesByName_t& nodesbyname;
	NodesByName_t& terminalsbyname;
	//@}

	/** Current depth penalty for adjusting node/terminal selection
	 * \sa weightedpick()
	 */
	double depthpenalty;

	/** Output each node on its own line in the order in which they were inserted
	 * \pre Requires NodeBase ostream operator<<
	 */
	friend std::ostream& operator<<(std::ostream&, const NodeContainer&);

};

} // namespace vgp

#endif /*VGP_NODECONTAINER_HPP_*/
