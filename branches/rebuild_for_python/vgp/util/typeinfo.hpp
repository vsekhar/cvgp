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
 * Doesn't work:
 * 	typedef boost::reference_wrapper<std::type_info const> TypeInfo;
 */
typedef boost::reference_wrapper<std::type_info const> TypeInfo_base;

/** A reference wrapper for the std::type_info object returned by the typeid()
 * operator.
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
 * This operator compares type_info structures using the underlying == operator.
 */
bool operator==(const TypeInfo&, const std::type_info&);

/// Stream output of TypeInfo objects (outputs the compiler-provided name)
std::ostream& operator<<(std::ostream&, const vgp::util::TypeInfo&);

/// Hash function for TypeInfo (hashes the compiler-provided name of the type)
std::size_t hash_value(const std::type_info &);

typedef std::list<TypeInfo> TypeInfoList;
typedef std::vector<TypeInfo> TypeInfoVector;

/** Functor to be used with mpl::for_each to transfer types from mpl containers
 * into runtime containers.
 */
struct TypeInfoInserter {
	TypeInfoInserter(util::TypeInfoVector &v) : vector(v) {}
	template <class TOINSERT>
	void operator()(TOINSERT) {
		vector.push_back(util::TypeInfo(typeid(TOINSERT)));
	}
	util::TypeInfoVector& vector;
};

} // namespace util
} // namespace vgp

#endif /*VGP_TYPEINFO_HPP_*/
