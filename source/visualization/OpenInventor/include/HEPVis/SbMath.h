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
#ifndef HEPVis_SbMath_h
#define HEPVis_SbMath_h

#include <cmath>
#ifndef M_PI
#define M_PI       3.1415926535897931160E0
#define M_PI_2     1.5707963267948965580E0  
#endif

#define SbMinimum(a,b) ((a)<(b)?a:b)
#define SbMaximum(a,b) ((a)>(b)?a:b)

#define FCOS(x)   ((G4float)cos((G4double)(x)))
#define FSIN(x)   ((G4float)sin((G4double)(x)))
#define FACOS(x)  ((G4float)acos((G4double)(x)))
#define FASIN(x)  ((G4float)asin((G4double)(x)))
#define FTAN(x)   ((G4float)tan((G4double)(x)))
#define FATAN(x)  ((G4float)atan((G4double)(x)))
#define FSQRT(x)  ((G4float)sqrt((G4double)(x)))
#define FPOW(x,y) ((G4float)pow((G4double)(x),(G4double)(y)))
#define FLOG(x)   ((G4float)log((G4double)(x)))
#define FLOG10(x) ((G4float)log10((G4double)(x)))
#define FFLOOR(x) ((G4float)floor((G4double)(x)))
#define FFABS(x)  ((G4float)fabs((G4double)(x)))
#define FCEIL(x)  ((G4float)ceil((G4double)(x)))

#endif
