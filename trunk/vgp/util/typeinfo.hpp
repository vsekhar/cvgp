#ifndef VGP_TYPEINFO_HPP_
#define VGP_TYPEINFO_HPP_

#include <typeinfo>
#include <iostream>
#include <list>
#include <vector>

#include <boost/ref.hpp>

namespace vgp {
namespace util {

/** A boost reference wrapper for the std::type_info object returned bythe typeid() operator
 */
typedef boost::reference_wrapper<std::type_info const> TypeInfo;

/// Hash function for TypeInfo (hashes the compiler-provided name of the type)
std::size_t hash_value(const std::type_info &);

typedef std::list<TypeInfo> TypeInfoList;
typedef std::vector<TypeInfo> TypeInfoVector;

} // namespace util
} // namespace vgp

namespace std {

/// Stream output of TypeInfo objects
std::ostream& operator<<(std::ostream&, const vgp::util::TypeInfo&);

/// Specialization to allow TypeInfo objects to be placed in sorted containers
template <>
struct less<vgp::util::TypeInfo> {
	bool operator()(const vgp::util::TypeInfo&, const vgp::util::TypeInfo&) const;
};
} // namespace std

#endif /*VGP_TYPEINFO_HPP_*/
