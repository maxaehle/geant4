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
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software 
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
// 
// If you use this example, please cite the following publication:
// Rad. Prot. Dos. 133 (2009) 2-11

#include "CellParameterisation.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

// SINGLETON
CellParameterisation * CellParameterisation::gInstance = 0;

CellParameterisation::CellParameterisation
(G4Material * nucleus1,  G4Material * cytoplasm1,
 G4Material * nucleus2,  G4Material * cytoplasm2,
 G4Material * nucleus3,  G4Material * cytoplasm3
 )
{
   fNucleusMaterial1   = nucleus1;
   fCytoplasmMaterial1 = cytoplasm1;
   fNucleusMaterial2   = nucleus2;
   fCytoplasmMaterial2 = cytoplasm2;
   fNucleusMaterial3   = nucleus3;
   fCytoplasmMaterial3 = cytoplasm3;

   G4int ncols,nlines;
   G4int shiftX, shiftY, shiftZ;
   G4double x,y,z,mat,den,tmp,density;  
   G4double denCyto1, denCyto2, denCyto3, denNucl1, denNucl2, denNucl3;

   ncols = nlines = shiftX = shiftY = shiftZ = 0;
   x = y = z = mat = den = tmp = density =
     denCyto1 = denCyto2 = denCyto3 = denNucl1 = denNucl2 = denNucl3 = 0.0;
   
   // READ PHANTOM    
   fNucleusMass = 0;
   fCytoplasmMass = 0;

   fDimCellBoxX = fDimCellBoxY = fDimCellBoxZ = micrometer;
    
   FILE *fMap;
   fMap = fopen("phantom.dat","r");
    
   while (1) 
   {  
      if (nlines == 0) 
      {
        ncols = fscanf(fMap,"%i %i %i",&fPhantomTotalPixels,&fNucleusTotalPixels,&fCytoplasmTotalPixels);
	fMapCell    = new G4ThreeVector[fPhantomTotalPixels];
        fMaterial   = new G4double[fPhantomTotalPixels];
        fMass       = new G4double[fPhantomTotalPixels];
        fTissueType = new G4int[fPhantomTotalPixels];
      }
      
      if (nlines == 1)
      { 
        passivedouble fDimCellBoxX_val, fDimCellBoxY_val, fDimCellBoxZ_val;
        ncols = fscanf(fMap,"%lf %lf %lf",&fDimCellBoxX_val,&fDimCellBoxY_val,&fDimCellBoxZ_val);        
        fDimCellBoxX = fDimCellBoxX_val;
        fDimCellBoxY = fDimCellBoxY_val;
        fDimCellBoxZ = fDimCellBoxZ_val;
	fDimCellBoxX=fDimCellBoxX*micrometer;
        fDimCellBoxY=fDimCellBoxY*micrometer;
        fDimCellBoxZ=fDimCellBoxZ*micrometer;
      }

      // VOXEL SHIFT IN Z ASSUMED TO BE NEGATIVE
      if (nlines == 2) ncols = fscanf(fMap,"%i %i %i",&shiftX,&shiftY,&shiftZ);
      
      if (nlines == 3){
        passivedouble denCyto1_val, denCyto2_val, denCyto3_val;
        ncols = fscanf(fMap,"%lf %lf %lf",&denCyto1_val, &denCyto2_val, &denCyto3_val);
        denCyto1 = denCyto1_val; denCyto2 = denCyto2_val; denCyto3 = denCyto3_val;
      }
      
      if (nlines == 4){
        passivedouble denNucl1_val, denNucl2_val, denNucl3_val;
        ncols = fscanf(fMap,"%lf %lf %lf",&denNucl1_val, &denNucl2_val, &denNucl3_val);
        denNucl1 = denNucl1_val; denNucl2 = denNucl2_val; denNucl3 = denNucl3_val;
      }
      
      if (nlines >  4){
        passivedouble x_val, y_val, z_val, mat_val, den_val, tmp_val;
        ncols = fscanf(fMap,"%lf %lf %lf %lf %lf %lf",&x,&y,&z,&mat,&den,&tmp);
        x = x_val; y = y_val; z = z_val;
        mat = mat_val; den = den_val; tmp = tmp_val;
      }
      
      if (ncols  <  0) break;

      // VOXEL SHIFT IN ORDER TO CENTER PHANTOM
      G4ThreeVector v(x+shiftX,y+shiftY,z-1500/(fDimCellBoxZ/micrometer)-shiftZ); 
      if (nlines>4) 
      {

	  fMapCell[nlines-5]=v; 
	  fMaterial[nlines-5]=mat;
	  fMass[nlines-5]=den;
	  
	  // fTissueType: 1 is Cytoplasm - 2 is Nucleus
	  
          if( fMaterial[nlines-5] == 2 ) // fMaterial 2 is nucleus
          {
	    if( fMass[nlines-5] == 1 ) 
	 	{
		  fTissueType[nlines-5]=2;
		}
	    if( fMass[nlines-5] == 2 ) 
	 	{
		  fTissueType[nlines-5]=2;
		}
	    if( fMass[nlines-5] == 3 ) 
	 	{
		  fTissueType[nlines-5]=2;
		}
	  } 
	
          else if( fMaterial[nlines-5] == 1 ) // fMaterial 1 is cytoplasm
	  {
	    if( fMass[nlines-5] == 1 ) 
	 	{
		  fTissueType[nlines-5]=1;
		}
	    if( fMass[nlines-5] == 2 ) 
	 	{
		  fTissueType[nlines-5]=2;
		}
	    if( fMass[nlines-5] == 3 ) 
	 	{
		  fTissueType[nlines-5]=1;
		}
	  } 
	  	  	  
	  //	  
	  	  
	  if (std::abs(mat-2)<1.e-30) // NUCLEUS
    	  {
	    if (std::abs(den-1)<1.e-30) density = denNucl1*(g/cm3);
	    if (std::abs(den-2)<1.e-30) density = denNucl2*(g/cm3);
	    if (std::abs(den-3)<1.e-30) density = denNucl3*(g/cm3);
	    fNucleusMass   = fNucleusMass   + density * fDimCellBoxX * fDimCellBoxY * fDimCellBoxZ ;
    	  }

          if (std::abs(mat-1)<1.e-30) // CYTOPLASM
    	  { 
	    if (std::abs(den-1)<1e-30) density = denCyto1*(g/cm3);
	    if (std::abs(den-2)<1e-30) density = denCyto2*(g/cm3);
	    if (std::abs(den-3)<1e-30) density = denCyto3*(g/cm3);
	    fCytoplasmMass = fCytoplasmMass + density * fDimCellBoxX * fDimCellBoxY * fDimCellBoxZ ;
	  }
	  
      }	  

      nlines++;    
   }
   fclose(fMap);

  // NUCLEUS IN GREEN 
  
  fNucleusAttributes1 = new G4VisAttributes;
  fNucleusAttributes1->SetColour(G4Colour(0,.8,0));
  fNucleusAttributes1->SetForceSolid(false);

  fNucleusAttributes2 = new G4VisAttributes;
  fNucleusAttributes2->SetColour(G4Colour(0,.9,0));
  fNucleusAttributes2->SetForceSolid(false);

  fNucleusAttributes3 = new G4VisAttributes;
  fNucleusAttributes3->SetColour(G4Colour(0,1,0));
  fNucleusAttributes3->SetForceSolid(false);

  // CYTOPLASM IN RED
  
  fCytoplasmAttributes1 = new G4VisAttributes;
  fCytoplasmAttributes1->SetColour(G4Colour(1,0,0));
  fCytoplasmAttributes1->SetForceSolid(false);

  fCytoplasmAttributes2 = new G4VisAttributes; // nucleoli in yellow
  fCytoplasmAttributes2->SetColour(G4Colour(1.,1.,0));
  fCytoplasmAttributes2->SetForceSolid(false);

  fCytoplasmAttributes3 = new G4VisAttributes;
  fCytoplasmAttributes3->SetColour(G4Colour(1,0,0));
  fCytoplasmAttributes3->SetForceSolid(false);

  //
  gInstance = this;
 }

