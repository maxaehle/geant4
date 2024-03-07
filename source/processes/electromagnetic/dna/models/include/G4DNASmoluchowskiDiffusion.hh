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
 * G4DNASmoluchowskiDiffusion.hh
 *
 *  Created on: 2 févr. 2015
 *      Author: matkara
 */

#ifndef G4DNASMOLUCHOWSKIDIFFUSION_HH_
#define G4DNASMOLUCHOWSKIDIFFUSION_HH_

//#if __cplusplus >= 201103L
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

//#define DNADEV_TEST

#ifdef DNADEV_TEST
#include <TF1.h>
#endif

#include <cassert>

#ifndef DNADEV_TEST
#include "globals.hh"
#include "Randomize.hh"
#endif

#ifdef DNADEV_TEST
#include "TRandom.h"
TRandom root_random;
G4double G4UniformRand()
{
  return root_random.Rndm();
}

#define G4cout std::cout
#define G4endl std::endl
#endif

#include "G4Exp.hh"

class G4DNASmoluchowskiDiffusion
{
public:
  G4DNASmoluchowskiDiffusion(G4double epsilon = 1e-5);
  virtual ~G4DNASmoluchowskiDiffusion();

  static G4double ComputeS(G4double r, G4double D, G4double t)
  {
    G4double sTransform = r / (2. * std::sqrt(D * t));
    return sTransform;
  }

  static G4double ComputeDistance(G4double sTransform, G4double D, G4double t)
  {
    return sTransform * 2. * std::sqrt(D * t);
  }

  static G4double ComputeTime(G4double sTransform, G4double D, G4double r)
  {
    return std::pow(r / sTransform, 2.) / (4. * D);
  }

  //====================================================

  G4double GetRandomDistance(G4double _time, G4double D)
  {
    G4double proba = G4UniformRand();
//    G4cout << "proba = " << proba << G4endl;
    G4double sTransform = GetInverseProbability(proba);
//    G4cout << "sTransform = " << sTransform << G4endl;
    return ComputeDistance(sTransform, D, _time);
  }

  G4double GetRandomTime(G4double distance, G4double D)
  {
    G4double proba = G4UniformRand();
    G4double sTransform = GetInverseProbability(proba);
    return ComputeTime(sTransform, D, distance);
  }

  G4double EstimateCrossingTime(G4double proba,
                              G4double distance,
                              G4double D)
  {
    G4double sTransform = GetInverseProbability(proba);
    return ComputeTime(sTransform, D, distance);
  }

  //====================================================
  // 1-value transformation

  // WARNING : this is NOT the differential probability
  // this is the derivative of the function GetCumulativeProbability
  static G4double GetDifferential(G4double sTransform)
  {
    static G4double constant = -4./std::sqrt(3.141592653589793);
    return sTransform*sTransform*G4Exp(-sTransform*sTransform)*constant; // -4*sTransform*sTransform*exp(-sTransform*sTransform)/sqrt(3.141592653589793)
  }

  static G4double GetDensityProbability(G4double r, G4double _time, G4double D)
  {
    static G4double my_pi = 3.141592653589793;
    static G4double constant = 4.*my_pi/std::pow(4.*my_pi, 1.5);
    return r*r/std::pow(D * _time,1.5)*G4Exp(-r*r/(4. * D * _time))*constant;
  }

  //====================================================
  // BOUNDING BOX
  struct BoundingBox
  {
    G4double fXmax;
    G4double fXmin;
    G4double fXmaxDef;
    G4double fXminDef;
    G4double fToleranceY;
    G4double fSum;
    G4double    fIncreasingCumulativeFunction;

    enum PreviousAction
    {
      IncreaseProba,
      DecreaseProba,
      Undefined
    };

    PreviousAction fPreviousAction;

    BoundingBox(G4double xmin,
                G4double xmax,
                G4double toleranceY) :
     fXmax(xmax), fXmin(xmin),
     fToleranceY(toleranceY),
     fSum(0)
    {
      if(fXmax < fXmin)
      {
        G4double tmp = fXmin;
        fXmin = fXmax;
        fXmax = tmp;
      }
      
      fXminDef = fXmin;
      fXmaxDef = fXmax;
      fPreviousAction = BoundingBox::Undefined;
      fIncreasingCumulativeFunction = (GetCumulativeProbability(fXmax) - GetCumulativeProbability(fXmin))/(fXmax-fXmin);
    }
    
    void Print()
    {
      G4cout << "fXmin: " << fXmin << " | fXmax: " << fXmax << G4endl;
    }

