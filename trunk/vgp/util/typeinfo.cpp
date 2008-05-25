#include "typeinfo.hpp"

namespace std {

ostream& operator<<(ostream& os, const vgp::util::TypeInfo & ti) {
	const type_info &type = ti;
	return os << type.name();
}

bool less<vgp::util::TypeInfo>::operator()
(const vgp::util::TypeInfo& t1, const vgp::util::TypeInfo& t2) const {
		return ((const std::type_info&)t1).before(t2);
}

} // namespace std
