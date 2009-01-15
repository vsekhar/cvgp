#include <string>
#include <sstream>

#include "nodebase.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>

namespace vgp {
namespace detail {

using namespace boost::lambda;

std::size_t NodeBase::count() const {
	std::size_t ret = 1;
	BOOST_FOREACH(const NodeBase &child, children)
		ret += child.count();
	return ret;
}

std::string NodeBase::getID() const {
	std::string ret;
	ret += name();
	ret += "[";
	ret += getresulttype().name();
	ret += "](";
	BOOST_FOREACH(const util::TypeInfo &curtype, getparamtypes()) {
		ret += curtype.get().name();
		ret += ",";
	}
	if(*ret.rbegin() == ',')
		ret.erase(ret.size()-1, 1);	// remove trailing comma
	ret += ")";
	return ret;
}

bool NodeBase::complete() const {
	if(children.size() != _arity) return false;
	BOOST_FOREACH(const NodeBase &child, children)
		if(!child.complete()) return false;;
	return true;
}

void NodeBase::init() {
	do_init();
	BOOST_FOREACH(NodeBase &child, children)
		child.init();
}
void NodeBase::mutate() {
	do_mutate();
	BOOST_FOREACH(NodeBase &child, children)
		child.mutate();
}

std::ostream& operator<<(std::ostream& o, const NodeBase& n) {
	o << n.name() << "[" << n.result_type.get().name();
	if(!n.param_types.empty()) {
		BOOST_FOREACH(const util::TypeInfo &curtype, n.param_types)
			o << "," << curtype;
	}
	o << "]";

	if(n.children.empty()) return o;
	o << "(";
	BOOST_FOREACH(const NodeBase& child, n.children)
		o << "," << child;
	o << ")";
	return o;
}

void delete_clone(const NodeBase *n) {delete n;}

} // namespace detail
} // namespace vgp
