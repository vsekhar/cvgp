#ifndef VGP_ADAPTER_HPP_
#define VGP_ADAPTER_HPP_

namespace vgp {
namespace detail {

template <typename SOURCE, typename TARGET>
TARGET adapter(SOURCE s) {return s;}

} // end namespace detail
} // end namespace vgp

#endif /*VGP_ADAPTER_HPP_*/
