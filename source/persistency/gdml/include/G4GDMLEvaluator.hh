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
// G4GDMLEvaluator
//
// Class description:
//
// GDML class for evaluation of expressions

// Author: Zoltan Torzsok, November 2007
// --------------------------------------------------------------------
#ifndef G4GDMLEVALUATOR_HH
#define G4GDMLEVALUATOR_HH 1

#include <vector>

#include "G4Evaluator.hh"

class G4GDMLEvaluator
{
  public:

    G4GDMLEvaluator();

    void Clear();
    void DefineConstant(const G4String&, G4double);
    void DefineVariable(const G4String&, G4double);
    void DefineMatrix(const G4String&, G4int, std::vector<G4double>);
    void SetVariable(const G4String&, G4double);
    G4bool IsVariable(const G4String&) const;
    G4String SolveBrackets(const G4String&);
    G4double Evaluate(const G4String&);
    G4int EvaluateInteger(const G4String&);
    G4double GetConstant(const G4String&);
    G4double GetVariable(const G4String&);
    G4String ConvertToString(G4int ival);
    G4String ConvertToString(G4double dval);

  private:

    G4Evaluator eval;
    std::vector<G4String> variableList;
};

#endif
