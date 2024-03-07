// -*- C++ -*-
// ---------------------------------------------------------------------------

#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Transform3D.h"

namespace HepGeom {
  //--------------------------------------------------------------------------
  Point3D<G4float> &
  Point3D<G4float>::transform(const Transform3D & m) {
    G4double vx = x(), vy = y(), vz = z();
    set(m.xx()*vx + m.xy()*vy + m.xz()*vz + m.dx(),
	m.yx()*vx + m.yy()*vy + m.yz()*vz + m.dy(),
	m.zx()*vx + m.zy()*vy + m.zz()*vz + m.dz());
    return *this;
  }

  //--------------------------------------------------------------------------
  Point3D<G4float>
  operator*(const Transform3D & m, const Point3D<G4float> & v) {
    G4double vx = v.x(), vy = v.y(), vz = v.z();
    return Point3D<G4float>
      (m.xx()*vx + m.xy()*vy + m.xz()*vz + m.dx(),
       m.yx()*vx + m.yy()*vy + m.yz()*vz + m.dy(),
       m.zx()*vx + m.zy()*vy + m.zz()*vz + m.dz());
  }

  //--------------------------------------------------------------------------
  Point3D<G4double> &
  Point3D<G4double>::transform(const Transform3D & m) {
    G4double vx = x(), vy = y(), vz = z();
    set(m.xx()*vx + m.xy()*vy + m.xz()*vz + m.dx(),
	m.yx()*vx + m.yy()*vy + m.yz()*vz + m.dy(),
	m.zx()*vx + m.zy()*vy + m.zz()*vz + m.dz());
    return *this;
  }

  //--------------------------------------------------------------------------
  Point3D<G4double>
  operator*(const Transform3D & m, const Point3D<G4double> & v) {
    G4double vx = v.x(), vy = v.y(), vz = v.z();
    return Point3D<G4double>
      (m.xx()*vx + m.xy()*vy + m.xz()*vz + m.dx(),
       m.yx()*vx + m.yy()*vy + m.yz()*vz + m.dy(),
       m.zx()*vx + m.zy()*vy + m.zz()*vz + m.dz());
  }
} /* namespace HepGeom */
