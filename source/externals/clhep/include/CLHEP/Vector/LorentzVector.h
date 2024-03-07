// -*- C++ -*-
// CLASSDOC OFF
// ---------------------------------------------------------------------------
// CLASSDOC ON
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// HepLorentzVector is a Lorentz vector consisting of Hep3Vector and
// G4double components. Lorentz transformations (rotations and boosts)
// of these vectors are perfomed by multiplying with objects of
// the HepLorenzRotation class.
//
// .SS See Also
// ThreeVector.h, Rotation.h, LorentzRotation.h
//
// .SS Authors
// Leif Lonnblad and Anders Nilsson. Modified by Evgueni Tcherniaev, Mark Fischler
//

#ifndef HEP_LORENTZVECTOR_H
#define HEP_LORENTZVECTOR_H

#include <iostream>
#include "CLHEP/Vector/ThreeVector.h"

namespace CLHEP {

// Declarations of classes and global methods
class HepLorentzVector;
class HepLorentzRotation;
class HepRotation;
class HepAxisAngle;
class HepEulerAngles;
class Tcomponent;
HepLorentzVector rotationXOf( const HepLorentzVector & vec, G4double delta );
HepLorentzVector rotationYOf( const HepLorentzVector & vec, G4double delta );
HepLorentzVector rotationZOf( const HepLorentzVector & vec, G4double delta );
HepLorentzVector rotationOf
    ( const HepLorentzVector & vec, const Hep3Vector & axis, G4double delta );
HepLorentzVector rotationOf
    ( const HepLorentzVector & vec, const HepAxisAngle & ax );
HepLorentzVector rotationOf
    ( const HepLorentzVector & vec, const HepEulerAngles & e );
HepLorentzVector rotationOf
    ( const HepLorentzVector & vec, G4double phi,
                                    G4double theta,
                                    G4double psi );
inline 
HepLorentzVector  boostXOf( const HepLorentzVector & vec, G4double beta );
inline 
HepLorentzVector  boostYOf( const HepLorentzVector & vec, G4double beta );
inline 
HepLorentzVector  boostZOf( const HepLorentzVector & vec, G4double beta );
inline HepLorentzVector  boostOf
    ( const HepLorentzVector & vec, const Hep3Vector & betaVector );
inline HepLorentzVector  boostOf
    ( const HepLorentzVector & vec, const Hep3Vector & axis,  G4double beta );

enum ZMpvMetric_t { TimePositive, TimeNegative };


/**
 * @author
 * @ingroup vector
 */

class HepLorentzVector {

public:

  enum { X=0, Y=1, Z=2, T=3, NUM_COORDINATES=4, SIZE=NUM_COORDINATES };
  // Safe indexing of the coordinates when using with matrices, arrays, etc.
  // (BaBar)

  inline HepLorentzVector(G4double x, G4double y,
			  G4double z, G4double t);
  // Constructor giving the components x, y, z, t.

  inline HepLorentzVector(G4double x, G4double y, G4double z);
  // Constructor giving the components x, y, z with t-component set to 0.0.

  explicit HepLorentzVector(G4double t);
  // Constructor giving the t-component with x, y and z set to 0.0.

  inline HepLorentzVector();
  // Default constructor with x, y, z and t set to 0.0.

  inline HepLorentzVector(const Hep3Vector & p, G4double e);
  inline HepLorentzVector(G4double e, const Hep3Vector & p);
  // Constructor giving a 3-Vector and a time component.

  inline HepLorentzVector(const HepLorentzVector &);
  inline HepLorentzVector(HepLorentzVector &&) = default;
  // Copy and move constructors.

  inline ~HepLorentzVector();
  // The destructor.

  inline operator const Hep3Vector & () const;
  inline operator Hep3Vector & ();
  // Conversion (cast) to Hep3Vector.

  inline G4double x() const;
  inline G4double y() const;
  inline G4double z() const;
  inline G4double t() const;
  // Get position and time.

  inline void setX(G4double);
  inline void setY(G4double);
  inline void setZ(G4double);
  inline void setT(G4double);
  // Set position and time.

  inline G4double px() const;
  inline G4double py() const;
  inline G4double pz() const;
  inline G4double e() const;
  // Get momentum and energy.