    bool Propose(G4double proposedXValue,
                 G4double proposedProba,
                 G4double nextProba,
                 G4double& returnedValue)
    {
//      G4cout << "---------------------------" << G4endl;
//      G4cout << "Proposed x value: " << proposedXValue
//          << "| proposedProba: " << proposedProba
//          << "| nextProba: " << nextProba
//          << " | fXmin: " << fXmin << " (" << G4DNASmoluchowskiDiffusion::GetCumulativeProbability(fXmin) <<")"
//          << " | fXmax: " << fXmax << " (" << G4DNASmoluchowskiDiffusion::GetCumulativeProbability(fXmax) <<")"
//          << G4endl;

      bool returnFlag = false;
      
      if(proposedProba < nextProba-fToleranceY) // proba trop petite ==> augmente
      {
        // G4cout << "proposedProba < nextProba-fToleranceY" << G4endl;

        if(fIncreasingCumulativeFunction > 0) // croissant
        {
          if(proposedXValue > fXmin)
            fXmin = proposedXValue;
        }
        else if(fIncreasingCumulativeFunction < 0) // decroissant
        {
          if(proposedXValue < fXmax)
            fXmax = proposedXValue;
        }
        
        returnedValue = (fXmax + fXmin)/2;
        returnFlag = false;
        fPreviousAction = BoundingBox::IncreaseProba;
      }
      else if(proposedProba > nextProba+fToleranceY) // proba trop grande
      {
        // G4cout << "proposedProba > nextProba+fToleranceY" << G4endl;

        if(fIncreasingCumulativeFunction>0)
        {
          if(proposedXValue < fXmax)
            fXmax = proposedXValue;
        }
        else if(fIncreasingCumulativeFunction<0)
        {
          if(proposedXValue > fXmin)
          {
            fXmin = proposedXValue;
          }
        }
        
        returnedValue = (fXmax + fXmin)/2;
        returnFlag = false;
        fPreviousAction = BoundingBox::DecreaseProba;
      }
      else
      {
        // G4cout << "IN THE INTERVAL !! : " << nextProba << G4endl;
        fSum = proposedProba;
        
        // Assuming search for next proba is increasing
        if(fIncreasingCumulativeFunction<0)
        {
         fXmin = fXminDef;
         fXmax = proposedXValue;
        }
        else if(fIncreasingCumulativeFunction>0)
        {
          fXmin = proposedXValue;
          fXmax = fXmaxDef;
        }
        returnFlag = true;
        fPreviousAction = BoundingBox::Undefined;
      }
      
      return returnFlag;
    }
  };
  // END OF BOUNDING BOX
  //==============================
  
  void PrepareReverseTable(G4double xmin, G4double xmax)
  {
    G4double x = xmax;
    int index = 0;
    G4double nextProba = fEpsilon;
    G4double proposedX;

    BoundingBox boundingBox(xmin, xmax, fEpsilon*1e-5);

    while(index <= fNbins)
    // in case GetCumulativeProbability is exact (digitally speaking), replace with:
    // while(index <= fNbins+1)
    {
      nextProba = fEpsilon*index;

      G4double newProba = GetCumulativeProbability(x);

      if(boundingBox.Propose(x, newProba, nextProba, proposedX))
      {
        fInverse[index] = x;
        index++;
      }
      else
      {
        if(x == proposedX)
        {
          G4cout << "BREAK : x= " << x << G4endl;
          G4cout << "index= " << index << G4endl;
          G4cout << "nextProba= " << nextProba << G4endl;
          G4cout << "newProba= " << newProba << G4endl;
          abort();
        }
        x = proposedX;
      }
    }
    
    fInverse[fNbins+1] = 0; // P(1) = 0, because we want it exact !
    // Tips to improve the exactness: get an better value of pi, get better approximation of erf and exp, use long G4double instead of G4double
//    boundingBox.Print();
  }

  static G4double GetCumulativeProbability(G4double sTransform)
  {
    static G4double constant = 2./std::sqrt(3.141592653589793);
    return erfc(sTransform) + constant*sTransform*G4Exp(-sTransform*sTransform);
  }

  G4double GetInverseProbability(G4double proba) // returns sTransform
  {
    size_t index_low = (size_t) trunc(proba/fEpsilon);
    
    if(index_low == 0) // assymptote en 0
    {
      index_low = 1;
      size_t index_up = 2;
      G4double low_y = fInverse[index_low];
      G4double up_y = fInverse[index_up];
      G4double low_x = index_low*fEpsilon;
      G4double up_x = proba+fEpsilon;
      G4double tangente = (low_y-up_y)/(low_x - up_x); // ou utiliser GetDifferential(proba) ?
      // G4double tangente = GetDifferential(proba);
      return low_y + tangente*(proba-low_x);
    }

    size_t index_up = index_low+1;
    if(index_low > fInverse.size()) return fInverse.back();
    G4double low_y = fInverse[index_low];
    G4double up_y = fInverse[index_up];

    G4double low_x = index_low*fEpsilon;
    G4double up_x = low_x+fEpsilon;

    if(up_x > 1) // P(1) = 0
    {
      up_x = 1;
      up_y = 0; // more general : fInverse.back()
    }

    G4double tangente = (low_y-up_y)/(low_x - up_x);

    return low_y + tangente*(proba-low_x);
  }

  G4double PlotInverse(G4double* x, G4double* )
  {
    return GetInverseProbability(x[0]);
  }

  G4double Plot(G4double* x, G4double* )
  {
    return GetDifferential(x[0]);
  }


  void InitialiseInverseProbability(G4double xmax = 3e28)
  {
    // x > x'
    // P'(x) = p(x') = lim(x->x') (P(x') - P(x))/(x'-x)
    // x'-x = (P(x') - P(x))/p(x')
    // x = x' - (P(x') - P(x))/p(x')

    // fInverse initialized in the constructor

    assert(fNbins !=0);
    PrepareReverseTable(0,xmax);
  }

  std::vector<G4double> fInverse;
  int fNbins;
  G4double fEpsilon;
};

#endif /* SOURCE_PROCESSES_ELECTROMAGNETIC_DNA_MODELS_G4DNASMOLUCHOWSKIDIFFUSION_HH_ */
