/*
 * generate.cpp
 *
 *  Created on: 2010-02-07
 */

#include <boost/foreach.hpp>
#include <boost/python.hpp>

#include <cvgp/defaults.hpp>
#include <cvgp/detail/generate.hpp>
#include <cvgp/detail/nodebase.hpp>
#include <cvgp/detail/nodestorage.hpp>
#include <cvgp/detail/trees.hpp>
#include <cvgp/detail/adf.hpp>
#include <cvgp/util/random.hpp>
#include <cvgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

// return an index [0,end) with an exponentially declining probability
// curve across the possible values (steepness==0 means flat curve)
std::size_t probability_curve(std::size_t end, double steepness) {

	using std::exp;

	/**********************************************************
	 * Using partial sum of e^x:
	 * sum_(k=0)^ne^(-k x) = (e^(n (-x)) (e^(n x+x)-1))/(e^x-1)
	 * http://www.wolframalpha.com/input/?i=sum+of+e^%28-k*x%29
	 **********************************************************/
	const signed int m = end - 1;
	const double sum =
		(exp((-m)*steepness) * (exp((m+1) * steepness) - 1)) / (exp(steepness) - 1);
	const double threshold = boost::uniform_real<>(0,sum)(util::default_generator);

	double search_sum = 0;
	std::size_t i;
	for(i = 0; i < end ; i++) {
		search_sum += exp(signed(-i)*steepness);
		if(search_sum >= threshold) break; // must be '>=', not just '>'
	}

	return i;
}

NodeBase* generate(
		util::TypeInfo t,
		const Trees& trees,
		const Trees::iterator& cur_tree,
		std::size_t depth)
{
	if(depth >= VGP_MAX_DEPTH)
		throw MaxDepthReached("generate reached max depth");

	std::size_t regular_count = nodesbyresulttype.count(t);
	std::size_t adf_count;
	if(cur_tree == trees.end())
		adf_count = 0; // nothing there, we must be creating the first tree
	else
		adf_count = trees.byresulttype.count(t) - 1; // at least 1 is in there (the cur_tree)
	std::size_t count = regular_count + adf_count;
	std::size_t index = probability_curve(count, (depth+1) * VGP_DEPTH_FACTOR);

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

void pyexport_generate() {
	using namespace boost::python;
	def("pcurve", probability_curve);
	class_<GenerateError>("GenerateError", no_init);
}

} // namespace detail
} // namespace vgp
