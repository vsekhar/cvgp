/*
 * organism.cpp
 *
 *  Created on: 2010-01-31
 */

#include <boost/foreach.hpp>
#include <vgp/organism.hpp>

namespace vgp {

void Organism::init() {
	BOOST_FOREACH(detail::NodeBase& n, trees)
			do_init(n);
}

void Organism::do_init(detail::NodeBase& node) {
	node.init();
	try {
		detail::NodeVector& children = node.getchildren();
		BOOST_FOREACH(detail::NodeBase& n, children)
			do_init(n);
	}
	catch(detail::NoChildren) {}
}

} // namespace vgp
