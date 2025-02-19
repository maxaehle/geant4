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
// Authors: Elena Guardincerri (Elena.Guardincerri@ge.infn.it)
//          Alfonso Mantero (Alfonso.Mantero@ge.infn.it)
//
// History:
// -----------
//  
//  16 Sept 2001  First committed to cvs
//  12 Sep  2003  Bug in auger production fixed
//
// -------------------------------------------------------------------

#include "G4AtomicDeexcitation.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4AtomicTransitionManager.hh"
#include "G4FluoTransition.hh"

G4AtomicDeexcitation::G4AtomicDeexcitation():
  minGammaEnergy(100.*eV),
  minElectronEnergy(100.*eV),
  fAuger(false)
{

  G4cout << " ********************************************************** " << G4endl; 
  G4cout << " *                  W A R N I N G ! ! !                   * " << G4endl;
  G4cout << " ********************************************************** " << G4endl;
  G4cout << " *                                                        * " << G4endl; 
  G4cout << " *  Class G4AtomicDeexcitation is obsolete. It has been   * " << G4endl;
  G4cout << " * discontinued and is going to be removed by next Geant4 * " << G4endl; 
  G4cout << " *     release please migrate to G4UAtomDeexcitation.     * " << G4endl;
  G4cout << " *                                                        * " << G4endl;
  G4cout << " ********************************************************** " << G4endl; 

  augerVacancyId=0;
  newShellId=0;
}

G4AtomicDeexcitation::~G4AtomicDeexcitation()
{}

std::vector<G4DynamicParticle*>* G4AtomicDeexcitation::GenerateParticles(G4int Z,G4int givenShellId)
{ 

  std::vector<G4DynamicParticle*>* vectorOfParticles;
  vectorOfParticles = new std::vector<G4DynamicParticle*>;

  G4DynamicParticle* aParticle;
  G4int provShellId = 0;
  G4int counter = 0;
  
  // The aim of this loop is to generate more than one fluorecence photon 
  // from the same ionizing event 
  do
    {
      if (counter == 0) 
	// First call to GenerateParticles(...):
	// givenShellId is given by the process
	{
	  provShellId = SelectTypeOfTransition(Z, givenShellId);
	  
	  if  ( provShellId >0) 
	    {
	      aParticle = GenerateFluorescence(Z,givenShellId,provShellId);  
	    }
	  else if ( provShellId == -1)
	    {
	      aParticle = GenerateAuger(Z, givenShellId);
	    }
	  else
	    {
	      G4Exception("G4AtomicDeexcitation::Constructor", "de0002", JustWarning, "Transition selection invalid, energy local deposited");
	    }
	}
      else 
	// Following calls to GenerateParticles(...):
	// newShellId is given by GenerateFluorescence(...)
	{
	  provShellId = SelectTypeOfTransition(Z,newShellId);
	  if  (provShellId >0)
	    {
	      aParticle = GenerateFluorescence(Z,newShellId,provShellId);
	    }
	  else if ( provShellId == -1)
	    {
	      aParticle = GenerateAuger(Z, newShellId);
	    }
	  else
	    {
	      G4Exception("G4AtomicDeexcitation::constructor", "de0002", JustWarning, "Transition selection invalid, energy local deposited" );
	    }
	}
      counter++;
      if (aParticle != 0) {vectorOfParticles->push_back(aParticle);}
      else {provShellId = -2;}
    }
  
  // Look this in a particular way: only one auger emitted! // ????
  while (provShellId > -2); 

  // debug  
  // if (vectorOfParticles->size() > 0) {
  //   G4cout << " DEEXCITATION!" << G4endl;
  // }

  return vectorOfParticles;
}

G4int G4AtomicDeexcitation::SelectTypeOfTransition(G4int Z, G4int shellId)
{
  if (shellId <=0 ) 
    {G4Exception("G4AtomicDeexcitation::SelectTypeOfTransition()","de0002", JustWarning ,"zero or negative shellId");}

  //G4bool fluoTransitionFoundFlag = false;
  
  const G4AtomicTransitionManager*  transitionManager = 
        G4AtomicTransitionManager::Instance();
  G4int provShellId = -1;
  G4int shellNum = 0;
  G4int maxNumOfShells = transitionManager->NumberOfReachableShells(Z);  
  
  const G4FluoTransition* refShell = transitionManager->ReachableShell(Z,maxNumOfShells-1);

  // This loop gives shellNum the value of the index of shellId
  // in the vector storing the list of the shells reachable through
  // a radiative transition
  if ( shellId <= refShell->FinalShellId())
    {
      while (shellId != transitionManager->ReachableShell(Z,shellNum)->FinalShellId())
	{
	  if(shellNum ==maxNumOfShells-1)
	    {
	      break;
	    }
	  shellNum++;
	}
      G4int transProb = 0; //AM change 29/6/07 was 1
   
      G4double partialProb = G4UniformRand();      
      G4double partSum = 0;
      const G4FluoTransition* aShell = transitionManager->ReachableShell(Z,shellNum);      
      G4int trSize =  (aShell->TransitionProbabilities()).size();
    
      // Loop over the shells wich can provide an electron for a 
      // radiative transition towards shellId:
      // in every loop the partial sum of the first transProb shells
      // is calculated and compared with a random number [0,1].
      // If the partial sum is greater, the shell whose index is transProb
      // is chosen as the starting shell for a radiative transition
      // and its identity is returned
      // Else, terminateded the loop, -1 is returned
      while(transProb < trSize){
	
	 partSum += aShell->TransitionProbability(transProb);

	 if(partialProb <= partSum)
	   {
	     provShellId = aShell->OriginatingShellId(transProb);
	     //fluoTransitionFoundFlag = true;

	     break;
	   }
	 transProb++;
      }

      // here provShellId is the right one or is -1.
      // if -1, the control is passed to the Auger generation part of the package 
    }
  else 
    provShellId = -1;

  return provShellId;
}

