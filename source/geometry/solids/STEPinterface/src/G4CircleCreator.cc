// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4CircleCreator.cc,v 1.3 2000/02/25 16:36:18 gcosmo Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// ----------------------------------------------------------------------
// Class G4CircleCreator
//
// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
//
// History:
//   18-Nov-1999: First step of re-engineering - G.Cosmo
// ----------------------------------------------------------------------

#include "G4CircleCreator.hh"
#include "G4GeometryTable.hh"
#include "G4CircularCurve.hh"


G4CircleCreator G4CircleCreator::csc;

G4CircleCreator::G4CircleCreator()
{
  G4GeometryTable::RegisterObject(this);
}

G4CircleCreator::~G4CircleCreator() {}

void G4CircleCreator::CreateG4Geometry(STEPentity& Ent)
{
  G4Axis2Placement3D place;

  G4double radius;
  
  Ent.ResetAttributes();
  STEPattribute* Attr = Ent.NextAttribute();
  while(Attr->NonRefType() == STRING_TYPE ||
	Attr->NonRefType() == sdaiSTRING )
    Attr = Ent.NextAttribute();
  
  // Get the placement
  // SelectNode* SelectN = (SelectNode*)Attr->ptr.sh;
  // SdaiSelect* Select = (SdaiSelect*)SelectN;
  SdaiSelect* Select = Attr->ptr.sh;
  SdaiAxis2_placement* Place = (SdaiAxis2_placement*)Select;
  STEPentity* TmpEnt = Place->operator SdaiAxis2_placement_3dH();
  void * tmp =G4GeometryTable::CreateObject(*TmpEnt);
 
  if (tmp)
    place = *(G4Axis2Placement3D*)tmp;
  else
    G4cerr << "WARNING - G4CircleCreator::CreateG4Geometry" << G4endl
           << "\tUnexpected NULL axis placement (G4Axis2Placement3D) detected." << G4endl;
  
  Attr = Ent.NextAttribute();	
  radius = *Attr->ptr.r;

  G4CircularCurve* circle = new G4CircularCurve();
  circle->Init(place , radius);
  createdObject = circle;
}

void G4CircleCreator::CreateSTEPGeometry(void* G4obj)
{
}