#include "CommonHeader.h"

// -*- C++ -*-
// CLASSDOC OFF
// ---------------------------------------------------------------------------
// CLASSDOC ON
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This is the definition of the HepLorentzRotation class for performing 
// Lorentz transformations (rotations and boosts) on objects of the
// HepLorentzVector class.
//
// HepLorentzRotation is a concrete implementation of Hep4RotationInterface.
//
// .SS See Also
// RotationInterfaces.h
// ThreeVector.h, LorentzVector.h
// Rotation.h, Boost.h
//
// .SS Author
// Leif Lonnblad, Mark Fischler

#ifndef HEP_LORENTZROTATION_H
#define HEP_LORENTZROTATION_H

#include "CLHEP/Vector/RotationInterfaces.h"
#include "CLHEP/Vector/Rotation.h" 
#include "CLHEP/Vector/Boost.h" 
#include "CLHEP/Vector/LorentzVector.h"

namespace CLHEP {

// Global methods

inline HepLorentzRotation inverseOf ( const HepLorentzRotation & lt );
HepLorentzRotation operator * (const HepRotation & r,
                                        const HepLorentzRotation & lt);
HepLorentzRotation operator * (const HepRotationX & r,
                                        const HepLorentzRotation & lt);
HepLorentzRotation operator * (const HepRotationY & r,
                                        const HepLorentzRotation & lt);
HepLorentzRotation operator * (const HepRotationZ & r,
                                        const HepLorentzRotation & lt);

/**
 * @author
 * @ingroup vector
 */
class HepLorentzRotation {

public:
  // ----------  Identity HepLorentzRotation:

  DLL_API static const HepLorentzRotation IDENTITY;

  // ----------  Constructors and Assignment:

  inline HepLorentzRotation();
  // Default constructor. Gives a unit matrix.

  inline  HepLorentzRotation (const HepLorentzRotation & r);
  inline  HepLorentzRotation (HepLorentzRotation && r) = default;
  // Copy and move constructors.

  inline           HepLorentzRotation (const HepRotation  & r);
  inline  explicit HepLorentzRotation (const HepRotationX & r);
  inline  explicit HepLorentzRotation (const HepRotationY & r);
  inline  explicit HepLorentzRotation (const HepRotationZ & r);
  inline           HepLorentzRotation (const HepBoost  &    b);
  inline  explicit HepLorentzRotation (const HepBoostX &    b);
  inline  explicit HepLorentzRotation (const HepBoostY &    b);
  inline  explicit HepLorentzRotation (const HepBoostZ &    b);
  // Constructors from special cases.  

  inline HepLorentzRotation & operator = (HepLorentzRotation && m) = default;
  inline HepLorentzRotation & operator = (const HepLorentzRotation & m);
  inline HepLorentzRotation & operator = (const HepRotation        & m);
  inline HepLorentzRotation & operator = (const HepBoost           & m);
  // Assignment.

         HepLorentzRotation & set (G4double bx, G4double by, G4double bz);
  inline HepLorentzRotation & set (const Hep3Vector & p);
  inline HepLorentzRotation & set (const HepRotation  & r);
  inline HepLorentzRotation & set (const HepRotationX & r);
  inline HepLorentzRotation & set (const HepRotationY & r);
  inline HepLorentzRotation & set (const HepRotationZ & r);
  inline HepLorentzRotation & set (const HepBoost & boost);
  inline HepLorentzRotation & set (const HepBoostX & boost);
  inline HepLorentzRotation & set (const HepBoostY & boost);
  inline HepLorentzRotation & set (const HepBoostZ & boost);
  inline HepLorentzRotation (G4double bx, G4double by, G4double bz);
  inline HepLorentzRotation (const Hep3Vector & p);
  // Other Constructors giving a Lorentz-boost.

         HepLorentzRotation & set( const HepBoost & B, const HepRotation & R );
  inline HepLorentzRotation (      const HepBoost & B, const HepRotation & R );
  //   supply B and R:  T = B R:

