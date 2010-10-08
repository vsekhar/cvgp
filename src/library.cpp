/*
 * library.cpp
 *
 *  Created on: 2010-10-08
 */

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

void int_arithmetic() {
	using namespace vgp::register_;
	node(VGP_NAME(iadd));
	node(VGP_NAME(isubtract));
	node(VGP_NAME(imultiply));
	node(VGP_NAME(idivide));
}

void double_arithmetic() {
	using namespace vgp::register_;
	node(VGP_NAME(dadd));
	node(VGP_NAME(dsubtract));
	node(VGP_NAME(dmultiply));
	node(VGP_NAME(ddivide));
}

//////////////
// Test nodes
//////////////

int f1(char c, double d) {if(d>0) return c+1; else return c;}
char f2() {return 'b';}
double f3() {return 2.1;}
int const_0() {return 0;}
int const_1() {return 1;}
int const_2() {return 2;}
int const_10() {return 10;}

void f4_init(double& state) {state = 0.0;}
void f4_mutate(double& state) {state+=1;}
double f4(const double& state) {return state;}

void test_nodes() {
	using namespace vgp::register_;
	node(VGP_NAME(f1));
	terminal(VGP_NAME(f2));
	terminal(VGP_NAME(f3));
	terminal(f4, f4_init, f4_mutate, "f4");
	terminal(VGP_NAME(const_0));
	terminal(VGP_NAME(const_1));
	terminal(VGP_NAME(const_2));
	terminal(VGP_NAME(const_10));
	adapter<char,int>();
}

} // namespace library
} // namespace vgp
