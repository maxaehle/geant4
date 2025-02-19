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
// 
//---------------------------------------------------------------
//
//  G4FastStep.hh
//
//  Description:
//    The G4FastStep class insures a friendly interface
//    to manage the primary/secondaries final state for 
//    Fast Simulation Models. This includes final states of parent
//    particle (normalized direction of the momentum, energy, etc) and 
//    secondary particles generated by the parameterisation.
//
//    The G4FastStep class acts also as the G4ParticleChange
//    for the Fast Simulation Process. So it inherites from 
//    the G4VParticleChange class and redefines the four virtual 
//    methods :
//
//     virtual G4Step* UpdateStepForAtRest(G4Step* Step);
//     virtual G4Step* UpdateStepForAlongStep(G4Step* Step);
//     virtual G4Step* UpdateStepForPostStep(G4Step* Step);
//     virtual void Initialize(const G4Track&);
//
//  History:
//    Oct 97: Verderi && MoraDeFreitas - First Implementation.
//    Dec 97: Verderi - ForceSteppingHitInvocation(),
//                      Set/GetTotalEnergyDeposited() methods.
//    Apr 98: MoraDeFreitas - G4FastStep becomes the G4ParticleChange
//                      for the Fast Simulation Process.
//    Nov 04: Verderi - Add ProposeXXX methods. SetXXX ones are kept
//                      for backward compatibility. 
//
//---------------------------------------------------------------


#ifndef G4FastStep_h
#define G4FastStep_h

#include "globals.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"
class G4DynamicParticle;
#include "G4VParticleChange.hh"
#include "G4FastTrack.hh"

//-------------------------------------------
//
//        G4FastStep class
//
//-------------------------------------------

// Class Description:
//  The final state of the particles after parameterisation has to be returned through a G4FastStep 
//  reference. This final state is described as "requests" the tracking will apply after your 
//  parameterisation has been invoked.
//
//  To facilitate the developers work, changes of position/normalized direction of the 
//  momentum/polarization can be specified in the local coordinate system of the envelope or in the 
//  global one.
//  The default is local system coordinates.
//

class G4FastStep: public G4VParticleChange
{
public: // with Description
  void KillPrimaryTrack();
  // Set the kinetic energy of the primary to zero, and set the "fStopAndKill" signal
  // used by the stepping.

  // -- Methods used to change the position, normalized direction of 
  // the momentum, time etc... of the primary.
  // .. space and time:
  void ProposePrimaryTrackFinalPosition (const G4ThreeVector &, 
					 G4bool localCoordinates = true);
  // Set the primary track final position.
  void     SetPrimaryTrackFinalPosition (const G4ThreeVector &, 
					 G4bool localCoordinates = true);
  // Set the primary track final position -- maintained for backward compatibility.
  
  
  void ProposePrimaryTrackFinalTime (G4double);
  // Set the primary track final time.
  void    SetPrimaryTrackFinalTime (G4double);
  // Set the primary track final time -- maintained for backward compatibility.


  void ProposePrimaryTrackFinalProperTime (G4double);
  // Set the primary final track Proper Time.
  void     SetPrimaryTrackFinalProperTime (G4double);
  // Set the primary final track Proper Time -- maintained for backward compatibility.


  // .. dynamics:
  void ProposePrimaryTrackFinalMomentumDirection (const G4ThreeVector &, 
						  G4bool localCoordinates = true);
  // Be careful: the Track Final Momentum means the normalized direction 
  // of the momentum!
  void     SetPrimaryTrackFinalMomentum          (const G4ThreeVector &, 
						  G4bool localCoordinates = true);
  // Set the primary track final momentum -- maintained for backward compatibility. Same as ProposePrimaryTrackMomentumDirection(...)


  void ProposePrimaryTrackFinalKineticEnergy (G4double);
  // Set the primary track final kinetic energy.
  void     SetPrimaryTrackFinalKineticEnergy (G4double);
  // Set the primary track final kinetic energy-- maintained for backward compatibility.


  void ProposePrimaryTrackFinalKineticEnergyAndDirection(G4double, 
							 const G4ThreeVector &, 
							 G4bool localCoordinates 
							 = true);
  // Set the primary track final kinetic energy and direction.
  void     SetPrimaryTrackFinalKineticEnergyAndDirection(G4double, 
							 const G4ThreeVector &, 
							 G4bool localCoordinates 
							 = true);
  // Set the primary track final kinetic energy and direction -- maintained for backward compatibility.



