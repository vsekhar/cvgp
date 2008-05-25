#include "format.hpp"
#include <sstream>

#include <boost/tokenizer.hpp>

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
	for(tokenizer::iterator itr = tokens.begin();
		itr != tokens.end(); itr++) {
		if(itr->compare(")") == 0) {
			indentcount--;
			indent(ret, indentcount);
			ret << ")" << std::endl;
		}
		else if(itr->compare("(") == 0) {
			indent(ret, indentcount);
			ret << "(" << std::endl;
			indentcount++;
		}
		else {
			indent(ret, indentcount);
			ret << *itr << std::endl;
		}
			
	}
	return ret.str();
}

} // namespace util
} // namespace vgp
