// -*- C++ -*-
// ---------------------------------------------------------------------------

#include "CLHEP/Geometry/Normal3D.h"
#include "CLHEP/Geometry/Transform3D.h"

namespace HepGeom {
  //--------------------------------------------------------------------------
  Normal3D<G4float> &
  Normal3D<G4float>::transform(const Transform3D & m) {
    G4double vx = x(),    vy = y(),    vz = z();
    G4double xx = m.xx(), xy = m.xy(), xz = m.xz();
    G4double yx = m.yx(), yy = m.yy(), yz = m.yz();
    G4double zx = m.zx(), zy = m.zy(), zz = m.zz();
    set((yy*zz-yz*zy)*vx+(yz*zx-yx*zz)*vy+(yx*zy-yy*zx)*vz,
	(zy*xz-zz*xy)*vx+(zz*xx-zx*xz)*vy+(zx*xy-zy*xx)*vz,
	(xy*yz-xz*yy)*vx+(xz*yx-xx*yz)*vy+(xx*yy-xy*yx)*vz);
    return *this;
  }

  //--------------------------------------------------------------------------
  Normal3D<G4float>
  operator*(const Transform3D & m, const Normal3D<G4float> & v) {
    G4double vx = v.x(),  vy = v.y(),  vz = v.z();
    G4double xx = m.xx(), xy = m.xy(), xz = m.xz();
    G4double yx = m.yx(), yy = m.yy(), yz = m.yz();
    G4double zx = m.zx(), zy = m.zy(), zz = m.zz();
    return Normal3D<G4float>
      ((yy*zz-yz*zy)*vx+(yz*zx-yx*zz)*vy+(yx*zy-yy*zx)*vz,
       (zy*xz-zz*xy)*vx+(zz*xx-zx*xz)*vy+(zx*xy-zy*xx)*vz,
       (xy*yz-xz*yy)*vx+(xz*yx-xx*yz)*vy+(xx*yy-xy*yx)*vz);
  }

  //--------------------------------------------------------------------------
  Normal3D<G4double> &
  Normal3D<G4double>::transform(const Transform3D & m) {
    G4double vx = x(),    vy = y(),    vz = z();
    G4double xx = m.xx(), xy = m.xy(), xz = m.xz();
    G4double yx = m.yx(), yy = m.yy(), yz = m.yz();
    G4double zx = m.zx(), zy = m.zy(), zz = m.zz();
    set((yy*zz-yz*zy)*vx+(yz*zx-yx*zz)*vy+(yx*zy-yy*zx)*vz,
	(zy*xz-zz*xy)*vx+(zz*xx-zx*xz)*vy+(zx*xy-zy*xx)*vz,
	(xy*yz-xz*yy)*vx+(xz*yx-xx*yz)*vy+(xx*yy-xy*yx)*vz);
    return *this;
  }

  //--------------------------------------------------------------------------
  Normal3D<G4double>
  operator*(const Transform3D & m, const Normal3D<G4double> & v) {
    G4double vx = v.x(),  vy = v.y(),  vz = v.z();
    G4double xx = m.xx(), xy = m.xy(), xz = m.xz();
    G4double yx = m.yx(), yy = m.yy(), yz = m.yz();
    G4double zx = m.zx(), zy = m.zy(), zz = m.zz();
    return Normal3D<G4double>
      ((yy*zz-yz*zy)*vx+(yz*zx-yx*zz)*vy+(yx*zy-yy*zx)*vz,
       (zy*xz-zz*xy)*vx+(zz*xx-zx*xz)*vy+(zx*xy-zy*xx)*vz,
       (xy*yz-xz*yy)*vx+(xz*yx-xx*yz)*vy+(xx*yy-xy*yx)*vz);
  }
} /* namespace HepGeom */
