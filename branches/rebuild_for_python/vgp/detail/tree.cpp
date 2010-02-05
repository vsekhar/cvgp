/*
 * tree.cpp
 *
 *  Created on: 2010-01-31
 */

#include <iostream>
#include <cmath>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/python.hpp>
#include <vgp/detail/tree.hpp>
#include <vgp/detail/node.hpp>
#include <vgp/detail/nodestorage.hpp>
#include <vgp/util/random.hpp>
#include <vgp/defaults.hpp>

using std::cout;
using std::endl;

namespace vgp {
namespace detail {

NodeBase* generate_recursive(const util::TypeInfo &t, std::size_t depth) {

	// some checks
	if(depth >= VGP_MAX_DEPTH)
		throw MaxDepthReached();
	std::size_t count = nodesbyresulttype.count(t);
	if(count == 0)
		throw TreeGenerateFailed();

	// create adjusted probability vector and normalize
	std::vector<double> probs(count);
	const double base_prob = 1.0/count;
	const double steepness = depth * VGP_DEPTH_FACTOR;
	signed int i(0);
	const signed int c(count);
	for(; i < c ; i++)
		probs[i] = base_prob * std::exp((-i)*steepness);
	double sum = 0;
	BOOST_FOREACH(const double& d, probs) sum+=d;
	BOOST_FOREACH(double& d, probs) d/=sum;

	// get iterators
	NodesByResultType::const_iterator beg;
	NodesByResultType::const_iterator end;
	boost::tie(beg, end) = nodesbyresulttype.equal_range(t);

	// find a random one
	const double threshold = boost::uniform_real<>(0,sum)(util::default_generator);
	double search_sum = 0;
	std::size_t index = 0;
	BOOST_FOREACH(const double& d, probs) {
		search_sum += d;
		if(search_sum >= threshold)
			break;
		else
			++index;
	}
	if(index > count-1) {
		cout << "Index > count (" << index << ", " << count << ") when looking for '" << t
				<< "' with random " << threshold << endl;
		BOOST_FOREACH(const double &d, probs)
			cout << d << ", ";
		cout << endl;
		throw TreeGenerateFailed();
	}
	for(std::size_t i = 0; i < index; i++)
		++beg;

	// create and populate children
	NodeBase* ret = beg->prototype->clone();
	if(!beg->terminal()) {
		NodeVector &children = ret->getchildren();
		BOOST_FOREACH(const util::TypeInfo& t, beg->parameter_types)
			children.push_back(generate_recursive(t, depth+1));
	}
	return ret;
}

NodeBase* generate(const util::TypeInfo& t) {return generate_recursive(t,0);}

std::ostream& operator<<(std::ostream& o, const NodeBase& n) {
	const Node_w_ptr* node = reinterpret_cast<const Node_w_ptr*>(&n);
	void_fptr_t fptr = node->fptr;
	NodesByFptr::const_iterator itr = nodesbyfptr.find(fptr);
	if(itr == nodesbyfptr.end()) return o;
	o << itr->name << "[" << itr->result_type << "](";
	if(itr->arity) {
		const children_t& c = node->getchildren();
		BOOST_FOREACH(const NodeBase& n, c)
			o << n;
	}
	o << ")";
	return o;
}

std::ostream& operator<<(std::ostream& o, const tree& t) {
	return o << *t.root;
}

void init_tree(NodeBase& n) {
	n.init();
	try {
		NodeVector& children = n.getchildren();
		BOOST_FOREACH(detail::NodeBase& child, children)
			init_tree(child);
	}
	catch(NoChildren) {}
}

void init_tree(tree& t) {init_tree(*t.root);}

void pyexport_tree() {
	using namespace boost::python;
	class_<tree>("tree", no_init)
			.def(self_ns::str(self)) // gcc hiccups without the namespace here
			;
}

} // namespace detail
} // namespace vgp
