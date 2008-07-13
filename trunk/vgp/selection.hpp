#ifndef SELECTION_HPP_
#define SELECTION_HPP_

#include <vgp/population.hpp>
#include <vgp/sourcedata.hpp>
#include <vgp/nodes/nodeloader.hpp>

namespace vgp {
namespace selection {

class SelectResult {};

double avgfitness();
std::size_t nodecount();
double avgnodecount();

template <class T>
void selectionnodes(T loader) {
	VGP_LOAD_TERMINAL(loader, avgfitness);
	VGP_LOAD_TERMINAL(loader, nodecount);
	VGP_LOAD_TERMINAL(loader, avgnodecount);
}

} // namespace selection
} // namespace vgp



#endif /*SELECTION_HPP_*/
