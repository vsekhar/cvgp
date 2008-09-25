#include <stdexcept>

#include <boost/assert.hpp>

#include <vgp/util/random.hpp>

#include "organism.hpp"
#include "nodes/nodecontainer.hpp"

namespace vgp {

const std::type_info& Organism::getresulttype() const {
	if(root)
		return root->getresulttype();
	else
		throw std::runtime_error("Organism::getresulttype(): No root node");
}

std::size_t Organism::generate(const std::type_info& t) {
	if(!root || root->getresulttype() != t) {
		detail::NodePtr newroot(Nodes.getrandomnode(t, 0));
		root.swap(newroot);
	}
	return 3 + generate_recursive(root.get(), 0, Nodes);
}

std::size_t Organism::generate_recursive(detail::NodeBase* curnode, std::size_t depth, const NodeContainer& nodes) {
	curnode->clearchildren();
	detail::NodeBase::ChildrenContainer &curchildren = curnode->children;
	util::TypeInfoVector paramtypes = curnode->getparamtypes();
	std::size_t ret = 0;
	for(std::size_t i = 0 ; i < paramtypes.size(); i++) {
		detail::NodeBase* newnode = nodes.getrandomnode(paramtypes[i], depth);
		if(!newnode)
			throw std::invalid_argument("Error finding node to allocate");
		curchildren.push_back(newnode);
		ret += 1 + generate_recursive(newnode, depth+1, nodes);
	}
#ifdef __DEBUG__
	BOOST_ASSERT(curchildren.size() == paramtypes.size());
#endif
	return ret;
}

double Organism::avgdepth() const {
	if(!root.get()) return 0;
	std::list<std::size_t> depths;
	avgdepth(*root.get(), 1, depths);
	std::list<std::size_t>::const_iterator i = depths.begin();
	double accum = 0;
	for( ; i != depths.end(); i++)
		accum += *i;
	return accum/depths.size();
}

void Organism::avgdepth(const detail::NodeBase& curnode, std::size_t curdepth, std::list<std::size_t> &depths) const {
	if(curnode.arity() == 0)
		depths.push_back(curdepth);
	else {
		const detail::NodeBase::ChildrenContainer &vec = curnode.children;
		detail::NodeBase::ChildrenContainer::const_iterator i = vec.begin();
		for( ; i != vec.end(); i++)
			avgdepth(*i, curdepth+1, depths);
	}
}

void Organism::mutateall(detail::NodeBase& curnode) {
	curnode.mutate();
	detail::NodeBase::ChildrenContainer &curchildren = curnode.children;
	detail::NodeBase::ChildrenContainer::iterator i = curchildren.begin();
	for( ; i != curchildren.end(); i++)
		mutateall(*i);
}

bool Organism::crossover(Organism &other) {
	// Gather maps of return types
	NodeTypeMap ntm1, ntm2;
	gathertypes(ntm1);
	other.gathertypes(ntm2);

	// Filter second map for types not in first
	std::list<TypeInfo> types_to_remove;
	BOOST_FOREACH(const NodeTypeEntry & t, ntm1) {
		if(ntm2.find(t.first) == ntm2.end())
			types_to_remove.push_back(t.first);
	}
	types_to_remove.sort();
	types_to_remove.unique();
	BOOST_FOREACH(const TypeInfo& t, types_to_remove) {
		ntm1.erase(t);
	}

	// ntm1 now contains only nodes with types in ntm2

	BOOST_ASSERT(!ntm1.empty()); // they must at least share a root type

	// Find a node from ntm1
	boost::uniform_int<unsigned int> random2(0, ntm1.size());
	unsigned int index = random2(util::default_generator);
	NodeTypeMap::iterator node1 = ntm1.begin();
	std::advance(node1, index);

	// Find a node from ntm2 with matching type
	NodeTypeMap::iterator node2, end;
	boost::tie(node2, end) = ntm2.equal_range(node1->first);
	unsigned int count = ntm2.count(node1->first);
	BOOST_ASSERT(node2 != end);	// must find something
	BOOST_ASSERT(count > 0);
	boost::uniform_int<unsigned int> random3(0, count-1);
	unsigned int randomindex = random3(util::default_generator);
	std::advance(node2, randomindex);
	BOOST_ASSERT(node2 != end);

	// Perform crossover
	detail::NodeBase* n1_ptr = node1->second.first;
	unsigned int n1_idx = node1->second.second;
	detail::NodeBase* n2_ptr = node2->second.first;
	unsigned int n2_idx = node2->second.second;
	if(n1_ptr == root.get()) {
		if(n2_ptr == other.root.get())
		std::swap(root.get(), n2_ptr->children[n2_idx]);
	}
	else if(n2_ptr);

	// TODO: Convert this to iterators (change the contents of NodeTypeMap)
	//		and then use std::swap(itr1.base(), itr2.base())
}

Organism& Organism::operator=(const Organism& o) {
	if(o.root) {
		detail::NodePtr newnode(o.root->clone());
		root.swap(newnode);
		fitness = o.fitness;
	}
	else {
		root.reset();
		fitness = 0;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Organism &org) {
	os << "organism{";
	if(org.root) os << *org.root;
	else os << "empty";
	os << "}";
	return os;
}

} // namespace vgp
