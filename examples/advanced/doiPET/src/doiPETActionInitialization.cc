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

//GEANT4 - Depth-of-Interaction enabled Positron emission tomography (PET) advanced example 

//Authors and contributors

// Author list to be updated, with names of co-authors and contributors from National Institute of Radiological Sciences (NIRS)

// Abdella M. Ahmed (1, 2), Andrew Chacon (1, 2), Harley Rutherford (1, 2),
// Hideaki Tashima (3), Go Akamatsu (3), Akram Mohammadi (3), Eiji Yoshida (3), Taiga Yamaya (3)
// Susanna Guatelli (2), and Mitra Safavi-Naeini (1, 2)

// (1) Australian Nuclear Science and Technology Organisation, Australia
// (2) University of Wollongong, Australia
// (3) National Institute of Radiological Sciences, Japan


#include "doiPETActionInitialization.hh"
#include "doiPETRunAction.hh"
#include "doiPETEventAction.hh"
#include "doiPETPrimaryGeneratorAction.hh"
#include "doiPETSteppingAction.hh"

////////////////////////  doiPETActionInitialization   ////////////////////////////////
/*
doiPETActionInitialization::doiPETActionInitialization()
: G4VUserActionInitialization()
{}*/
doiPETActionInitialization::doiPETActionInitialization(doiPETAnalysis* analysisMan) : G4VUserActionInitialization()
{
	analysis = analysisMan;
}

doiPETActionInitialization::~doiPETActionInitialization()
{}


//
void doiPETActionInitialization::BuildForMaster() const
{
#ifdef ANALYSIS_USE
	 doiPETRunAction* runAction = new doiPETRunAction(analysis);
#else
 	doiPETRunAction* runAction = new doiPETRunAction();
#endif
	SetUserAction(runAction);
}

//
void doiPETActionInitialization::Build() const
{
	doiPETPrimaryGeneratorAction* primary = new doiPETPrimaryGeneratorAction();
	SetUserAction(primary);

#ifdef ANALYSIS_USE
	 doiPETRunAction* runAction = new doiPETRunAction(analysis);
#else
 	doiPETRunAction* runAction = new doiPETRunAction();
#endif
	SetUserAction(runAction);

	SetUserAction(new doiPETEventAction());  

	SetUserAction(new doiPETSteppingAction());
}  
//
////////////////////////////////////////////////////////////////////////////////
