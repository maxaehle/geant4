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
/*
 * G4DNARevertFunction.hh
 *
 *  Created on: Aug 25, 2015
 *      Author: mkaramit
 */

#ifndef SOURCE_PROCESSES_ELECTROMAGNETIC_DNA_UTILS_INCLUDE_G4DNAREVERTPROBABILITY_HH_
#define SOURCE_PROCESSES_ELECTROMAGNETIC_DNA_UTILS_INCLUDE_G4DNAREVERTPROBABILITY_HH_

#include <functional>

//------------------------------------------------------------------------------
// *****************************************************
// Revert a probability by binary search
// *****************************************************

class G4DNARevertProbability
{
public:
  G4double fXmax;
  G4double fXmin;
  G4double fXmaxDef;
  G4double fXminDef;
  G4double fToleranceProba;
  G4double fIncreasingCumulativeFunction;

  G4DNARevertProbability(G4double toleranceY) :
      fToleranceProba(toleranceY)
  {
    fXmax = 0.;
    fXmin = 0.;
    fXminDef = 0.;
    fXmaxDef = 0.;
    fIncreasingCumulativeFunction = 0.;
  }

  G4DNARevertProbability(G4double xmin, G4double xmax, G4double toleranceY) :
      fXmax(xmax), fXmin(xmin), fToleranceProba(toleranceY)
  {
    if(fXmax < fXmin)
    {
      G4double tmp = fXmin;
      fXmin = fXmax;
      fXmax = tmp;
    }

    fXminDef = fXmin;
    fXmaxDef = fXmax;
    fIncreasingCumulativeFunction = 0;
  }

  void Reset()
  {
    fXmin = fXminDef;
    fXmax = fXmaxDef;
  }

  void SetBoundaries(G4double xmin, G4double xmax)
  {
    fXmax = xmax;
    fXmin = xmin;
    if(fXmax < fXmin)
    {
      G4double tmp = fXmin;
      fXmin = fXmax;
      fXmax = tmp;
    }

    fXminDef = fXmin;
    fXmaxDef = fXmax;
  }

  void SetTolerance(G4double toleranceY)
  {
    fToleranceProba = toleranceY;
  }

  bool Propose(G4double proposedXValue,
               G4double proposedProba,
               G4double nextProba,
               G4double& returnedValue)
  {
    bool returnFlag = false;

    if(proposedProba < nextProba - fToleranceProba) // proba trop petite ==> augmente
    {
      if(fIncreasingCumulativeFunction > 0) // croissant
      {
        if(proposedXValue > fXmin) fXmin = proposedXValue;
      }
      else if(fIncreasingCumulativeFunction < 0) // decroissant
      {
        if(proposedXValue < fXmax) fXmax = proposedXValue;
      }

      returnedValue = (fXmax + fXmin) / 2.;
      returnFlag = false;
    }
    else if(proposedProba > nextProba + fToleranceProba) // proba trop grande
    {
      if(fIncreasingCumulativeFunction > 0)
      {
        if(proposedXValue < fXmax)
        {
          fXmax = proposedXValue;
        }
      }
      else if(fIncreasingCumulativeFunction < 0)
      {
        if(proposedXValue > fXmin)
        {
          fXmin = proposedXValue;
        }
      }

      returnedValue = (fXmax + fXmin) / 2.;
      returnFlag = false;
    }
    else
    {
      // Assuming search for next proba is increasing
      if(fIncreasingCumulativeFunction < 0)
      {
        fXmin = fXminDef;
        fXmax = proposedXValue;
      }
      else if(fIncreasingCumulativeFunction > 0)
      {
        fXmin = proposedXValue;
        fXmax = fXmaxDef;
      }
      returnFlag = true;
    }

    return returnFlag;
  }

  //**********************************
  // *** REVERT THE FUNCTION "FUNCT"
  // *********************************

  G4double Revert(G4double probaForSearchedTime,
                std::function<G4double(G4double)>& funct)
  {
    Reset();
    bool notFound = true;
    G4double proposedX;
    G4double x = fXmax;

    fIncreasingCumulativeFunction = (funct(fXmax) - funct(fXmin))
        / (fXmax - fXmin);

    while(notFound)
    {
      G4double newProba = funct(x);

      if(Propose(x, newProba, probaForSearchedTime, proposedX))
      {
        notFound = false;
      }
      else
      {
        if(x == proposedX)
        {
          return x;
//          G4cout << "BREAK : x= " << x << G4endl;
//          G4cout << "nextProba= " << probaForSearchedTime << G4endl;
//          G4cout << "newProba= " << newProba << G4endl;
//          abort();
        }
        x = proposedX;
      }
    }
    return x;
  }
};
#endif /* SOURCE_PROCESSES_ELECTROMAGNETIC_DNA_UTILS_INCLUDE_G4DNAREVERTPROBABILITY_HH_ */
