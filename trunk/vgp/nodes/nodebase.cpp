#include <sstream>

#include "nodebase.hpp"

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

namespace vgp {
namespace detail {

using namespace boost::lambda;

std::string NodeBase::getID() const {
	std::string ret;
	std::string nodename = name();
	ret += nodename;
	ret += "[";
	const std::type_info& t = getresulttype();
	std::string tname = t.name();
	ret += tname;
	ret += "](";
	util::TypeInfoVector types = getparamtypes();
	for(unsigned int i = 0; i < types.size(); i++) {
		ret += types[i].get().name();
		if((i+1) != types.size()) ret += ",";		
	}
	ret += ")";
	return ret;
}

void NodeBase::deepcopychildren(const NodeBase& nb) {
	clearchildren();
	const ptr_vector &srcchildren = nb.children;
	ptr_vector &newchildren = children;
	ptr_vector::const_iterator s = srcchildren.begin();
	for( ; s != srcchildren.end(); s++)
		newchildren.push_back(s->clone());
}

std::ostream& operator<<(std::ostream& o, const NodeBase& n) {
	o << n.name() << "[" << n.result_type.get().name();
	if(!n.param_types.empty()) {
		for(util::TypeInfoVector::const_iterator i = n.param_types.begin();
			i != n.param_types.end(); i++)
		o << "," << *i;
	}
	o << "]";
	const NodeBase::ptr_vector &children = n.children;
	if(!children.empty()) {
		NodeBase::ptr_vector::const_iterator i = children.begin();
		o << "(" << *i++;
		for(; i != children.end(); i++) {
			o << "," << *i;
		}
		o << ")";
	}
	return o;
}

//NodeBase* new_clone(const NodeBase &n) {return n.clone();}

} // namespace detail
} // namespace vgp
