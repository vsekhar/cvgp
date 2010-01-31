/*
 * testnodes.cpp
 *
 *  Created on: 2010-01-30
 *      Author: vsekhar
 */

#include <vgp/vgp.hpp>
#include "testnodes.hpp"

int f1(char c, double d) {if(d>0) return c+1; else return c;}
char f2() {return 'b';}
double f3() {return 2.1;}

void f4_init(double& state) {state = 0.0;}
void f4_mutate(double& state) {state+=1;}
double f4(const double& state) {return state;}

void load_testnodes() {
	vgp::register_::node(f1, "f1");
	vgp::register_::terminal(f2, "f2");
	vgp::register_::terminal(f3, "f3");
	vgp::register_::terminal(f4, f4_init, f4_mutate, "f4");
}
