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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("testem15")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);   //enable inactivation of histograms
  
  // Define histograms start values
  const G4int kMaxHisto = 17;
  const G4String id[] = { "0", "1", "2", "3" , "4", "5", "6" , "7", "8", "9",
                          "10","11","12","13","14","15","16"};
  const G4String title[] = 
           { "dummy",                                                    //0
             "Multiple Scattering. True step length",                    //1
             "Multiple Scattering. Geom step length",                    //2
             "Multiple Scattering. Ratio geomSl/trueSl",                 //3
             "Multiple Scattering. Lateral displacement: radius",        //4
             "Multiple Scattering. Lateral displac: psi_space",          //5
             "Multiple Scattering. Angular distrib: theta_plane",        //6
             "Multiple Scattering. Phi-position angle",                  //7
             "Multiple Scattering. Phi-direction angle",                 //8
             "Multiple Scattering. Correlation: std::cos(phiPos-phiDir)",//9
             "Gamma Conversion. Open Angle * Egamma",                    //10
             "Gamma Conversion. Log10(P recoil)",                        //11
             "Gamma Conversion. Phi P recoil angle",                     //12
             "Gamma Conversion. Phi P plus angle",                       //13
             "Gamma Conversion. 2 * cos(phiplus + phiminus) Asymmetry",  //14
             "Gamma Conversion. E plus / E gamma",                       //15
             "Gamma Conversion. Phi of Gamma Polarization"               //16
           };
            
  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
