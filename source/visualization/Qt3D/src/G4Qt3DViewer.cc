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
// John Allison  17th June 2019

#include "G4Qt3DViewer.hh"

#include "G4Qt3DSceneHandler.hh"
#include "G4Qt3DUtils.hh"

#include "G4Scene.hh"
#include "G4UImanager.hh"
#include "G4UIQt.hh"
#include "G4SystemOfUnits.hh"

G4Qt3DViewer::G4Qt3DViewer
(G4Qt3DSceneHandler& sceneHandler, const G4String& name)
: G4VViewer(sceneHandler, sceneHandler.IncrementViewCount(), name)
, fQt3DSceneHandler(sceneHandler)
, fKeyPressed(false)
, fMousePressed(false)
, fMousePressedX(0.)
, fMousePressedY(0.)
{}

void G4Qt3DViewer::Initialise()
{
  setObjectName(fName.c_str());

  fVP.SetAutoRefresh(true);
  fDefaultVP.SetAutoRefresh(true);

  auto UI = G4UImanager::GetUIpointer();
  auto uiQt = dynamic_cast<G4UIQt*>(UI->GetG4UIWindow());
  if (!uiQt) {
    fViewId = -1;  // This flags an error.
    G4cerr << "G4Qt3DViewer::G4Qt3DViewer requires G4UIQt"
         << G4endl;
    return;
  }
  fUIWidget = QWidget::createWindowContainer(this);
  fUIWidget->setMinimumSize(QSize(200, 100));
  fUIWidget->setMaximumSize(screen()->size());
//  fUIWidget->setFocusPolicy(Qt::NoFocus);  //??
  uiQt->AddTabWidget(fUIWidget,QString(fName));

  setRootEntity(fQt3DSceneHandler.fpQt3DScene);
}

G4Qt3DViewer::~G4Qt3DViewer()
{}

void G4Qt3DViewer::SetView()
{
  // Background colour
  defaultFrameGraph()->setClearColor(G4Qt3DUtils::ConvertToQColor(fVP.GetBackgroundColour()));

  // Get radius of scene, etc.
  // Note that this procedure properly takes into account zoom, dolly and pan.
  const G4Point3D targetPoint
    = fSceneHandler.GetScene()->GetStandardTargetPoint()
    + fVP.GetCurrentTargetPoint ();
  G4double radius = fSceneHandler.GetScene()->GetExtent().GetExtentRadius();
  if(radius<=0.) radius = 1.;
  const G4double cameraDistance = fVP.GetCameraDistance (radius);
  const G4Point3D cameraPosition =
    targetPoint + cameraDistance * fVP.GetViewpointDirection().unit();
  const GLG4double pnear  = fVP.GetNearDistance (cameraDistance, radius);
  const GLG4double pfar   = fVP.GetFarDistance  (cameraDistance, pnear, radius);
  const GLG4double right  = fVP.GetFrontHalfHeight (pnear, radius);
  const GLG4double left   = -right;
  const GLG4double top    = fVP.GetFrontHalfHeight (pnear, radius);
  const GLG4double bottom = -top;

  camera()->setObjectName((fName + " camera").c_str());
  camera()->setViewCenter(G4Qt3DUtils::ConvertToQVector3D(targetPoint));
  camera()->setPosition(G4Qt3DUtils::ConvertToQVector3D(cameraPosition));
  camera()->setUpVector(G4Qt3DUtils::ConvertToQVector3D(fVP.GetUpVector()));

//  auto lightEntity = new Qt3DCore::QEntity(fQt3DSceneHandler.fpQt3DScene);
//  auto directionalLight = new Qt3DRender::QDirectionalLight(lightEntity);
////  directionalLight->setColor("white");
////  directionalLight->setIntensity(1.);
//  directionalLight->setWorldDirection(G4Qt3DUtils::ConvertToQVector3D(fVP.GetActualLightpointDirection()));
//  lightEntity->addComponent(directionalLight);

  const auto& size = fUIWidget->size();
  G4double w = size.width();
  G4double h = size.height();
#ifdef G4QT3DDEBUG
  // Curiously w,h are wrong first time - 640,480 instead of (my Mac) 991,452.
  G4cout << "W,H: " << w << ',' << h << G4endl;
#endif
  const G4double aspectRatio = w/h;
  if (fVP.GetFieldHalfAngle() == 0.) {
    camera()->lens()->setOrthographicProjection
    (left*aspectRatio,right*aspectRatio,bottom,top,pnear,pfar);
  } else {
    camera()->lens()->setPerspectiveProjection
    (2.*fVP.GetFieldHalfAngle()/deg,aspectRatio,pnear,pfar);
  }
}

void G4Qt3DViewer::ClearView()
{}

