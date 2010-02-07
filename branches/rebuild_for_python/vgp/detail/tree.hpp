/*
 * tree.hpp
 *
 *  Created on: 2010-01-31
 *      Author: vsekhar
 */

#ifndef TREE_HPP_
#define TREE_HPP_

//#include <stdexcept>
#include <ostream>
//#include <string>
#include <vgp/detail/nodebase_fwd.hpp>
//#include <vgp/detail/nodestorage.hpp>
#include <vgp/util/typeinfo.hpp>
//#include <boost/assert.hpp>

namespace vgp {
namespace detail {

struct tree {
	tree(const tree&);
	tree(NodeBase*);
	~tree();

	void init();
	void mutate();

	NodeBase* root;
};

util::TypeInfo result_type(const tree&);
std::ostream& operator<<(std::ostream&, const NodeBase&);
std::ostream& operator<<(std::ostream&, const tree&);

void pyexport_tree();

} // namespace detail
} // namespace vgp


#endif /* TREE_HPP_ */
