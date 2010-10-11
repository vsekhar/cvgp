/*
 * arithmetic.cpp
 *
 *  Created on: 2010-10-08
 */

#include <string>

#include <cvgp/library.hpp>
#include <cvgp/register.hpp>

namespace vgp {
namespace library {

//////////////
// Arithmetic
//////////////

int iadd(int a, int b) {return a+b;}
int isubtract(int a, int b) {return a-b;}
int imultiply(int a, int b) {return a*b;}
int idivide(int a, int b) {if(b) return a/b; else return 0;}

double dadd(double a, double b) {return a+b;}
double dsubtract(double a, double b) {return a-b;}
double dmultiply(double a, double b) {return a*b;}
double ddivide(double a, double b) {if(b) return a/b; else return 0;}

size_t int_arithmetic() {
	using namespace vgp::register_;
	node(VGP_NAME(iadd));
	node(VGP_NAME(isubtract));
	node(VGP_NAME(imultiply));
	node(VGP_NAME(idivide));
	return 4;
}

size_t double_arithmetic() {
	using namespace vgp::register_;
	node(VGP_NAME(dadd));
	node(VGP_NAME(dsubtract));
	node(VGP_NAME(dmultiply));
	node(VGP_NAME(ddivide));
	return 4;
}

} // namespace library
} // namespace vgp