void G4Qt3DViewer::DrawView()
{
  // First, a view should decide when to re-visit the G4 kernel.
  // Sometimes it might not be necessary, e.g., if the scene is stored
  // in a graphical database (e.g., OpenGL's display lists) and only
  // the viewing angle has changed.  But graphics systems without a
  // graphical database will always need to visit the G4 kernel.

  // The fNeedKernelVisit flag might have been set by the user in
  // /vis/viewer/rebuild, but if not, make decision and set flag only
  // if necessary...
  if (!fNeedKernelVisit) KernelVisitDecision();
  G4bool kernelVisitWasNeeded = fNeedKernelVisit; // Keep (ProcessView resets).
  fLastVP = fVP;

  ProcessView ();  // Clears store and processes scene only if necessary.

  if (kernelVisitWasNeeded) {
    // We might need to do something if the kernel was visited.
  } else {
  }

  // ...before finally...
  FinishView ();       // Flush streams and/or swap buffers.
}

void G4Qt3DViewer::ShowView()
{
  // show() may only be called from master thread
  if (G4Threading::IsMasterThread()) {
    show();
  }
  // The way Qt seems to work, we don't seem to need a show() anyway, but
  // we'll leave it in - it seems not to have any effect, good or bad.
}

void G4Qt3DViewer::FinishView()
{
  if (G4Threading::IsMasterThread()) {
    show();
  }
}

namespace {
  QThread* masterQThread = nullptr;
  QThread* visSubThreadQThread = nullptr;
}

#  include <chrono>

void G4Qt3DViewer::MovingToVisSubThread()
{
#ifdef G4QT3DDEBUG
  G4cout << "G4Qt3DViewer::MovingToVisSubThread" << G4endl;
#endif
  // Still on master thread but vis thread has been launched
  // Make note of master QThread
  masterQThread = QThread::currentThread();
  // Wait until SwitchToVisSubThread has found vis sub-thread QThread
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  // Move relevant stuff to vis sub-thread QThread
  auto p1 = fQt3DSceneHandler.fpQt3DScene->parent();
  auto p2 = p1->parent();
  p2->moveToThread(visSubThreadQThread);
}

