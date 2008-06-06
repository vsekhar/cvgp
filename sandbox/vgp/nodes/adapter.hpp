#ifndef ADAPTER_HPP_
#define ADAPTER_HPP_

namespace vgp {
namespace detail {

template <typename SOURCE, typename TARGET>
TARGET adapter(SOURCE s) {return s;}

} // end namespace detail
} // end namespace vgp

#endif /*ADAPTER_HPP_*/
