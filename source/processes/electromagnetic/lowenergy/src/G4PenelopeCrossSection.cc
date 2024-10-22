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
// Author: Luciano Pandola
//
// History:
// --------
// 18 Mar 2010   L Pandola    First implementation
// 09 Mar 2012   L. Pandola   Add public method (and machinery) to return
//                            the absolute and the normalized shell cross
//                            sections independently.
//
#include "G4PenelopeCrossSection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsFreeVector.hh"
#include "G4DataVector.hh"
#include "G4Exp.hh"
#include "G4Log.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...
G4PenelopeCrossSection::G4PenelopeCrossSection(size_t nPointsE,size_t nShells) :
  fSoftCrossSections(nullptr),
  fHardCrossSections(nullptr),fShellCrossSections(nullptr),
  fShellNormalizedCrossSections(nullptr),
  fNumberOfEnergyPoints(nPointsE),fNumberOfShells(nShells)
{
  //check the number of points is not zero
  if (!fNumberOfEnergyPoints)
    {
      G4ExceptionDescription ed;
      ed << "G4PenelopeCrossSection: invalid number of energy points " << G4endl;
      G4Exception("G4PenelopeCrossSection::G4PenelopeCrossSection()",
		  "em2017",FatalException,ed);
    }

  fIsNormalized = false;

  // 1) soft XS table
  fSoftCrossSections = new G4PhysicsTable();
  //the table contains 3 G4PhysicsFreeVectors,
  //(fSoftCrossSections)[0] -->  log XS0 vs. log E
  //(fSoftCrossSections)[1] -->  log XS1 vs. log E
  //(fSoftCrossSections)[2] -->  log XS2 vs. log E

  //everything is log-log
  for (size_t i=0;i<3;i++)
    fSoftCrossSections->push_back(new G4PhysicsFreeVector(fNumberOfEnergyPoints));

  //2) hard XS table
  fHardCrossSections = new G4PhysicsTable();
  //the table contains 3 G4PhysicsFreeVectors,
  //(fHardCrossSections)[0] -->  log XH0 vs. log E
  //(fHardCrossSections)[1] -->  log XH1 vs. log E
  //(fHardCrossSections)[2] -->  log XH2 vs. log E

  //everything is log-log
  for (size_t i=0;i<3;i++)
    fHardCrossSections->push_back(new G4PhysicsFreeVector(fNumberOfEnergyPoints));

  //3) shell XS table, if it is the case
  if (fNumberOfShells)
    {
      fShellCrossSections = new G4PhysicsTable();
      fShellNormalizedCrossSections = new G4PhysicsTable();
      //the table has to contain numberofShells G4PhysicsFreeVectors,
      //(theTable)[ishell] --> cross section for shell #ishell
      for (size_t i=0;i<fNumberOfShells;i++)
	{
	  fShellCrossSections->push_back(new G4PhysicsFreeVector(fNumberOfEnergyPoints));
	  fShellNormalizedCrossSections->push_back(new G4PhysicsFreeVector(fNumberOfEnergyPoints));
	}
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...
G4PenelopeCrossSection::~G4PenelopeCrossSection()
{
  //clean up tables
  if (fShellCrossSections)
    {
      fShellCrossSections->clearAndDestroy();
      delete fShellCrossSections;
    }
  if (fShellNormalizedCrossSections)
    {
      fShellNormalizedCrossSections->clearAndDestroy();
      delete fShellNormalizedCrossSections;
    }
  if (fSoftCrossSections)
    {
      fSoftCrossSections->clearAndDestroy();
      delete fSoftCrossSections;
    }
  if (fHardCrossSections)
    {
      fHardCrossSections->clearAndDestroy();
      delete fHardCrossSections;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...
void G4PenelopeCrossSection::AddCrossSectionPoint(size_t binNumber,G4double energy,
						  G4double XH0,
						  G4double XH1, G4double XH2,
						  G4double XS0, G4double XS1,
						  G4double XS2)
{
  if (!fSoftCrossSections || !fHardCrossSections)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::AddCrossSectionPoint" <<
	G4endl;
      G4cout << "Trying to fill un-initialized tables" << G4endl;
      return;
    }

  //fill vectors
  G4PhysicsFreeVector* theVector = (G4PhysicsFreeVector*) (*fSoftCrossSections)[0];

  if (binNumber >= fNumberOfEnergyPoints)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::AddCrossSectionPoint" <<
	G4endl;
      G4cout << "Trying to register more points than originally declared" << G4endl;
      return;
    }
   G4double logEne = G4Log(energy);

   //XS0
   G4double val = G4Log(std::max(XS0,1e-42*cm2)); //avoid log(0)
   theVector->PutValues(binNumber,logEne,val);

   //XS1
   theVector = (G4PhysicsFreeVector*) (*fSoftCrossSections)[1];
   val =  G4Log(std::max(XS1,1e-42*eV*cm2)); //avoid log(0)
   theVector->PutValues(binNumber,logEne,val);

   //XS2
   theVector = (G4PhysicsFreeVector*) (*fSoftCrossSections)[2];
   val =  G4Log(std::max(XS2,1e-42*eV*eV*cm2)); //avoid log(0)
   theVector->PutValues(binNumber,logEne,val);

   //XH0
   theVector = (G4PhysicsFreeVector*) (*fHardCrossSections)[0];
   val =  G4Log(std::max(XH0,1e-42*cm2)); //avoid log(0)
   theVector->PutValues(binNumber,logEne,val);

   //XH1
   theVector = (G4PhysicsFreeVector*) (*fHardCrossSections)[1];
   val =  G4Log(std::max(XH1,1e-42*eV*cm2)); //avoid log(0)
   theVector->PutValues(binNumber,logEne,val);

    //XH2
   theVector = (G4PhysicsFreeVector*) (*fHardCrossSections)[2];
   val =  G4Log(std::max(XH2,1e-42*eV*eV*cm2)); //avoid log(0)
   theVector->PutValues(binNumber,logEne,val);

   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...

void G4PenelopeCrossSection::AddShellCrossSectionPoint(size_t binNumber,
						       size_t shellID,
						       G4double energy,
						       G4double xs)
{
  if (!fShellCrossSections)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::AddShellCrossSectionPoint" <<
	G4endl;
      G4cout << "Trying to fill un-initialized table" << G4endl;
      return;
    }

  if (shellID >= fNumberOfShells)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::AddShellCrossSectionPoint" <<
	G4endl;
      G4cout << "Trying to fill shell #" << shellID << " while the maximum is "
	     <<  fNumberOfShells-1 << G4endl;
      return;
    }

  //fill vector
  G4PhysicsFreeVector* theVector = (G4PhysicsFreeVector*) (*fShellCrossSections)[shellID];

  if (binNumber >= fNumberOfEnergyPoints)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::AddShellCrossSectionPoint" <<
	G4endl;
      G4cout << "Trying to register more points than originally declared" << G4endl;
      return;
    }
   G4double logEne = G4Log(energy);
   G4double val = G4Log(std::max(xs,1e-42*cm2)); //avoid log(0)
   theVector->PutValues(binNumber,logEne,val);

   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...

G4double G4PenelopeCrossSection::GetTotalCrossSection(G4double energy) const
{
  G4double result = 0;
  //take here XS0 + XH0
  if (!fSoftCrossSections || !fHardCrossSections)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetTotalCrossSection" <<
	G4endl;
      G4cout << "Trying to retrieve from un-initialized tables" << G4endl;
      return result;
    }

  // 1) soft part
  G4PhysicsFreeVector* theVector = (G4PhysicsFreeVector*) (*fSoftCrossSections)[0];
  if (theVector->GetVectorLength() < fNumberOfEnergyPoints)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetTotalCrossSection" <<
	G4endl;
      G4cout << "Soft cross section table looks not filled" << G4endl;
      return result;
    }
  G4double logene = G4Log(energy);
  G4double logXS = theVector->Value(logene);
  G4double softXS = G4Exp(logXS);

   // 2) hard part
  theVector = (G4PhysicsFreeVector*) (*fHardCrossSections)[0];
  if (theVector->GetVectorLength() < fNumberOfEnergyPoints)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetTotalCrossSection" <<
	G4endl;
      G4cout << "Hard cross section table looks not filled" << G4endl;
      return result;
    }
  logXS = theVector->Value(logene);
  G4double hardXS = G4Exp(logXS);

  result = hardXS + softXS;
  return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...

