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
// Author: Alfonso Mantero (Alfonso.Mantero@ge.infn.it)
//
// History:
// -----------
//  2 June 2002 First committed to cvs
//
// -------------------------------------------------------------------
// Class description:
// Low Energy Electromagnetic Physics
// This Class loads and stores all the information of auger effect (shellIds, 
// probabilities and  energies of the electrons emitted) 
// -------------------------------------------------------------------

#ifndef G4AUGERDATA_HH
#define G4AUGERDATA_HH 1

#include "globals.hh"
#include <vector>
#include <map>
#include "G4AugerTransition.hh"

class G4DataVector;

class G4AugerData
{
public:

  explicit G4AugerData();
  ~G4AugerData();

  /// The method returns the number of shells in wich a 
  /// vacancy can be filled by a NON-radiative transition, given the atomic number
  size_t NumberOfVacancies(G4int Z) const;

  /// Given the index of the vacancy (and the atomic number Z) returns its identity
  G4int VacancyId(G4int Z, G4int vacancyIndex) const;
  
  /// Given the index of a vacancy in the atom with the atomc number Z, returns the number of
  /// shells starting from wich an electron can fill the vacancy
  size_t NumberOfTransitions(G4int Z, G4int vacancyIndex) const;

  /// Given the atomic number Z, the Index of the initial vacancy shell 
  /// and the index of the starting shell for the 
  /// transition, returns the identity of the shell originating the electron transition
  G4int StartShellId(G4int Z, G4int initialVacancyIndex, G4int transitionShellIndex) const;

  /// Given the atomic number , the indexes of the starting, the auger originating shell, 
  /// and the transition shell Id, returns the transition energy
  G4double StartShellEnergy(G4int Z, G4int vacancyIndex, G4int transitionId, G4int augerIndex) const;

  /// Given the atomic number, the  index of the starting shell, the auger originating shells, 
  /// and the transition shell Id, returns the transition probability
  G4double StartShellProb(G4int Z, G4int vacancyIndex,G4int transitionId,G4int augerIndex) const;

  /// Given the atomic number, the index of the starting vacancy shell and the transition shell Id,
  /// returns the number of shells wich an auger electron can come from.
  size_t NumberOfAuger(G4int Z, G4int initIndex, G4int vacancyId) const;

  /// Given the atomic number, th index of the starting and the auger originating shell, 
  /// and the transition shell Id, returns the ager originating shell Id
  size_t AugerShellId(G4int Z, G4int vacancyIndex, G4int transId, G4int augerIndex) const;

  std::vector<G4AugerTransition> LoadData(G4int Z);
  void BuildAugerTransitionTable();
  void PrintData(G4int Z);

  /// Given the atomic number and the vacancy initial shell index  returns 
  /// the AugerTransition object related to that shell
  G4AugerTransition* GetAugerTransition(G4int Z, G4int vacancyShellIndex);
  
  /// Given the atomic number returns a vector of possible AugerTransition objects
  std::vector<G4AugerTransition>* GetAugerTransitions(G4int Z);

private:
  typedef std::map<G4int,std::vector<G4AugerTransition>,std::less<G4int> > trans_Table;
  trans_Table augerTransitionTable;

  std::vector<G4int> nInitShells;
  std::vector<G4int> numberOfVacancies;
  
};

#endif





