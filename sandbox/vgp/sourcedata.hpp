#ifndef SOURCEDATA_HPP_
#define SOURCEDATA_HPP_

#include <map>
#include <string>
#include <sstream>
#include <stdexcept>

#include <boost/any.hpp>

#include <vgp/util/singleton.hpp>

namespace vgp {

typedef std::map<std::string, const boost::any> SourceData_t;
//typedef util::singleton0<SourceDataMap> SourceData;
extern SourceData_t SourceData;

void addsourcedata(const std::string&, const boost::any);

SourceData_t::iterator getsourcedata(const std::string&);

} // namespace vgp

#endif /*SOURCEDATA_HPP_*/
