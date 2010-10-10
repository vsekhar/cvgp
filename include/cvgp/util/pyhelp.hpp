#include <boost/python/dict.hpp>

namespace vgp {
namespace util {

template <class RESULT>
RESULT pyget(std::string name, boost::python::dict d) {
	using namespace boost::python;
	return extract<std::string>(d[name]);
}

} // namespace util
} // namespace vgp
