/*
 * evolver.hpp
 *
 *  Created on: Dec 8, 2008
 *      Author: vsekhar
 */

#ifndef EVOLVER_HPP_
#define EVOLVER_HPP_

#include <iostream>

namespace vgp {

template <class OUTPUT_TYPE>
class Evolver {
public:
	Evolver(int argc, char** argv, std::istream in, std::ostream out, std::ostream err):
		instream(in), outstream(out), errstream(err)
	{

	}

private:
	std::istream& instream;
	std::ostream& outstream;
	std::ostream& errstream;

}; // class Evolver

} // namespace vgp


#endif /* EVOLVER_HPP_ */
