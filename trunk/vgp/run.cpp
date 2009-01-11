/*
 * run.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: vsekhar
 */

#include <iostream>
#include <string>
#include <sstream>

#include <boost/tokenizer.hpp>

#include <vgp/detail/vgprot.hpp>
#include <vgp/detail/text_archives.hpp>
#include <vgp/util/random.hpp>
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
			std::getline(cin, buffer);
			boost::escaped_list_separator<char> sep("\\", " ,", "\"");
			typedef boost::tokenizer<boost::escaped_list_separator<char> > escaped_tokenizer;
			escaped_tokenizer tok(buffer, sep);
			typedef escaped_tokenizer::iterator tokiter;

			std::string command(*tok.begin());

			if(command == "quit" || command == "exit") return 0;

			// Operations
			else if(command == "updatefitness") evolver.updatefitness(), cout << "Done." << endl;
			else if(command == "sort") evolver.pop.sort(), cout << "Done." << endl;
			else if(command == "advance") evolver.advance(), cout << "Done." << endl;
			else if(command == "insert") {
				tokiter param = ++tok.begin();
				if(param == tok.end())
					cerr << "Bad organism to insert" << endl;
				else {
					std::stringstream sstr(*param);
					text_archive_types::iarchive_type ar(sstr);
					Organism* neworg = new Organism();
					ar >> *neworg;
					evolver.pop.push_back(neworg);
					cout << "Done." << endl;
				}
			}

			// Queries
			else if(command == "stats") cout << evolver.stats() << endl;
			else if(command == "best") cout << evolver.best();
			else if(command == "worst") cout << evolver.worst();
			else if(command == "random" || command == "pullrandom") {
				tokiter param = ++tok.begin();
				unsigned int n = 0;
				if(param==tok.end()) n=1;
				else {
					std::stringstream sstr(*param);
					sstr >> n;
				}
				for(unsigned int i = 0; i < n; i++) {
					boost::uniform_int<> rand(0,evolver.pop.size()-1);
					Population::iterator itr = evolver.pop.begin();
					std::advance(itr, rand(util::default_generator));
					cout << '\"' << *itr << '\"' << endl;
					if(command == "pullrandom") evolver.pop.erase(itr);
				}
			}

			// Unknown command
			else cerr << "Unknown command" << endl;
		}
	} // end Evolution (Evolver and population destroyed)

}

} // namespace vgp
