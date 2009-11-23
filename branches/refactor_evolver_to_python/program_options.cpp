/*
 * program_options.cpp
 *
 *  Created on: Dec 8, 2008
 *      Author: vsekhar
 */

#include <iostream>
#include <vgp/program_options.hpp>
#include <vgp/defaults.hpp>
#include <vgp/exception.hpp>
#include <vgp/detail/nodestorage.hpp>

namespace vgp {

namespace bpo = ::boost::program_options;

using bpo::options_description;
using bpo::variables_map;
using bpo::positional_options_description;
using bpo::value;
using bpo::store;
using bpo::parse_command_line;
using bpo::notify;

options_description getdescription() {
	options_description ret;
	ret.add_options()
		("help,h", "print help message")
		("version,v", "output version information")
		("list-nodes", "print types")
		("result-type", "print result type of organisms")
		("population,p", value<unsigned int>()->default_value(100), "size of population to simulate")
		("depth-penalty,d", value<double>()->default_value(VGP_DEFAULT_DEPTH_PENALTY),
				"depth penalty (lower -> shallower trees)")
		("pc", value<double>()->default_value(VGP_DEFAULT_PC),
				"proportion of organisms crossed-over in each generation (0<=pc<=1), remaining organisms are asexually, fitness-proportionally reproduced")
		("pm", value<double>()->default_value(VGP_DEFAULT_PM),
				"proportion of organisms randomly mutated in each generation (0<=pm<=1)")
		("load-population,l", value<std::string>(), "load a population from file before evolving")
		("save-population,s", value<std::string>(), "save population to file after evolving")
		("checkpoint,c", value<unsigned int>()->default_value(0), "checkpoint every n generations (0 = no checkpoints)")
		("checkpoint-file", value<std::string>(), "file to save checkpoint to (default save-population file)")
		;
	return ret;
}

variables_map parsecmdline(unsigned int argc, char **argv) {
	variables_map ret;

	options_description desc = getdescription();

	positional_options_description posdesc;
	//posdesc.add("name", position);

	bpo::store(bpo::command_line_parser(argc, argv).
			options(desc).positional(posdesc).run(), ret);
	notify(ret);

	// Deal with actions that need to be handled by user code
	if(ret.count("help")) {
		std::cout << desc << std::endl;
		throw vgp::exit();
	}
	if(ret.count("version"))
		throw vgp::print_version();

	if(ret.count("list-nodes")) {
		std::cout << vgp::Nodes;
		throw vgp::exit();
	}

	return ret;
}

} // namespace vgp
