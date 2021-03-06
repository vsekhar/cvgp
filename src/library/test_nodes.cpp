/*
 * test_nodes.cpp
 *
 *  Created on: 2010-10-08
 */

#include <string>

#include <cvgp/library.hpp>
#include <cvgp/register.hpp>

namespace vgp {
namespace library {

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
