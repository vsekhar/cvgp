/*
 * run.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: vsekhar
 */

#include <iostream>
#include <string>

#include <boost/tokenizer.hpp>

#include <vgp/detail/vgprot.hpp>
#include <vgp/program_options.hpp>
#include <vgp/evolver.hpp>
#include <vgp/run.hpp>

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

namespace vgp {

unsigned int run(int argc, char** argv, FitnessFunctor fitnessfunc, util::TypeInfo result_type) {

	// Process command line
	vgp::variables_map pomap;
	try {
		pomap = vgp::parsecmdline(argc, argv);
	}
	catch(const vgp::print_version &e) {
		cout << "vgptest v. 0.1" << endl;
		return 0;
	}
	catch(const vgp::exit &e) {
		return 0;
	}

	{
		// Create evolver
		vgp::Evolver evolver(pomap, fitnessfunc, result_type);
		while(1) {
			std::string buffer;
			std::cin >> buffer;
			boost::tokenizer<> tok(buffer);
			std::string command(*tok.begin());

			if(command == "quit") return 0;
			else if(command == "updatefitness") evolver.updatefitness();
			else if(command == "stats") cout << evolver.stats() << endl;
			else if(command == "sort") evolver.pop.sort();
			else if(command == "advance") evolver.advance();
			else if(command == "best") cout << evolver.best();
			else if(command == "worst") cout << evolver.worst();
			else cerr << "Unknown command" << endl;
		}
	} // end Evolution (Evolver and population destroyed)

}

} // namespace vgp
