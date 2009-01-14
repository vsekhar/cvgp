#include "format.hpp"
#include <string>
#include <sstream>

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

namespace vgp {
namespace util {

std::ostream &indent(std::ostream& o, unsigned int count) {
	for(unsigned int i = 0; i < count; i++)
		o << "  ";
	return o;
}

std::string maketree(const std::string &str) {
	std::stringstream ret;
	boost::char_separator<char> sep(",","()");
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	tokenizer tokens(str, sep);
	unsigned int indentcount = 0;
	BOOST_FOREACH(const std::string &token, tokens) {
		if(token.compare(")") == 0) {
			indentcount--;
			indent(ret, indentcount);
			ret << ")" << std::endl;
		}
		else if(token.compare("(") == 0) {
			indent(ret, indentcount);
			ret << "(" << std::endl;
			indentcount++;
		}
		else {
			indent(ret, indentcount);
			ret << token << std::endl;
		}
	}
	return ret.str();
}

} // namespace util
} // namespace vgp
