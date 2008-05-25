#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

// shamelessly borrowed from: boost pool library (vs)
// boost CVS:
// Revision 1.5 - (download) (annotate)
// Tue Sep 28 12:55:44 2004 UTC (3 years, 5 months ago) by johnmaddock
// Branch: MAIN

// see original for extensive documentation
// TODO: replace with boost::singleton whenever they get it finished

namespace vgp {
namespace util {

/** \brief A simple singleton implementation for default-constructable types
 * \pre type T must be default constructable
 */
template <typename T>
struct singleton0
{
private:
	struct object_creator {
		object_creator() { singleton0<T>::instance(); }
		inline void do_nothing() const { }
	};
	static object_creator create_object;
	singleton0();

public:
	/// The object type
	typedef T object_type;
	
	/// Obtain object
	/// \sa get()
	static inline object_type & instance() {
		static object_type obj;
		create_object.do_nothing();
		return obj;
	}
	
	/// Obtain object 
	/// \sa instance()
	static inline object_type & get() {
		return instance();
	}
	
	/// \note Implicit conversion operators cannot be static, so none are included here
};

template <typename T>
typename singleton0<T>::object_creator
singleton0<T>::create_object;

} // namespace util
} // namespace vgp

#endif /*SINGLETON_HPP_*/
