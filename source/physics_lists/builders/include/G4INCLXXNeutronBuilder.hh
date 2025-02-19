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
#ifndef G4INCLXXNeutronBuilder_h
#define G4INCLXXNeutronBuilder_h 1

#include "globals.hh"

#include "G4HadronElasticProcess.hh"
#include "G4NeutronFissionProcess.hh"
#include "G4NeutronCaptureProcess.hh"
#include "G4VNeutronBuilder.hh"

#include "G4INCLXXInterface.hh"   
#include "G4VPreCompoundModel.hh"

/**
 * Builder for neutron processes using the INCL++ intra-nuclear
 * cascade model.
 *
 * By default the INCL++ model is used for projectile energies 0 - 3 GeV.
 *
 * The builder uses INCL++ cascade model with G4ExcitationHandler
 * de-excitation. This is implemented in interface
 * G4INCLXXInterface.
 *
 * @see G4INCLXXInterface
 * @see G4INCLXXProtonBuilder
 * @see G4INCLXXPiKBuilder
 */
class G4INCLXXNeutronBuilder : public G4VNeutronBuilder
{
  public: 
    G4INCLXXNeutronBuilder();
    virtual ~G4INCLXXNeutronBuilder() {}

  public: 
    virtual void Build(G4HadronElasticProcess *) final override {}
    virtual void Build(G4NeutronFissionProcess *) final override {}
    virtual void Build(G4NeutronCaptureProcess *) final override {}
    virtual void Build(G4HadronInelasticProcess * aP) final override;
    
    virtual void SetMinEnergy(G4double aM) final override {theMin = aM;}
    virtual void SetMaxEnergy(G4double aM) final override {theMax = aM;}

    void SetMinPreCompoundEnergy(G4double aM) {thePreCompoundMin = aM;}
    void SetMaxPreCompoundEnergy(G4double aM) {thePreCompoundMax = aM;}

    void UsePreCompound(const G4bool w) { withPreCompound = w;}

    using G4VNeutronBuilder::Build; //Prevent compiler warning
  private:
    G4INCLXXInterface * theModel;    
    G4VPreCompoundModel * thePreCompoundModel;    
    G4bool withPreCompound;
    G4double thePreCompoundMin;
    G4double thePreCompoundMax;
    G4double theMin;
    G4double theMax;

};

#endif