CellParameterisation::~CellParameterisation()
{
  delete[] fMapCell;
  delete[] fMaterial;
  delete[] fMass;
  delete[] fTissueType;
}

void CellParameterisation::ComputeTransformation
(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  G4ThreeVector origin
  (
      fMapCell[copyNo].x()*fDimCellBoxX,
      fMapCell[copyNo].y()*fDimCellBoxY,
      fMapCell[copyNo].z()*fDimCellBoxZ
  );

  physVol->SetTranslation(origin);   
}

void CellParameterisation::ComputeDimensions
(G4Box&, const G4int, const G4VPhysicalVolume*) const
{}

G4Material*
CellParameterisation::ComputeMaterial(const G4int copyNo,
                                      G4VPhysicalVolume* physVol,
                                      const G4VTouchable*)
{
    if( fMaterial[copyNo] == 2 ) // fMaterial 2 is nucleus
        {
	 if( fMass[copyNo] == 1 ) 
	 	{
	 	  physVol->GetLogicalVolume()->SetVisAttributes( fNucleusAttributes1 );
		  return fNucleusMaterial1;
		}
	 if( fMass[copyNo] == 2 ) 
	 	{
	 	  physVol->GetLogicalVolume()->SetVisAttributes( fNucleusAttributes2 );
		  return fNucleusMaterial2;
		}
	 if( fMass[copyNo] == 3 ) 
	 	{
	 	  physVol->GetLogicalVolume()->SetVisAttributes( fNucleusAttributes3 );
		  return fNucleusMaterial3;
		}
	} 
	
    else if( fMaterial[copyNo] == 1 ) // fMaterial 1 is cytoplasm
	{
	 if( fMass[copyNo] == 1 ) 
	 	{
	 	  physVol->GetLogicalVolume()->SetVisAttributes( fCytoplasmAttributes1 );
	 	  return fCytoplasmMaterial1;
		}
	 if( fMass[copyNo] == 2 ) 
	 	{
	 	  // nucleoli so taken as nucleus !
	 	  physVol->GetLogicalVolume()->SetVisAttributes( fCytoplasmAttributes2 );
	 	  return fCytoplasmMaterial2;
		}
	 if( fMass[copyNo] == 3 ) 
	 	{
	 	  physVol->GetLogicalVolume()->SetVisAttributes( fCytoplasmAttributes3 );
	 	  return fCytoplasmMaterial3;
		}
	} 

    return physVol->GetLogicalVolume()->GetMaterial();
}
