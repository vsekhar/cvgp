#ifndef SOURCEDATA_HPP_
#define SOURCEDATA_HPP_

#include <map>
#include <string>
#include <sstream>
#include <stdexcept>

#include <boost/any.hpp>

namespace vgp {

typedef std::map<std::string, const boost::any> SourceData_t;
extern SourceData_t SourceData;

void addsourcedata(const std::string&, const boost::any);

SourceData_t::iterator getsourcedata(const std::string&);

template <typename T>
T& getsourcedata(const std::string& name) {
	SourceData_t::iterator itr = getsourcedata(name);
	return *boost::any_cast<T*>(*itr);
}

} // namespace vgp

#endif /*SOURCEDATA_HPP_*/
