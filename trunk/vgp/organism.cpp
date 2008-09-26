#include <stdexcept>
#include <list>
#include <iostream>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include <vgp/util/random.hpp>

#include "organism.hpp"
#include "nodes/nodecontainer.hpp"

using std::cout;
using std::endl;

using vgp::detail::NodeBase;
using vgp::util::TypeInfo;

namespace vgp {

const std::type_info& Organism::getresulttype() const {
	if(root.get())
		return root->getresulttype();
	else
		throw std::runtime_error("Organism::getresulttype(): No root node");
}

std::size_t Organism::generate(const std::type_info& t) {
	if(!root.get() || root->getresulttype() != t) {
		root.reset(Nodes.getrandomnode(t, 0));
	}
	validfitness_ = false;
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
	validfitness_ = false;
}

bool Organism::crossover(Organism &other) {
	bool output = false;

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
	boost::uniform_int<unsigned int> random2(0, ntm1.size()-1);
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
	if(n1_ptr == NULL) {
		if(n2_ptr == NULL) {
			NodeBase *holder = other.root.release();
			other.root.reset(root.release());
			root.reset(holder);
		}
		else {
			NodeBase *holder = &n2_ptr->children[n2_idx];
			n2_ptr->children.base()[n2_idx] = root.release();
			root.reset(holder);
		}
	}
	else if(n2_ptr == NULL) {
		NodeBase *holder = &n1_ptr->children[n1_idx];
		n1_ptr->children.base()[n1_idx] = other.root.release();
		other.root.reset(holder);
	}
	else {
		NodeBase *holder = &n2_ptr->children[n2_idx];
		n2_ptr->children.base()[n2_idx] = n1_ptr->children.base()[n1_idx];
		n1_ptr->children.base()[n1_idx] = holder;
	}


	validfitness_ = false;
	other.validfitness_ = false;

	return true;
}

Organism& Organism::operator=(const Organism& o) {
	if(o.root.get()) {
		root.reset(o.root->clone());
		fitness = o.fitness;
		validfitness_ = o.validfitness_;
	}
	else {
		root.reset();
		fitness = 0;
		validfitness_ = false;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Organism &org) {
	os << "organism{";
	if(org.root.get()) os << *org.root;
	else os << "empty";
	os << "}";
	return os;
}

} // namespace vgp
