#ifndef VGP_SELECTION_HPP_
#define VGP_SELECTION_HPP_

#include <vgp/population.hpp>
#include <vgp/sourcedata.hpp>
#include <vgp/nodes/nodeloader.hpp>

namespace vgp {
namespace selection {

typedef Population::const_iterator SelectResult;

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



#endif /*VGP_SELECTION_HPP_*/