void G4Qt3DViewer::SwitchToVisSubThread()
{
#ifdef G4QT3DDEBUG
  G4cout << "G4Qt3DViewer::SwitchToVisSubThread" << G4endl;
#endif
  // On vis sub-thread before any drawing
  // Make note of vis-subthread QThread for MovingToVisSubThread
  visSubThreadQThread = QThread::currentThread();
  // Wait until SwitchToVisSubThread has moved stuff
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void G4Qt3DViewer::MovingToMasterThread()
{
#ifdef G4QT3DDEBUG
  G4cout << "G4Qt3DViewer::MovingToMasterThread" << G4endl;
#endif
  // On vis sub-thread just before exit
  // Move relevant stuff to master QThread.
  auto p1 = fQt3DSceneHandler.fpQt3DScene->parent();
  auto p2 = p1->parent();
  p2->moveToThread(masterQThread);
  // Zero - will be different next run
  visSubThreadQThread = nullptr;
}

void G4Qt3DViewer::SwitchToMasterThread()
{
#ifdef G4QT3DDEBUG
  G4cout << "G4Qt3DViewer::SwitchToMasterThread" << G4endl;
#endif
  // On master thread after vis sub-thread has terminated
  // Nothing to do
}

void G4Qt3DViewer::KernelVisitDecision () {
  
  // If there's a significant difference with the last view parameters
  // of either the scene handler or this viewer, trigger a rebuild.

  if (CompareForKernelVisit(fLastVP)) {
    NeedKernelVisit ();  // Sets fNeedKernelVisit.
  }
}

G4bool G4Qt3DViewer::CompareForKernelVisit(G4ViewParameters& lastVP)
{
  // Typical comparison.  Taken from OpenGL.
  if (
      (lastVP.GetDrawingStyle ()    != fVP.GetDrawingStyle ())    ||
      (lastVP.GetNumberOfCloudPoints()  != fVP.GetNumberOfCloudPoints())  ||
      (lastVP.IsAuxEdgeVisible ()   != fVP.IsAuxEdgeVisible ())   ||
      (lastVP.IsCulling ()          != fVP.IsCulling ())          ||
      (lastVP.IsCullingInvisible () != fVP.IsCullingInvisible ()) ||
      (lastVP.IsDensityCulling ()   != fVP.IsDensityCulling ())   ||
      (lastVP.IsCullingCovered ()   != fVP.IsCullingCovered ())   ||
      (lastVP.GetCBDAlgorithmNumber() !=
       fVP.GetCBDAlgorithmNumber())                               ||
      (lastVP.IsSection ()          != fVP.IsSection ())          ||
      (lastVP.IsCutaway ()          != fVP.IsCutaway ())          ||
      (lastVP.IsExplode ()          != fVP.IsExplode ())          ||
      (lastVP.GetNoOfSides ()       != fVP.GetNoOfSides ())       ||
      (lastVP.GetGlobalMarkerScale()    != fVP.GetGlobalMarkerScale())    ||
      (lastVP.GetGlobalLineWidthScale() != fVP.GetGlobalLineWidthScale()) ||
      (lastVP.IsMarkerNotHidden ()  != fVP.IsMarkerNotHidden ())  ||
      (lastVP.GetDefaultVisAttributes()->GetColour() !=
       fVP.GetDefaultVisAttributes()->GetColour())                ||
      (lastVP.GetDefaultTextVisAttributes()->GetColour() !=
       fVP.GetDefaultTextVisAttributes()->GetColour())            ||
      (lastVP.GetBackgroundColour ()!= fVP.GetBackgroundColour ())||
      (lastVP.IsPicking ()          != fVP.IsPicking ())          ||
      (lastVP.GetVisAttributesModifiers() !=
       fVP.GetVisAttributesModifiers())                           ||
      (lastVP.IsSpecialMeshRendering() !=
       fVP.IsSpecialMeshRendering())
      ) {
    return true;
  }

  if (lastVP.IsDensityCulling () &&
      (lastVP.GetVisibleDensity () != fVP.GetVisibleDensity ()))
    return true;

  if (lastVP.GetCBDAlgorithmNumber() > 0) {
    if (lastVP.GetCBDParameters().size() != fVP.GetCBDParameters().size()) return true;
    else if (lastVP.GetCBDParameters() != fVP.GetCBDParameters()) return true;
  }

  if (lastVP.IsExplode () &&
      (lastVP.GetExplodeFactor () != fVP.GetExplodeFactor ()))
    return true;

  if (lastVP.IsSpecialMeshRendering() &&
      (lastVP.GetSpecialMeshVolumes() != fVP.GetSpecialMeshVolumes()))
    return true;

  return false;
}

void G4Qt3DViewer::keyPressEvent(QKeyEvent* ev)
{
  fKeyPressed = true;
  fKey = ev->key();
}

void G4Qt3DViewer::keyReleaseEvent(QKeyEvent* /*ev*/)
{
  fKeyPressed = false;
}

void G4Qt3DViewer::mouseDoubleClickEvent(QMouseEvent* /*ev*/) {}

void G4Qt3DViewer::mouseMoveEvent(QMouseEvent* ev)
{
  // I think we only want these if a mouse button is pressed.
  // But they come even when not pressed (on my MacBook Pro trackpad).
  // Documentation says:
  /* Mouse move events will occur only when a mouse button is pressed down,
   unless mouse tracking has been enabled with QWidget::setMouseTracking().*/
  // But this is a window not a widget.
  // As a workaround we maintain a flag changed by mousePress/ReleaseEvent.

  G4double x = ev->x();
  G4double y = ev->y();
  G4double dx = x-fMousePressedX;
  G4double dy = y-fMousePressedY;
  fMousePressedX = x;
  fMousePressedY = y;

  if (fMousePressed) {

    if (fKeyPressed && fKey == Qt::Key_Shift) {  // Translation (pan)

      const G4double sceneRadius = fQt3DSceneHandler.fpScene->GetExtent().GetExtentRadius();
      const G4double scale = 300;  // Roughly pixels per window, empirically chosen
      const G4double dxScene = dx*sceneRadius/scale;
      const G4double dyScene = dy*sceneRadius/scale;
      fVP.IncrementPan(-dxScene,dyScene);

    } else {  // Rotation

      // Simple ad-hoc algorithms
      const G4Vector3D& x_prime = fVP.GetViewpointDirection().cross(fVP.GetUpVector());
      const G4Vector3D& y_prime = x_prime.cross(fVP.GetViewpointDirection());
      const G4double scale = 200;  // Roughly pixels per window, empirically chosen
      G4Vector3D newViewpointDirection = fVP.GetViewpointDirection();
      newViewpointDirection += dx*x_prime/scale;
      newViewpointDirection += dy*y_prime/scale;
      fVP.SetViewpointDirection(newViewpointDirection.unit());

      if (fVP.GetRotationStyle() == G4ViewParameters::freeRotation) {
        G4Vector3D newUpVector = fVP.GetUpVector();
        newUpVector += dx*x_prime/scale;
        newUpVector += dy*y_prime/scale;
        fVP.SetUpVector(newUpVector.unit());
      }
    }
  }

  SetView();
  DrawView();
}

void G4Qt3DViewer::mousePressEvent(QMouseEvent* ev)
{
  fMousePressed = true;
  fMousePressedX = ev->x();
  fMousePressedY = ev->y();
}

void G4Qt3DViewer::mouseReleaseEvent(QMouseEvent* /*ev*/)
{
  fMousePressed = false;
}

void G4Qt3DViewer::wheelEvent(QWheelEvent* ev)
{
  // Take note of up-down motion only
  const G4double angleY = ev->angleDelta().y();

  if (fVP.GetFieldHalfAngle() == 0.) {  // Orthographic projection
    const G4double scale = 500;  // Empirically chosen
    fVP.MultiplyZoomFactor(1.+angleY/scale);
  } else {                              // Perspective projection
    const G4double scale = fVP.GetFieldHalfAngle()/(10.*deg);  // Empirical
    fVP.SetDolly(fVP.GetDolly()+angleY/scale);
  }
  
  SetView();
  DrawView();
}
