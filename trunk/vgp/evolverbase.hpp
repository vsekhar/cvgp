#ifndef VGP_EVOLVERBASE_HPP_
#define VGP_EVOLVERBASE_HPP_

#include <boost/assert.hpp>
#include <vgp/population.hpp>
#include <vgp/util/typeinfo.hpp>

namespace vgp {

struct EvolverBase {
	EvolverBase() {}
	virtual void setsize(std::size_t) = 0;
	std::size_t size() const {BOOST_ASSERT(_mainpop.size()+_temppop.size()==_size); return _size;}
	void setresulttype(std::type_info& t) {_result_type = t;}
	util::TypeInfo getresulttype() const {return _result_type;}
	virtual void process() = 0;
private:
	EvolverBase(const EvolverBase&);

	util::TypeInfo _result_type;
	std::size_t _size;
	Population _mainpop;
	Population _temppop;
};

}

#endif /*VGP_EVOLVERBASE_HPP_*/
