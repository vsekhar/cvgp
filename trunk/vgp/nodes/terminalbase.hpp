#ifndef TERMINALBASE_HPP_
#define TERMINALBASE_HPP_

#include <vgp/nodes/nodebase.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {
namespace detail {

struct TerminalBase : NodeBase {
	TerminalBase(std::string name, util::TypeInfo t, unsigned int arity) :
		NodeBase(name, t, arity) {}
};
template <class OARCHIVE>
struct TerminalBase_savable : virtual TerminalBase {
	TerminalBase_savable(std::string name, util::TypeInfo t, unsigned int arity) :
		TerminalBase(name, t, arity) {}
	virtual void save_state(OARCHIVE &) = 0;	
};
template <class IARCHIVE>
struct TerminalBase_loadable : virtual TerminalBase {
	TerminalBase_loadable(std::string name, util::TypeInfo t, unsigned int arity) :
		TerminalBase(name, t, arity) {}
	virtual void load_state(IARCHIVE &) = 0;	
};

template <class ARCHIVES>
struct TerminalBase_stateful :
	TerminalBase_savable<typename ARCHIVES::oarchive_type>,
	TerminalBase_loadable<typename ARCHIVES::iarchive_type>
{
	TerminalBase_stateful(std::string name, util::TypeInfo t, unsigned int arity) :
		TerminalBase(name, t, arity),
		TerminalBase_savable<typename ARCHIVES::oarchive_type>(name, t, arity),
		TerminalBase_loadable<typename ARCHIVES::iarchive_type>(name, t, arity)
	{}
};



} // end namespace detail
} // end namespace vgp

#endif /*TERMINALBASE_HPP_*/
