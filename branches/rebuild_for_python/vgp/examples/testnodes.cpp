/*
 * testnodes.cpp
 *
 *  Created on: 2010-01-30
 *      Author: vsekhar
 */

#include <vgp/vgp.hpp>
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
	vgp::register_::node(add, "add");
	vgp::register_::node(subtract, "subtract");
	vgp::register_::node(multiply, "multiply");
	vgp::register_::node(divide, "divide");
	vgp::register_::node(f1, "f1");
	vgp::register_::terminal(f2, "f2");
	vgp::register_::terminal(f3, "f3");
	vgp::register_::terminal(f4, f4_init, f4_mutate, "f4");
	vgp::register_::terminal(const_0, "const_0");
	vgp::register_::terminal(const_1, "const_1");
	vgp::register_::terminal(const_2, "const_2");
	vgp::register_::terminal(const_10, "const_10");
	vgp::register_::adapter<char,int>();
}
