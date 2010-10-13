User Code
=========

In this directory, users can create their own initialization and node code.

Structure
---------

Customizing user code involves writing your user code functions as well as two functions that cvgp will call at the appropriate time to initialize and register the user code.

For example, the user's node functions can be declared and defined as:

mycode.hpp:
	
	int my_node_func(int);
	int my_terminal_func();

mycode.cpp:
	
	#include "mycode.hpp"
	int my_node_func(int i) {
		return i;
	}
	int my_terminal_func() {
		return 42;
	}

This code is then initialized and registered with cvgp as follows:

initialize_and_register.cpp:

	#include <boost/python/dict.hpp>
	#include <cvgp/usrcode.hpp>  // TODO: eventually this header will be the only one required
	#include <cvgp/python/helpers.hpp>
	#include <cvgp/register.hpp>
	
	#include "mycode.hpp"
	
	namespace vgp {
	namespace usr {
		
		bool initialize(boost::python::dict kwargs) {
			// read arguments from the python dictionary
			std::string some_param = pyget<std::string>("initparam", kwargs);
			
			// do what is required and return true if successful, false if not
			return true;
		}
		
		void register_nodes() {
			using namespace vgp::register_;
			node(my_node_func, "my_node_func");
			terminal(my_terminal_func, "my_terminal_func");
		}
		
	} // namespace usr
	} // namespace vgp

Node Library
------------

cvgp also provides some libraries of common functions and can be pulled in by adding the appropriate calls to the register_nodes() function:

	#include <cvgp/library.hpp> // (in addition to the usual headers)
	
	void register_nodes() {
		vgp::library::int_arithmetic();
		vgp::library::double_arithmetic();
		vgp::library::helloworld();
		vgp::library::test_nodes();
	}
	
Overloaded Functions
--------------------

The registration code takes as argument anything that can be converted to a function object. If a function is overloaded, a pointer to a particular variant must be specified by type.

For functions:

	int overloaded(int);
	double overloaded(double);

registration must be done as follows:

	void register_nodes() {
		int (*n1)(int) = overloaded;		// resolves overload using
		double (*n2)(double) = overloaded;	// function pointer types
		node(n1, "overloaded");
		node(n2, "overloaded");
	}

Repository Example
------------------

The two files in the repository (register.cpp and initialize.cpp) provide a more detailed example of how to write, initialize and register user code with vgp. They should be replaced with actual user code files written as detailed above.
