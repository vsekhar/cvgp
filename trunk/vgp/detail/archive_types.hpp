#ifndef VGP_ARCHIVETYPES_HPP_
#define VGP_ARCHIVETYPES_HPP_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace vgp {

struct text_archive_types {
	typedef boost::archive::text_iarchive iarchive_type;
	typedef boost::archive::text_oarchive oarchive_type;
};

} // namespace vgp

#endif /*VGP_ARCHIVETYPES_HPP_*/