  void ProposePrimaryTrackFinalPolarization(const G4ThreeVector &, 
					    G4bool localCoordinates = true);
  // Set the primary track final polarization.
  void     SetPrimaryTrackFinalPolarization(const G4ThreeVector &, 
					    G4bool localCoordinates = true);
  // Set the primary track final polarization.


  void ProposePrimaryTrackPathLength (G4double);
  // Set the true path length of the primary track during the step.
  void     SetPrimaryTrackPathLength (G4double);
  // Set the true path length of the primary track during the step -- maintained for backward compatibility.

  void ProposePrimaryTrackFinalEventBiasingWeight (G4double);
  // Set the weight applied for event biasing mechanism.
  void     SetPrimaryTrackFinalEventBiasingWeight (G4double);
  // Set the weight applied for event biasing mechanism -- kept for backward compatibility.

  // ------------------------------
  // -- Management of secondaries:
  // ------------------------------

  // ----------------------------------------------------
  // -- The creation of secondaries is Done in two steps:
  // --      1) Give the total number of secondaries
  // --         that the FastStep returns
  // --         to the tracking using:
  // --         SetNumberOfSecondaryTracks()
  // --
  // --      2) Invoke the CreateSecondaryTrack() method
  // --         to create one secondary at each time.
  // ----------------------------------------------------

  // -- Total Number of secondaries to be created,
  // -- (to be called first)
  void SetNumberOfSecondaryTracks(G4int);
  // Set the total number of secondaries that will be created.

  // -- Number of secondaries effectively stored:
  // -- (incremented at each CreateSecondaryTrack()
  // -- call)
  G4int GetNumberOfSecondaryTracks();
  // Returns the number of secondaries effectively stored.

  // -- Create a secondary: the arguments are:
  // --     * G4DynamicsParticle: see header file, many constructors exist
  // --                           (allow to set particle type + energy + 
  // -- the normalized direction of momentum...)
  // --     * G4ThreeVector     : Polarization (not in G4ParticleChange constructor)
  // --     * G4ThreeVector     : Position
  // --     * G4double          : Time
  // --     * G4bool            : says if Position/Momentum are given in the
  // --                           local coordinate system (true by default)
  // -- Returned value: pointer to the track created.
  G4Track* CreateSecondaryTrack(const G4DynamicParticle&,
				G4ThreeVector,
				G4ThreeVector,
				G4double,
				G4bool localCoordinates=true);
  // Create a secondary. The arguments are:
  // 
  //   G4DynamicsParticle: see the G4DynamicsParticle reference, many constructors exist
  //                       (allow to set particle type + energy + the normalized direction of 
  //                       momentum...);
  //   G4ThreeVector     : Polarization;
  //   G4ThreeVector     : Position;
  //   G4double          : Time;
  //   G4bool            : says if Position/Momentum are given in the local envelope coordinate 
  //                       system (true by default).
  //
  // Returned value: pointer to the track created.
  //
  
  //-- Create a secondary: the difference with he above declaration
  //-- is that the Polarization is not given and is assumed already set
  //-- in the G4DynamicParticle.
  //-- Returned value: pointer to the track created
  G4Track* CreateSecondaryTrack(const G4DynamicParticle&,
				G4ThreeVector,
				G4double,
				G4bool localCoordinates=true);
  //  Create a secondary. The difference with he above declaration is that the Polarization is not 
  //  given and is assumed already set in the G4DynamicParticle.
  //
  //  Returned value: pointer to the track created

  

  G4Track* GetSecondaryTrack(G4int);
  // Returns a pointer on the i-th secondary track created.

  //------------------------------------------------
  //
  //   Total energy deposit in the "fast Step"
  //   (a default should be provided in future,
  //    which can be:
  //      delta energy of primary -
  //      energy of the secondaries)
  //   This allow the user to Store a consistent
  //   information in the G4Trajectory.
  //
  //------------------------------------------------
   void ProposeTotalEnergyDeposited(G4double anEnergyPart);
  // Set the total energy deposited.
  void      SetTotalEnergyDeposited(G4double anEnergyPart);
  // Set the total energy deposited -- kept for backward compatibility.
  // It should be the delta energy of primary less the energy of the secondaries.