  inline void setPx(G4double);
  inline void setPy(G4double);
  inline void setPz(G4double);
  inline void setE(G4double);
  // Set momentum and energy.

  inline Hep3Vector vect() const;
  // Get spatial component. 

  inline void setVect(const Hep3Vector &);
  // Set spatial component. 

  inline G4double theta() const;
  inline G4double cosTheta() const;
  inline G4double phi() const;
  inline G4double rho() const;
  // Get spatial vector components in spherical coordinate system.

  inline void setTheta(G4double);
  inline void setPhi(G4double);
  inline void setRho(G4double);
  // Set spatial vector components in spherical coordinate system.

  G4double operator () (int) const;
  inline G4double operator [] (int) const;
  // Get components by index.

  G4double & operator () (int);
  inline G4double & operator [] (int);
  // Set components by index.

  inline HepLorentzVector & operator = (const HepLorentzVector &);
  inline HepLorentzVector & operator = (HepLorentzVector &&) = default;
  // Copy and move assignment operators. 

  inline HepLorentzVector   operator +  (const HepLorentzVector &) const;
  inline HepLorentzVector & operator += (const HepLorentzVector &);
  // Additions.

  inline HepLorentzVector   operator -  (const HepLorentzVector &) const;
  inline HepLorentzVector & operator -= (const HepLorentzVector &);
  // Subtractions.

  inline HepLorentzVector operator - () const;
  // Unary minus.

  inline HepLorentzVector & operator *= (G4double);
         HepLorentzVector & operator /= (G4double);
  // Scaling with real numbers.

  inline bool operator == (const HepLorentzVector &) const;
  inline bool operator != (const HepLorentzVector &) const;
  // Comparisons.

  inline G4double perp2() const;
  // Transverse component of the spatial vector squared.

  inline G4double perp() const;
  // Transverse component of the spatial vector (R in cylindrical system).

  inline void setPerp(G4double);
  // Set the transverse component of the spatial vector.

  inline G4double perp2(const Hep3Vector &) const;
  // Transverse component of the spatial vector w.r.t. given axis squared.

  inline G4double perp(const Hep3Vector &) const;
  // Transverse component of the spatial vector w.r.t. given axis.

  inline G4double angle(const Hep3Vector &) const;
  // Angle wrt. another vector.

  inline G4double mag2() const;
  // Dot product of 4-vector with itself. 
  // By default the metric is TimePositive, and mag2() is the same as m2().

  inline G4double m2() const;
  // Invariant mass squared.

  inline G4double mag() const;
  inline G4double m() const;
  // Invariant mass. If m2() is negative then -sqrt(-m2()) is returned.

  inline G4double mt2() const;
  // Transverse mass squared.

  inline G4double mt() const;
  // Transverse mass.

  inline G4double et2() const;
  // Transverse energy squared.

  inline G4double et() const;
  // Transverse energy.

  inline G4double dot(const HepLorentzVector &) const;
  inline G4double operator * (const HepLorentzVector &) const;
  // Scalar product.

  inline G4double invariantMass2( const HepLorentzVector & w ) const;
  // Invariant mass squared of pair of 4-vectors 

  G4double invariantMass ( const HepLorentzVector & w ) const;
  // Invariant mass of pair of 4-vectors 

  inline void setVectMag(const Hep3Vector & spatial, G4double magnitude);
  inline void setVectM(const Hep3Vector & spatial, G4double mass);
  // Copy spatial coordinates, and set energy = sqrt(mass^2 + spatial^2)

  inline G4double plus() const;
  inline G4double minus() const;
  // Returns the positive/negative light-cone component t +/- z.

  Hep3Vector boostVector() const;
  // Boost needed from rest4Vector in rest frame to form this 4-vector
  // Returns the spatial components divided by the time component.

  HepLorentzVector & boost(G4double, G4double, G4double);
  inline HepLorentzVector & boost(const Hep3Vector &);
  // Lorentz boost.

  HepLorentzVector & boostX( G4double beta );
  HepLorentzVector & boostY( G4double beta );
  HepLorentzVector & boostZ( G4double beta );
  // Boost along an axis, by magnitue beta (fraction of speed of light)

  G4double rapidity() const;
  // Returns the rapidity, i.e. 0.5*ln((E+pz)/(E-pz))

