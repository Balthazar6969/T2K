#ifndef Sorted4Vector_HXX
#define Sorted4Vector_HXX

#include "TEveVector.h"
#include "TLorentzVector.h"
#include <iostream>
using std::cout;
using std::endl;

/// A TReconNode that can be sorted
class TSorted4Vector {

public:
  double x, y, z, t;

public:
  virtual ~TSorted4Vector(){};
  TSorted4Vector(const TLorentzVector &source) {

    x = source.X();
    y = source.Y();
    z = source.Z();
    t = source.T();
  }

  TSorted4Vector &operator=(const TSorted4Vector &rhs) {

    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    t = rhs.t;
    return *this;
  }
  TSorted4Vector(const TSorted4Vector &rn) {

    x = rn.x;
    y = rn.y;
    z = rn.z;
    t = rn.t;
  };

  static bool SortByT(const TSorted4Vector v1, const TSorted4Vector v2) {

    return v1.t < v2.t;
    //
  }
  TEveVectorD GetEveVector() { return TEveVectorD(x, y, z); }
  float distanceTo(const TSorted4Vector that) {

    return sqrt((this->x - that.x) * (this->x - that.x) +
                (this->y - that.y) * (this->y - that.y) +
                (this->z - that.z) * (this->z - that.z));
  }

};

#endif
