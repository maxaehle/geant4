#include "CommonHeader.h"

// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
// 
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// AxisAngle.h - provide HepAxisAngle class
//
// History:
//   23-Jan-1998  WEB  Initial draft
//   15-Jun-1998  WEB  Added namespace support
//   02-May-2000  WEB  No global using
//   27-Jul-2000  MF   CLHEP version
//
// ----------------------------------------------------------------------

#ifndef HEP_AXISANGLE_H
#define HEP_AXISANGLE_H

#include <iostream>
#include "CLHEP/Vector/ThreeVector.h"

namespace CLHEP {

// Declarations of classes and global methods
class HepAxisAngle;
std::ostream & operator<<( std::ostream & os, const HepAxisAngle & aa );
std::istream & operator>>( std::istream & is,       HepAxisAngle & aa );

/**
 * @author
 * @ingroup vector
 */
class HepAxisAngle {

public:
  typedef G4double Scalar;

protected:
  typedef HepAxisAngle AA;         // just an abbreviation
  static Scalar tolerance;      // to determine relative nearness

public:

  // ----------  Constructors:
  inline HepAxisAngle();
  inline HepAxisAngle( const Hep3Vector axis, Scalar delta );

  // ----------  Destructor, copy constructor, assignment:
  // use C++ defaults

  // ----------  Accessors:

public:
  inline Hep3Vector            getAxis() const;
  inline Hep3Vector            axis() const;
  inline AA &                  setAxis( const Hep3Vector axis );

  inline G4double             getDelta() const;
  inline G4double             delta() const ;
  inline AA &                  setDelta( Scalar delta );

  inline AA & set( const Hep3Vector axis, Scalar delta );

  // ----------  Operations:

  //   comparisons:
  inline int  compare   ( const AA & aa ) const;

  inline bool operator==( const AA & aa ) const;
  inline bool operator!=( const AA & aa ) const;
  inline bool operator< ( const AA & aa ) const;
  inline bool operator<=( const AA & aa ) const;
  inline bool operator> ( const AA & aa ) const;
  inline bool operator>=( const AA & aa ) const;

  //   relative comparison:
  inline static G4double getTolerance();
  inline static G4double setTolerance( Scalar tol );

protected:
    G4double distance( const HepAxisAngle & aa ) const;
public:

  bool isNear ( const AA & aa, Scalar epsilon = tolerance ) const;
  G4double  howNear( const AA & aa ) const;

  // ----------  I/O:

  friend std::ostream & operator<<( std::ostream & os, const AA & aa );
  friend std::istream & operator>>( std::istream & is,       AA & aa );

private:
  Hep3Vector axis_;  // Note:  After construction, this is always of mag 1
  G4double  delta_;

};  // HepAxisAngle


}  // namespace CLHEP

#include "CLHEP/Vector/AxisAngle.icc"

#endif  // HEP_AXISANGLE_H
