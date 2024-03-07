// -*- C++ -*-
// CLASSDOC OFF
// ---------------------------------------------------------------------------
// CLASSDOC ON
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This contains the definition of two abstract interface classes:
// Hep4RotationInterface 
// Hep3RotationInterface.  
// However, these are mostly for defining methods which should be present in
// any 4- or 3-rotation class, however specialized.  The actual classes do
// not inherit from these.  The virtual function overhead turns out 
// to be too steep for that to be practical.
//
// It may be desirable in the future to turn these classes into constraints
// in the Stroustrup sense, so as to enforce this interface, still without 
// inheritance.  However, they do contain an important static:
// static G4double tolerance to set criteria for relative nearness.
//
// This file also defines structs 
// HepRep3x3;
// HepRep4x4;
// HepRep4x4Symmetric;
// which are used by various Rotation classes.
// 
// Hep4RotationInterface 
//	contains all the methods to get attributes of either a
// 	HepLorentzRotation or a HepRotation -- any information 
//	that pertains to a LorentzRotation can also be defined
//	for a HepRotation.(For example, the 4x4 representation
//	would just have 0's in the space-time entries and 1 in
//	the time-time entry.) 
//
// Hep3RotationInterface 
//	inherits from Hep4RotationInterface,  and adds methods
//	which are well-defined only in the case of a Rotation.
//	For example, a 3x3 representation is an attribute only
//	if the generic LorentzRotation involves no boost.
//
// In terms of classes in the ZOOM PhysicsVectors package, 
//	Hep4RotationInterface <--> LorentzTransformationInterface
//	Hep3RotationInterface <--> RotationInterface
//
// Hep4RotationInterface defines the required methods for:
//	HepLorentzRotation
//	HepBoost
//	HepBoostX
//	HepBoostY
//	HepBoostZ
//
// Hep3RotationInterface defines the required methods for:
//	HepRotation
//	HepRotationX
//	HepRotationY
//	HepRotationZ
//
// .SS See Also
// Rotation.h, LorentzRotation.h
//
// .SS Author
// Mark Fischler
//

#ifndef HEP_ROTATION_INTERFACES_H
#define HEP_ROTATION_INTERFACES_H

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/AxisAngle.h"

namespace CLHEP {

struct HepRep3x3;
struct HepRep4x4;
struct HepRep4x4Symmetric;

class HepRotation;
class HepRotationX;
class HepRotationY;
class HepRotationZ;
class HepLorentzRotation;
class HepBoost;
class HepBoostX;
class HepBoostY;
class HepBoostZ;

//-******************************
//
// Hep4RotationInterface 
//
//-******************************

/**
 * @author
 * @ingroup vector
 */
class Hep4RotationInterface  {

  // All attributes of shared by HepLorentzRotation, HepBoost, 
  // HepBoostX, HepBoostY, HepBoostZ.  HepRotation, HepRotationX, 
  // HepRotationY, HepRotationZ also share this attribute interface.

  friend class  HepRotation;
  friend class  HepRotationX;
  friend class  HepRotationY;
  friend class  HepRotationZ;
  friend class  HepLorentzRotation;
  friend class  HepBoost;
  friend class  HepBoostX;
  friend class  HepBoostY;
  friend class  HepBoostZ;

public:

  DLL_API static G4double tolerance;        // to determine relative nearness

  // ----------  Accessors:

#ifdef ONLY_IN_CONCRETE_CLASSES
  //  orthosymplectic 4-vectors:
  HepLorentzVector col1() const;
  HepLorentzVector col2() const;
  HepLorentzVector col3() const;
  HepLorentzVector col4() const;
  HepLorentzVector row1() const;
  HepLorentzVector row2() const;
  HepLorentzVector row3() const;
  HepLorentzVector row4() const;