         HepLorentzRotation & set( const HepRotation & R, const HepBoost & B );
  inline HepLorentzRotation (      const HepRotation & R, const HepBoost & B );
  //   supply R and B:  T = R B:

  HepLorentzRotation ( const HepLorentzVector & col1,
		       const HepLorentzVector & col2,
		       const HepLorentzVector & col3,
		       const HepLorentzVector & col4 );
  // Construct from four *orthosymplectic* LorentzVectors for the columns:
	// NOTE:
        //      This constructor, and the two set methods below,
        //      will check that the columns (or rows) form an orthosymplectic
        //      matrix, and will adjust values so that this relation is
        //      as exact as possible.
	//	Orthosymplectic means the dot product USING THE METRIC
	//	of two different coumns will be 0, and of a column with
	//	itself will be one. 

  HepLorentzRotation & set( const HepLorentzVector & col1,
                            const HepLorentzVector & col2,
                            const HepLorentzVector & col3,
                            const HepLorentzVector & col4 );
  //   supply four *orthosymplectic* HepLorentzVectors for the columns

  HepLorentzRotation & setRows( const HepLorentzVector & row1,
                                const HepLorentzVector & row2,
                                const HepLorentzVector & row3,
                                const HepLorentzVector & row4 );
  //   supply four *orthosymplectic* HepLorentzVectors for the columns

  inline HepLorentzRotation & set( const HepRep4x4 & rep );
  inline HepLorentzRotation      ( const HepRep4x4 & rep );
  //   supply a HepRep4x4 structure (16 numbers)
  // WARNING:
  //            This constructor and set method will assume the
  //            HepRep4x4 supplied is in fact an orthosymplectic matrix.
  //            No checking or correction is done.  If you are
  //            not certain the matrix is orthosymplectic, break it
  //            into four HepLorentzVector columns and use the form
  //            HepLorentzRotation (col1, col2, col3, col4)

  // ----------  Accessors:

  inline G4double xx() const;
  inline G4double xy() const;
  inline G4double xz() const;
  inline G4double xt() const;
  inline G4double yx() const;
  inline G4double yy() const;
  inline G4double yz() const;
  inline G4double yt() const;
  inline G4double zx() const;
  inline G4double zy() const;
  inline G4double zz() const;
  inline G4double zt() const;
  inline G4double tx() const;
  inline G4double ty() const;
  inline G4double tz() const;
  inline G4double tt() const;
  // Elements of the matrix.

  inline HepLorentzVector col1() const;
  inline HepLorentzVector col2() const;
  inline HepLorentzVector col3() const;
  inline HepLorentzVector col4() const;
  // orthosymplectic column vectors

  inline HepLorentzVector row1() const;
  inline HepLorentzVector row2() const;
  inline HepLorentzVector row3() const;
  inline HepLorentzVector row4() const;
  // orthosymplectic row vectors

  inline HepRep4x4 rep4x4() const;
  //   4x4 representation:

  // ------------  Subscripting:

  class HepLorentzRotation_row {
  public:
    inline HepLorentzRotation_row(const HepLorentzRotation &, int);
    inline G4double operator [] (int) const;
  private:
    const HepLorentzRotation & rr;
    int ii;
  };
  // Helper class for implemention of C-style subscripting r[i][j] 

  inline const HepLorentzRotation_row operator [] (int) const; 
  // Returns object of the helper class for C-style subscripting r[i][j]

  G4double operator () (int, int) const;
  // Fortran-style subscripting: returns (i,j) element of the matrix.

  // ----------  Decomposition:

  void decompose (Hep3Vector & boost, HepAxisAngle & rotation) const;
  void decompose (HepBoost   & boost, HepRotation  & rotation) const;
  // Find B and R such that L = B*R

  void decompose (HepAxisAngle & rotation, Hep3Vector & boost) const;
  void decompose (HepRotation  & rotation, HepBoost   & boost) const;
  // Find R and B such that L = R*B 

