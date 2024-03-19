#include "CommonHeader.h"

// -*- C++ -*-
// ---------------------------------------------------------------------------

#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Geometry/Transform3D.h"

namespace HepGeom {
  //--------------------------------------------------------------------------
  Vector3D<G4float> &
  Vector3D<G4float>::transform(const Transform3D & m) {
    G4double vx = x(), vy = y(), vz = z();
    set(m.xx()*vx + m.xy()*vy + m.xz()*vz,
	m.yx()*vx + m.yy()*vy + m.yz()*vz,
	m.zx()*vx + m.zy()*vy + m.zz()*vz);
    return *this;
  }

  //--------------------------------------------------------------------------
  Vector3D<G4float>
  operator*(const Transform3D & m, const Vector3D<G4float> & v) {
    G4double vx = v.x(), vy = v.y(), vz = v.z();
    return Vector3D<G4float>
      (m.xx()*vx + m.xy()*vy + m.xz()*vz,
       m.yx()*vx + m.yy()*vy + m.yz()*vz,
       m.zx()*vx + m.zy()*vy + m.zz()*vz);
  }

  //--------------------------------------------------------------------------
  Vector3D<G4double> &
  Vector3D<G4double>::transform(const Transform3D & m) {
    G4double vx = x(), vy = y(), vz = z();
    set(m.xx()*vx + m.xy()*vy + m.xz()*vz,
	m.yx()*vx + m.yy()*vy + m.yz()*vz,
	m.zx()*vx + m.zy()*vy + m.zz()*vz);
    return *this;
  }

  //--------------------------------------------------------------------------
  Vector3D<G4double>
  operator*(const Transform3D & m, const Vector3D<G4double> & v) {
    G4double vx = v.x(), vy = v.y(), vz = v.z();
    return Vector3D<G4double>
      (m.xx()*vx + m.xy()*vy + m.xz()*vz,
       m.yx()*vx + m.yy()*vy + m.yz()*vz,
       m.zx()*vx + m.zy()*vy + m.zz()*vz);
  }
} /* namespace HepGeom */
