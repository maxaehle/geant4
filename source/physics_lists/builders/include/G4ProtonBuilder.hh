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
//---------------------------------------------------------------------------
//
// ClassName:   G4ProtonBuilder
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 16.11.2005 G.Folger: don't  keep processes as data members, but new these
// 13.06.2006 G.Folger: (re)move elastic scatterring 
// 09.04.2017 A.Dotti: adding inheritance
//
//----------------------------------------------------------------------------
//
#ifndef G4ProtonBuilder_h
#define G4ProtonBuilder_h 1

#include "G4PhysicsBuilderInterface.hh"
#include "globals.hh"

#include "G4HadronInelasticProcess.hh"
#include "G4VProtonBuilder.hh"
#include <vector>
#include <type_traits>

class G4ProtonBuilder : public G4PhysicsBuilderInterface
{
  public: 
    G4ProtonBuilder();
    virtual ~G4ProtonBuilder() {}

    virtual void Build() final override;
    virtual void RegisterMe(G4PhysicsBuilderInterface* aB) final override;

  private:
    G4HadronInelasticProcess * theProtonInelastic;
    
    std::vector<G4VProtonBuilder *> theModelCollections;

};

#endif

