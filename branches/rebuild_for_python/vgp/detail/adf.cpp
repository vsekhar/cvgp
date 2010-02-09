/*
 * adf.cpp
 *
 *  Created on: 2010-02-07
 */

#include <utility>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

#include <vgp/detail/adf.hpp>
#include <vgp/detail/nodestorage.hpp>
#include <vgp/util/random.hpp>

namespace vgp {
namespace detail {

NodeBase* expand_adf(const NodeBase& n) {
	const adf::ADF_base &adf = dynamic_cast<const adf::ADF_base&>(n);
	const adf::state_t tree_itr = adf.get();
	NodeBase* ret = tree_itr->root->clone();
	expand_adfs(*ret);
	return ret;
}

struct adfs_expander {
	void operator()(NodeBase& parent, children_t::iterator child_itr) {
		if(!child_itr->isADF()) return;
		parent.getchildren().replace(child_itr, expand_adf(*child_itr));
		expand_adfs(*child_itr);
	}
};

void expand_adfs(NodeBase& n) {
	adfs_expander x;
	walk_tree_children(n, x);
}

void expand_adfs(tree& t) {
	if(t.root->isADF()) {
		NodeBase* old_root = t.root;
		try {
			t.root = expand_adf(*t.root);
			delete old_root;
		}
		catch(...) {
			t.root = old_root;
			old_root = 0;
			throw;
		}
	}
	else {
		expand_adfs(*t.root);
	}
}

struct parent_child_gatherer {
	typedef std::pair<NodeBase*, children_t::iterator> entry_t;
	std::vector<entry_t> result;
	void operator()(NodeBase& parent, children_t::iterator child_itr) {
		result.push_back(std::make_pair(&parent, child_itr));
	}
};

struct tree_modifier {
	tree_modifier(NodeBase* n, children_t::iterator c, NodeBase* a, NodeBase*& o)
		: node_to_change(n), child_itr(c), adf_node(a), old_tree(o) {}
	void operator()(tree& t) {
		children_t &children = node_to_change->getchildren();
		old_tree = children.replace(child_itr, adf_node).release();
	}
	NodeBase* node_to_change;
	children_t::iterator child_itr;
	NodeBase* adf_node;
	NodeBase*& old_tree;
};

struct tree_root_setter {
	tree_root_setter(NodeBase* n) : new_root(n) {}
	void operator()(tree& t) {
		t.root = new_root;
	}
	NodeBase* new_root;
};

// pick a random node
void make_random_adf(Trees::iterator itr, Trees &trees) {
	parent_child_gatherer g;
	walk_tree_children(*itr, g); // excludes root
	std::size_t count = g.result.size();
	std::size_t index = boost::uniform_int<>(0, count)(util::default_generator);
	if(index < count) {
		// something below the root was chosen
		parent_child_gatherer::entry_t entry = g.result[index];

		const NodeEntry& ne = node_entry(entry.first->getpointer());
		NodeBase* adf_node = ne.prototype_adf->clone();
		NodeBase* old_node;
		tree_modifier m(entry.first, entry.second, adf_node, old_node);
		trees.modify(itr, m);

		Trees::const_iterator new_tree_itr;
		boost::tie(new_tree_itr, boost::tuples::ignore)
			= trees.push_back(tree(old_node));

		adf::ADF_base* adf_base = dynamic_cast<adf::ADF_base*>(adf_node);
		adf_base->set(new_tree_itr);
	}
	else if (index == count) {
		// the root was chosen
		const NodeEntry& ne = node_entry(itr->root->getpointer());
		NodeBase* adf_node = ne.prototype_adf->clone();
		Trees::const_iterator new_tree_itr;
		boost::tie(new_tree_itr, boost::tuples::ignore)
			= trees.push_back(tree(itr->root));
		adf::ADF_base* adf_base = dynamic_cast<adf::ADF_base*>(adf_node);
		adf_base->set(new_tree_itr);
		trees.modify(itr, tree_root_setter(adf_node)); // todo: segfaults
	}
	else // logic error
		BOOST_ASSERT(0);
}

// pick a random tree
void make_random_adf(Trees &trees) {
	std::vector<std::size_t> sizes;
	std::size_t sum = 0;
	BOOST_FOREACH(const tree& t, trees) {
		std::size_t cur_count = t.count();
		sizes.push_back(cur_count);
		sum += cur_count;
	}
	std::size_t rand = boost::uniform_int<>(0, sum-1)(util::default_generator);
	std::size_t search_sum = 0;
	std::size_t index;
	for(index=0; index < sizes.size(); index++) {
		search_sum += sizes[index];
		if(search_sum >= rand)
			break;
	}
	Trees::iterator itr = trees.begin();
	std::advance(itr, index);
	make_random_adf(itr, trees);
}

} // namespace detail
} // namespace vgp
