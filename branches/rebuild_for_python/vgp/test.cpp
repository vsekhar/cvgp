#include <iostream>
#include <list>

#include <vgp/node.hpp>
#include <vgp/make_node.hpp>

int f1(char c, double d) {if(d>0) return c+1; else return c;}
char f2() {return 'b';}
double f3() {return 2.1;}

void f4_init(double& state) {state = 0.0;}
void f4_mutate(double& state) {state+=1;}
double f4(const double& state) {return state;}

int main()
{
	using vgp::make_node;
	using vgp::make_terminal;
	using vgp::runner;
	vgp::NodeBase *n1 = make_node(f1, "f1");
	vgp::NodeBase *n2 = make_terminal(f2, "f2");
	//vgp::NodeBase *n3 = make_terminal(f3, "f3");
	//double state = 0;
	vgp::NodeBase *n4 = make_terminal(f4, f4_init, f4_mutate, "f4");

	std::cout << runner<char>(*n2) << std::endl;

	vgp::children_t& c = n1->getchildren();
	c.push_back(n2);
	c.push_back(n4);
	c[1].init();
	std::cout << runner<int>(*n1) << std::endl;
	c[1].mutate();
	std::cout << runner<int>(*n1) << std::endl;
}
