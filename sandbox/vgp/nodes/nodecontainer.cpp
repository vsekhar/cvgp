#include "nodecontainer.hpp"

#include <stdexcept>
#include <sstream>

#include <boost/tuple/tuple.hpp>
#include <boost/assert.hpp>

#include <vgp/util/random.hpp>
#include <vgp/util/rounder.hpp>

namespace vgp {

NodeContainer Nodes;

// TODO: fix the lookups here to check nodes and terminals when appropriate
// TODO: make all functions throw exceptions instead of return null 

void NodeContainer::push_back(const detail::NodeBase* n) {
	if(n->isterminal())
		terminals.push_back(n);
	else
		nodes.push_back(n);
}

detail::NodeBase* NodeContainer::getnode(const std::string& str) const {
	NodesByID_t::const_iterator i = nodesbyID.find(str);
	if(i == nodesbyID.end()) {
		i = terminalsbyID.find(str);
		if(i == terminalsbyID.end())
			return 0;
		else
			return (*i)->clone();
	}
	else
		return (*i)->clone();
}

detail::NodeBase* NodeContainer::getrandomnode(const std::type_info& t, std::size_t depth) const {
	std::size_t nodecount = nodesbyresulttype.count(util::TypeInfo(t));
	std::size_t terminalcount = terminalsbyresulttype.count(util::TypeInfo(t));
	if(!nodecount && !terminalcount)
		throw std::invalid_argument(std::string("Bad type given for random lookup: ")+t.name());
	
	bool terminal;
	std::size_t index;
	boost::tie(terminal, index) = weightedpick(nodecount, terminalcount, depth);
	NodesByResultType_t::const_iterator cur, end;
	if(terminal)
		boost::tie(cur, end) = terminalsbyresulttype.equal_range(util::TypeInfo(t));
	else
		boost::tie(cur, end) = nodesbyresulttype.equal_range(util::TypeInfo(t));
	if(cur==end)
		throw std::invalid_argument(std::string("Bad type given for random lookup: ")+t.name());
	for(unsigned int i = 0; i < index; i++, cur++);
	return (*cur)->clone();
}
detail::NodeBase* NodeContainer::getrandomnode(const std::string& name, std::size_t depth) const {
	std::size_t nodecount = nodesbyname.count(name);
	if(!nodecount)
		return 0;
	NodesByName_t::const_iterator cur, end;
	boost::tie(cur, end) = nodesbyname.equal_range(name);
	util::RandomRangedInt random(0, nodecount-1);
	unsigned int chosennode = random();
	for(std::size_t i = 0; i < chosennode; i++) cur++;
	return (*cur)->clone();
}

const std::type_info& NodeContainer::gettypeinfo(const std::string& name) const {
	NodesByResultTypeName_t::const_iterator i;
	i = nodesbyresulttypename.find(name);
	if(i == nodesbyresulttypename.end()) {
		i = terminalsbyresulttypename.find(name);
		if(i == terminalsbyresulttypename.end()) {
			std::stringstream ss;
			ss << "Error: cannot find node or terminal with result type named '" << name << "'";
			throw std::invalid_argument(ss.str());
		}
	}
	return (*i)->getresulttype();
}

/** \f[
 * 		ratio=1-(0.5\times depthpenalty^depth)
 * \f]
 * \f[
 * 		p(node)=1+\frac{nodecount\times(1-ratio)}{terminals\times ratio}
 * \f]
 * \f[
 * 		p(terminal)=1+\frac{terminalcount\times ratio}{nodecount\times (1-ratio)}
 * \f]
 * A binary random variable is used to select between the two pools, then an integer random
 * variable, ranged [0,1-pool_size], is used to select the index. 
 * \param nodecount number of nodes to choose from
 * \param terminalcount number of terminals to choose from
 * \param depth depth for which the node is requested (requests for root nodes have depth of 0)
 * \return a tuple of two elements:
 * \return the bool is true if the second pool is selected (terminals), false for the first (nodes)
 * \return index within the selected pool of the selected node 
 */
boost::tuple<bool, std::size_t>
NodeContainer::weightedpick(std::size_t nodecount, std::size_t terminalcount, std::size_t depth) const {
	double depth_d = depth;
	double ratio = 1-(0.5*std::pow(depthpenalty,depth_d));

	BOOST_ASSERT(ratio >= 0 && ratio <= 1);
	// Get natural probabilities (based on proportions of nodes and terminals)
	double p_node = (0.0+nodecount) / (nodecount + terminalcount);
	double p_terminal = (0.0+terminalcount) / (nodecount + terminalcount);
	
	// Weight probabilities based on ratio:
	//	ratio == 0: always nodes
	// 	ratio == 0.5: nodes and terminals in proportion to each of their number
	// 	ratio == 1: always terminals
	p_node *= (1-ratio);
	p_terminal *= ratio;
	
	// Normalize
	double p_total = p_node + p_terminal;
	p_node /= p_total;
	p_terminal /= p_total;
	
	util::RandomBool random;
	if(random(p_node) && nodecount) {
		//node
		util::RandomRangedInt random_int(0, nodecount-1);
		return boost::make_tuple(false, random_int());
	}
	else if(terminalcount) {
		//terminal
		util::RandomRangedInt random_int(0, terminalcount-1);
		return boost::make_tuple(true, random_int());
	}
	else
		throw std::invalid_argument("weightedpick: Failed to find random node");
}

std::ostream& operator<<(std::ostream &o, const NodeContainer& container) {
	NodeContainer::NodesBySequence_t::const_iterator itr
		= container.nodes.begin();
	for( ; itr != container.nodes.end(); itr++) {
		o << (*itr)->getID() << std::endl;
	}
	itr = container.terminals.begin();
	for( ; itr != container.terminals.end(); itr++) {
		o << (*itr)->getID() << std::endl;
	}
	return o;
}


} // namespace vgp
