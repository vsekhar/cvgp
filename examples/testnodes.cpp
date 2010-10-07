/*
 * testnodes.cpp
 *
 *  Created on: 2010-01-30
 */

#include <cvgp/vgp.hpp>
#include "testnodes.hpp"

int add(int a, int b) {return a+b;}
int subtract(int a, int b) {return a-b;}
int multiply(int a, int b) {return a*b;}
int divide(int a, int b) {if(b) return a/b; else return 0;}

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

void load_testnodes() {
	using namespace vgp::register_;
	node(add, "add");
	node(subtract, "subtract");
	node(multiply, "multiply");
	node(divide, "divide");
	node(f1, "f1");
	terminal(f2, "f2");
	terminal(f3, "f3");
	terminal(f4, f4_init, f4_mutate, "f4");
	terminal(const_0, "const_0");
	terminal(const_1, "const_1");
	terminal(const_2, "const_2");
	terminal(const_10, "const_10");
	adapter<char,int>();
}
