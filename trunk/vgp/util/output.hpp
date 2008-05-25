#ifndef OUTPUT_HPP_
#define OUTPUT_HPP_

#include <ostream>

#include <boost/mpl/for_each.hpp>

namespace vgp { namespace util {
using std::ostream;

struct outputtypes_t {
	outputtypes_t() : out(std::cerr) {};
	outputtypes_t(ostream& o) : out(o) {}
	template <class T>
	void operator()(const T&) { 
		out << typeid(T).name() << std::endl;
	}
protected:
	ostream &out;
};

struct outputwrappedtypes_t : public outputtypes_t {
	outputwrappedtypes_t() : outputtypes_t() {}
	outputwrappedtypes_t(ostream& o) : outputtypes_t(o) {}
	template <class T>
	void operator()(const wrap<T>&) {
		out << typeid(T).name() << std::endl;
	}
};

template <class TYPES, class OUTPUTTER>
std::ostream& output(std::ostream& o) {
	mpl::for_each<TYPES>(OUTPUTTER(o));
	return o;
}

template <class TYPES>
std::ostream& outputtypes(std::ostream& o) {
	return output<TYPES, outputtypes_t>(o);
}

template <class TYPES>
std::ostream& outputwrappedtypes(std::ostream& o) {
	return output<TYPES, outputwrappedtypes_t>(o);
}

} // namespace util
} // namespace vgp

#endif /*OUTPUT_HPP_*/
