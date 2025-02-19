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
// G4GDMLWriteStructure
//
// Class description:
//
// GDML class for export of structures.

// Author: Zoltan Torzsok, November 2007
// --------------------------------------------------------------------
#ifndef G4GDMLWRITESTRUCTURE_HH
#define G4GDMLWRITESTRUCTURE_HH 1

#include "G4Types.hh"
#include "G4Transform3D.hh"
#include "G4GDMLWriteParamvol.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4PVDivision;
class G4LogicalBorderSurface;
class G4LogicalSkinSurface;
class G4OpticalSurface;
class G4SurfaceProperty;
class G4ReflectionFactory;
class G4AssemblyTriplet;

class G4GDMLWriteStructure : public G4GDMLWriteParamvol
{
  public:

    G4GDMLWriteStructure();
    virtual ~G4GDMLWriteStructure();

    virtual void StructureWrite(xercesc::DOMElement*);
    void AddVolumeAuxiliary(G4GDMLAuxStructType myaux,
                            const G4LogicalVolume* const);

    void SetEnergyCutsExport(G4bool);
    void SetSDExport(G4bool);

    G4int GetMaxExportLevel() const;
    void SetMaxExportLevel(G4int);

  protected:

    void DivisionvolWrite(xercesc::DOMElement*, const G4PVDivision* const);
    void PhysvolWrite(xercesc::DOMElement*,
                      const G4VPhysicalVolume* const topVol,
                      const G4Transform3D& transform,
                      const G4String& moduleName);
    void ReplicavolWrite(xercesc::DOMElement*, const G4VPhysicalVolume* const);
    void AssemblyWrite(xercesc::DOMElement*, const int assemblyID);
    G4Transform3D TraverseVolumeTree(const G4LogicalVolume* const topVol,
                                     const G4int depth);
    void SurfacesWrite();
    void BorderSurfaceCache(const G4LogicalBorderSurface* const);
    void SkinSurfaceCache(const G4LogicalSkinSurface* const);
    const G4LogicalBorderSurface* GetBorderSurface(
                          const G4VPhysicalVolume* const);
    const G4LogicalSkinSurface* GetSkinSurface(const G4LogicalVolume* const);
    G4bool FindOpticalSurface(const G4SurfaceProperty*);
    void ExportEnergyCuts(const G4LogicalVolume* const);
    void ExportSD(const G4LogicalVolume* const);

  protected:

    xercesc::DOMElement* structureElement = nullptr;
    std::vector<xercesc::DOMElement*> borderElementVec;
    std::vector<xercesc::DOMElement*> skinElementVec;
    std::map<const G4LogicalVolume*, G4GDMLAuxListType> auxmap;

  private:  // cache for optical surfaces...

    std::vector<const G4OpticalSurface*> opt_vec;

    G4ReflectionFactory* reflFactory = nullptr;

    G4bool cexport = false;
      // Flag for optional export of energy cuts per volume
    G4bool sdexport= false;
      // Flag for optional export of SD per volume
    G4int maxLevel = 0;
      // Maximum number of levels to export
    static G4int levelNo;
      // Counter for level being exported

    std::map<const G4VPhysicalVolume*, G4int> assemblyVolMap;
      // Map of phys volumes to assembly IDs
    std::map<const G4VPhysicalVolume*, G4int> imprintsMap;
      // Map of phys volumes to imprints IDs
    std::vector<G4int> addedAssemblies;
      // Vector of assemblies IDs already added to the structure
};

#endif
