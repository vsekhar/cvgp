#ifndef VGP_BINARY_ARCHIVES_HPP_
#define VGP_BINARY_ARCHIVES_HPP_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

namespace vgp {

struct binary_archive_types {
	typedef boost::archive::binary_iarchive iarchive_type;
	typedef boost::archive::binary_oarchive oarchive_type;
};

} // namespace vgp

#endif /*VGP_BINARY_ARCHIVES_HPP_*/