G4DynamicParticle* G4AtomicDeexcitation::GenerateFluorescence(G4int Z, 
							      G4int shellId,
							      G4int provShellId )
{ 
  const G4AtomicTransitionManager*  transitionManager = G4AtomicTransitionManager::Instance();
  //  G4int provenienceShell = provShellId;

  //isotropic angular distribution for the outcoming photon
  G4double newcosTh = 1.-2.*G4UniformRand();
  G4double  newsinTh = std::sqrt(1.-newcosTh*newcosTh);
  G4double newPhi = twopi*G4UniformRand();
  
  G4double xDir =  newsinTh*std::sin(newPhi);
  G4double yDir = newsinTh*std::cos(newPhi);
  G4double zDir = newcosTh;
  
  G4ThreeVector newGammaDirection(xDir,yDir,zDir);
  
  G4int shellNum = 0;
  G4int maxNumOfShells = transitionManager->NumberOfReachableShells(Z);
  
  // find the index of the shell named shellId
  while (shellId != transitionManager->
	 ReachableShell(Z,shellNum)->FinalShellId())
    {
      if(shellNum == maxNumOfShells-1)
	{
	  break;
	}
      shellNum++;
    }
  // number of shell from wich an electron can reach shellId
  size_t transitionSize = transitionManager->
    ReachableShell(Z,shellNum)->OriginatingShellIds().size();
  
  size_t index = 0;
  
  // find the index of the shell named provShellId in the vector
  // storing the shells from which shellId can be reached 
  while (provShellId != transitionManager->
	 ReachableShell(Z,shellNum)->OriginatingShellId(index))
    {
      if(index ==  transitionSize-1)
	{
	  break;
	}
      index++;
    }
  // energy of the gamma leaving provShellId for shellId
  G4double transitionEnergy = transitionManager->
    ReachableShell(Z,shellNum)->TransitionEnergy(index);
  
  // This is the shell where the new vacancy is: it is the same
  // shell where the electron came from
  newShellId = transitionManager->
    ReachableShell(Z,shellNum)->OriginatingShellId(index);
  
  G4DynamicParticle* newPart = new G4DynamicParticle(G4Gamma::Gamma(), 
						     newGammaDirection,
						     transitionEnergy);
  return newPart;
}