  //  individual elements:
  G4double xx() const  ;
  G4double xy() const  ;
  G4double xz() const  ;
  G4double xt() const  ;
  G4double yx() const  ;
  G4double yy() const  ;
  G4double yz() const  ;
  G4double yt() const  ;
  G4double zx() const  ;
  G4double zy() const  ;
  G4double zz() const  ;
  G4double zt() const  ;
  G4double tx() const  ;
  G4double ty() const  ;
  G4double tz() const  ;
  G4double tt() const  ;

  //   4x4 representation:
//HepRep4x4 rep4x4() const;	JMM  Declared here but not defined anywhere!

  // ----------  Operations:
  //   comparisons:

  inline int compare( const Hep4RotationInterface & lt ) const;
  // Dictionary-order comparisons, utilizing the decompose(b,r) method

  //   decomposition:

  void decompose (HepAxisAngle & rotation, Hep3Vector & boost)const;
  // Decompose as T= R * B, where R is pure rotation, B is pure boost.

  void decompose (Hep3Vector & boost, HepAxisAngle & rotation)const;
  // Decompose as T= B * R, where R is pure rotation, B is pure boost.

  bool operator == (const Hep4RotationInterface & r) const;
  bool operator != (const Hep4RotationInterface & r) const;

  //   relative comparison:

  G4double norm2() const  ;
  G4double  distance2( const Hep4RotationInterface & lt ) const  ;
  G4double  howNear( const Hep4RotationInterface & lt ) const  ;
  bool isNear (const Hep4RotationInterface & lt, 
				   G4double epsilon=tolerance) const  ;

  void rectify()  ;
  // non-const but logically const correction for accumulated roundoff errors

  // ----------  Apply LorentzTransformations:

  HepLorentzVector operator* ( const HepLorentzVector & w ) const  ;
  HepLorentzVector operator()( const HepLorentzVector & w ) const  ;
  // Apply to a 4-vector

  // ----------  I/O:

  std::ostream & print( std::ostream & os ) const;

#endif /* ONLY_IN_CONCRETE_CLASSES */

  static G4double getTolerance();
  static G4double setTolerance( G4double tol );

  enum { ToleranceTicks = 100 };

protected:

  ~Hep4RotationInterface() {}	// protect destructor to forbid instatiation

};  // Hep4RotationInterface


//-******************************
//
// Hep3RotationInterface 
//
//-******************************

/**
 * @author
 * @ingroup vector
 */
class Hep3RotationInterface : public Hep4RotationInterface {

  // All attributes of HepRotation, HepRotationX, HepRotationY, HepRotationZ
  // beyond those available by virtue of being a Hep3RotationInterface.

  friend class  HepRotation;
  friend class  HepRotationX;
  friend class  HepRotationY;
  friend class  HepRotationZ;

public:

#ifdef ONLY_IN_CONCRETE_CLASSES

  //   Euler angles:
  G4double getPhi  () const  ;
  G4double getTheta() const  ;
  G4double getPsi  () const  ;
  G4double    phi  () const  ;
  G4double    theta() const  ;
  G4double    psi  () const  ;
  HepEulerAngles eulerAngles() const  ;

  //   axis & angle of rotation:
  G4double  getDelta() const  ;
  Hep3Vector getAxis () const  ;
  G4double     delta() const  ;
  Hep3Vector    axis () const  ;
  HepAxisAngle axisAngle() const  ;

  //   orthogonal unit-length vectors:
  Hep3Vector rowX() const;
  Hep3Vector rowY() const;
  Hep3Vector rowZ() const;

  Hep3Vector colX() const;
  Hep3Vector colY() const;
  Hep3Vector colZ() const;

//HepRep3x3 rep3x3() const;	JMM  Declared here but not defined anywhere!
  //   3x3 representation

  //  orthosymplectic 4-vectors treating this as a 4-rotation:
  HepLorentzVector col1() const;
  HepLorentzVector col2() const;
  HepLorentzVector col3() const;
  HepLorentzVector col4() const;
  HepLorentzVector row1() const;
  HepLorentzVector row2() const;
  HepLorentzVector row3() const;
  HepLorentzVector row4() const;

