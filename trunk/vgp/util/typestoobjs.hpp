#ifndef TYPESTOOBJS_HPP_
#define TYPESTOOBJS_HPP_

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <vgp/util/wrap.hpp>

/////////////////
// A relatively dumb instantiator from an MPL vector of types.
// Instatiates each type and inserts it (or its pointer or base pointer) to
// a runtime container
//
// Usage: types_to_objs<VECTOROFTYPES, INSERTER>(container_t &c)
//
//	VECTOROFTYPES is any MPL vector that can be fed into boost::mpl::for_each
//
//	INSERTER is a type that has a constructor taking a container_t& and
//		an operator() taking a [optionally wrapped] reference to the
//		newly instantiated type
//
// Some sample inserters are provided for inserting into containers of
// objects, pointers and base pointers, as well as maps using a getkey()
// method of the created object.
/////////////////


namespace vgp { namespace util {
namespace mpl = ::boost::mpl;

template <class CONTAINER>
struct inserter {
	typedef CONTAINER container_t;
	inserter(CONTAINER& c) : container(c) {}
	template <class T>
	void operator()(wrap<T>) const {
		container.insert(T());
	}
protected:
	CONTAINER& container;
};

template <class CONTAINER, class BASE>
struct baseptr_inserter : public inserter<CONTAINER> {
	baseptr_inserter(CONTAINER& c) : inserter<CONTAINER> (c) {}
	template <class T> void operator()(wrap<T>) const {
		inserter<CONTAINER>::container.insert((BASE*)new T());
	}
};

template <class CONTAINER, class BASE>
struct baseptr_push_back : public inserter<CONTAINER> {
	baseptr_push_back(CONTAINER& c) : inserter<CONTAINER> (c) {}
	template <class T> void operator()(wrap<T>) const {
		inserter<CONTAINER>::container.push_back(/*(BASE*)*/new T());
	}
};

template <class CONTAINER, class BASE, template <class> class WRAPPER >
struct baseptr_wrapped_push_back : public inserter<CONTAINER> {
	baseptr_wrapped_push_back(CONTAINER& c) : inserter<CONTAINER> (c) {}
	template <class T> void operator()(wrap<T>) const {
		inserter<CONTAINER>::container.push_back((BASE*)new WRAPPER<T>());
	}
};

template <class MAP>
struct map_inserter : public inserter<MAP> {
	map_inserter(MAP& m) : inserter<MAP> (m) {}
	template <class T> void operator()(wrap<T>) const {
		T t = T();
		inserter<MAP>::container.insert(std::make_pair(t.getkey(), t));
	}
};

template <class MAP>
struct map_inserter_ptr : public inserter<MAP> {
	map_inserter_ptr(MAP& m) : inserter<MAP> (m) {}
	template <class T> void operator()(wrap<T>) const {
		T *newobj = new T();
		inserter<MAP>::container.insert(std::make_pair(newobj->getkey(), newobj));
	}
};

template <class MAP, class BASE>
struct map_inserter_baseptr : public inserter<MAP> {
	map_inserter_baseptr(MAP& m) : inserter<MAP> (m) {}
	template <class T> void operator()(wrap<T>) const {
		BASE *newobj = new T();
		inserter<MAP>::container.insert(std::make_pair(newobj->getkey(), newobj));
	}
};

template <class TYPES, class INSERTER>
unsigned int types_to_objs(typename INSERTER::container_t& c) {
	mpl::for_each<TYPES, wrap<mpl::_1> >(INSERTER(c));
	return mpl::size<TYPES>::value;
}

} // namespace util
} // namespace vgp

#endif /*TYPESTOOBJS_HPP_*/