  // ----------  Comparisons:

  int compare( const HepLorentzRotation & m  ) const;
  // Dictionary-order comparison, in order tt,tz,...zt,zz,zy,zx,yt,yz,...,xx
  // Used in operator<, >, <=, >=

  inline bool operator == (const HepLorentzRotation &) const;
  inline bool operator != (const HepLorentzRotation &) const;
  inline bool operator <= (const HepLorentzRotation &) const;
  inline bool operator >= (const HepLorentzRotation &) const;
  inline bool operator <  (const HepLorentzRotation &) const;
  inline bool operator >  (const HepLorentzRotation &) const;

  inline bool isIdentity() const;
  // Returns true if the Identity matrix.

  G4double distance2( const HepBoost & b  ) const;
  G4double distance2( const HepRotation & r  ) const;
  G4double distance2( const HepLorentzRotation & lt  ) const;
  // Decomposes L = B*R, returns the sum of distance2 for B and R.

  G4double howNear(   const HepBoost & b ) const;
  G4double howNear(   const HepRotation & r) const;
  G4double howNear(   const HepLorentzRotation & lt ) const;

  bool isNear(const HepBoost & b,
             G4double epsilon=Hep4RotationInterface::tolerance) const;
  bool isNear(const HepRotation & r,
             G4double epsilon=Hep4RotationInterface::tolerance) const;
  bool isNear(const HepLorentzRotation & lt,
             G4double epsilon=Hep4RotationInterface::tolerance) const;

  // ----------  Properties:

  G4double norm2() const;
  // distance2 (IDENTITY), which involves decomposing into B and R and summing 
  // norm2 for the individual B and R parts. 

  void rectify();
  // non-const but logically moot correction for accumulated roundoff errors
        // rectify averages the matrix with the orthotranspose of its actual
        // inverse (absent accumulated roundoff errors, the orthotranspose IS
        // the inverse)); this removes to first order those errors.
        // Then it formally decomposes that, extracts axis and delta for its
	// Rotation part, forms a LorentzRotation from a true HepRotation 
	// with those values of axis and delta, times the true Boost
	// with that boost vector.

  // ---------- Application:

  inline HepLorentzVector vectorMultiplication(const HepLorentzVector&) const;
  inline HepLorentzVector operator()( const HepLorentzVector & w ) const;
  inline HepLorentzVector operator* ( const HepLorentzVector & p ) const;
  // Multiplication with a Lorentz Vector.

  // ---------- Operations in the group of 4-Rotations

  HepLorentzRotation matrixMultiplication(const HepRep4x4 & m) const;

  inline HepLorentzRotation operator * (const HepBoost & b) const;
  inline HepLorentzRotation operator * (const HepRotation & r) const;
  inline HepLorentzRotation operator * (const HepLorentzRotation & lt) const;
  // Product of two Lorentz Rotations (this) * lt - matrix multiplication  

  inline  HepLorentzRotation & operator *= (const HepBoost & b);
  inline  HepLorentzRotation & operator *= (const HepRotation & r);
  inline  HepLorentzRotation & operator *= (const HepLorentzRotation & lt);
  inline  HepLorentzRotation & transform   (const HepBoost & b);
  inline  HepLorentzRotation & transform   (const HepRotation & r);
  inline  HepLorentzRotation & transform   (const HepLorentzRotation & lt);
  // Matrix multiplication.
  // Note a *= b; <=> a = a * b; while a.transform(b); <=> a = b * a;

  // Here there is an opportunity for speedup by providing specialized forms
  // of lt * r and lt * b where r is a RotationX Y or Z or b is a BoostX Y or Z
  // These are, in fact, provided below for the transform() methods.

  HepLorentzRotation & rotateX(G4double delta);
  // Rotation around the x-axis; equivalent to LT = RotationX(delta) * LT

  HepLorentzRotation & rotateY(G4double delta);
  // Rotation around the y-axis; equivalent to LT = RotationY(delta) * LT

