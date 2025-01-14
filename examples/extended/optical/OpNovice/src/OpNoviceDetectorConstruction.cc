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
/// \file OpNovice/src/OpNoviceDetectorConstruction.cc
/// \brief Implementation of the OpNoviceDetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "OpNoviceDetectorConstruction.hh"
#include "OpNoviceDetectorMessenger.hh"

#include "G4Box.hh"
#include "G4Element.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OpNoviceDetectorConstruction::OpNoviceDetectorConstruction()
  : G4VUserDetectorConstruction()
{
  fDumpGdmlFileName = "OpNovice_dump.gdml";
  fVerbose          = false;
  fDumpGdml         = false;
  // create a messenger for this class
  fDetectorMessenger = new OpNoviceDetectorMessenger(this);
  fWorld_x = fWorld_y = fWorld_z = 15.0 * m;
  fExpHall_x = fExpHall_y = fExpHall_z = 10.0 * m;
  fTank_x = fTank_y = fTank_z = 5.0 * m;
  fBubble_x = fBubble_y = fBubble_z = 0.5 * m;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OpNoviceDetectorConstruction::~OpNoviceDetectorConstruction()
{
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* OpNoviceDetectorConstruction::Construct()
{
  G4bool checkOverlaps = true;
  // ------------- Materials -------------
  G4double a, z, density;
  G4int nelements;

  // Air
  G4Element* N = new G4Element("Nitrogen", "N", z = 7, a = 14.01 * g / mole);
  G4Element* O = new G4Element("Oxygen", "O", z = 8, a = 16.00 * g / mole);
  G4Material* air =
    new G4Material("Air", density = 1.29 * mg / cm3, nelements = 2);
  air->AddElement(N, 70. * perCent);
  air->AddElement(O, 30. * perCent);
  //
  // Water
  G4Element* H = new G4Element("Hydrogen", "H", z = 1, a = 1.01 * g / mole);
  G4Material* water =
    new G4Material("Water", density = 1.0 * g / cm3, nelements = 2);
  water->AddElement(H, 2);
  water->AddElement(O, 1);

  // ------------ Generate & Add Material Properties Table ------------
  //
  std::vector<G4double> photonEnergy = {
    2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV,
    2.256 * eV, 2.298 * eV, 2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV,
    2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV, 2.757 * eV, 2.820 * eV,
    2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
    3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV,
    4.002 * eV, 4.136 * eV
  };

  // Water
  std::vector<G4double> refractiveIndex1 = {
    1.3435, 1.344,  1.3445, 1.345,  1.3455, 1.346,  1.3465, 1.347,
    1.3475, 1.348,  1.3485, 1.3492, 1.35,   1.3505, 1.351,  1.3518,
    1.3522, 1.3530, 1.3535, 1.354,  1.3545, 1.355,  1.3555, 1.356,
    1.3568, 1.3572, 1.358,  1.3585, 1.359,  1.3595, 1.36,   1.3608
  };
  std::vector<G4double> absorption = {
    3.448 * m,  4.082 * m,  6.329 * m,  9.174 * m,  12.346 * m, 13.889 * m,
    15.152 * m, 17.241 * m, 18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m,
    45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m, 55.556 * m, 52.632 * m,
    52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
    30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m,
    17.500 * m, 14.500 * m
  };

  // Material properties can be added as arrays. However, in this case it is
  // up to the user to make sure both arrays have the same number of elements.
  G4double scintilFastArray[]{ 1.0, 1.0 };
  G4double energyArray[]{ 2.034 * eV, 4.136 * eV };
  G4int lenArray = 2;

  std::vector<G4double> scintilSlow = {
    0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
    7.00, 6.00, 4.00, 3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00, 4.00,
    5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 5.00, 4.00
  };

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  // Values can be added to the material property table individually.
  // With this method, spline interpolation cannot be set. Arguments
  // createNewKey and spline both take their default values of false.
  // Need to specify the number of entries (1) in the arrays, as an argument
  // to AddProperty.
  G4int numEntries = 1;
  myMPT1->AddProperty("RINDEX", &photonEnergy[0], &refractiveIndex1[0],
                      numEntries);

  for(size_t i = 1; i < photonEnergy.size(); ++i)
  {
    myMPT1->AddEntry("RINDEX", photonEnergy[i], refractiveIndex1[i]);
  }

  // Check that group velocity is calculated from RINDEX
  if(myMPT1->GetProperty("RINDEX")->GetVectorLength() !=
     myMPT1->GetProperty("GROUPVEL")->GetVectorLength())
  {
    G4ExceptionDescription ed;
    ed << "Error calculating group velocities. Incorrect number of entries "
          "in group velocity material property vector.";
    G4Exception("OpNovice::OpNoviceDetectorConstruction", "OpNovice001",
                FatalException, ed);
  }

  myMPT1->AddProperty("ABSLENGTH", photonEnergy, absorption, false, true);
  // adding property with a C-style array
  myMPT1->AddProperty("SCINTILLATIONCOMPONENT1", energyArray, scintilFastArray,
                      lenArray, false, true);
  myMPT1->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, scintilSlow,
                      false, true);
  myMPT1->AddConstProperty("SCINTILLATIONYIELD", 50. / MeV);
  myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
  myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1. * ns);
  myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10. * ns);
  myMPT1->AddConstProperty("SCINTILLATIONYIELD1", 0.8);
  myMPT1->AddConstProperty("SCINTILLATIONYIELD2", 0.2);
  std::vector<G4double> energy_water = {
    1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV, 1.65333 * eV,
    1.67567 * eV, 1.69863 * eV, 1.72222 * eV, 1.74647 * eV, 1.77142 * eV,
    1.7971 * eV,  1.82352 * eV, 1.85074 * eV, 1.87878 * eV, 1.90769 * eV,
    1.93749 * eV, 1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
    2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV, 2.25454 * eV,
    2.29629 * eV, 2.33962 * eV, 2.38461 * eV, 2.43137 * eV, 2.47999 * eV,
    2.53061 * eV, 2.58333 * eV, 2.63829 * eV, 2.69565 * eV, 2.75555 * eV,
    2.81817 * eV, 2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
    3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV, 3.54285 * eV,
    3.64705 * eV, 3.75757 * eV, 3.87499 * eV, 3.99999 * eV, 4.13332 * eV,
    4.27585 * eV, 4.42856 * eV, 4.59258 * eV, 4.76922 * eV, 4.95999 * eV,
    5.16665 * eV, 5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV
  };

  // Rayleigh scattering length calculated by G4OpRayleigh
  // assume 100 times larger than the rayleigh scattering for now.
  std::vector<G4double> mie_water = {
    167024.4 * m, 158726.7 * m, 150742 * m,   143062.5 * m, 135680.2 * m,
    128587.4 * m, 121776.3 * m, 115239.5 * m, 108969.5 * m, 102958.8 * m,
    97200.35 * m, 91686.86 * m, 86411.33 * m, 81366.79 * m, 76546.42 * m,
    71943.46 * m, 67551.29 * m, 63363.36 * m, 59373.25 * m, 55574.61 * m,
    51961.24 * m, 48527.00 * m, 45265.87 * m, 42171.94 * m, 39239.39 * m,
    36462.50 * m, 33835.68 * m, 31353.41 * m, 29010.30 * m, 26801.03 * m,
    24720.42 * m, 22763.36 * m, 20924.88 * m, 19200.07 * m, 17584.16 * m,
    16072.45 * m, 14660.38 * m, 13343.46 * m, 12117.33 * m, 10977.70 * m,
    9920.416 * m, 8941.407 * m, 8036.711 * m, 7202.470 * m, 6434.927 * m,
    5730.429 * m, 5085.425 * m, 4496.467 * m, 3960.210 * m, 3473.413 * m,
    3032.937 * m, 2635.746 * m, 2278.907 * m, 1959.588 * m, 1675.064 * m,
    1422.710 * m, 1200.004 * m, 1004.528 * m, 833.9666 * m, 686.1063 * m
  };

  // gforward, gbackward, forward backward ratio
  G4double mie_water_const[3] = { 0.99, 0.99, 0.8 };

  myMPT1->AddProperty("MIEHG", energy_water, mie_water, false, true);
  myMPT1->AddConstProperty("MIEHG_FORWARD", mie_water_const[0]);
  myMPT1->AddConstProperty("MIEHG_BACKWARD", mie_water_const[1]);
  myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO", mie_water_const[2]);

  G4cout << "Water G4MaterialPropertiesTable:" << G4endl;
  myMPT1->DumpTable();

  water->SetMaterialPropertiesTable(myMPT1);

  // Set the Birks Constant for the Water scintillator
  water->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

  // Air
  std::vector<G4double> refractiveIndex2 = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                                             1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                                             1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                                             1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                                             1.0, 1.0, 1.0, 1.0 };

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2);

  G4cout << "Air G4MaterialPropertiesTable:" << G4endl;
  myMPT2->DumpTable();

  air->SetMaterialPropertiesTable(myMPT2);

  // ------------- Volumes --------------
  //
  // The world
  G4Box* world_box = new G4Box("World", fWorld_x, fWorld_y, fWorld_z);
  G4LogicalVolume* world_log =
    new G4LogicalVolume(world_box, air, "World", 0, 0, 0);
  G4VPhysicalVolume* world_phys = new G4PVPlacement(
    0, G4ThreeVector(), world_log, "world", 0, false, 0, checkOverlaps);

  // The experimental Hall
  G4Box* expHall_box = new G4Box("expHall", fExpHall_x, fExpHall_y, fExpHall_z);
  G4LogicalVolume* expHall_log =
    new G4LogicalVolume(expHall_box, air, "expHall", 0, 0, 0);
  G4VPhysicalVolume* expHall_phys = new G4PVPlacement(
    0, G4ThreeVector(), expHall_log, "expHall", world_log, false, 0);

  // The Water Tank
  G4Box* waterTank_box = new G4Box("Tank", fTank_x, fTank_y, fTank_z);
  G4LogicalVolume* waterTank_log =
    new G4LogicalVolume(waterTank_box, water, "Tank", 0, 0, 0);
  G4VPhysicalVolume* waterTank_phys = new G4PVPlacement(
    0, G4ThreeVector(), waterTank_log, "Tank", expHall_log, false, 0);

  // The Air Bubble
  G4Box* bubbleAir_box = new G4Box("Bubble", fBubble_x, fBubble_y, fBubble_z);
  G4LogicalVolume* bubbleAir_log =
    new G4LogicalVolume(bubbleAir_box, air, "Bubble", 0, 0, 0);
  new G4PVPlacement(0, G4ThreeVector(0, 2.5 * m, 0), bubbleAir_log, "Bubble",
                    waterTank_log, false, 0);

  // ------------- Surfaces --------------

  // Water Tank
  G4OpticalSurface* opWaterSurface = new G4OpticalSurface("WaterSurface");
  opWaterSurface->SetType(dielectric_LUTDAVIS);
  opWaterSurface->SetFinish(Rough_LUT);
  opWaterSurface->SetModel(DAVIS);

  G4LogicalBorderSurface* waterSurface = new G4LogicalBorderSurface(
    "WaterSurface", waterTank_phys, expHall_phys, opWaterSurface);

  G4OpticalSurface* opticalSurface = dynamic_cast<G4OpticalSurface*>(
    waterSurface->GetSurface(waterTank_phys, expHall_phys)
      ->GetSurfaceProperty());
  if(opticalSurface)
    opticalSurface->DumpInfo();

  // Air Bubble
  G4OpticalSurface* opAirSurface = new G4OpticalSurface("AirSurface");
  opAirSurface->SetType(dielectric_dielectric);
  opAirSurface->SetFinish(polished);
  opAirSurface->SetModel(glisur);

  G4LogicalSkinSurface* airSurface =
    new G4LogicalSkinSurface("AirSurface", bubbleAir_log, opAirSurface);

  opticalSurface = dynamic_cast<G4OpticalSurface*>(
    airSurface->GetSurface(bubbleAir_log)->GetSurfaceProperty());
  if(opticalSurface)
    opticalSurface->DumpInfo();

  // Generate & Add Material Properties Table attached to the optical surfaces
  //
  std::vector<G4double> ephoton = { 2.034 * eV, 4.136 * eV };

  // OpticalAirSurface
  std::vector<G4double> reflectivity = { 0.3, 0.5 };
  std::vector<G4double> efficiency   = { 0.8, 1.0 };

  G4MaterialPropertiesTable* myST2 = new G4MaterialPropertiesTable();

  myST2->AddProperty("REFLECTIVITY", ephoton, reflectivity);
  myST2->AddProperty("EFFICIENCY", ephoton, efficiency);
  if(fVerbose)
  {
    G4cout << "Air Surface G4MaterialPropertiesTable:" << G4endl;
    myST2->DumpTable();
  }
  opAirSurface->SetMaterialPropertiesTable(myST2);

  if(fDumpGdml)
  {
    G4GDMLParser* parser = new G4GDMLParser();
    parser->Write(fDumpGdmlFileName, world_phys);
  }

  ////////////////////////////////////////////////////////////////////////////
  // test user-defined properties
  G4String ed;
  if(myMPT1->GetProperty("USERDEFINED") != nullptr)
  {
    ed = "USERDEFINED != nullptr";
    PrintError(ed);
  }
  myMPT1->AddProperty("USERDEFINED", energy_water, mie_water, true, true);
  if(myMPT1->GetProperty("USERDEFINED") == nullptr)
  {
    ed = "USERDEFINED == nullptr";
    PrintError(ed);
  }
  [[maybe_unused]] G4int index_userdefined = -1;
  if(myMPT1->GetProperty("USERDEFINED") != nullptr)
  {
    index_userdefined = myMPT1->GetPropertyIndex("USERDEFINED");
  }
  if(!(index_userdefined >= 0 &&
        index_userdefined <
        (G4int) myMPT1->GetMaterialPropertyNames().size()))
  {
    ed = "USERDEFINED index out of range";
    PrintError(ed);
  }
  myMPT1->RemoveProperty("USERDEFINED");
  if(myMPT1->GetProperty("USERDEFINED") != nullptr)
  {
    ed = "USERDEFINED != nullptr at end";
    PrintError(ed);
  }

  if(myMPT1->ConstPropertyExists("USERDEFINEDCONST") == true)
  {
    ed = "ConstProperty USERDEFINEDCONST already exists.";
    PrintError(ed);
  }
  myMPT1->AddConstProperty("USERDEFINEDCONST", 3.14, true);
  if(myMPT1->ConstPropertyExists("USERDEFINEDCONST") == false)
  {
    ed = "ConstProperty USERDEFINEDCONST doesn't exist.";
    PrintError(ed);
  }
  [[maybe_unused]] G4int index_pi = -1;
  if(myMPT1->ConstPropertyExists("USERDEFINEDCONST") == true)
  {
    index_pi = myMPT1->GetConstPropertyIndex("USERDEFINEDCONST");
  }
  if (!(index_pi >= 0 &&
         index_pi < (G4int) myMPT1->GetMaterialConstPropertyNames().size()))
  {
    ed = "ConstProperty USERDEFINEDCONST index out of range.";
    PrintError(ed);
  }
  myMPT1->RemoveConstProperty("USERDEFINEDCONST");
  if (myMPT1->ConstPropertyExists("USERDEFINEDCONST") == true)
  {
    ed = "ConstProperty USERDEFINEDCONST still exists.";
    PrintError(ed);
  }
  // done testing user-defined properties
  ////////////////////////////////////////////////////////////////////////////

  return world_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceDetectorConstruction::SetDumpGdml(G4bool val) { fDumpGdml = val; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool OpNoviceDetectorConstruction::IsDumpGdml() const { return fDumpGdml; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceDetectorConstruction::SetVerbose(G4bool val) { fVerbose = val; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool OpNoviceDetectorConstruction::IsVerbose() const { return fVerbose; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceDetectorConstruction::SetDumpGdmlFile(G4String filename)
{
  fDumpGdmlFileName = filename;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String OpNoviceDetectorConstruction::GetDumpGdmlFile() const
{
  return fDumpGdmlFileName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void OpNoviceDetectorConstruction::PrintError(G4String ed)
{
  G4Exception("OpNoviceDetectorConstruction:MaterialProperty test", "op001",
              FatalException, ed);
}