  inline G4double pseudoRapidity() const;
  // Returns the pseudo-rapidity, i.e. -ln(tan(theta/2))

  inline bool isTimelike() const;
  // Test if the 4-vector is timelike

  inline bool isSpacelike() const;
  // Test if the 4-vector is spacelike

  inline bool isLightlike(G4double epsilon=tolerance) const;
  // Test for lightlike is within tolerance epsilon

  HepLorentzVector &  rotateX(G4double);
  // Rotate the spatial component around the x-axis.

  HepLorentzVector &  rotateY(G4double);
  // Rotate the spatial component around the y-axis.

  HepLorentzVector &  rotateZ(G4double);
  // Rotate the spatial component around the z-axis.

  HepLorentzVector &  rotateUz(const Hep3Vector &);
  // Rotates the reference frame from Uz to newUz (unit vector).

  HepLorentzVector & rotate(G4double, const Hep3Vector &);
  // Rotate the spatial component around specified axis.

  inline HepLorentzVector & operator *= (const HepRotation &);
  inline HepLorentzVector & transform(const HepRotation &);
  // Transformation with HepRotation.

  HepLorentzVector & operator *= (const HepLorentzRotation &);
  HepLorentzVector & transform(const HepLorentzRotation &);
  // Transformation with HepLorenzRotation.

// = = = = = = = = = = = = = = = = = = = = = = = =
//
// Esoteric properties and operations on 4-vectors:  
//
// 0 - Flexible metric convention and axial unit 4-vectors
// 1 - Construct and set 4-vectors in various ways 
// 2 - Synonyms for accessing coordinates and properties
// 2a - Setting space coordinates in different ways 
// 3 - Comparisions (dictionary, near-ness, and geometric)
// 4 - Intrinsic properties 
// 4a - Releativistic kinematic properties 
// 4b - Methods combining two 4-vectors
// 5 - Properties releative to z axis and to arbitrary directions
// 7 - Rotations and Boosts
//
// = = = = = = = = = = = = = = = = = = = = = = = =

// 0 - Flexible metric convention 

  static ZMpvMetric_t setMetric( ZMpvMetric_t met );
  static ZMpvMetric_t getMetric();

// 1 - Construct and set 4-vectors in various ways 

  inline void set        (G4double x, G4double y, G4double z, G4double  t);
  inline void set        (G4double x, G4double y, G4double z, Tcomponent t);
  inline HepLorentzVector(G4double x, G4double y, G4double z, Tcomponent t);
  // Form 4-vector by supplying cartesian coordinate components

  inline void set        (Tcomponent t, G4double x, G4double y, G4double z);
  inline HepLorentzVector(Tcomponent t, G4double x, G4double y, G4double z);
  // Deprecated because the 4-G4doubles form uses x,y,z,t, not t,x,y,z.

  inline void set                 ( G4double t );

  inline void set                 ( Tcomponent t );
  inline explicit HepLorentzVector( Tcomponent t );
  // Form 4-vector with zero space components, by supplying t component

  inline void set                 ( const Hep3Vector & v );
  inline explicit HepLorentzVector( const Hep3Vector & v );
  // Form 4-vector with zero time component, by supplying space 3-vector 

  inline HepLorentzVector & operator=( const Hep3Vector & v );
  // Form 4-vector with zero time component, equal to space 3-vector 

  inline void set ( const Hep3Vector & v, G4double t );
  inline void set ( G4double t, const Hep3Vector & v );
  // Set using specified space vector and time component

// 2 - Synonyms for accessing coordinates and properties

  inline G4double getX() const;
  inline G4double getY() const;
  inline G4double getZ() const;
  inline G4double getT() const;
  // Get position and time.

  inline Hep3Vector v() const;
  inline Hep3Vector getV() const;
  // Get spatial component.   Same as vect.

  inline void setV(const Hep3Vector &);
  // Set spatial component.   Same as setVect.

// 2a - Setting space coordinates in different ways 

  inline void setV( G4double x, G4double y, G4double z );

  inline void setRThetaPhi( G4double r, G4double theta, G4double phi);
  inline void setREtaPhi( G4double r, G4double eta, G4double phi);
  inline void setRhoPhiZ( G4double rho, G4double phi, G4double z );

// 3 - Comparisions (dictionary, near-ness, and geometric)

