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
// G4AdjointSimManager
//
// Class description:
//
// This class represents the Manager of an adjoint/reverse MC simulation.
// An adjoint run is divided in a serie of alternative adjoint and forward
// tracking of adjoint and normal particles.
//
//   Reverse tracking phase:
//   -----------------------
// An adjoint particle of a given type  (adjoint_e-, adjoint_gamma,...) is
// first generated on the so called adjoint source with a random energy (1/E
// distribution) and direction. The adjoint source is the external surface
// of a user defined volume or of a user defined sphere. The adjoint source
// should contain one or several sensitive volumes and should be small compared
// to the entire geometry. The user can set the min and max energy of the
// adjoint source. After its generation the adjoint primary  particle is tracked
// bacward in the geometry till a user  defined external surface (spherical or
// boundary of a volume) or is killed before if it reaches a user defined
// upper energy limit that represents the maximum energy of the external
// source. During the reverse tracking, reverse processes take place where
// the adjoint particle being tracked can be either scattered or transformed
// in another type of adjoint paticle. During the reverse tracking the
// G4SimulationManager replaces the user defined Primary, Run, ... actions, by
// its own actions.
//
//   Forward tracking phase
//   -----------------------
// When an adjoint particle reaches the external surface its weight,type,
// position, and directions are registered and a normal primary particle
// with a type equivalent to the last generated primary adjoint is generated
// with the same energy, position but opposite direction and is tracked
// normally in the sensitive region as in a fwd MC simulation. During this
// forward tracking phase the event, stacking, stepping, tracking actions
// defined by the user for its general fwd application are used. By this clear
// separation between adjoint and fwd tracking phases, the code of the user
// developed for a fwd simulation should be only slightly modified to adapt it
// for an adjoint simulation. Indeed  the computation of the signal is done by
// the same actions or classes that the one used in the fwd simulation mode.
//
//   Modification to bring in an existing G4 application to use the ReverseMC
//   ------------------------------------------------------------------------
// In order to be able to use the ReverseMC method in his simulation, the
// user should modify its code as such:
//  1) Adapt its physics list to use ReverseProcesses for adjoint particles.
//     An example of such physics list is provided in an extended example.
//  2) Create an instance of G4AdjointSimManager somewhere in the main code.
//  3) Modify the analysis part of the code to normalise the signal computed
//     during the fwd phase to the weight of the last adjoint particle that
//     reaches the external surface. This is done by using the following
//     method of G4AdjointSimManager:
//
//     G4int GetIDOfLastAdjParticleReachingExtSource()
//     G4ThreeVector GetPositionAtEndOfLastAdjointTrack()
//     G4ThreeVector GetDirectionAtEndOfLastAdjointTrack()
//     G4double GetEkinAtEndOfLastAdjointTrack()
//     G4double GetEkinNucAtEndOfLastAdjointTrack()
//     G4double GetWeightAtEndOfLastAdjointTrack()
//     G4double GetCosthAtEndOfLastAdjointTrack()
//     G4String GetFwdParticleNameAtEndOfLastAdjointTrack()
//     G4int GetFwdParticlePDGEncodingAtEndOfLastAdjointTrack()
//     G4int GetFwdParticleIndexAtEndOfLastAdjointTrack().
//
// In order to have a code working for both forward and adjoint simulation
// mode, the extra code needed in user actions for the adjoint simulation
// mode can be separated from the code needed only for the normal forward
// simulation by using the following method:
//   G4bool GetAdjointSimMode()
// that returns true if an adjoint simulation is running and false if not!
//
//   Example of modification in the analysis part of the code
//   --------------------------------------------------------
// Let's say that in the forward simulation a G4 application computes the
// energy deposited in a volume. The user wants to normalise its results for an
// external isotropic source of e- with differential spectrum given by f(E). A
// possible modification of the code where the deposited energy Edep during an
// event is registered would be the following:
//
//   G4AdjointSimManager* theAdjSimManager = G4AdjointSimManager::GetInstance();
//   if (theAdjSimManager->GetAdjointSimMode())
//   {
//     // code of the user that should be consider only for forward simulation
//     G4double normalised_edep = 0.;
//     if (theAdjSimManager->GetFwdParticleNameAtEndOfLastAdjointTrack()=="e-")
//     {
//       G4double ekin_prim =
//         theAdjSimManager->GetEkinAtEndOfLastAdjointTrack();
//       G4double weight_prim =
//         theAdjSimManager->GetWeightAtEndOfLastAdjointTrack();
//       normalised_edep = weight_prim*f(ekin_prim);
//     }
//     // then follow the code where normalised_edep is printed, or registered
//     // or whatever ....
//   }
//   else
//   {
//     // code that should be considered only for forward simulation
//   }
//
// Note that in this example a normalisation to only primary e- with only
// one spectrum f(E) is considered. The example code could be easily
// adapted for a normalisation to several spectra and several types of
// primary particles in the same simulation.

