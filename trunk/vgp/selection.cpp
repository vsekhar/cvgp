#include <vgp/selection.hpp>

namespace vgp {
namespace selection {

double avgfitness() {
	const Population &pop = *getsourcedata<boost::reference_wrapper<Population> >("vgp_population_cref");
	return pop.avgfitness();
}

std::size_t nodecount() {
	const Population &pop = *getsourcedata<boost::reference_wrapper<Population> >("vgp_population_cref");
	return pop.nodecount();
}

double avgnodecount() {
	const Population &pop = *getsourcedata<boost::reference_wrapper<Population> >("vgp_population_cref");
	return pop.avgnodecount();
}

} // end namespace selection
} // end namespace vgp
