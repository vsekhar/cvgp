#ifndef SOURCEDATA_HPP_
#define SOURCEDATA_HPP_

#include <map>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include <boost/any.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace vgp {

typedef boost::ptr_map<std::string, boost::any> SourceData_t;
extern SourceData_t SourceData;

template <typename T>
void addsourcedata(std::string name, const T* data) {
	if(SourceData.count(name)) {
		std::stringstream ss;
		ss << "Error: Duplicate source data \"" << name << "\"";
		throw std::invalid_argument(ss.str().c_str());
	}
	SourceData.insert(name, new boost::any(data));
}

SourceData_t::iterator getsourcedata(const std::string&);

template <typename T>
const T* getsourcedata(const std::string& name) {
	SourceData_t::iterator itr = getsourcedata(name);
	boost::any &data_any = *itr->second;
	const T* ret = boost::any_cast<const T*>(data_any);
	return ret;
}

} // namespace vgp

#endif /*SOURCEDATA_HPP_*/
