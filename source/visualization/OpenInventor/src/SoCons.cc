#include "CommonHeader.h"

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//
/*-----------------------------HEPVis---------------------------------------*/
/*                                                                          */
/* Node:             SoCons                                                 */
/* Description:      Represents the G4Cons Geant Geometry entity            */
/* Author:           Joe Boudreau Nov 11 1996                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/

// this :
#include "HEPVis/nodes/SoCons.h"

#include <assert.h>
#include <cmath>
#include <Inventor/SbBox.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>

#include "HEPVis/SbMath.h"

// This statement is required
SO_NODE_SOURCE(SoCons)

// Constructor
SoCons::SoCons() {
  // This statement is required
  SO_NODE_CONSTRUCTOR(SoCons);

  // Data fields are initialized like this:
  SO_NODE_ADD_FIELD(fRmin1,               (0.0));
  SO_NODE_ADD_FIELD(fRmin2,               (0.0));
  SO_NODE_ADD_FIELD(fRmax1,               (1.0));
  SO_NODE_ADD_FIELD(fRmax2,               (1.0));
  SO_NODE_ADD_FIELD(fDz,                 (10.0));
  SO_NODE_ADD_FIELD(fSPhi,                (0.0));
  SO_NODE_ADD_FIELD(fDPhi,             ((G4float)(2*M_PI)));
  SO_NODE_ADD_FIELD(smoothDraw,          (TRUE));
  SO_NODE_ADD_FIELD(alternateRep,        (NULL));
  children = new SoChildList(this);
}

// Destructor
SoCons::~SoCons() {
  delete children;
}


// initClass
void SoCons::initClass(){
  // This statement is required.
  static bool first = true;
  if (first) {
    first = false;
    SO_NODE_INIT_CLASS(SoCons,SoShape,"Shape");
  }
}


// generatePrimitives
void SoCons::generatePrimitives(SoAction *action) {
  // This variable is used to store each vertex
  SoPrimitiveVertex pv;

  // Access the stat from the action
  SoState *state = action->getState();

  // See if we have to use a texture coordinate function,
  // rather than generating explicit texture coordinates.
  SbBool useTexFunction=
    (SoTextureCoordinateElement::getType(state) == 
     SoTextureCoordinateElement::FUNCTION);

  // If we need to generate texture coordinates with a function,
  // we'll need an SoGLTextureCoordinateElement.  Otherwise, we'll
  // set up the coordinates directly.
  const SoTextureCoordinateElement *tce = NULL;
  SbVec4f texCoord;
  if (useTexFunction) {
    tce = SoTextureCoordinateElement::getInstance(state);
  } else {
    texCoord[2] = 0.0;
    texCoord[3] = 1.0;
  }
  SbVec3f point, normal;

  ///////////////////////////////////////////////////////
  //-----------------------------------------------------
#define GEN_VERTEX(pv,x,y,z,s,t,nx,ny,nz)               \
  point.setValue((G4float)(x),(G4float)(y),(G4float)(z));     \
  normal.setValue((G4float)(nx),(G4float)(ny),(G4float)(nz)); \
  if (useTexFunction) {                                 \
    texCoord=tce->get(point,normal);                    \
  } else {                                              \
    texCoord[0]=(G4float)(s);                             \
    texCoord[1]=(G4float)(t);                             \
  }                                                     \
  pv.setPoint(point);                                   \
  pv.setNormal(normal);                                 \
  pv.setTextureCoords(texCoord);                        \
  shapeVertex(&pv);
  //-----------------------------------------------------
  ///////////////////////////////////////////////////////


  int NPHI = (int)(2+22*std::fabs(fDPhi.getValue()/(2.0*M_PI)));
  G4double deltaPhi = fDPhi.getValue()/NPHI;
  G4double phi0     = fSPhi.getValue();
  G4double phi1     = phi0 + fDPhi.getValue();
  G4double rMax1    = fRmax1.getValue();
  G4double rMin1    = fRmin1.getValue();
  G4double rMax2    = fRmax2.getValue();
  G4double rMin2    = fRmin2.getValue();
  G4double zMax     = fDz.getValue();
  G4double zMin     = -zMax;
  G4double cosPhi0  = std::cos(phi0);
  G4double sinPhi0  = std::sin(phi0);
  G4double cosPhi1  = std::cos(phi1);
  G4double sinPhi1  = std::sin(phi1);
  G4double cosDeltaPhi = std::cos(deltaPhi);
  G4double sinDeltaPhi = std::sin(deltaPhi);
  //
  // The outer surface!
  //
  beginShape(action,TRIANGLE_STRIP);
  int    i;
  G4double sinPhi=sinPhi0;
  G4double cosPhi=cosPhi0;
  for (i = 0; i<=NPHI; i++) {
    GEN_VERTEX(pv,rMax2*cosPhi,rMax2*sinPhi,zMax,0.0,0.0,cosPhi,sinPhi,0);   
    GEN_VERTEX(pv,rMax1*cosPhi,rMax1*sinPhi,zMin,1.0,1.0,cosPhi,sinPhi,0);   
    inc(sinPhi, cosPhi, sinDeltaPhi, cosDeltaPhi);    
  }
  endShape();
  //
  // The inner surface!
  //
  beginShape(action,TRIANGLE_STRIP);
  sinPhi=sinPhi0;
  cosPhi=cosPhi0;
  for (i = 0; i<=NPHI; i++) {
    GEN_VERTEX(pv,rMin2*cosPhi,rMin2*sinPhi,zMax,0.0,0.0,-cosPhi,-sinPhi,0);   
    GEN_VERTEX(pv,rMin1*cosPhi,rMin1*sinPhi,zMin,1.0,1.0,-cosPhi,-sinPhi,0);   
    inc(sinPhi, cosPhi, sinDeltaPhi, cosDeltaPhi);    
  } 
  endShape();
  if (std::fabs(deltaPhi)<2.0*M_PI) { 
    //
    // The end 
    //
    beginShape(action,TRIANGLE_STRIP);
    sinPhi=sinPhi0;
    cosPhi=cosPhi0;
    GEN_VERTEX(pv,rMax2*cosPhi,rMax2*sinPhi,zMax,0.0,0.0,sinPhi,-cosPhi,0);   
    GEN_VERTEX(pv,rMax1*cosPhi,rMax1*sinPhi,zMin,1.0,1.0,sinPhi,-cosPhi,0);   
    GEN_VERTEX(pv,rMin2*cosPhi,rMin2*sinPhi,zMax,1.0,0.0,sinPhi,-cosPhi,0);   
    GEN_VERTEX(pv,rMin1*cosPhi,rMin1*sinPhi,zMin,0.0,1.0,sinPhi,-cosPhi,0);   
    endShape();
    //
    // The other end 
    //
    beginShape(action,TRIANGLE_STRIP);
    sinPhi=sinPhi1;
    cosPhi=cosPhi1;
    GEN_VERTEX(pv,rMax2*cosPhi,rMax2*sinPhi, zMax,0.0,0.0,-sinPhi,+cosPhi,0);  
    GEN_VERTEX(pv,rMax1*cosPhi,rMax1*sinPhi, zMin,1.0,1.0,-sinPhi,+cosPhi,0);  
    GEN_VERTEX(pv,rMin2*cosPhi,rMin2*sinPhi, zMax,1.0,0.0,-sinPhi,+cosPhi,0);  
    GEN_VERTEX(pv,rMin1*cosPhi,rMin1*sinPhi, zMin,0.0,1.0,-sinPhi,+cosPhi,0);  
    endShape();
  }

  //
  // The outer surface at z=+PDZ
  //
  beginShape(action,TRIANGLE_STRIP);
  sinPhi=sinPhi0;
  cosPhi=cosPhi0;
  for (i = 0; i<=NPHI; i++) {
    GEN_VERTEX(pv,rMin2*cosPhi,rMin2*sinPhi,zMax,0.0,0.0,0,0,1);   
    GEN_VERTEX(pv,rMax2*cosPhi,rMax2*sinPhi,zMax,1.0,1.0,0,0,1);   
    inc(sinPhi, cosPhi, sinDeltaPhi, cosDeltaPhi);    
  }
  endShape();
  //
  // The outer surface at z=-PDZ
  //
  beginShape(action,TRIANGLE_STRIP);
  sinPhi=sinPhi0;
  cosPhi=cosPhi0;
  for (i = 0; i<=NPHI; i++) {
    GEN_VERTEX(pv,rMin1*cosPhi,rMin1*sinPhi,zMin,0.0,0.0,0,0,-1);   
    GEN_VERTEX(pv,rMax1*cosPhi,rMax1*sinPhi,zMin,1.0,1.0,0,0,-1);   
    inc(sinPhi, cosPhi, sinDeltaPhi, cosDeltaPhi);    
  }
  endShape();

}

// getChildren
SoChildList *SoCons::getChildren() const {
  return children;
}


// computeBBox
void SoCons::computeBBox(SoAction *, SbBox3f &box, SbVec3f &center ){
  G4float fRmax= fRmax1.getValue();  
  if (fRmax2.getValue() > fRmax) fRmax = fRmax2.getValue(); 

  SbVec3f vmin(-fRmax,-fRmax,-fDz.getValue()), 
          vmax( fRmax, fRmax, fDz.getValue());
  center.setValue(0,0,0);
  box.setBounds(vmin,vmax);
}




// updateChildren
void SoCons::updateChildren() {


  // Redraw the G4Cons....

  assert(children->getLength()==1);
  SoSeparator       *sep                = (SoSeparator *)  ( *children)[0];
  SoCoordinate3     *theCoordinates     = (SoCoordinate3 *)      ( sep->getChild(0));
  SoNormal          *theNormals         = (SoNormal *)           ( sep->getChild(1)); 
  SoNormalBinding   *theNormalBinding   = (SoNormalBinding *)    ( sep->getChild(2));
  SoIndexedFaceSet  *theFaceSet         = (SoIndexedFaceSet *)   ( sep->getChild(3));

  const int NPHI=24, NPOINTS=2*(2*NPHI+2), NFACES=4*NPHI+2, NINDICES = NFACES*5;
  G4float points[NPOINTS][3], normals[NFACES][3];
#ifdef INVENTOR2_0
  static long     indices[NINDICES];
#else
  static int32_t  indices[NINDICES];
#endif
  static int init=0;
  G4double phi, pp, DeltaPhi;

  // Indices need to be generated once! This is here to keep it close to the point
  // generation, since otherwise it will be confusing.

  int i;
  if (!init) {
    init = 1;
    // Outer face
    for (i = 0; i< NPHI; i++) {
      // 0 1 3 2;
      indices[5*i+0] =  2*i+0;
      indices[5*i+1] =  2*i+1;
      indices[5*i+2] =  2*i+3;
      indices[5*i+3] =  2*i+2;
      indices[5*i+4] = SO_END_FACE_INDEX;
    }
    // the inner face
    for (i=0;i<NPHI;i++) {
      indices[5*1*NPHI + 5*i+0] = 2*NPHI+2 + 2*i+0;  
      indices[5*1*NPHI + 5*i+1] = 2*NPHI+2 + 2*i+1;
      indices[5*1*NPHI + 5*i+2] = 2*NPHI+2 + 2*i+3;
      indices[5*1*NPHI + 5*i+3] = 2*NPHI+2 + 2*i+2;
      indices[5*1*NPHI + 5*i+4] = SO_END_FACE_INDEX;
    }
    // the top side
    for (i=0;i<NPHI;i++) {
      indices[5*2*NPHI + 5*i+0] = 2*i+0;
      indices[5*2*NPHI + 5*i+1] = 2*i+2;
      indices[5*2*NPHI + 5*i+2] = NPOINTS - (2*i+4);
      indices[5*2*NPHI + 5*i+3] = NPOINTS - (2*i+2);
      indices[5*2*NPHI + 5*i+4] = SO_END_FACE_INDEX;
    }
    // the bottom side
    for (i=0;i<NPHI;i++) {
      indices[5*3*NPHI + 5*i+0] = 2*i+1;
      indices[5*3*NPHI + 5*i+1] = NPOINTS - (2*i+1);
      indices[5*3*NPHI + 5*i+2] = NPOINTS - (2*i+3);
      indices[5*3*NPHI + 5*i+3] = 2*i+3;
      indices[5*3*NPHI + 5*i+4] = SO_END_FACE_INDEX;
    }
    // the odd side
    indices[5*4*NPHI +0] = 2*NPHI;
    indices[5*4*NPHI +1] = 2*NPHI+1;
    indices[5*4*NPHI +2] = 2*NPHI+3;
    indices[5*4*NPHI +3] = 2*NPHI+2;
    indices[5*4*NPHI +4] = SO_END_FACE_INDEX;
    // aother odd side
    indices[5*4*NPHI +5 +0] = 0;
    indices[5*4*NPHI +5 +1] = NPOINTS-2;
    indices[5*4*NPHI +5 +2] = NPOINTS-1;
    indices[5*4*NPHI +5 +3] = 1;
    indices[5*4*NPHI +5 +4] = SO_END_FACE_INDEX;
  }
  // Points need to be generated each time:
  // The outer surface
  DeltaPhi = fDPhi.getValue()/NPHI, phi = fSPhi.getValue();
  G4float  t,st,ct;
  t = FATAN((fRmax2.getValue()-fRmax1.getValue())/(2*fDz.getValue()));
  st = FSIN(t);
  ct = FCOS(t);
  for (i = 0; i<=NPHI; i++) {
    points[2*i+0][0] = fRmax2.getValue()*FCOS(phi); 
    points[2*i+0][1] = fRmax2.getValue()*FSIN(phi); 
    points[2*i+0][2] = +fDz.getValue();
    points[2*i+1][0] = fRmax1.getValue()*FCOS(phi); 
    points[2*i+1][1] = fRmax1.getValue()*FSIN(phi); 
    points[2*i+1][2] = -fDz.getValue();
    pp = phi+DeltaPhi/2.0;
    if (i!=NPHI) {
      normals[i][0] = ct * FCOS(pp); 
      normals[i][1] = ct * FSIN(pp); 
      normals[i][2] = -st;
    }
    phi+=DeltaPhi;
  }
  // The inner surface
  phi = fSPhi.getValue() + fDPhi.getValue();
  t = FATAN((fRmin2.getValue()-fRmin1.getValue())/(2*fDz.getValue()));
  st = FSIN(t);
  ct = FCOS(t);
  for (i = 0; i<=NPHI; i++) {
    points[2*NPHI+2+2*i+0][0] = fRmin2.getValue()*FCOS(phi); 
    points[2*NPHI+2+2*i+0][1] = fRmin2.getValue()*FSIN(phi); 
    points[2*NPHI+2+2*i+0][2] = +fDz.getValue();
    points[2*NPHI+2+2*i+1][0] = fRmin1.getValue()*FCOS(phi); 
    points[2*NPHI+2+2*i+1][1] = fRmin1.getValue()*FSIN(phi); 
    points[2*NPHI+2+2*i+1][2] = -fDz.getValue();
    pp = phi-DeltaPhi/2.0;
    if (i!=NPHI) {
      normals[NPHI+i][0] = -ct*FCOS(pp); 
      normals[NPHI+i][1] = -ct*FSIN(pp); 
      normals[NPHI+i][2] = st;
    }
    phi-=DeltaPhi;
  }
  // The top side
  for (i=0;i<NPHI;i++) {
    normals[2*NPHI+i][0]=normals[2*NPHI+i][1]=0; 
    normals[2*NPHI+i][2]=  1.0;
  } 
  // The bottom side
  for (i=0;i<NPHI;i++) {
    normals[3*NPHI+i][0]=normals[3*NPHI+i][1]=0; 
    normals[3*NPHI+i][2]= -1.0;
  } 
  // The odd side
  phi = fSPhi.getValue(); 
  normals[4*NPHI+0][0]=  FSIN(phi); 
  normals[4*NPHI+0][1]= -FCOS(phi); 
  normals[4*NPHI+0][2]= 0;

  // Another odd side
  phi = fSPhi.getValue()+fDPhi.getValue(); 
  normals[4*NPHI+1][0]= -FSIN(phi); 
  normals[4*NPHI+1][1]= +FCOS(phi); 
  normals[4*NPHI+1][2]=0;

  for (int np=0;np<NPOINTS;np++) theCoordinates->point.set1Value(np,points[np][0],points[np][1],points[np][2]);
  theFaceSet->coordIndex.setValues(0,NINDICES,indices);
  if (smoothDraw.getValue()) {
    //    This Line is replaced by the next one because of an apparent Bug in Inventor (mem. leak).
    //    theNormals->vector.deleteValues(0);
    for (int nf=0;nf<NFACES;nf++) theNormals->vector.set1Value(nf,normals[nf][0],normals[nf][1],normals[nf][2]);
    theNormalBinding->value=SoNormalBinding::PER_FACE;
  }
  else {
    for (int nf=0;nf<NFACES;nf++) theNormals->vector.set1Value(nf,normals[nf][0],normals[nf][1],normals[nf][2]);
    theNormalBinding->value=SoNormalBinding::PER_FACE;
  }
}

// generateChildren
void SoCons::generateChildren() {

  // This routines creates one SoSeparator, one SoCoordinate3, and
  // one SoLineSet, and puts it in the child list.  This is done only
  // once, whereas redrawing the position of the coordinates occurs each
  // time an update is necessary, in the updateChildren routine. 

  assert(children->getLength() ==0);
  SoSeparator      *sep              = new SoSeparator(); 
  SoCoordinate3    *theCoordinates   = new SoCoordinate3();
  SoNormal         *theNormals       = new SoNormal(); 
  SoNormalBinding  *theNormalBinding = new SoNormalBinding();
  SoIndexedFaceSet *theFaceSet       = new SoIndexedFaceSet();
  // 
  // This line costs some in render quality! but gives speed.
  // 
  sep->addChild(theCoordinates);
  sep->addChild(theNormals);
  sep->addChild(theNormalBinding);
  sep->addChild(theFaceSet);
  children->append(sep);
}

// generateAlternateRep
void SoCons::generateAlternateRep() {

  // This routine sets the alternate representation to the child
  // list of this mode.  

  if (children->getLength() == 0) generateChildren();
  updateChildren();
  alternateRep.setValue((SoSeparator *)  ( *children)[0]);
}

// clearAlternateRep
void SoCons::clearAlternateRep() {
  alternateRep.setValue(NULL);
}