G4double G4PenelopeCrossSection::GetHardCrossSection(G4double energy) const
{
  G4double result = 0;
  //take here XH0
  if (!fHardCrossSections)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetHardCrossSection" <<
	G4endl;
      G4cout << "Trying to retrieve from un-initialized tables" << G4endl;
      return result;
    }

  G4PhysicsFreeVector* theVector = (G4PhysicsFreeVector*) (*fHardCrossSections)[0];
  if (theVector->GetVectorLength() < fNumberOfEnergyPoints)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetHardCrossSection" <<
	G4endl;
      G4cout << "Hard cross section table looks not filled" << G4endl;
      return result;
    }
  G4double logene = G4Log(energy);
  G4double logXS = theVector->Value(logene);
  result = G4Exp(logXS);

  return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...

G4double G4PenelopeCrossSection::GetSoftStoppingPower(G4double energy) const
{
  G4double result = 0;
  //take here XH0
  if (!fSoftCrossSections)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetSoftStoppingPower" <<
	G4endl;
      G4cout << "Trying to retrieve from un-initialized tables" << G4endl;
      return result;
    }

  G4PhysicsFreeVector* theVector = (G4PhysicsFreeVector*) (*fSoftCrossSections)[1];
  if (theVector->GetVectorLength() < fNumberOfEnergyPoints)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetSoftStoppingPower" <<
	G4endl;
      G4cout << "Soft cross section table looks not filled" << G4endl;
      return result;
    }
  G4double logene = G4Log(energy);
  G4double logXS = theVector->Value(logene);
  result = G4Exp(logXS);

  return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo..

