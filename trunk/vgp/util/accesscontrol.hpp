#ifndef ACCESSCONTROL_HPP_
#define ACCESSCONTROL_HPP_

#include <cstddef>

namespace vgp {
namespace util {

/* \brief Controlled access to data. Provides a dirty-bit
 * Stores data of type T, provides access and tracks whether that access has modified the data.
 * Can be queried using isdirty() to see if data has been changed since the last clean()
 */

struct AccessControlBase {
	static std::size_t getcount;
	static std::size_t getmutablecount;
};

template <class T>
class AccessControl : AccessControlBase {
public:
	/** Default constructor
	 * \pre type T must be default constructible
	 */
	AccessControl() : _data(), dirty(true) {}
	
	/** Copy another AccessControl object of the same type, inheriting data and dirty status
	 * \pre type T must be copy constructable
	 */
	AccessControl(const AccessControl<T>& a)
		: _data(a._data), dirty(a.dirty) {}
	
	/** Constructs an AccessControl object from data, sets dirty = true
	 * \pre type T must be copy constructable
	 */
	AccessControl(const T& d) : _data(d), dirty(true) {}
	
	/// Clear the dirty bit
	void clean() const { dirty = false; }
	/// Check if the class is dirty
	bool isdirty() const {return dirty;}
	
	/// Get write access (marks data as dirty, regardless of whether changes occurred)
	T& getmutable() {dirty = true; getmutablecount++; return _data;}
	/// Get read-only access (no change to dirty status)
	const T& get() const {getcount++; return _data;}

private:
	T _data;
	mutable bool dirty;
};

} // namespace util
} // namespace vgp

#endif /*ACCESSCONTROL_HPP_*/
