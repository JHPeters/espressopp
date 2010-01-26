#ifndef _ESUTIL_REAL3DREF_HPP
#define _ESUTIL_REAL3DREF_HPP

#include "esconfig.hpp"
#include "Real3D.hpp"
#include <iostream>

namespace espresso {
  class Real3DRef {
    real *data;

  public:
    Real3DRef(class Real3D &v) : data(v.data) {};
    Real3DRef(real v[3]): data(v) {}

    // assignment is not the same as initialization
    Real3DRef &operator=(const Real3DRef &v) {
      data[0] = v[0];
      data[1] = v[1];
      data[2] = v[2];
    }

    real &operator[](const int i) { return data[i]; };
    const real &operator[](const int i) const { return data[i]; };

    real &at(const int i) {
      if (i < 0 || i > 2)
	throw std::out_of_range("Real3D::at");
      return (*this)[i];
    }

    const real &at(const int i) const {
      if (i < 0 || i > 2)
	throw std::out_of_range("Real3D::at");
      return (*this)[i];
    }

    real sqr() const
    { return data[0]*data[0] + data[1]*data[1] + data[2]*data[2]; }

    real abs() const
    { return sqrt(this->sqr()); }
    
    Real3DRef& 
    operator+=(const Real3DRef &b) 
    { for (int i = 0; i < 3; i++) data[i] += b.data[i]; return *this; }
    
    Real3DRef& 
    operator-=(const Real3DRef &b)
    { for (int i = 0; i < 3; i++) data[i] -= b.data[i]; return *this; }
    
    Real3DRef& 
    operator*=(const real v)
    { for (int i = 0; i < 3; i++) data[i] *= v; return *this; }
    
    Real3DRef& 
    operator/=(const real v) { 
      real v_1 = 1.0/v;
      for (int i = 0; i < 3; i++) 
	data[i] *= v_1; 
      return *this; 
    }

    bool operator==(const Real3DRef &v) const { 
      return 
	(data[0] == v.data[0]) &&
	(data[1] == v.data[1]) &&
	(data[2] == v.data[2]);
    }

    bool operator!=(const Real3DRef &v) const {
      return ! (*this == v);
    }
    
  };

  inline std::ostream &operator<<(std::ostream &out, const Real3DRef &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
  }
}

#endif