G4double G4PenelopeCrossSection::GetShellCrossSection(size_t shellID,G4double energy) const
{
  G4double result = 0;
  if (!fShellCrossSections)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetShellCrossSection" <<
	G4endl;
      G4cout << "Trying to retrieve from un-initialized tables" << G4endl;
      return result;
    }
  if (shellID >= fNumberOfShells)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetShellCrossSection" <<
	G4endl;
      G4cout << "Trying to retrieve shell #" << shellID << " while the maximum is "
	     <<  fNumberOfShells-1 << G4endl;
      return result;
    }

  G4PhysicsFreeVector* theVector = (G4PhysicsFreeVector*) (*fShellCrossSections)[shellID];

  if (theVector->GetVectorLength() < fNumberOfEnergyPoints)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetShellCrossSection" <<
	G4endl;
      G4cout << "Shell cross section table looks not filled" << G4endl;
      return result;
    }
  G4double logene = G4Log(energy);
  G4double logXS = theVector->Value(logene);
  result = G4Exp(logXS);

  return result;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo..

G4double G4PenelopeCrossSection::GetNormalizedShellCrossSection(size_t shellID,G4double energy) const
{
  G4double result = 0;
  if (!fShellNormalizedCrossSections)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetShellCrossSection" <<
	G4endl;
      G4cout << "Trying to retrieve from un-initialized tables" << G4endl;
      return result;
    }

  if (!fIsNormalized)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetShellCrossSection" << G4endl;
      G4cout << "The table of normalized cross section is not initialized" << G4endl;
    }

  if (shellID >= fNumberOfShells)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetShellCrossSection" <<
	G4endl;
      G4cout << "Trying to retrieve shell #" << shellID << " while the maximum is "
	     <<  fNumberOfShells-1 << G4endl;
      return result;
    }

  const G4PhysicsFreeVector* theVector =
    (G4PhysicsFreeVector*) (*fShellNormalizedCrossSections)[shellID];

  if (theVector->GetVectorLength() < fNumberOfEnergyPoints)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetShellCrossSection" <<
	G4endl;
      G4cout << "Shell cross section table looks not filled" << G4endl;
      return result;
    }
  G4double logene = G4Log(energy);
  G4double logXS = theVector->Value(logene);
  result = G4Exp(logXS);

  return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo..

void G4PenelopeCrossSection::NormalizeShellCrossSections()
{
  if (fIsNormalized) //already done!
    {
      G4cout << "G4PenelopeCrossSection::NormalizeShellCrossSections()" << G4endl;
      G4cout << "already invoked. Ignore it" << G4endl;
      return;
    }

  if (!fShellNormalizedCrossSections)
    {
      G4cout << "Something wrong in G4PenelopeCrossSection::GetShellCrossSection" <<
	G4endl;
      G4cout << "Trying to retrieve from un-initialized tables" << G4endl;
      return;
    }

  for (size_t i=0;i<fNumberOfEnergyPoints;i++) //loop on energy
    {
      //energy grid is the same for all shells

      //Recalculate manually the XS factor, to avoid problems with
      //underflows
      G4double normFactor = 0.;
      for (size_t shellID=0;shellID<fNumberOfShells;shellID++)
	{
	  G4PhysicsFreeVector* theVec =
	    (G4PhysicsFreeVector*) (*fShellCrossSections)[shellID];

	  normFactor += G4Exp((*theVec)[i]);
	}
      G4double logNormFactor = G4Log(normFactor);
      //Normalize
      for (size_t shellID=0;shellID<fNumberOfShells;shellID++)
	{
	 G4PhysicsFreeVector* theVec =
	    (G4PhysicsFreeVector*) (*fShellNormalizedCrossSections)[shellID];
	 G4PhysicsFreeVector* theFullVec =
	   (G4PhysicsFreeVector*) (*fShellCrossSections)[shellID];
	 G4double previousValue = (*theFullVec)[i]; //log(XS)
	 G4double logEnergy = theFullVec->GetLowEdgeEnergy(i);
	 //log(XS/normFactor) = log(XS) - log(normFactor)
	 theVec->PutValues(i,logEnergy,previousValue-logNormFactor);
	}
    }
  fIsNormalized = true;

  return;
}
