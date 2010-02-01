/*
 * organism.hpp
 *
 *  Created on: 2010-01-31
 *      Author: vsekhar
 */

#ifndef ORGANISM_HPP_
#define ORGANISM_HPP_

#include <vector>
#include <vgp/detail/node.hpp>
#include <vgp/detail/tree.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {

struct Organism {
	template <typename T>
	T run() const {
		const detail::NodeBase* r = root.root;
		return reinterpret_cast<detail::Node_returning<T>* >(r)->run_node();
	}

	void init();

private:
	void do_init(detail::NodeBase&);
	std::vector<std::size_t> adfs(const util::TypeInfo&) const;

	detail::tree root;
	detail::tree select;
	detail::tree mate;
	detail::NodeVector adf;
};

} // namespace vgp


#endif /* ORGANISM_HPP_ */