  HepLorentzRotation & rotateZ(G4double delta);
  // Rotation around the z-axis; equivalent to LT = RotationZ(delta) * LT

  inline HepLorentzRotation & rotate(G4double delta, const Hep3Vector& axis);
  inline HepLorentzRotation & rotate(G4double delta, const Hep3Vector *axis);
  // Rotation around specified vector - LT = Rotation(delta,axis)*LT

  HepLorentzRotation & boostX(G4double beta);
  // Pure boost along the x-axis; equivalent to LT = BoostX(beta) * LT

  HepLorentzRotation & boostY(G4double beta);
  // Pure boost along the y-axis; equivalent to LT = BoostX(beta) * LT

  HepLorentzRotation & boostZ(G4double beta);
  // Pure boost along the z-axis; equivalent to LT = BoostX(beta) * LT

  inline HepLorentzRotation & boost(G4double, G4double, G4double);
  inline HepLorentzRotation & boost(const Hep3Vector &);
  // Lorenz boost.

  inline HepLorentzRotation inverse() const;
  // Return the inverse.

  inline HepLorentzRotation & invert();
  // Inverts the LorentzRotation matrix.

  // ---------- I/O:

  std::ostream & print( std::ostream & os ) const;
  // Aligned six-digit-accurate output of the transformation matrix. 

  // ---------- Tolerance

  static inline G4double getTolerance();
  static inline G4double setTolerance(G4double tol); 

  friend HepLorentzRotation inverseOf ( const HepLorentzRotation & lt );

protected:

  inline HepLorentzRotation
       (G4double mxx, G4double mxy, G4double mxz, G4double mxt,
	G4double myx, G4double myy, G4double myz, G4double myt,
	G4double mzx, G4double mzy, G4double mzz, G4double mzt,
	G4double mtx, G4double mty, G4double mtz, G4double mtt);
  // Protected constructor.
  // DOES NOT CHECK FOR VALIDITY AS A LORENTZ TRANSFORMATION.

  inline void setBoost(G4double, G4double, G4double);
  // Set elements according to a boost vector.

  G4double mxx, mxy, mxz, mxt,
            myx, myy, myz, myt,
            mzx, mzy, mzz, mzt,
            mtx, mty, mtz, mtt;
  // The matrix elements.

};  // HepLorentzRotation

inline std::ostream & operator<<
		( std::ostream & os, const  HepLorentzRotation& lt ) 
  {return lt.print(os);}

inline bool operator==(const HepRotation &r, const HepLorentzRotation & lt)
  { return lt==HepLorentzRotation(r); }
inline bool operator!=(const HepRotation &r, const HepLorentzRotation & lt)
  { return lt!=HepLorentzRotation(r); }
inline bool operator<=(const HepRotation &r, const HepLorentzRotation & lt)
  { return lt<=r; }
inline bool operator>=(const HepRotation &r, const HepLorentzRotation & lt)
  { return lt>=r; }
inline bool operator<(const HepRotation &r, const HepLorentzRotation & lt)
  { return lt<r; }
inline bool operator>(const HepRotation &r, const HepLorentzRotation & lt)
  { return lt>r; }

inline bool operator==(const HepBoost &b, const HepLorentzRotation & lt)
  { return lt==HepLorentzRotation(b); }
inline bool operator!=(const HepBoost &b, const HepLorentzRotation & lt)
  { return lt!=HepLorentzRotation(b); }
inline bool operator<=(const HepBoost &b, const HepLorentzRotation & lt)
  { return lt<=b; }
inline bool operator>=(const HepBoost &b, const HepLorentzRotation & lt)
  { return lt>=b; }
inline bool operator<(const HepBoost &b, const HepLorentzRotation & lt)
  { return lt<b; }
inline bool operator>(const HepBoost &b, const HepLorentzRotation & lt)
  { return lt>b; }

}  // namespace CLHEP

#include "CLHEP/Vector/LorentzRotation.icc"

#endif /* HEP_LORENTZROTATION_H */

