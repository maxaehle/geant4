// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4UserPhysicsListMessenger.hh,v 1.6 2000/11/08 10:01:59 kurasige Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 
//---------------------------------------------------------------
//
//  G4UserPhysicsListMessenger.hh
//
//  Class Description:
//    This is a messenger class to interface to exchange information
//    between ParticleUserList and UI.
// --
//  the List of Directory and Commands
// -       
//  /run/particle/   Paricle control commands.
//   Commands : 
//    SetCuts *  Set default cut value
//    dumpList * Dump List of particles in G4VUserPhysicsList.
//    dumpCutValue * Dump cut value information
//    verbose * Set the Verbose level of G4VUserPhysicsList.
//    addProcessManager * add process manager
//    buildPhysicsTable * build physics table
//    storePhysicsTable * store physics table into files
//    retreivePhysicsTable * retreive physics table from files
//    
// ------------------------------------------------------------
//	History
//        first version                   09 Jan. 1998 by H.Kurashige 
//        second version                  24 Jan. 1998 by H.Kurashige 
//        add buildPhysicsTable command   13 Apr. 1999 by H.Kurashige
//        add store/retreivePhysicsTable  08 Nov. 2000 by H.Kurashige
// ------------------------------------------------------------

#ifndef G4UserPhysicsListMessenger_h
#define G4UserPhysicsListMessenger_h 1

class G4VUserPhysicsList;

class G4VUserPhysicsList;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString; 

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UserPhysicsListMessenger: public G4UImessenger
{
  private:
  // hide default constructor
    G4UserPhysicsListMessenger(){}

  public:
    G4UserPhysicsListMessenger(G4VUserPhysicsList* pParticleList);
    virtual ~G4UserPhysicsListMessenger();
    
public: // with description
    virtual  void SetNewValue(G4UIcommand * command,G4String newValues);
    virtual  G4String GetCurrentValue(G4UIcommand * command);

  protected:
    G4VUserPhysicsList* thePhysicsList;
    
  private: //commands
    G4UIdirectory *             theDirectory;
    G4UIcmdWithADoubleAndUnit * setCutCmd; 
    G4UIcmdWithAnInteger *      verboseCmd;
    G4UIcmdWithoutParameter *   dumpListCmd;
    G4UIcmdWithAString *        dumpCutValuesCmd;
    G4UIcmdWithAString *        addProcManCmd;
    G4UIcmdWithAString *        buildPTCmd;
    G4UIcmdWithAString *        storeCmd;
    G4UIcmdWithAString *        retrieveCmd;
};

#endif

