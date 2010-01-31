/*
 * organism.hpp
 *
 *  Created on: 2010-01-31
 *      Author: vsekhar
 */

#ifndef ORGANISM_HPP_
#define ORGANISM_HPP_

#include <vgp/detail/node.hpp>

namespace vgp {

struct Organism {
	template <typename T>
	T run() const {
		return reinterpret_cast<detail::Node_returning<T>* >(trees[0])->run_node();
	}

	void init();

private:
	void do_init(detail::NodeBase&);

	detail::NodeVector trees;
};

} // namespace vgp


#endif /* ORGANISM_HPP_ */
