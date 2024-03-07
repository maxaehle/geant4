// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// History:
// 09.09.96 E.Chernyaev - initial version
// 12.06.01 E.Chernyaev - CLHEP-1.7: introduction of BasicVector3D to decouple
//                        the functionality from CLHEP::Hep3Vector
// 01.04.03 E.Chernyaev - CLHEP-1.9: template version
//

#ifndef HEP_VECTOR3D_H
#define HEP_VECTOR3D_H

#include <iosfwd>
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/BasicVector3D.h"

namespace HepGeom {

  class Transform3D;

  /**
   * Geometrical 3D Vector.
   * This is just a declaration of the class needed to define
   * specializations Vector3D<G4float> and Vector3D<G4double>.
   *
   * @ingroup geometry
   * @author Evgeni Chernyaev <Evgueni.Tcherniaev@cern.ch>
   */
  template<class T>
  class Vector3D : public BasicVector3D<T> {};

  /**
   * Geometrical 3D Vector with components of G4float type.
   *
   * @author Evgeni Chernyaev <Evgueni.Tcherniaev@cern.ch>
   * @ingroup geometry
   */
  template<>
  class Vector3D<G4float> : public BasicVector3D<G4float> {
  public:
    /**
     * Default constructor. */
    Vector3D() = default;

    /**
     * Constructor from three numbers. */
    Vector3D(G4float x1, G4float y1, G4float z1) : BasicVector3D<G4float>(x1,y1,z1) {}

    /**
     * Constructor from array of G4floats. */
    explicit Vector3D(const G4float * a)
      : BasicVector3D<G4float>(a[0],a[1],a[2]) {}

    /**
     * Copy constructor. */
    Vector3D(const Vector3D<G4float> &) = default;

    /**
     * Move constructor. */
    Vector3D(Vector3D<G4float> &&) = default;

    /**
     * Constructor from BasicVector3D<G4float>. */
    Vector3D(const BasicVector3D<G4float> & v) : BasicVector3D<G4float>(v) {}

    /**
     * Destructor. */
    ~Vector3D() = default;

    /**
     * Assignment. */
    Vector3D<G4float> & operator=(const Vector3D<G4float> &) = default;

    /**
     * Assignment from BasicVector3D<G4float>. */
    Vector3D<G4float> & operator=(const BasicVector3D<G4float> & v) {
      this->BasicVector3D<G4float>::operator=(v);
      return *this;
    }

    /**
     * Move assignment. */
    Vector3D<G4float> & operator=(Vector3D<G4float> &&) = default;

    /**
     * Transformation by Transform3D. */
    Vector3D<G4float> & transform(const Transform3D & m);
  };

  /**
   * Transformation of Vector<G4float> by Transform3D.
   * @relates Vector3D
   */
  Vector3D<G4float>
  operator*(const Transform3D & m, const Vector3D<G4float> & v);

  /**
   * Geometrical 3D Vector with components of G4double type.
   *
   * @author Evgeni Chernyaev <Evgueni.Tcherniaev@cern.ch>
   * @ingroup geometry
   */
  template<>
  class Vector3D<G4double> : public BasicVector3D<G4double> {
  public:
    /**
     * Default constructor. */
    Vector3D() = default;

    /**
     * Constructor from three numbers. */
    Vector3D(G4double x1, G4double y1, G4double z1) : BasicVector3D<G4double>(x1,y1,z1) {}

    /**
     * Constructor from array of G4floats. */
    explicit Vector3D(const G4float * a)
      : BasicVector3D<G4double>(a[0],a[1],a[2]) {}

    /**
     * Constructor from array of G4doubles. */
    explicit Vector3D(const G4double * a)
      : BasicVector3D<G4double>(a[0],a[1],a[2]) {}

    /**
     * Copy constructor. */
    Vector3D(const Vector3D<G4double> &) = default;

    /**
     * Move constructor. */
    Vector3D(Vector3D<G4double> &&) = default;

    /**
     * Constructor from BasicVector3D<G4float>. */
    Vector3D(const BasicVector3D<G4float> & v) : BasicVector3D<G4double>(v) {}

    /**
     * Constructor from BasicVector3D<G4double>. */
    Vector3D(const BasicVector3D<G4double> & v) : BasicVector3D<G4double>(v) {}

    /**
     * Destructor. */
    ~Vector3D() = default;

    /**
     * Constructor from CLHEP::Hep3Vector.
     * This constructor is needed only for backward compatibility and
     * in principle should be absent.
     */
    Vector3D(const CLHEP::Hep3Vector & v)
      : BasicVector3D<G4double>(v.x(),v.y(),v.z()) {}

    /**
     * Conversion (cast) to CLHEP::Hep3Vector.
     * This operator is needed only for backward compatibility and
     * in principle should not exit.
     */
    operator CLHEP::Hep3Vector () const { return CLHEP::Hep3Vector(x(),y(),z()); }

    /**
     * Assignment. */
    Vector3D<G4double> & operator=(const Vector3D<G4double> &) = default;

    /**
     * Assignment from BasicVector3D<G4float>. */
    Vector3D<G4double> & operator=(const BasicVector3D<G4float> & v) {
      this->BasicVector3D<G4double>::operator=(v);
      return *this;
    }

    /**
     * Assignment from BasicVector3D<G4double>. */
    Vector3D<G4double> & operator=(const BasicVector3D<G4double> & v) {
      this->BasicVector3D<G4double>::operator=(v);
      return *this;
    }

    /**
     * Move assignment. */
    Vector3D<G4double> & operator=(Vector3D<G4double> &&) = default;

    /**
     * Transformation by Transform3D. */
    Vector3D<G4double> & transform(const Transform3D & m);
  };

  /**
   * Transformation of Vector<G4double> by Transform3D.
   * @relates Vector3D
   */
  Vector3D<G4double>
  operator*(const Transform3D & m, const Vector3D<G4double> & v);

} /* namespace HepGeom */

#endif /* HEP_VECTOR3D_H */
