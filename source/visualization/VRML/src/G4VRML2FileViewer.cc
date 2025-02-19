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
// G4VRML2FileViewer.cc
// Satoshi Tanaka & Yasuhide Sawada

//#define DEBUG_FR_VIEW

#include <cmath>

#include "G4VisManager.hh"
#include "G4Scene.hh"
#include "G4VRML2FileViewer.hh"
#include "G4VRML2FileSceneHandler.hh"
#include "G4VRML2File.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

G4VRML2FileViewer::G4VRML2FileViewer(G4VRML2FileSceneHandler& sceneHandler,
                                     const G4String& name)
  : G4VViewer(sceneHandler, sceneHandler.IncrementViewCount(), name)
  , fSceneHandler(sceneHandler)
  , fDest(sceneHandler.fDest)
{
  fViewHalfAngle = 30. * deg;
  fsin_VHA       = std::sin(fViewHalfAngle);
}

G4VRML2FileViewer::~G4VRML2FileViewer() {}

void G4VRML2FileViewer::SetView()
{
#if defined DEBUG_FR_VIEW
  if(G4VisManager::GetVerbosity() >= G4VisManager::errors)
    G4cout << "***** G4VRML2FileViewer::SetView(): No effects" << G4endl;
#endif

  // Do nothing, since VRML a browser is running as a different process.
  // SendViewParameters () will do this job instead.
}

void G4VRML2FileViewer::DrawView()
{
#if defined DEBUG_FR_VIEW
  if(G4VisManager::GetVerbosity() >= G4VisManager::errors)
    G4cout << "***** G4VRML2FileViewer::DrawView()" << G4endl;
#endif

  fSceneHandler.VRMLBeginModeling();

  // Viewpoint node
  SendViewParameters();

  // Here is a minimal DrawView() function.
  NeedKernelVisit();
  ProcessView();
  FinishView();
}

void G4VRML2FileViewer::ClearView(void)
{
#if defined DEBUG_FR_VIEW
  if(G4VisManager::GetVerbosity() >= G4VisManager::errors)
    G4cout << "***** G4VRML2File1View::ClearView()" << G4endl;
#endif
  if(fSceneHandler.fFlagDestOpen)
  {
    fSceneHandler.fDest.close();
    // Re-open with same filename...
    fSceneHandler.fDest.open(fSceneHandler.fVRMLFileName);
    fSceneHandler.fDest << "#VRML V2.0 utf8"
                        << "\n";
    fSceneHandler.fDest << "# Generated by VRML 2.0 driver of GEANT4\n"
                        << "\n";
  }
}

void G4VRML2FileViewer::ShowView(void)
{
#if defined DEBUG_FR_VIEW
  if(G4VisManager::GetVerbosity() >= G4VisManager::errors)
    G4cout << "***** G4VRML2FileViewer::ShowView()" << G4endl;
#endif
  fSceneHandler.VRMLEndModeling();
}

void G4VRML2FileViewer::FinishView(void)
{
#if defined DEBUG_FR_VIEW
  if(G4VisManager::GetVerbosity() >= G4VisManager::errors)
    G4cout << "***** G4VRML2FileViewer::FinishView(): No effects" << G4endl;
#endif
}

void G4VRML2FileViewer::SendViewParameters()
{
  // Calculates view representation based on extent of object being
  // viewed and (initial) direction of camera.  (Note: it can change
  // later due to user interaction via visualization system's GUI.)

#if defined DEBUG_FR_VIEW
  if(G4VisManager::GetVerbosity() >= G4VisManager::errors)
    G4cout << "***** G4VRML2FileViewer::SendViewParameters()\n";
#endif

  // error recovery
  if(fsin_VHA < 1.0e-6)
  {
    return;
  }

  // camera distance
  G4double extent_radius =
    fSceneHandler.GetScene()->GetExtent().GetExtentRadius();
  G4double camera_distance = extent_radius / fsin_VHA;

  // camera position on Z axis
  const G4Point3D& target_point =
    fSceneHandler.GetScene()->GetStandardTargetPoint() +
    fVP.GetCurrentTargetPoint();
  G4double E_z = target_point.z() + camera_distance;
  G4Point3D E(0.0, 0.0, E_z);

  // VRML codes are generated below
  fDest << G4endl;
  fDest << "#---------- CAMERA" << G4endl;
  fDest << "Viewpoint {" << G4endl;
  fDest << "\t"
        << "position ";
  fDest << E.x() << " ";
  fDest << E.y() << " ";
  fDest << E.z() << G4endl;
  fDest << "}" << G4endl;
  fDest << G4endl;
}
