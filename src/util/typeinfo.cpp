#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <cvgp/util/typeinfo.hpp>

namespace vgp {
namespace util {

bool operator<(const TypeInfo& t1, const std::type_info& t2) {
	return t1.get().before(t2);
}

bool operator==(const TypeInfo& t1, const std::type_info& t2) {
	return t1.get() == t2;
}

std::ostream& operator<<(std::ostream& os, const TypeInfo &t) {
	return os << t.get().name();
}

void pyexport_typeinfo() {
	using namespace boost::python;
	class_<TypeInfo>("TypeInfo", no_init)
			.def(self_ns::str(self)) // gcc hiccups without the namespace here
			.def(self == self)
			.def(self < self)
			;
	class_<TypeInfoVector>("TypeInfoVector")
			.def(vector_indexing_suite<TypeInfoVector>())
			;
}

} // namespace util
} // namespace vgp
