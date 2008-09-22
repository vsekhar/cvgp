#include <stdexcept>

#include <boost/assert.hpp>

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
	detail::NodeBase::ptr_vector &curchildren = curnode->children;
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
		const detail::NodeBase::ptr_vector &vec = curnode.children;
		detail::NodeBase::ptr_vector::const_iterator i = vec.begin();
		for( ; i != vec.end(); i++)
			avgdepth(*i, curdepth+1, depths);
	}
}

void Organism::mutateall(detail::NodeBase& curnode) {
	curnode.mutate();
	detail::NodeBase::ptr_vector &curchildren = curnode.children;
	detail::NodeBase::ptr_vector::iterator i = curchildren.begin();
	for( ; i != curchildren.end(); i++)
		mutateall(*i);
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
