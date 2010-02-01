/*
 * tree.cpp
 *
 *  Created on: 2010-01-31
 */

#include <stdexcept>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <vgp/detail/tree.hpp>
#include <vgp/detail/node.hpp>
#include <vgp/detail/nodestorage.hpp>
#include <vgp/util/random.hpp>
#include <vgp/defaults.hpp>

namespace vgp {
namespace detail {

struct TreeGenerateFailed : virtual std::exception {};
struct MaxDepthReached : virtual std::exception {};

NodeBase* generate_recursive(const util::TypeInfo &t, std::size_t depth) {

	// some checks
	if(depth >= VGP_MAX_DEPTH)
		throw MaxDepthReached();
	std::size_t count = nodesbyresulttype.count(t);
	if(count == 0)
		throw TreeGenerateFailed();

	// create adjusted probability vector
	std::vector<double> probs(count);
	double depth_ratio = (depth+1) / VGP_MAX_DEPTH;
	for(std::size_t i = 0; i < count; i++) {
		double i_ratio = double(i)/count;
		probs.push_back(std::pow(depth_ratio, i_ratio));
	}

	// sum and normalize
	double sum = 0;
	BOOST_FOREACH(const double& d, probs) sum+=d;
	BOOST_FOREACH(double& d, probs) d/=sum;

	// get iterators
	NodesByResultType::const_iterator beg;
	NodesByResultType::const_iterator end;
	boost::tie(beg, end) = nodesbyresulttype.equal_range(t);

	// find a random one
	double threshold = boost::uniform_real<>(0,sum)(util::default_generator);
	double search_sum = 0;
	std::size_t index = 0;
	BOOST_FOREACH(const double& d, probs) {
		search_sum += d;
		if(search_sum >= threshold)
			break;
		else
			++index;
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


} // namespace detail
} // namespace vgp