G4DynamicParticle* G4AtomicDeexcitation::GenerateAuger(G4int Z, G4int shellId)
{
  if(!fAuger) return 0;
  
  const G4AtomicTransitionManager*  transitionManager = 
        G4AtomicTransitionManager::Instance();

  if (shellId <=0 ) 
    {G4Exception("G4AtomicDeexcitation::GenerateAuger()","de0002", JustWarning ,"zero or negative shellId");}
  
  // G4int provShellId = -1;
  G4int maxNumOfShells = transitionManager->NumberOfReachableAugerShells(Z);  
  
  const G4AugerTransition* refAugerTransition = 
        transitionManager->ReachableAugerShell(Z,maxNumOfShells-1);


  // This loop gives to shellNum the value of the index of shellId
  // in the vector storing the list of the vacancies in the variuos shells 
  // that can originate a NON-radiative transition
  G4int shellNum = 0;

  if ( shellId <= refAugerTransition->FinalShellId() ) 
    //"FinalShellId" is final from the point of view of the elctron who makes the transition, 
    // being the Id of the shell in which there is a vacancy
    {
      G4int pippo = transitionManager->ReachableAugerShell(Z,shellNum)->FinalShellId();
      if (shellId  != pippo ) {
	do { 
	  shellNum++;
 	  if(shellNum == maxNumOfShells)
 	    {

 	      //G4Exception("G4AtomicDeexcitation: No Auger transition found");
	      return 0;
 	    }
	}
 	while (shellId != (transitionManager->ReachableAugerShell(Z,shellNum)->FinalShellId()) ) ;
      }

      G4int transitionLoopShellIndex = 0;      
      G4double partSum = 0;
      const G4AugerTransition* anAugerTransition = 
            transitionManager->ReachableAugerShell(Z,shellNum);

      G4int transitionSize = 
            (anAugerTransition->TransitionOriginatingShellIds())->size();
      while (transitionLoopShellIndex < transitionSize) {

        std::vector<G4int>::const_iterator pos = 
               anAugerTransition->TransitionOriginatingShellIds()->begin();

        G4int transitionLoopShellId = *(pos+transitionLoopShellIndex);
        G4int numberOfPossibleAuger = 
              (anAugerTransition->AugerTransitionProbabilities(transitionLoopShellId))->size();
        G4int augerIndex = 0;

	if (augerIndex < numberOfPossibleAuger) {	  
	  do 
	    {
	      G4double thisProb = anAugerTransition->AugerTransitionProbability(augerIndex, 
										transitionLoopShellId);
	      partSum += thisProb;
	      augerIndex++;
	      
	    } while (augerIndex < numberOfPossibleAuger);
		}
        transitionLoopShellIndex++;
      }
      

      // Now we have the entire probability of an auger transition for the vacancy 
      // located in shellNum (index of shellId) 
      G4double totalVacancyAugerProbability = partSum;

      //And now we start to select the right auger transition and emission
      G4int transitionRandomShellIndex = 0;
      G4int transitionRandomShellId = 1;
      G4int augerIndex = 0;
      partSum = 0; 
      G4double partialProb = G4UniformRand();
      // G4int augerOriginatingShellId = 0;
      
      G4int numberOfPossibleAuger = 0;
      
      G4bool foundFlag = false;

      while (transitionRandomShellIndex < transitionSize) {
        std::vector<G4int>::const_iterator pos = 
               anAugerTransition->TransitionOriginatingShellIds()->begin();

        transitionRandomShellId = *(pos+transitionRandomShellIndex);
        
	augerIndex = 0;
	numberOfPossibleAuger = (anAugerTransition-> 
				 AugerTransitionProbabilities(transitionRandomShellId))->size();

        while (augerIndex < numberOfPossibleAuger) {
	  G4double thisProb =anAugerTransition->AugerTransitionProbability(augerIndex, 
									   transitionRandomShellId);

          partSum += thisProb;
          
          if (partSum >= (partialProb*totalVacancyAugerProbability) ) { // was /
	    foundFlag = true;
	    break;
	  }
          augerIndex++;
        }
        if (partSum >= (partialProb*totalVacancyAugerProbability) ) {break;} // was /
        transitionRandomShellIndex++;
      }

      // Now we have the index of the shell from wich comes the auger electron (augerIndex), 
      // and the id of the shell, from which the transition e- come (transitionRandomShellid)
      // If no Transition has been found, 0 is returned.  

      if (!foundFlag) {return 0;}      
      
      // Isotropic angular distribution for the outcoming e-
      G4double newcosTh = 1.-2.*G4UniformRand();
      G4double  newsinTh = std::sqrt(1.-newcosTh*newcosTh);
      G4double newPhi = twopi*G4UniformRand();
      
      G4double xDir =  newsinTh*std::sin(newPhi);
      G4double yDir = newsinTh*std::cos(newPhi);
      G4double zDir = newcosTh;
      
      G4ThreeVector newElectronDirection(xDir,yDir,zDir);
      
      // energy of the auger electron emitted
          
      G4double transitionEnergy = anAugerTransition->AugerTransitionEnergy(augerIndex, transitionRandomShellId);
      /*
	G4cout << "AUger TransitionId " << anAugerTransition->FinalShellId() << G4endl;
	G4cout << "augerIndex: " << augerIndex << G4endl;
	G4cout << "transitionShellId: " << transitionRandomShellId << G4endl;
      */
      
      // This is the shell where the new vacancy is: it is the same
      // shell where the electron came from
      newShellId = transitionRandomShellId;
     
      G4DynamicParticle* newPart = new G4DynamicParticle(G4Electron::Electron(), 
							 newElectronDirection,
							 transitionEnergy);
      return newPart;
    }
  else 
    {
      //G4Exception("G4AtomicDeexcitation: no auger transition found");
      return 0;
    }
}

void G4AtomicDeexcitation::SetCutForSecondaryPhotons(G4double cut)
{
  minGammaEnergy = cut;
}

void G4AtomicDeexcitation::SetCutForAugerElectrons(G4double cut)
{
  minElectronEnergy = cut;
}

void G4AtomicDeexcitation::ActivateAugerElectronProduction(G4bool val)
{
  fAuger = val;
}







