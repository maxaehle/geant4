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
//---------------------------------------------------------------------------
//
// GEANT4 Class file
//
// Description: Data on stopping power
//
// Author:      V.Ivanchenko 12.05.2006
//
// Modifications:
// 29.04.2009 A.Ivantchenko added revised data for G4WATER, provided by 
//            Prof.P.Sigmund Univ. Southern Denmark in the framework of 
//            the ESA Technology Research Programme (ESA contract 21435/08/NL/AT)
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4WaterStopping.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmCorrections.hh"
#include "G4PhysicsFreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4int Z[17] = {
  3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 26};
const G4double A[17] = {
  7, 9, 11, 12, 14, 16, 19, 20, 23, 24, 27, 28, 31, 32, 35, 40, 56};

G4WaterStopping::G4WaterStopping(G4EmCorrections* corr, G4bool splineFlag)
{
  spline = splineFlag;
  dedx.reserve(17);
  Initialise(corr);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4WaterStopping::~G4WaterStopping()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4WaterStopping::GetElectronicDEDX(G4int iz, G4double energy)
{
  G4double res = 0.0;
  G4int idx = iz - 3;

  if(iz == 26) { idx = 16; }
  else if (iz < 3 || iz > 18) { return res; }

  G4double scaledEnergy = energy/A[idx];
  if(scaledEnergy < emin) {
    res = (*(dedx[idx]))[0]*std::sqrt(scaledEnergy/emin);
  } else {
    res = (dedx[idx])->Value(scaledEnergy);
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4WaterStopping::AddData(const G4double* energy, 
			      const G4double* stoppower, 
			      G4double factor)
{
  G4PhysicsFreeVector* pv = 
    new G4PhysicsFreeVector(53,energy[0]*CLHEP::MeV,energy[52]*CLHEP::MeV,spline);
  dedx.push_back(pv);
  for(G4int i=0; i<53; ++i) {
    pv->PutValues(i,energy[i]*CLHEP::MeV,stoppower[i]*factor);
  }
  if(spline) { pv->FillSecondDerivatives(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4WaterStopping::Initialise(G4EmCorrections* corr)
{
  //..Reduced energies
  static const G4double E[53] = {0.025,0.03,0.04,0.05,0.06,0.07,0.08,0.09,0.1,0.15,0.2,0.25,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.5,2,2.5,3,4,5,6,7,8,9,10,15,20,25,30,40,50,60,70,80,90,100,150,200,250,300,400,500,600,700,800,900,1000};
  emin = E[0]*CLHEP::MeV;
  static const G4double factor = 1000.*CLHEP::MeV/CLHEP::cm;

  static const G4double G4_WATER_Li[53]={2.3193,2.5198,2.8539,3.1164,3.3203,3.4756,3.5914,3.6755,3.7347,3.8125,3.7349,3.6134,3.4818,3.2258,2.9949,2.7909,2.611,2.4517,2.3103,2.1841,1.7151,1.4139,1.2053,1.0525,0.84417,0.70862,0.61317,0.54214,0.48708,0.44305,0.40697,0.29312,0.23208,0.19364,0.16706,0.13252,0.11092,0.09608,0.08522,0.076915,0.07035,0.065026,0.048615,0.040137,0.034964,0.03149,0.027148,0.024579,0.022911,0.021761,0.020937,0.020327,0.019862};
  AddData(E,G4_WATER_Li,factor);
  static const G4double G4_WATER_Be[53]={2.872,3.1439,3.6102,3.9967,4.3169,4.5788,4.7897,4.9568,5.0872,5.387,5.4028,5.3185,5.1971,4.9243,4.6549,4.4036,4.1732,3.9629,3.771,3.5957,2.9117,2.4439,2.1062,1.8518,1.4956,1.2587,1.0901,0.96393,0.86589,0.78742,0.72313,0.52053,0.41214,0.34394,0.2968,0.2355,0.19717,0.17081,0.15152,0.13676,0.1251,0.11564,0.086471,0.071399,0.062202,0.056023,0.048303,0.043734,0.040767,0.038723,0.037256,0.036172,0.035345};
  AddData(E,G4_WATER_Be,factor);
  static const G4double G4_WATER_B[53]={3.2922,3.6315,4.2226,4.7242,5.1543,5.5219,5.8323,6.0911,6.3043,6.8888,7.0451,7.0309,6.9445,6.6925,6.4129,6.1372,5.8747,5.6283,5.3983,5.1841,4.3121,3.6826,3.2109,2.8459,2.3203,1.9619,1.7028,1.5072,1.3543,1.2315,1.1307,0.81305,0.64344,0.53693,0.46337,0.36777,0.30797,0.26684,0.23674,0.21371,0.1955,0.18072,0.13517,0.11163,0.097256,0.087601,0.075535,0.068395,0.063757,0.060561,0.058268,0.056574,0.05528};
  AddData(E,G4_WATER_B,factor);
  static const G4double G4_WATER_C[53]={3.6037,4.0035,4.7125,5.3248,5.8601,6.3287,6.7363,7.0875,7.3872,8.2986,8.635,8.7189,8.6879,8.485,8.2162,7.9331,7.6534,7.3839,7.1272,6.884,5.8573,5.0814,4.4808,4.0044,3.3005,2.808,2.4458,2.1691,1.9511,1.7751,1.6302,1.1714,0.9263,0.77269,0.66676,0.52925,0.44328,0.38415,0.34086,0.30773,0.28154,0.26028,0.19473,0.16083,0.14014,0.12624,0.10886,0.098575,0.091894,0.08729,0.083986,0.081545,0.079682};
  AddData(E,G4_WATER_C,factor);
  static const G4double G4_WATER_N[53]={3.8821,4.3278,5.1314,5.8371,6.4632,7.021,7.5168,7.9545,8.3378,9.5943,10.145,10.356,10.402,10.278,10.041,9.7677,9.4845,9.2035,8.9301,8.6668,7.5173,6.6126,5.8919,5.308,4.4226,3.7883,3.3138,2.9467,2.655,2.4179,2.2217,1.5965,1.2614,1.0516,0.90715,0.71995,0.60305,0.52268,0.46384,0.41882,0.3832,0.35431,0.26515,0.21903,0.19087,0.17194,0.14829,0.13429,0.12519,0.11892,0.11442,0.1111,0.10856};
  AddData(E,G4_WATER_N,factor);
  static const G4double G4_WATER_O[53]={4.1215,4.6063,5.4945,6.2868,6.9978,7.6391,8.2175,8.7372,9.201,10.808,11.596,11.955,12.096,12.077,11.89,11.639,11.364,11.081,10.799,10.523,9.2787,8.2615,7.4307,6.7431,5.6787,4.8981,4.3045,3.8393,3.4663,3.161,2.9069,2.0903,1.6501,1.3745,1.1851,0.94004,0.78733,0.68244,0.60568,0.54694,0.50048,0.46278,0.34643,0.28622,0.24945,0.22474,0.19384,0.17555,0.16367,0.15547,0.14959,0.14525,0.14194};
  AddData(E,G4_WATER_O,factor);
  static const G4double G4_WATER_F[53]={4.2951,4.8107,5.7678,6.6342,7.4196,8.1343,8.7858,9.3786,9.9152,11.857,12.89,13.408,13.652,13.749,13.62,13.398,13.136,12.857,12.573,12.291,10.982,9.88,8.9601,8.1871,6.9687,6.0574,5.3535,4.7954,4.3434,3.9704,3.658,2.642,2.0878,1.7393,1.4994,1.1892,0.99601,0.86336,0.76632,0.69207,0.63334,0.58568,0.43857,0.36242,0.3159,0.28462,0.24552,0.22237,0.20733,0.19696,0.18951,0.18401,0.17981};
  AddData(E,G4_WATER_F,factor);
  static const G4double G4_WATER_Ne[53]={4.4513,4.991,6.004,6.9338,7.7852,8.5662,9.284,9.9435,10.547,12.813,14.099,14.791,15.151,15.382,15.321,15.136,14.895,14.626,14.345,14.061,12.705,11.53,10.532,9.6823,8.3208,7.2846,6.4735,5.8234,5.2919,4.8502,4.4779,3.252,2.5745,2.146,1.8503,1.4675,1.2291,1.0654,0.94575,0.85419,0.78178,0.72301,0.54158,0.44763,0.39022,0.35162,0.30335,0.27477,0.25619,0.24339,0.23419,0.2274,0.22222};
  AddData(E,G4_WATER_Ne,factor);
  static const G4double G4_WATER_Na[53]={4.5914,5.1553,6.2244,7.2203,8.1435,8.9982,9.7906,10.526,11.206,13.848,15.453,16.384,16.916,17.369,17.442,17.344,17.16,16.93,16.675,16.407,15.045,13.799,12.706,11.753,10.187,8.9672,7.9956,7.2072,6.5562,6.0112,5.5493,4.0154,3.1635,2.6262,2.2573,1.7832,1.4904,1.2907,1.1451,1.0339,0.94615,0.87498,0.65548,0.54186,0.47243,0.42574,0.36734,0.33275,0.31027,0.29478,0.28364,0.27542,0.26915};
  AddData(E,G4_WATER_Na,factor);
  static const G4double G4_WATER_Mg[53]={4.7537,5.3178,6.3962,7.4137,8.3663,9.2554,10.085,10.859,11.581,14.455,16.279,17.373,18.018,18.598,18.727,18.654,18.479,18.25,17.99,17.716,16.313,15.026,13.895,12.907,11.277,9.9981,8.9727,8.1344,7.4376,6.8507,6.35,4.6625,3.7049,3.0916,2.666,2.1135,1.7695,1.5336,1.3613,1.2296,1.1255,1.041,0.7802,0.64511,0.56252,0.50698,0.43749,0.39634,0.36958,0.35114,0.33789,0.3281,0.32063};
  AddData(E,G4_WATER_Mg,factor);
  static const G4double G4_WATER_Al[53]={4.911,5.4856,6.5852,7.6302,8.6193,9.551,10.426,11.248,12.018,15.152,17.23,18.531,19.33,20.11,20.354,20.352,20.224,20.024,19.785,19.521,18.12,16.795,15.611,14.565,12.819,11.431,10.305,9.3772,8.6003,7.9413,7.3761,5.45,4.342,3.6272,3.129,2.4808,2.0767,1.7997,1.5975,1.4429,1.3207,1.2216,0.91581,0.75739,0.66053,0.59537,0.51383,0.46554,0.43413,0.41248,0.39693,0.38544,0.37667};
  AddData(E,G4_WATER_Al,factor);
  static const G4double G4_WATER_Si[53]={5.0697,5.6529,6.77,7.8376,8.8564,9.8229,10.736,11.597,12.409,15.773,18.09,19.594,20.549,21.535,21.9,21.975,21.896,21.73,21.513,21.265,19.878,18.525,17.298,16.202,14.354,12.867,11.651,10.64,9.7876,9.0607,8.4341,6.2756,5.0169,4.1981,3.6247,2.8758,2.4078,2.0868,1.8523,1.6731,1.5315,1.4167,1.0623,0.8787,0.76644,0.69091,0.59637,0.54036,0.50394,0.47883,0.46078,0.44746,0.43728};
  AddData(E,G4_WATER_Si,factor);
  static const G4double G4_WATER_P[53]={5.2616,5.8538,6.9867,8.074,9.1192,10.117,11.065,11.964,12.815,16.396,18.945,20.657,21.779,22.993,23.502,23.672,23.659,23.54,23.356,23.132,21.792,20.425,19.162,18.02,16.066,14.472,13.155,12.051,11.114,10.311,9.6149,7.1917,5.7614,4.8249,4.1667,3.305,2.7661,2.3966,2.1269,1.9215,1.7603,1.6263,1.2197,1.0091,0.88027,0.79361,0.68512,0.62083,0.57902,0.55019,0.52947,0.51417,0.50248};
  AddData(E,G4_WATER_P,factor);
  static const G4double G4_WATER_S[53]={5.4129,6.0193,7.1761,8.2871,9.36,10.39,11.373,12.308,13.196,16.986,19.762,21.683,22.976,24.431,25.091,25.363,25.421,25.354,25.208,25.013,23.734,22.366,21.074,19.891,17.84,16.146,14.731,13.536,12.516,11.636,10.869,8.1727,6.5617,5.4997,4.7505,3.7671,3.1514,2.7293,2.4215,2.1866,2.0012,1.8509,1.3881,1.1485,1.002,0.90348,0.78008,0.70695,0.65938,0.62657,0.60299,0.58558,0.57229};
  AddData(E,G4_WATER_S,factor);
  static const G4double G4_WATER_Cl[53]={5.6171,6.2307,7.3984,8.5209,9.6097,10.661,11.669,12.632,13.551,17.518,20.497,22.615,24.076,25.769,26.58,26.953,27.082,27.066,26.958,26.791,25.579,24.214,22.901,21.685,19.553,17.771,16.271,14.996,13.9,12.949,12.118,9.1688,7.385,6.2003,5.3604,4.2535,3.5588,3.0821,2.7343,2.4689,2.2595,2.0898,1.5673,1.297,1.1318,1.0205,0.88128,0.79874,0.74504,0.708,0.68138,0.66172,0.64671};
  AddData(E,G4_WATER_Cl,factor);
  static const G4double G4_WATER_Ar[53]={5.7158,6.3394,7.5204,8.6525,9.7528,10.82,11.849,12.836,13.78,17.904,21.07,23.375,24.999,26.928,27.889,28.361,28.559,28.592,28.521,28.381,27.228,25.869,24.541,23.299,21.103,19.255,17.686,16.346,15.19,14.183,13.3,10.137,8.2021,6.9062,5.9819,4.757,3.9841,3.4523,3.0636,2.7668,2.5324,2.3425,1.7573,1.4546,1.2694,1.1448,0.98872,0.8962,0.83601,0.79448,0.76464,0.74259,0.72575};
  AddData(E,G4_WATER_Ar,factor);

  static const G4double G4_WATER_Fe [53]={6.5394, 7.3060, 8.7367, 10.0690, 11.3310, 12.5470, 13.7280, 14.8780, 15.9980, 21.1160, 25.4850, 29.1260, 32.0640, 36.1770, 38.6920, 40.2760, 41.2950, 41.9530, 42.3710, 42.6210, 42.5910, 41.6900, 40.5190, 39.2690, 36.8000, 34.5040, 32.4190, 30.5410, 28.8480, 27.3170, 25.9310, 20.6170, 17.0680, 14.5540, 12.6930, 10.1410, 8.4892, 7.3402, 6.4976, 5.8545, 5.3479, 4.9387, 3.6892, 3.0503, 2.6620, 2.4014, 2.0756, 1.8825, 1.7569, 1.6702, 1.6079, 1.5619, 1.5267};
  AddData(E,G4_WATER_Fe,factor);

  if(nullptr != corr) {
    for(G4int i=0; i<17; ++i) {
      corr->AddStoppingData(Z[i], A[i], "G4_WATER", dedx[i]);
    }
  }
}