  int compare( const HepLorentzVector & w ) const;

  bool operator >( const HepLorentzVector & w ) const;
  bool operator <( const HepLorentzVector & w ) const;
  bool operator>=( const HepLorentzVector & w ) const;
  bool operator<=( const HepLorentzVector & w ) const;

  bool   isNear ( const HepLorentzVector & w, 
					G4double epsilon=tolerance ) const;
  G4double howNear( const HepLorentzVector & w ) const;
  // Is near using Euclidean measure t**2 + v**2

  bool   isNearCM ( const HepLorentzVector & w, 
					G4double epsilon=tolerance ) const;
  G4double howNearCM( const HepLorentzVector & w ) const;
  // Is near in CM frame:  Applicable only for two timelike HepLorentzVectors

        // If w1 and w2 are already in their CM frame, then w1.isNearCM(w2)
        // is exactly equivalent to w1.isNear(w2).
        // If w1 and w2 have T components of zero, w1.isNear(w2) is exactly
        // equivalent to w1.getV().isNear(w2.v()).  

  bool isParallel( const HepLorentzVector & w, 
					G4double epsilon=tolerance ) const;
  // Test for isParallel is within tolerance epsilon
  G4double howParallel (const HepLorentzVector & w) const;

  static G4double getTolerance();
  static G4double setTolerance( G4double tol );
  // Set the tolerance for HepLorentzVectors to be considered near
  // The same tolerance is used for determining isLightlike, and isParallel

  G4double deltaR(const HepLorentzVector & v) const;
  // sqrt ( (delta eta)^2 + (delta phi)^2 ) of space part

// 4 - Intrinsic properties 

         G4double howLightlike() const;
  // Close to zero for almost lightlike 4-vectors; up to 1.

  inline G4double euclideanNorm2()  const;
  // Sum of the squares of time and space components; not Lorentz invariant. 

  inline G4double euclideanNorm()  const; 
  // Length considering the metric as (+ + + +); not Lorentz invariant.


// 4a - Relativistic kinematic properties 

// All Relativistic kinematic properties are independent of the sense of metric

  inline G4double restMass2() const;
  inline G4double invariantMass2() const; 
  // Rest mass squared -- same as m2()

  inline G4double restMass() const;
  inline G4double invariantMass() const; 
  // Same as m().  If m2() is negative then -sqrt(-m2()) is returned.

// The following properties are rest-frame related, 
// and are applicable only to non-spacelike 4-vectors

  HepLorentzVector rest4Vector() const;
  // This 4-vector, boosted into its own rest frame:  (0, 0, 0, m()) 
          // The following relation holds by definition:
          // w.rest4Vector().boost(w.boostVector()) == w

  // Beta and gamma of the boost vector
  G4double beta() const;
  // Relativistic beta of the boost vector

  G4double gamma() const;
  // Relativistic gamma of the boost vector

  inline G4double eta() const;
  // Pseudorapidity (of the space part)

  inline G4double eta(const Hep3Vector & ref) const;
  // Pseudorapidity (of the space part) w.r.t. specified direction

  G4double rapidity(const Hep3Vector & ref) const;
  // Rapidity in specified direction

  G4double coLinearRapidity() const;
  // Rapidity, in the relativity textbook sense:  atanh (|P|/E)

  Hep3Vector findBoostToCM() const;
  // Boost needed to get to center-of-mass  frame:
          // w.findBoostToCM() == - w.boostVector()
          // w.boost(w.findBoostToCM()) == w.rest4Vector()

  Hep3Vector findBoostToCM( const HepLorentzVector & w ) const;
  // Boost needed to get to combined center-of-mass frame:
          // w1.findBoostToCM(w2) == w2.findBoostToCM(w1)
          // w.findBoostToCM(w) == w.findBoostToCM()

  inline G4double et2(const Hep3Vector &) const;
  // Transverse energy w.r.t. given axis squared.

  inline G4double et(const Hep3Vector &) const;
  // Transverse energy w.r.t. given axis.

// 4b - Methods combining two 4-vectors

  inline G4double diff2( const HepLorentzVector & w ) const;
  // (this - w).dot(this-w); sign depends on metric choice