// --------------------------------------------------------------------
//      Class Name:        G4AdjointSimManager
//        Author:               L. Desorgher, 2007-2009
//         Organisation:         SpaceIT GmbH
//        Contract:        ESA contract 21435/08/NL/AT
//         Customer:             ESA/ESTEC
// --------------------------------------------------------------------
#ifndef G4AdjointSimManager_hh
#define G4AdjointSimManager_hh 1

#include <vector>

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4UserRunAction.hh"

class G4UserEventAction;
class G4VUserPrimaryGeneratorAction;
class G4UserTrackingAction;
class G4UserSteppingAction;
class G4UserStackingAction;
class G4AdjointRunAction;
class G4AdjointPrimaryGeneratorAction;
class G4AdjointSteppingAction;
class G4AdjointEventAction;
class G4AdjointStackingAction;
class G4AdjointTrackingAction;
class G4ParticleDefinition;
class G4AdjointSimMessenger;
class G4PhysicsLogVector;
class G4Run;

class G4AdjointSimManager : public G4UserRunAction
{
  public:

    static G4AdjointSimManager* GetInstance();

    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);
    void RunAdjointSimulation(G4int nb_evt);

    inline G4int GetNbEvtOfLastRun() { return nb_evt_of_last_run; }

    void SetAdjointTrackingMode(G4bool aBool);
    G4bool GetAdjointTrackingMode();
      // true if an adjoint track is being processed

    inline G4bool GetAdjointSimMode()
    {
      return adjoint_sim_mode;
    } // true if an adjoint simulation is running

    G4bool GetDidAdjParticleReachTheExtSource();
    void RegisterAtEndOfAdjointTrack();
    void RegisterAdjointPrimaryWeight(G4double aWeight);
    void ResetDidOneAdjPartReachExtSourceDuringEvent();

    inline G4int GetIDOfLastAdjParticleReachingExtSource()
    {
      return ID_of_last_particle_that_reach_the_ext_source;
    }

    G4ThreeVector GetPositionAtEndOfLastAdjointTrack(std::size_t i = 0);
    G4ThreeVector GetDirectionAtEndOfLastAdjointTrack(std::size_t i = 0);
    G4double GetEkinAtEndOfLastAdjointTrack(std::size_t i = 0);
    G4double GetEkinNucAtEndOfLastAdjointTrack(std::size_t i = 0);
    G4double GetWeightAtEndOfLastAdjointTrack(std::size_t i = 0);
    G4double GetCosthAtEndOfLastAdjointTrack(std::size_t i = 0);
    const G4String& GetFwdParticleNameAtEndOfLastAdjointTrack();
    G4int GetFwdParticlePDGEncodingAtEndOfLastAdjointTrack(std::size_t i = 0);
    G4int GetFwdParticleIndexAtEndOfLastAdjointTrack(std::size_t i = 0);
    std::size_t GetNbOfAdointTracksReachingTheExternalSurface();
    void ClearEndOfAdjointTrackInfoVectors();
    G4ParticleDefinition* GetLastGeneratedFwdPrimaryParticle();

    std::vector<G4ParticleDefinition*>* GetListOfPrimaryFwdParticles();
    std::size_t GetNbOfPrimaryFwdParticles();

    G4bool DefineSphericalExtSource(G4double radius, G4ThreeVector pos);
    G4bool DefineSphericalExtSourceWithCentreAtTheCentreOfAVolume(
           G4double radius, const G4String& volume_name);
    G4bool DefineExtSourceOnTheExtSurfaceOfAVolume(const G4String& volume_name);
    void SetExtSourceEmax(G4double Emax);

    // Definition of adjoint source
    //----------------------------
    G4bool DefineSphericalAdjointSource(G4double radius, G4ThreeVector pos);
    G4bool DefineSphericalAdjointSourceWithCentreAtTheCentreOfAVolume(
           G4double radius, const G4String& volume_name);
    G4bool DefineAdjointSourceOnTheExtSurfaceOfAVolume(
           const G4String& volume_name);
    void SetAdjointSourceEmin(G4double Emin);
    void SetAdjointSourceEmax(G4double Emax);
    inline G4double GetAdjointSourceArea()
    {
      return area_of_the_adjoint_source;
    }
    void ConsiderParticleAsPrimary(const G4String& particle_name);
    void NeglectParticleAsPrimary(const G4String& particle_name);
    void SetPrimaryIon(G4ParticleDefinition* adjointIon,
                       G4ParticleDefinition* fwdIon);
    const G4String& GetPrimaryIonName();

    inline void SetNormalisationMode(G4int n) { normalisation_mode = n; }
    inline G4int GetNormalisationMode() { return normalisation_mode; }
    inline G4double GetNumberNucleonsInIon() { return nb_nuc; }

    // Definition of user actions for the adjoint tracking phase
    //----------------------------
    void SetAdjointEventAction(G4UserEventAction* anAction);
    void SetAdjointSteppingAction(G4UserSteppingAction* anAction);
    void SetAdjointStackingAction(G4UserStackingAction* anAction);
    void SetAdjointRunAction(G4UserRunAction* anAction);

    // Set methods for user run actions
    //--------------------------------
    inline void UseUserStackingActionInFwdTrackingPhase(G4bool aBool)
    {
      use_user_StackingAction = aBool;
    }
    inline void UseUserTrackingActionInFwdTrackingPhase(G4bool aBool)
    {
      use_user_TrackingAction = aBool;
    }

    // Set nb of primary fwd gamma
    //---------------------------
    void SetNbOfPrimaryFwdGammasPerEvent(G4int);

    // Set nb of adjoint primaries for reverse splitting
    //-------------------------------------------------
    void SetNbAdjointPrimaryGammasPerEvent(G4int);
    void SetNbAdjointPrimaryElectronsPerEvent(G4int);

    // Convergence test
    //-----------------------
    /*
     void RegisterSignalForConvergenceTest(G4double aSignal);
     void DefineExponentialPrimarySpectrumForConvergenceTest(
          G4ParticleDefinition* aPartDef, G4double E0);
     void DefinePowerLawPrimarySpectrumForConvergenceTest(
          G4ParticleDefinition* aPartDef, G4double alpha);
    */

    void SwitchToAdjointSimulationMode();
    void BackToFwdSimulationMode();

  private:  // methods

    static G4ThreadLocal G4AdjointSimManager* instance;

    void SetRestOfAdjointActions();
    void SetAdjointPrimaryRunAndStackingActions();
    void SetAdjointActions();
    void ResetRestOfUserActions();
    void ResetUserPrimaryRunAndStackingActions();
    void ResetUserActions();
    void DefineUserActions();

    G4AdjointSimManager();
   ~G4AdjointSimManager();
      // private constructor and destructor

  private:  // attributes

    // Messenger
    //----------
    G4AdjointSimMessenger* theMessenger = nullptr;

    // user defined actions for the normal fwd simulation.
    // Taken from the G4RunManager
    //-------------------------------------------------
    G4bool user_action_already_defined = false;
    G4UserRunAction* fUserRunAction = nullptr;
    G4UserEventAction* fUserEventAction = nullptr;
    G4VUserPrimaryGeneratorAction* fUserPrimaryGeneratorAction = nullptr;
    G4UserTrackingAction* fUserTrackingAction = nullptr;
    G4UserSteppingAction* fUserSteppingAction = nullptr;
    G4UserStackingAction* fUserStackingAction = nullptr;
    G4bool use_user_StackingAction = false; // only for fwd part of adjoint sim
    G4bool use_user_TrackingAction = false;

    // action for adjoint simulation
    //-----------------------------
    G4UserRunAction* theAdjointRunAction = nullptr;
    G4UserEventAction* theAdjointEventAction = nullptr;
    G4AdjointPrimaryGeneratorAction* theAdjointPrimaryGeneratorAction = nullptr;
    G4AdjointTrackingAction* theAdjointTrackingAction = nullptr;
    G4AdjointSteppingAction* theAdjointSteppingAction = nullptr;
    G4AdjointStackingAction* theAdjointStackingAction = nullptr;

    // adjoint mode
    //-------------
    G4bool adjoint_tracking_mode = false;
    G4bool adjoint_sim_mode = false;

    // adjoint particle information on the external surface
    //-----------------------------
    std::vector<G4ThreeVector> last_pos_vec;
    std::vector<G4ThreeVector> last_direction_vec;
    std::vector<G4double> last_ekin_vec;
    std::vector<G4double> last_ekin_nuc_vec;
    std::vector<G4double> last_cos_th_vec;
    std::vector<G4double> last_weight_vec;
    std::vector<G4int> last_fwd_part_PDGEncoding_vec;
    std::vector<G4int> last_fwd_part_index_vec;
    std::vector<G4int> ID_of_last_particle_that_reach_the_ext_source_vec;

    G4ThreeVector last_pos;
    G4ThreeVector last_direction;
    G4double last_ekin = 0.0, last_ekin_nuc = 0.0;
      // last_ekin_nuc=last_ekin/nuc, nuc is 1 if not a nucleus
    G4double last_cos_th = 0.0;
    G4String last_fwd_part_name;
    G4int last_fwd_part_PDGEncoding = 0;
    G4int last_fwd_part_index = 0;
    G4double last_weight = 0.0;
    G4int ID_of_last_particle_that_reach_the_ext_source = 0;

    G4int nb_evt_of_last_run = 0;
    G4int normalisation_mode = 3;

    // Adjoint source
    //--------------
    G4double area_of_the_adjoint_source = 0.0;
    G4double nb_nuc = 1.0;
    G4double theAdjointPrimaryWeight = 0.0;

    // Weight Analysis
    //----------
    /*G4PhysicsLogVector* electron_last_weight_vector;
      G4PhysicsLogVector* proton_last_weight_vector;
      G4PhysicsLogVector* gamma_last_weight_vector;*/

    G4bool welcome_message = true;

    /* For the future
      //Convergence test
      //----------------

       G4double normalised_signal;
       G4double error_signal;
       G4bool convergence_test_is_used;
       G4bool power_law_spectrum_for_convergence_test; // true PowerLaw
       G4ParticleDefinition* the_par_def_for_convergence_test;
    */
};

#endif
