#include <sstream>

#include "sourcedata.hpp"

namespace vgp {

SourceData_t SourceData;

void addsourcedata(const std::string& name, const boost::any val) {
	if(SourceData.count(name)) {
		std::stringstream ss;
		ss << "Error: Duplicate source data \"" << name << "\"";
		throw std::invalid_argument(ss.str().c_str());
	}
	SourceData.insert(std::make_pair(name, val));
}

SourceData_t::iterator getsourcedata(const std::string& name) {
	SourceData_t::iterator ret = SourceData.find(name);
	if(ret != SourceData.end())
		return ret;
	else {
		std::stringstream ss;
		ss << "Error: missing source data '" << name << "'";
		throw std::invalid_argument(ss.str().c_str());
	}
}

} // namespace vgp
