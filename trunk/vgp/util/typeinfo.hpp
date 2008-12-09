#ifndef VGP_TYPEINFO_HPP_
#define VGP_TYPEINFO_HPP_

#include <typeinfo>
#include <iostream>
#include <list>
#include <vector>

#include <boost/ref.hpp>

namespace vgp {
namespace util {

/* NB: We can't use a boost::reference_wrapper directly for TypeInfo because
 * it doesn't latch into ADL, meaning our comparison operators won't be found.
 */
// Doesn't work:
// 	typedef boost::reference_wrapper<std::type_info const> TypeInfo;
typedef boost::reference_wrapper<std::type_info const> TypeInfo_base;

/** A boost reference wrapper for the std::type_info object returned by the typeid() operator
 */
struct TypeInfo : TypeInfo_base {
	/// Simple forwarding copy constructor
	template <class T> TypeInfo(const T& t) :
		TypeInfo_base(t) {}
};

/** Weak-strict ordering of build-in std::type_info structure (using the
 * structure's own before() member function). Not sure why this isn't
 * part of the library...
 */
bool operator<(const TypeInfo&, const std::type_info&);

/** Comparison of built-in std::type_info structures
 *
 * This operator compares type_info structures by comparing the names
 * associated with each structure (from the .name() member functions).
 *
 * @caveat The compiler's RTTI implementation must generate non-empty unique
 * names for each type. This is not required by the C++ standard, but seems
 * to be implemented by recent versions of GCC (v. 3+). If a compiler does not
 * do this, node lookups by name will fail.
 */
bool operator==(const TypeInfo&, const std::type_info&);

/// Stream output of TypeInfo objects
std::ostream& operator<<(std::ostream&, const vgp::util::TypeInfo&);

/// Hash function for TypeInfo (hashes the compiler-provided name of the type)
std::size_t hash_value(const std::type_info &);

typedef std::list<TypeInfo> TypeInfoList;
typedef std::vector<TypeInfo> TypeInfoVector;

} // namespace util
} // namespace vgp

#endif /*VGP_TYPEINFO_HPP_*/
