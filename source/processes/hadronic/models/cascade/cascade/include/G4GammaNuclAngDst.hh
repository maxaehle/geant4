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
// Author:  Michael Kelsey (SLAC)
// Date:    20 February 2013
//
// Description: class containing parametrized angular distributions
//              in the CM for gamma - nucleon two-body scattering
//

#ifndef G4GammaNuclAngDst_h
#define G4GammaNuclAngDst_h 1

#include "G4ParamExpTwoBodyAngDst.hh"


class G4GammaNuclAngDst : public G4ParamExpTwoBodyAngDst<10> {
public:
  G4GammaNuclAngDst(G4int verbose = 0);
  virtual ~G4GammaNuclAngDst() {;}
};

#endif
