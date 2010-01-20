/*
 * arithmetic.hpp
 *
 *  Created on: Sep 29, 2008
 *      Author: vsekhar
 */

#ifndef ARITHMETIC_HPP_
#define ARITHMETIC_HPP_

#include <vgp/defnode.hpp>

namespace vgp {
namespace nodes {

template <class T>
T add(T a, T b) { return a + b; }

template <class T>
T subtract(T a, T b) { return a - b; }

template <class T>
T multiply(T a, T b) { return a * b; }

template <class T>
T safedivide(T a, T b) { if(b) return a/b; else return a; }

double exponent(double a, double b) { return std::pow(a,b); }

template <class T>
void arithmeticnodes(T loader) {
	loader.makenode(add<int>, "vgp_add");
	loader.makenode(add<double>, "vgp_add");
	loader.makenode(subtract<int>, "vgp_subtract");
	loader.makenode(subtract<double>, "vgp_subtract");
	loader.makenode(multiply<int>, "vgp_multiply");
	loader.makenode(multiply<double>, "vgp_multiply");
	loader.makenode(safedivide<int>, "vgp_safedivide");
	loader.makenode(safedivide<double>, "vgp_safedivide");
	loader.makenode(exponent, "vgp_exponent");
}

} // namespace nodes
} // namespace vgp

#endif /* ARITHMETIC_HPP_ */
