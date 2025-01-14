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
// G4PSEnergyDeposit
#include "G4PSEnergyDeposit.hh"
#include "G4VScoreHistFiller.hh"
#include "G4UnitsTable.hh"
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 2010-07-22   Introduce Unit specification.
// 2020-09-03   Use G4VPrimitivePlotter and fill 1-D histo of energy deposit (x)
//              vs. track weight (y)                   (Makoto Asai)
//
///////////////////////////////////////////////////////////////////////////////

G4PSEnergyDeposit::G4PSEnergyDeposit(G4String name, G4int depth)
  : G4VPrimitivePlotter(name, depth)
  , HCID(-1)
  , EvtMap(0)
{
  SetUnit("MeV");
}

G4PSEnergyDeposit::G4PSEnergyDeposit(G4String name, const G4String& unit,
                                     G4int depth)
  : G4VPrimitivePlotter(name, depth)
  , HCID(-1)
  , EvtMap(0)
{
  SetUnit(unit);
}

G4PSEnergyDeposit::~G4PSEnergyDeposit() { ; }

G4bool G4PSEnergyDeposit::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep == 0.)
    return FALSE;
  G4double wei = aStep->GetPreStepPoint()->GetWeight();  // (Particle Weight)
  G4int index  = GetIndex(aStep);
  G4double edepwei = edep * wei;
  EvtMap->add(index, edepwei);

  if(hitIDMap.size() > 0 && hitIDMap.find(index) != hitIDMap.end())
  {
    auto filler = G4VScoreHistFiller::Instance();
    if(!filler)
    {
      G4Exception(
        "G4PSEnergyDeposit::ProcessHits", "SCORER0123", JustWarning,
        "G4TScoreHistFiller is not instantiated!! Histogram is not filled.");
    }
    else
    {
      filler->FillH1(hitIDMap[index], edep, wei);
    }
  }

  return TRUE;
}

void G4PSEnergyDeposit::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
                                    GetName());
  if(HCID < 0)
  {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*) EvtMap);
}

void G4PSEnergyDeposit::EndOfEvent(G4HCofThisEvent*) { ; }

void G4PSEnergyDeposit::clear() { EvtMap->clear(); }

void G4PSEnergyDeposit::DrawAll() { ; }

void G4PSEnergyDeposit::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int, G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++)
  {
    G4cout << "  copy no.: " << itr->first
           << "  energy deposit: " << *(itr->second) / GetUnitValue() << " ["
           << GetUnit() << "]" << G4endl;
  }
}

void G4PSEnergyDeposit::SetUnit(const G4String& unit)
{
  CheckAndSetUnit(unit, "Energy");
}