  G4double GetTotalEnergyDeposited() const;
  // Returns the total energy deposited.

  void ForceSteppingHitInvocation();
  // Control of the stepping manager Hit invocation.
  //
  // In a usual parameterisation, the control of the hits production is under the user
  // responsability in his G4VFastSimulationModel (he generally produces several hits at once.)
  //
  // However, in the particular case the G4FastSimulation user's model acts as the physics
  // replacement only (ie replaces all the ***DoIt() and leads to the construction of a meaningful
  // G4Step), the user can delegate to the G4SteppingManager the responsability to invoke
  // the Hit()method of the current sensitive if any.
  //
  // By default, the G4SteppingManager is asked to NOT invoke this Hit() method when parameterisation
  // is invoked.
  //   


public: // Without description
  //=======================================================
  // Implementation section and kernel interfaces.
  //=======================================================
  //------------------------
  // Constructor/Destructor
  //------------------------
  G4FastStep();
  virtual ~G4FastStep();
  
  // equal/unequal operator
  G4bool operator==(const G4FastStep &right) const;
  G4bool operator!=(const G4FastStep &right) const;

protected:
  // hide copy constructor and assignment operator as protected
  G4FastStep (const G4FastStep &right);
  G4FastStep & operator= (const G4FastStep &right);

public:
  // ===============================================
  // Stepping interface.
  // ===============================================
  // --- the following methods are for updating G4Step -----   
  // Return the pointer to the G4Step after updating the Step information
  // by using final state information of the track given by a Model.
  //
  // The Fast Simulation Mechanism doesn't change the track's final 
  // state on the AlongDoIt loop, so the default one all we need.
  //virtual G4Step* UpdateStepForAlongStep(G4Step* Step);

  G4Step* UpdateStepForAtRest(G4Step* Step);
  G4Step* UpdateStepForPostStep(G4Step* Step);

  // A Model gives the final state of the particle 
  // based on information of G4FastTrack. So the
  // Initialize method is an interface to the 
  // G4FastSimulationManager to Initialize the 
  // G4FastStep.

  void Initialize(const G4FastTrack&);

  // for Debug 
  void DumpInfo() const;
  G4bool CheckIt(const G4Track&);

private:
  //===================================================
  // Private Internal methods (implementation).
  //===================================================

  // G4FastStep should never be Initialized in this way
  // but we must define it to avoid compiler warnings.
  void Initialize(const G4Track&);

  //  -- Utility functions --
  //--- methods to keep information of the final state--
  //  IMPORTANT NOTE: Although the name of the class and methods are
  //   "Change", what it stores (and returns in get) are the "FINAL" 
  //   values of the Position, the normalized direction of Momentum, 
  //   etc.
  
  // Set theMomentumChange vector: it is the final unitary momentum 
  // direction.
  void SetMomentumChange(G4double Px, G4double Py, G4double Pz);
  void SetMomentumChange(const G4ThreeVector& Pfinal);
  
  //=====================================================
  // Data members.
  //=====================================================
  //  theMomentumChange is the vector containing the final momentum 
  //  direction after the invoked process. The application of the change
  //  of the momentum direction of the particle is not Done here.
  //  The responsibility to apply the change is up the entity
  //  which invoked the process.
  G4ParticleMomentum theMomentumChange;

  //  The changed (final) polarization of a given particle.
  G4ThreeVector thePolarizationChange;

  //  The final kinetic energy of the current particle.
  G4double theEnergyChange = 0.0;

  //  The changed (final) position of a given particle.
  G4ThreeVector thePositionChange;

  //  The changed (final) global time of a given particle.
  G4double theTimeChange = 0.0;

  //  The changed (final) proper time of a given particle.
  G4double theProperTimeChange = 0.0;

  // The reference G4FastTrack
  const G4FastTrack* fFastTrack = nullptr;

  // weight for event biasing mechanism:
  G4double theWeightChange = 0.0;
};

//*******************************************************************
//
//  Inline functions
//
//*******************************************************************

#include "G4FastStep.icc"

#endif
