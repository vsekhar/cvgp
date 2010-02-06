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

// return an index [0,max_index) with an exponentially declining probability
// curve across the possible values (steepness==0 means flat curve)
std::size_t probability_curve(std::size_t max_index, double steepness) {

	const double base_prob = 1.0 / max_index;
	signed int i(0); // prevent signed/unsigned warnings
	const signed int m(max_index);

	// create probability vector
	std::vector<double> probs(max_index);
	double sum = 0;
	for(; i < m ; i++) {
		probs[i] = base_prob * std::exp((-i)*steepness);
		sum += probs[i];
	}

	// choose
	const double threshold = boost::uniform_real<>(0,sum)(util::default_generator);
	double search_sum = 0;
	std::size_t index = 0;
	BOOST_FOREACH(const double& d, probs) {
		search_sum += d;
		if(search_sum >= threshold) break; // must be '>=', not just '>'
		else ++index;
	}

	if(index > max_index-1) {
#ifdef _DEBUG
		cout << "Index: " << index << ", max_index: " << max_index
				<< ", random: " << threshold << " from " << sum << endl;
		cout << "Vector: ";
		BOOST_FOREACH(const double &d, probs)
			cout << d << ", ";
		cout << endl;
#endif
		throw TreeGenerateFailed();
	}

	return index;
}

NodeBase* generate_recursive(const util::TypeInfo &t, std::size_t depth) {

	// some checks
	if(depth >= VGP_MAX_DEPTH)
		throw MaxDepthReached();
	std::size_t count = nodesbyresulttype.count(t);
	if(count == 0)
		throw TreeGenerateFailed();

	// get first matching node, advance by chosen index
	NodesByResultType::const_iterator beg = nodesbyresulttype.lower_bound(t);
	std::size_t index = probability_curve(count, depth * VGP_DEPTH_FACTOR);
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
	const Node_w_ptr* node = static_cast<const Node_w_ptr*>(&n);
	void_fptr_t fptr = node->fptr;
	NodesByFptr::const_iterator itr = nodesbyfptr.find(fptr);
	if(itr == nodesbyfptr.end()) return o;
	o << itr->name << "[" << itr->result_type << "](";
	if(itr->arity) {
		BOOST_FOREACH(const NodeBase& n, node->getchildren())
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
