#ifndef VGP_TEXT_ARCHIVES_HPP_
#define VGP_TEXT_ARCHIVES_HPP_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace vgp {

struct text_archive_types {
	typedef boost::archive::text_iarchive iarchive_type;
	typedef boost::archive::text_oarchive oarchive_type;
};

} // namespace vgp

#endif /*VGP_TEXT_ARCHIVES_HPP_*/
