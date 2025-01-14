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
// INCL++ intra-nuclear cascade model
// Alain Boudard, CEA-Saclay, France
// Joseph Cugnon, University of Liege, Belgium
// Jean-Christophe David, CEA-Saclay, France
// Pekka Kaitaniemi, CEA-Saclay, France, and Helsinki Institute of Physics, Finland
// Sylvie Leray, CEA-Saclay, France
// Davide Mancusi, CEA-Saclay, France
//
#define INCLXX_IN_GEANT4_MODE 1

#include "globals.hh"

/*
 * IAvatar.cc
 *
 *  \date 4 juin 2009
 * \author Pekka Kaitaniemi
 */

#include "G4INCLIAvatar.hh"
#include "G4INCLRandom.hh"
#include <sstream>

namespace G4INCL {

  G4ThreadLocal long IAvatar::nextID = 1;

  IAvatar::IAvatar() :
    type(UnknownAvatarType),
    theTime(0.)
  {
    ID = nextID;
    nextID++;
  }

  IAvatar::IAvatar(G4double time) :
    type(UnknownAvatarType),
    theTime(time)
  {
    ID = nextID;
    nextID++;
  }

  IAvatar::~IAvatar() {
    INCL_DEBUG("destroying avatar " << this << std::endl);
  }

  std::string IAvatar::toString() {
    std::stringstream entry;
    std::stringstream particleString;
    ParticleList const &pl = getParticles();
    G4int numberOfParticles = 0;
    for(ParticleIter i=pl.begin(), e=pl.end(); i!=e; ++i) {
      numberOfParticles++;
      particleString << (*i)->getID() << " ";
    }
    if(numberOfParticles == 1) particleString << "-1";
    entry << getID() << " "
	  << getType() << " "
	  << getTime() << " "
	  << particleString.str();
    return entry.str();
  }

  FinalState *IAvatar::getFinalState() {
    FinalState *fs = new FinalState;
    fillFinalState(fs);
    return fs;
  }

  void IAvatar::fillFinalState(FinalState *fs) {
    INCL_DEBUG("Random seeds before preInteraction: " << Random::getSeeds() << '\n');
    preInteraction();
    INCL_DEBUG("Random seeds before getChannel: " << Random::getSeeds() << '\n');
    IChannel *c = getChannel();
    if( !c )
      return;
    INCL_DEBUG("Random seeds before getFinalState: " << Random::getSeeds() << '\n');
    c->fillFinalState(fs);
    INCL_DEBUG("Random seeds before postInteraction: " << Random::getSeeds() << '\n');
    postInteraction(fs);
    delete c;
  }

}
