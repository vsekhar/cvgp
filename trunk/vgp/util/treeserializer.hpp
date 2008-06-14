#ifndef TREESERIALIZER_HPP_
#define TREESERIALIZER_HPP_

#include <vgp/nodes/nodebase.hpp>
#include <vgp/nodes/terminalbase.hpp>

namespace vgp {
namespace detail {

struct TreeSerializer {
	template <class Archive>
	static void save_recursive(Archive& ar, detail::NodeBase* curnode) {
		const std::string id = curnode->getID();
		ar << id;
		if(curnode->hasstate()) {
			detail::TerminalBase_savable<Archive> *castednode = 
			dynamic_cast<typename detail::TerminalBase_savable<Archive>*>(curnode);
			if(!castednode) {
				std::cerr << "Failed casting node while saving: " << curnode->getID() << std::endl;
				exit(1);
			}
			castednode->save_state(ar);
		}
		detail::NodeBase::ptr_vector::iterator itr = curnode->children.begin();
		for( ; itr != curnode->children.end(); itr++)
			save_recursive(ar, &(*itr));
	}
	
	// load_node is factored out of load_recursive because the loading of a single
	// node is separately needed for loading the root of an organism
	template <class Archive>
	static detail::NodeBase* load_node(Archive &ar) {
		std::string newnodeid;
		ar >> newnodeid;
		detail::NodeBase* newnode = vgp::Nodes.getnode(newnodeid);
		if(!newnode) {
			std::cerr << "ERROR: tree serializer could not create node" << std::endl;
			throw std::exception();
		}
		if(newnode->hasstate()) {
			detail::TerminalBase_loadable<Archive> *castednode = 
			dynamic_cast<typename detail::TerminalBase_loadable<Archive>*>(newnode);
			if(!castednode) {
				std::cerr << "Failed casting node while loading: " << newnode->getID() << std::endl;
				exit(1);
			}
			castednode->load_state(ar);
		}
		return newnode;
	}
	template <class Archive>
	static void load_recursive(Archive &ar, detail::NodeBase* curnode) {
		for(unsigned int i = 0; i < curnode->arity(); i++) {
			detail::NodeBase* newnode = load_node(ar);
			curnode->children.push_back(newnode);
			load_recursive(ar, newnode);
		}
	}
};

} // end namespace detail
} // end namespace vgp

#endif /*TREESERIALIZER_HPP_*/
