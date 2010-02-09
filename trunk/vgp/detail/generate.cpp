/*
 * generate.cpp
 *
 *  Created on: 2010-02-07
 */

#include <boost/foreach.hpp>

#include <vgp/defaults.hpp>
#include <vgp/detail/generate.hpp>
#include <vgp/detail/nodebase.hpp>
#include <vgp/detail/nodestorage.hpp>
#include <vgp/detail/trees.hpp>
#include <vgp/detail/adf.hpp>
#include <vgp/util/random.hpp>
#include <vgp/util/typeinfo.hpp>

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
		throw GenerateError();
	}

	return index;
}

NodeBase* generate(
		util::TypeInfo t,
		const Trees& trees,
		const Trees::iterator& cur_tree,
		std::size_t depth)
{
	if(depth >= VGP_MAX_DEPTH)
		throw MaxDepthReached();

	std::size_t regular_count = nodesbyresulttype.count(t);
	std::size_t adf_count;
	if(cur_tree == trees.end())
		adf_count = 0; // nothing there, we must be creating the first tree
	else
		adf_count = trees.byresulttype.count(t) - 1; // at least 1 is in there (the cur_tree)
	std::size_t count = regular_count + adf_count;
	std::size_t index = probability_curve(count, depth * VGP_DEPTH_FACTOR);

	NodeBase *ret;
	if(index < adf_count) {
		TreesByResultType::const_iterator beg = trees.byresulttype.lower_bound(t);
		std::advance(beg, index);
		Trees::iterator proj_itr = trees.project<bySequence>(beg);
		ret = node_entry(proj_itr->root).prototype_adf->clone();
		dynamic_cast<adf::ADF_base*>(ret)->set(proj_itr);
	}
	else {
		NodesByResultType::const_iterator beg = nodesbyresulttype.lower_bound(t);
		std::advance(beg, index - adf_count);
		ret = beg->prototype->clone();
		if(!beg->terminal()) {
			NodeVector &children = ret->getchildren();
			BOOST_FOREACH(const util::TypeInfo& t, beg->parameter_types)
				children.push_back(generate(t, trees, cur_tree, depth+1));
		}
	}
	return ret;
}

} // namespace detail
} // namespace vgp
