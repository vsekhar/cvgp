/*
 * nodestorage.hpp
 *
 *  Created on: 2010-01-27
 *      Author: vsekhar
 */

#ifndef NODESTORAGE_HPP_
#define NODESTORAGE_HPP_

#include <vgp/detail/nodecontainer.hpp>

namespace vgp {
namespace detail {

extern NodeContainer nodes;
void pyexport_nodestorage();

}
}


#endif /* NODESTORAGE_HPP_ */
