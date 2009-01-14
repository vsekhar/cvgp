#include <string>
#include <sstream>

#include "nodebase.hpp"

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

namespace vgp {
namespace detail {

using namespace boost::lambda;

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

void NodeBase::deepcopychildren(const NodeBase& nb) {
	clearchildren();
	BOOST_FOREACH(const NodeBase &srcchild, nb.children)
		children.push_back(srcchild.clone());
}

bool NodeBase::complete() const {
	bool ret = (children.size()==_arity);
	if(!ret) return false;
	BOOST_FOREACH(const NodeBase &child, children)
		ret = ret && child.complete();
	return ret;
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

//NodeBase* new_clone(const NodeBase &n) {return n.clone();}

} // namespace detail
} // namespace vgp