  inline G4double delta2Euclidean ( const HepLorentzVector & w ) const;
  // Euclidean norm of differnce:  (delta_T)^2  + (delta_V)^2

// 5 - Properties releative to z axis and to arbitrary directions

  G4double  plus(  const Hep3Vector & ref ) const;
  // t + projection in reference direction

  G4double  minus( const Hep3Vector & ref ) const;
  // t - projection in reference direction

// 7 - Rotations and boosts

  HepLorentzVector & rotate ( const Hep3Vector & axis, G4double delta );
  // Same as rotate (delta, axis)

  HepLorentzVector & rotate ( const HepAxisAngle & ax );
  HepLorentzVector & rotate ( const HepEulerAngles & e );
  HepLorentzVector & rotate ( G4double phi,
                              G4double theta,
                              G4double psi );
  // Rotate using these HepEuler angles - see Goldstein page 107 for conventions

  HepLorentzVector & boost ( const Hep3Vector & axis,  G4double beta );
  // Normalizes the Hep3Vector to define a direction, and uses beta to
  // define the magnitude of the boost.

  friend HepLorentzVector rotationXOf
    ( const HepLorentzVector & vec, G4double delta );
  friend HepLorentzVector rotationYOf
    ( const HepLorentzVector & vec, G4double delta );
  friend HepLorentzVector rotationZOf
    ( const HepLorentzVector & vec, G4double delta );
  friend HepLorentzVector rotationOf
    ( const HepLorentzVector & vec, const Hep3Vector & axis, G4double delta );
  friend HepLorentzVector rotationOf
    ( const HepLorentzVector & vec, const HepAxisAngle & ax );
  friend HepLorentzVector rotationOf
    ( const HepLorentzVector & vec, const HepEulerAngles & e );
  friend HepLorentzVector rotationOf
    ( const HepLorentzVector & vec, G4double phi,
                                    G4double theta,
                                    G4double psi );

  inline friend HepLorentzVector  boostXOf
    ( const HepLorentzVector & vec, G4double beta );
  inline friend HepLorentzVector  boostYOf
    ( const HepLorentzVector & vec, G4double beta );
  inline friend HepLorentzVector  boostZOf
    ( const HepLorentzVector & vec, G4double beta );
  inline friend HepLorentzVector  boostOf
    ( const HepLorentzVector & vec, const Hep3Vector & betaVector );
  inline friend HepLorentzVector  boostOf
    ( const HepLorentzVector & vec, const Hep3Vector & axis,  G4double beta );
 
private:

  Hep3Vector pp;
  G4double  ee;

  DLL_API static G4double tolerance;
  DLL_API static G4double metric;

};  // HepLorentzVector

// 8 - Axial Unit 4-vectors

static const HepLorentzVector X_HAT4 = HepLorentzVector( 1, 0, 0, 0 );
static const HepLorentzVector Y_HAT4 = HepLorentzVector( 0, 1, 0, 0 );
static const HepLorentzVector Z_HAT4 = HepLorentzVector( 0, 0, 1, 0 );
static const HepLorentzVector T_HAT4 = HepLorentzVector( 0, 0, 0, 1 );

// Global methods

std::ostream & operator << (std::ostream &, const HepLorentzVector &);
// Output to a stream.

std::istream & operator >> (std::istream &, HepLorentzVector &);
// Input from a stream.

typedef HepLorentzVector HepLorentzVectorD;
typedef HepLorentzVector HepLorentzVectorF;

inline HepLorentzVector operator * (const HepLorentzVector &, G4double a);
inline HepLorentzVector operator * (G4double a, const HepLorentzVector &);
// Scaling LorentzVector with a real number

       HepLorentzVector operator / (const HepLorentzVector &, G4double a);
// Dividing LorentzVector by a real number

// Tcomponent definition:

// Signature protection for 4-vector constructors taking 4 components
class Tcomponent {
private:
  G4double t_;
public:
  explicit Tcomponent(G4double t) : t_(t) {}
  operator G4double() const { return t_; }
};  // Tcomponent

}  // namespace CLHEP

#include "CLHEP/Vector/LorentzVector.icc"

#endif /* HEP_LORENTZVECTOR_H */
