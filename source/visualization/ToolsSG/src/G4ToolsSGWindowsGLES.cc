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
// Guy Barrand 6th March 2021


#include "G4ToolsSGWindowsGLES.hh"

#include "G4ToolsSGViewer.hh"
#include <tools/Windows/sg_viewer>

G4ToolsSGWindowsGLES::G4ToolsSGWindowsGLES():
parent
("TOOLSSG_WINDOWS_GLES",
 "TSG_WINDOWS_GLES",
 "TOOLSSG_WINDOWS_GLES is a graphics driver based on the g4tools tools/sg scene graph logic where\n\
 the rendering is done with Microsoft GLES and the windowing is done with Microsoft Windows.",
 parent::threeDInteractive)
,fSGSession(nullptr)
{}

G4ToolsSGWindowsGLES::~G4ToolsSGWindowsGLES() {
  delete fSGSession;
}

void G4ToolsSGWindowsGLES::Initialise() {
  if(fSGSession) return; //done.
  fSGSession = new tools::Windows::session(G4cout);
  if(!fSGSession->is_valid()) {
    G4cerr << "G4ToolsSGWindowsGLES::Initialise : session::is_valid() failed." << G4endl;
    delete fSGSession;
    fSGSession = nullptr;
    return;
  }
}

G4VSceneHandler* G4ToolsSGWindowsGLES::CreateSceneHandler(const G4String& a_name) {
  G4VSceneHandler* pScene = new G4ToolsSGSceneHandler(*this, a_name);
  return pScene;
}

G4VViewer* G4ToolsSGWindowsGLES::CreateViewer(G4VSceneHandler& a_scene,const G4String& a_name) {
  if(!fSGSession) Initialise();
  if(!fSGSession) return nullptr;
  G4VViewer* pView =
    new G4ToolsSGViewer<tools::Windows::session,tools::Windows::sg_viewer>(*fSGSession,(G4ToolsSGSceneHandler&)a_scene,a_name);
  if (pView) {
    if (pView->GetViewId() < 0) {
      G4cerr <<
      "G4ToolsSGWindowsGLES::CreateViewer: ERROR flagged by negative"
      " view id in G4ToolsSGViewer creation."
      "\n Destroying view and returning null pointer."
      << G4endl;
      delete pView;
      pView = nullptr;
    }
  }
  if (!pView) {
    G4cerr <<
    "G4ToolsSGWindowsGLES::CreateViewer: ERROR: null pointer on new G4ToolsSGViewer."
    << G4endl;
  }
  return pView;
}

G4bool G4ToolsSGWindowsGLES::IsUISessionCompatible () const
{
  G4bool isCompatible = false;
//  G4UImanager* ui = G4UImanager::GetUIpointer();
//  G4UIsession* session = ui->GetSession();
//
//  // If session is a batch session, it may be:
//  // a) this is a batch job (the user has not instantiated any UI session);
//  // b) we are currently processing a UI command, in which case the UI
//  //    manager creates a temporary batch session and to find out if there is
//  //    a genuine UI session that the user has instantiated we must drill
//  //    down through previous sessions to a possible non-batch session.
//  while (G4UIbatch* batch = dynamic_cast<G4UIbatch*>(session)) {
//    session = batch->GetPreviousSession();
//  }
//
//  // Qt windows are only appropriate in a Qt session.
//  if (session) {
//    // If non-zero, this is the originating non-batch session
//    // The user has instantiated a UI session...
//    if (dynamic_cast<G4UIQt*>(session)) {
//      // ...and it's a G4UIQt session, which is OK.
      isCompatible = true;
//    }
//  }
  return isCompatible;
}