  //  individual elements treating this as a 4-rotation:
  G4double xt() const; 
  G4double yt() const; 
  G4double zt() const; 
  G4double tx() const; 
  G4double ty() const;
  G4double tz() const;
  G4double tt() const;

  // ---------- Operations in the Rotation group

  HepRotation operator * ( const Hep3RotationInterface & r ) const  ;

  // ---------- Application

  HepLorentzVector operator* ( const HepLorentzVector & w ) const  ;
  HepLorentzVector operator()( const HepLorentzVector & w ) const  ;
  //   apply to HepLorentzVector

  Hep3Vector operator* ( const Hep3Vector & v ) const  ;
  Hep3Vector operator()( const Hep3Vector & v ) const  ;
  //   apply to Hep3Vector

  // ---------- I/O and a helper method

  std::ostream & print( std::ostream & os ) const;

#endif /* ONLY_IN_CONCRETE_CLASSES */

private:

  ~Hep3RotationInterface() {}	// private destructor to forbid instatiation

};  // Hep3RotationInterface

//-***************************
// 3x3 and 4x4 representations
//-***************************

struct HepRep3x3 {

  // -----  Constructors:

  inline HepRep3x3();

  inline HepRep3x3(  G4double xx, G4double xy, G4double xz
                   , G4double yx, G4double yy, G4double yz
                   , G4double zx, G4double zy, G4double zz
                   );

  inline HepRep3x3( const G4double * array );
  // construct from an array of G4doubles, holding the rotation matrix
  // in ROW order (xx, xy, ...)

  inline void setToIdentity();

  // -----  The data members are public:
  G4double xx_, xy_, xz_,
            yx_, yy_, yz_,
            zx_, zy_, zz_;

  inline void getArray ( G4double * array ) const;
  // fill array with the NINE G4doubles xx, xy, xz ... zz

};  // HepRep3x3

struct HepRep4x4 {

  // -----  Constructors:
  inline HepRep4x4();

  inline HepRep4x4(  G4double xx, G4double xy, G4double xz, G4double xt
                   , G4double yx, G4double yy, G4double yz, G4double yt
                   , G4double zx, G4double zy, G4double zz, G4double zt
                   , G4double tx, G4double ty, G4double tz, G4double tt
                   );

  inline HepRep4x4( const HepRep4x4Symmetric & rep );

  inline HepRep4x4( const G4double * array );
  // construct from an array of G4doubles, holding the transformation matrix
  // in ROW order xx, xy, ...

  inline void setToIdentity();

  // -----  The data members are public:
  G4double xx_, xy_, xz_, xt_,
            yx_, yy_, yz_, yt_,
            zx_, zy_, zz_, zt_,
            tx_, ty_, tz_, tt_;
                         
  inline void getArray ( G4double * array ) const;
  // fill array with the SIXTEEN G4doubles xx, xy, xz ... tz, tt

  inline bool operator==(HepRep4x4 const & r) const;
  inline bool operator!=(HepRep4x4 const & r) const;


};  // HepRep4x4

struct HepRep4x4Symmetric {

  // -----  Constructors:

  inline HepRep4x4Symmetric();

  inline HepRep4x4Symmetric
	( G4double xx, G4double xy, G4double xz, G4double xt
                      , G4double yy, G4double yz, G4double yt
                                    , G4double zz, G4double zt
                                                  , G4double tt );

  inline HepRep4x4Symmetric( const G4double * array );
  // construct from an array of G4doubles, holding the transformation matrix
  // elements in this order:  xx, xy, xz, xt, yy, yz, yt, zz, zt, tt

  inline void setToIdentity();

  // -----  The data members are public:
  G4double xx_, xy_, xz_, xt_,
                 yy_, yz_, yt_,
                      zz_, zt_,
                           tt_;

  inline void getArray ( G4double * array ) const;
  // fill array with the TEN G4doubles xx, xy, xz, xt, yy, yz, yt, zz, zt, tt

};

}  // namespace CLHEP

#include "CLHEP/Vector/RotationInterfaces.icc"

#endif // ROTATION_INTERFACES_H
