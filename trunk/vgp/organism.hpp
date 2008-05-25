#ifndef ORGANISM_HPP_
#define ORGANISM_HPP_

#include <ostream>
#include <string>
#include <stdexcept>

#include <boost/any.hpp>
#include <boost/function.hpp>

#include <vgp/nodes/nodebase.hpp>
#include <vgp/util/typeinfo.hpp>
#include <vgp/util/format.hpp>
#include <vgp/util/accesscontrol.hpp>

namespace vgp {

struct Organism {
	/// Default constructor, resets fitness to 0
	Organism() : hasfitness(false), fitness(0) {}
	
	/// Copy constructor uses assignment operator for actual copying
	Organism(const Organism &o) {
		*this=o;
	}
	
	/** \brief Create an organism with a given node tree
	 * 
	 * Creates a new organism and takes ownership of the given node tree
	 */ 
	Organism(detail::NodeBase* r) : hasfitness(false), fitness(0) {
		detail::NodePtr src(r);
		root.swap(src);
		//std::cout << "Org created with specified root: " << *r << std::endl;
	}
	
	/// Initialize all nodes in the tree
	inline void init() {if(root) root->init();}

	/// Get the result type of the organism (if a root exists)
	/// \throw Throws std::runtime_error if the organism hasn't been initialized with a node tree
	const std::type_info& getresulttype() const {
		if(root)
			return root->getresulttype();
		else
			throw std::runtime_error("getresulttype(): No root node");
	}
	
	/** \brief Execute the organism and return it's result
	 * 
	 * Builds the function object for the organism and executes it, returning the result.
	 * This will also update all the function buffers.
	 * \throw Throws boost::bad_any_cast if the organism's root doesn't return the type
	 * specified to the operator.  
	 */
	template <class T>
	T run() const {return root->getfunc<T>()();}
	
	/** \brief Get a function object returning the specified type
	 * 
	 * Allows for deferred execution of the program prepresented by the organism
	 * \sa run()
	 */ 
	template <class OUTPUT_TYPE>
	boost::function<OUTPUT_TYPE()> getfunc() const {return root->getfunc<OUTPUT_TYPE>();}
	
	/** \brief Get the organism's function object, wrapped in a boost::any object
	 * 
	 * Allows callers who are not statically-aware of the organism's result_type to obtain
	 * the function object and pass it along. Useful for building containers of functions, or
	 * handling the functions by intermediaries before they are eventually provided to a caller
	 * who is aware of their type and can do the appropriate boost::any_cast<>
	 */ 
	boost::any getfunc() const { return root->getfunc(); }
	
	/** Reset the organism, destroying any tree that might be present
	 * \post *this is an empty organism
	 */
	void reset() {root.reset();}
	
	/** Randomly generate the organism's tree
	 * \param t a type_info describing the type (as provided by typeid())  
	 * \return The number 
	 * \post The organism will have a tree returning a type matching t
	 * \throw node storage may throw if no appropriate node is found
	 */
	std::size_t generate(const std::type_info &t);
	
	/** @brief Mutate every node in the tree
	 * 
	 * Calls the node-specific mutate function for every node in the tree.
	 * @note This is not normally how organisms are mutated and is put here for testing
	 * purposes only.
	 */
	void mutateall() { if(root) mutateall(*root); }
	
	/// Returns the number of nodes in the tree (0 if there is no tree)
	std::size_t nodecount() const { if(root) return root->count(); else return 0; }
	
	/** @brief Returns the average depth of the tree
	 * 
	 * The average depth of a tree is the average of each terminal's depth
	 */
	double avgdepth() const;
	
	/** Set the organism's fitness level
	 * @sa getfitness
	 */
	void setfitness(double d) {fitness = d; hasfitness = true;}
	/** Get the organism's fitness level, set using setfitness()
	 * @sa setfitness
	 */
	double getfitness() const {return fitness;}

	/** Compare organisms based on their fitness
	 * @pre Fitness values must have been set for the organisms using setfitness.
	 * @sa setfitness
	 */
	bool operator<(const Organism& o) const {return fitness<o.fitness;}
	
	/// Assignment operator copies the tree (if present) and fitness value
	Organism& operator=(const Organism& o) {
		if(o.root) {
			detail::NodePtr newnode(o.root->clone());
			root.swap(newnode);
			fitness = o.fitness;
			hasfitness = true;
		}
		else {
			root.reset();
			fitness = 0;
			hasfitness = false;
		}
		return *this;
	}
	
	/// For internal testing use
	bool hasfitness;
protected:
	static std::size_t generate_recursive(detail::NodeBase*, std::size_t);
	void mutateall(detail::NodeBase&);
	void avgdepth(const detail::NodeBase&, std::size_t, std::list<std::size_t> &) const;
	
	detail::NodePtr root;
	double fitness;
	
	friend std::ostream& operator<<(std::ostream& os, const Organism &org) {return os<<*org.root;}
};

} // namespace vgp

#endif /*ORGANISM_HPP_*/
