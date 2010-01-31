/*
 * nodestorage.hpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#ifndef NODESTORAGE_HPP_
#define NODESTORAGE_HPP_

#include <vgp/detail/nodemultiindex.hpp>

namespace vgp {
namespace detail {

extern NodeMultiIndex nodes;
extern NodesBySequence &nodesbysequence;
extern NodesByName &nodesbyname;
extern NodesByResultType &nodesbyresulttype;

void pyexport_nodestorage();

} // namespace detail
} // namespace vgp


#endif /* NODESTORAGE_HPP_ */
