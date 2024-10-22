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
// G4GDMLReadSolids implementation
//
// Author: Zoltan Torzsok, November 2007
// --------------------------------------------------------------------

#include "G4GDMLReadSolids.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Ellipsoid.hh"
#include "G4EllipticalCone.hh"
#include "G4EllipticalTube.hh"
#include "G4Hype.hh"
#include "G4IntersectionSolid.hh"
#include "G4Orb.hh"
#include "G4Para.hh"
#include "G4Paraboloid.hh"
#include "G4Polycone.hh"
#include "G4GenericPolycone.hh"
#include "G4Polyhedra.hh"
#include "G4QuadrangularFacet.hh"
#include "G4ReflectedSolid.hh"
#include "G4ScaledSolid.hh"
#include "G4Sphere.hh"
#include "G4SolidStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4GenericTrap.hh"
#include "G4TessellatedSolid.hh"
#include "G4Tet.hh"
#include "G4Torus.hh"
#include "G4Transform3D.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4TriangularFacet.hh"
#include "G4Tubs.hh"
#include "G4CutTubs.hh"
#include "G4TwistedBox.hh"
#include "G4TwistedTrap.hh"
#include "G4TwistedTrd.hh"
#include "G4TwistedTubs.hh"
#include "G4UnionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4UnitsTable.hh"
#include "G4SurfaceProperty.hh"

// --------------------------------------------------------------------
G4GDMLReadSolids::G4GDMLReadSolids()
  : G4GDMLReadMaterials()
{
}

// --------------------------------------------------------------------
G4GDMLReadSolids::~G4GDMLReadSolids()
{
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::BooleanRead(
  const xercesc::DOMElement* const booleanElement, const BooleanOp op)
{
  G4String name;
  G4String first;
  G4String scnd;
  G4ThreeVector position(0.0, 0.0, 0.0);
  G4ThreeVector rotation(0.0, 0.0, 0.0);
  G4ThreeVector firstposition(0.0, 0.0, 0.0);
  G4ThreeVector firstrotation(0.0, 0.0, 0.0);

  const xercesc::DOMNamedNodeMap* const attributes =
    booleanElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::BooleanRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
  }

  for(xercesc::DOMNode* iter = booleanElement->getFirstChild(); iter != nullptr;
      iter                   = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::BooleanRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "first")
    {
      first = RefRead(child);
    }
    else if(tag == "second")
    {
      scnd = RefRead(child);
    }
    else if(tag == "position")
    {
      VectorRead(child, position);
    }
    else if(tag == "rotation")
    {
      VectorRead(child, rotation);
    }
    else if(tag == "positionref")
    {
      position = GetPosition(GenerateName(RefRead(child)));
    }
    else if(tag == "rotationref")
    {
      rotation = GetRotation(GenerateName(RefRead(child)));
    }
    else if(tag == "firstposition")
    {
      VectorRead(child, firstposition);
    }
    else if(tag == "firstrotation")
    {
      VectorRead(child, firstrotation);
    }
    else if(tag == "firstpositionref")
    {
      firstposition = GetPosition(GenerateName(RefRead(child)));
    }
    else if(tag == "firstrotationref")
    {
      firstrotation = GetRotation(GenerateName(RefRead(child)));
    }
    else
    {
      G4String error_msg = "Unknown tag in boolean solid: " + tag;
      G4Exception("G4GDMLReadSolids::BooleanRead()", "ReadError",
                  FatalException, error_msg);
    }
  }

  G4VSolid* firstSolid  = GetSolid(GenerateName(first));
  G4VSolid* secondSolid = GetSolid(GenerateName(scnd));

  G4Transform3D transform(GetRotationMatrix(rotation), position);

  if(((firstrotation.x() != 0.0) || (firstrotation.y() != 0.0) ||
      (firstrotation.z() != 0.0)) ||
     ((firstposition.x() != 0.0) || (firstposition.y() != 0.0) ||
      (firstposition.z() != 0.0)))
  {
    G4Transform3D firsttransform(GetRotationMatrix(firstrotation),
                                 firstposition);
    firstSolid = new G4DisplacedSolid(GenerateName("displaced_" + first),
                                      firstSolid, firsttransform);
  }

  if(op == UNION)
  {
    new G4UnionSolid(name, firstSolid, secondSolid, transform);
  }
  else if(op == SUBTRACTION)
  {
    new G4SubtractionSolid(name, firstSolid, secondSolid, transform);
  }
  else if(op == INTERSECTION)
  {
    new G4IntersectionSolid(name, firstSolid, secondSolid, transform);
  }
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::BoxRead(const xercesc::DOMElement* const boxElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double x     = 0.0;
  G4double y     = 0.0;
  G4double z     = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    boxElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::BoxRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::BoxRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "x")
    {
      x = eval.Evaluate(attValue);
    }
    else if(attName == "y")
    {
      y = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
  }

  x *= 0.5 * lunit;
  y *= 0.5 * lunit;
  z *= 0.5 * lunit;

  new G4Box(name, x, y, z);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::ConeRead(const xercesc::DOMElement* const coneElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double rmin1    = 0.0;
  G4double rmax1    = 0.0;
  G4double rmin2    = 0.0;
  G4double rmax2    = 0.0;
  G4double z        = 0.0;
  G4double startphi = 0.0;
  G4double deltaphi = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    coneElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::ConeRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::ConeRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::ConeRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "rmin1")
    {
      rmin1 = eval.Evaluate(attValue);
    }
    else if(attName == "rmax1")
    {
      rmax1 = eval.Evaluate(attValue);
    }
    else if(attName == "rmin2")
    {
      rmin2 = eval.Evaluate(attValue);
    }
    else if(attName == "rmax2")
    {
      rmax2 = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
  }

  rmin1 *= lunit;
  rmax1 *= lunit;
  rmin2 *= lunit;
  rmax2 *= lunit;
  z *= 0.5 * lunit;
  startphi *= aunit;
  deltaphi *= aunit;

  new G4Cons(name, rmin1, rmax1, rmin2, rmax2, z, startphi, deltaphi);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::ElconeRead(
  const xercesc::DOMElement* const elconeElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double dx    = 0.0;
  G4double dy    = 0.0;
  G4double zmax  = 0.0;
  G4double zcut  = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    elconeElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::ElconeRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::ElconeRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "dx")
    {
      dx = eval.Evaluate(attValue);
    }
    else if(attName == "dy")
    {
      dy = eval.Evaluate(attValue);
    }
    else if(attName == "zmax")
    {
      zmax = eval.Evaluate(attValue);
    }
    else if(attName == "zcut")
    {
      zcut = eval.Evaluate(attValue);
    }
  }

  zmax *= lunit;
  zcut *= lunit;

  new G4EllipticalCone(name, dx, dy, zmax, zcut);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::EllipsoidRead(
  const xercesc::DOMElement* const ellipsoidElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double ax    = 0.0;
  G4double by    = 0.0;
  G4double cz    = 0.0;
  G4double zcut1 = 0.0;
  G4double zcut2 = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    ellipsoidElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::EllipsoidRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::EllipsoidRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "ax")
    {
      ax = eval.Evaluate(attValue);
    }
    else if(attName == "by")
    {
      by = eval.Evaluate(attValue);
    }
    else if(attName == "cz")
    {
      cz = eval.Evaluate(attValue);
    }
    else if(attName == "zcut1")
    {
      zcut1 = eval.Evaluate(attValue);
    }
    else if(attName == "zcut2")
    {
      zcut2 = eval.Evaluate(attValue);
    }
  }

  ax *= lunit;
  by *= lunit;
  cz *= lunit;
  zcut1 *= lunit;
  zcut2 *= lunit;

  new G4Ellipsoid(name, ax, by, cz, zcut1, zcut2);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::EltubeRead(
  const xercesc::DOMElement* const eltubeElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double dx    = 0.0;
  G4double dy    = 0.0;
  G4double dz    = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    eltubeElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::EltubeRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::EltubeRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "dx")
    {
      dx = eval.Evaluate(attValue);
    }
    else if(attName == "dy")
    {
      dy = eval.Evaluate(attValue);
    }
    else if(attName == "dz")
    {
      dz = eval.Evaluate(attValue);
    }
  }

  dx *= lunit;
  dy *= lunit;
  dz *= lunit;

  new G4EllipticalTube(name, dx, dy, dz);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::XtruRead(const xercesc::DOMElement* const xtruElement)
{
  G4String name;
  G4double lunit = 1.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    xtruElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::XtruRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::XtruRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
  }

  std::vector<G4TwoVector> twoDimVertexList;
  std::vector<G4ExtrudedSolid::ZSection> sectionList;

  for(xercesc::DOMNode* iter = xtruElement->getFirstChild(); iter != nullptr;
      iter                   = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::XtruRead()", "InvalidRead", FatalException,
                  "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "twoDimVertex")
    {
      twoDimVertexList.push_back(TwoDimVertexRead(child, lunit));
    }
    else if(tag == "section")
    {
      sectionList.push_back(SectionRead(child, lunit));
    }
  }

  new G4ExtrudedSolid(name, twoDimVertexList, sectionList);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::HypeRead(const xercesc::DOMElement* const hypeElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double aunit = 1.0;
  G4double rmin  = 0.0;
  G4double rmax  = 0.0;
  G4double inst  = 0.0;
  G4double outst = 0.0;
  G4double z     = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    hypeElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::HypeRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::HypeRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::HypeRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "rmin")
    {
      rmin = eval.Evaluate(attValue);
    }
    else if(attName == "rmax")
    {
      rmax = eval.Evaluate(attValue);
    }
    else if(attName == "inst")
    {
      inst = eval.Evaluate(attValue);
    }
    else if(attName == "outst")
    {
      outst = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
  }

  rmin *= lunit;
  rmax *= lunit;
  inst *= aunit;
  outst *= aunit;
  z *= 0.5 * lunit;

  new G4Hype(name, rmin, rmax, inst, outst, z);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::MultiUnionNodeRead(
  const xercesc::DOMElement* const unionNodeElement,
  G4MultiUnion* const multiUnionSolid)
{
  G4String name;
  G4String solid;
  G4ThreeVector position(0.0, 0.0, 0.0);
  G4ThreeVector rotation(0.0, 0.0, 0.0);

  const xercesc::DOMNamedNodeMap* const attributes =
    unionNodeElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::MultiUnionNodeRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
  }

  for(xercesc::DOMNode* iter = unionNodeElement->getFirstChild();
                        iter != nullptr; iter = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::MultiUnionNodeRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "position")
    {
      VectorRead(child, position);
    }
    else if(tag == "rotation")
    {
      VectorRead(child, rotation);
    }
    else if(tag == "positionref")
    {
      position = GetPosition(GenerateName(RefRead(child)));
    }
    else if(tag == "rotationref")
    {
      rotation = GetRotation(GenerateName(RefRead(child)));
    }
    else if(tag == "solid")
    {
      solid = RefRead(child);
    }
    else
    {
      G4String error_msg = "Unknown tag in MultiUnion structure: " + tag;
      G4Exception("G4GDMLReadSolids::MultiUnionNodeRead()", "ReadError",
                  FatalException, error_msg);
    }
  }
  G4VSolid* solidNode = GetSolid(GenerateName(solid));
  G4Transform3D transform(GetRotationMatrix(rotation), position);
  multiUnionSolid->AddNode(*solidNode, transform);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::MultiUnionRead(
  const xercesc::DOMElement* const unionElement)
{
  G4String name;

  const xercesc::DOMNamedNodeMap* const attributes =
    unionElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::MultiUnionRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
  }

  G4MultiUnion* multiUnion = new G4MultiUnion(name);

  for(xercesc::DOMNode* iter = unionElement->getFirstChild(); iter != nullptr;
      iter                   = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::MultiUnionRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "multiUnionNode")
    {
      MultiUnionNodeRead(child, multiUnion);
    }
    else
    {
      G4String error_msg = "Unknown tag in MultiUnion structure: " + tag;
      G4Exception("G4GDMLReadSolids::MultiUnionRead()", "ReadError",
                  FatalException, error_msg);
    }
  }
  multiUnion->Voxelize();
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::OrbRead(const xercesc::DOMElement* const orbElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double r     = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    orbElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::OrbRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::OrbRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "r")
    {
      r = eval.Evaluate(attValue);
    }
  }

  r *= lunit;

  new G4Orb(name, r);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::ParaRead(const xercesc::DOMElement* const paraElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double aunit = 1.0;
  G4double x     = 0.0;
  G4double y     = 0.0;
  G4double z     = 0.0;
  G4double alpha = 0.0;
  G4double theta = 0.0;
  G4double phi   = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    paraElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::ParaRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::ParaRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::ParaRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "x")
    {
      x = eval.Evaluate(attValue);
    }
    else if(attName == "y")
    {
      y = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
    else if(attName == "alpha")
    {
      alpha = eval.Evaluate(attValue);
    }
    else if(attName == "theta")
    {
      theta = eval.Evaluate(attValue);
    }
    else if(attName == "phi")
    {
      phi = eval.Evaluate(attValue);
    }
  }

  x *= 0.5 * lunit;
  y *= 0.5 * lunit;
  z *= 0.5 * lunit;
  alpha *= aunit;
  theta *= aunit;
  phi *= aunit;

  new G4Para(name, x, y, z, alpha, theta, phi);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::ParaboloidRead(
  const xercesc::DOMElement* const paraElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double rlo   = 0.0;
  G4double rhi   = 0.0;
  G4double dz    = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    paraElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::ParaboloidRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::ParaboloidRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "rlo")
    {
      rlo = eval.Evaluate(attValue);
    }
    else if(attName == "rhi")
    {
      rhi = eval.Evaluate(attValue);
    }
    else if(attName == "dz")
    {
      dz = eval.Evaluate(attValue);
    }
  }

  rlo *= 1. * lunit;
  rhi *= 1. * lunit;
  dz *= 1. * lunit;

  new G4Paraboloid(name, dz, rlo, rhi);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::PolyconeRead(
  const xercesc::DOMElement* const polyconeElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double startphi = 0.0;
  G4double deltaphi = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    polyconeElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::PolyconeRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::PolyconeRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::PolyconeRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
  }

  startphi *= aunit;
  deltaphi *= aunit;

  std::vector<zplaneType> zplaneList;

  for(xercesc::DOMNode* iter = polyconeElement->getFirstChild(); iter!= nullptr;
      iter                   = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::PolyconeRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "zplane")
    {
      zplaneList.push_back(ZplaneRead(child));
    }
  }

  G4int numZPlanes = zplaneList.size();

  G4double* rmin_array = new G4double[numZPlanes];
  G4double* rmax_array = new G4double[numZPlanes];
  G4double* z_array    = new G4double[numZPlanes];

  for(G4int i = 0; i < numZPlanes; ++i)
  {
    rmin_array[i] = zplaneList[i].rmin * lunit;
    rmax_array[i] = zplaneList[i].rmax * lunit;
    z_array[i]    = zplaneList[i].z * lunit;
  }

  new G4Polycone(name, startphi, deltaphi, numZPlanes, z_array, rmin_array,
                 rmax_array);

  delete[] rmin_array;
  delete[] rmax_array;
  delete[] z_array;
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::GenericPolyconeRead(
  const xercesc::DOMElement* const polyconeElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double startphi = 0.0;
  G4double deltaphi = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    polyconeElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::GenericPolyconeRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::GenericPolyconeRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::GenericPolyconeRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
  }

  startphi *= aunit;
  deltaphi *= aunit;

  std::vector<rzPointType> rzPointList;

  for(xercesc::DOMNode* iter = polyconeElement->getFirstChild();
                        iter != nullptr; iter = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::GenericPolyconeRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "rzpoint")
    {
      rzPointList.push_back(RZPointRead(child));
    }
  }

  G4int numRZPoints = rzPointList.size();

  G4double* r_array = new G4double[numRZPoints];
  G4double* z_array = new G4double[numRZPoints];

  for(G4int i = 0; i < numRZPoints; ++i)
  {
    r_array[i] = rzPointList[i].r * lunit;
    z_array[i] = rzPointList[i].z * lunit;
  }
  new G4GenericPolycone(name, startphi, deltaphi, numRZPoints, r_array,
                        z_array);
  delete[] r_array;
  delete[] z_array;
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::PolyhedraRead(
  const xercesc::DOMElement* const polyhedraElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double startphi = 0.0;
  G4double deltaphi = 0.0;
  G4int numsides    = 0;

  const xercesc::DOMNamedNodeMap* const attributes =
    polyhedraElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::PolyhedraRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::PolyhedraRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::PolyhedraRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
    else if(attName == "numsides")
    {
      numsides = eval.EvaluateInteger(attValue);
    }
  }

  startphi *= aunit;
  deltaphi *= aunit;

  std::vector<zplaneType> zplaneList;

  for(xercesc::DOMNode* iter = polyhedraElement->getFirstChild();
                        iter != nullptr; iter = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::PolyhedraRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "zplane")
    {
      zplaneList.push_back(ZplaneRead(child));
    }
  }

  G4int numZPlanes = zplaneList.size();

  G4double* rmin_array = new G4double[numZPlanes];
  G4double* rmax_array = new G4double[numZPlanes];
  G4double* z_array    = new G4double[numZPlanes];

  for(G4int i = 0; i < numZPlanes; ++i)
  {
    rmin_array[i] = zplaneList[i].rmin * lunit;
    rmax_array[i] = zplaneList[i].rmax * lunit;
    z_array[i]    = zplaneList[i].z * lunit;
  }

  new G4Polyhedra(name, startphi, deltaphi, numsides, numZPlanes, z_array,
                  rmin_array, rmax_array);

  delete[] rmin_array;
  delete[] rmax_array;
  delete[] z_array;
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::GenericPolyhedraRead(
  const xercesc::DOMElement* const polyhedraElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double startphi = 0.0;
  G4double deltaphi = 0.0;
  G4int numsides    = 0;

  const xercesc::DOMNamedNodeMap* const attributes =
    polyhedraElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::GenericPolyhedraRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::GenericPolyhedraRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::GenericPolyhedraRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
    else if(attName == "numsides")
    {
      numsides = eval.EvaluateInteger(attValue);
    }
  }

  startphi *= aunit;
  deltaphi *= aunit;

  std::vector<rzPointType> rzpointList;

  for(xercesc::DOMNode* iter = polyhedraElement->getFirstChild();
                        iter != nullptr; iter = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::GenericPolyhedraRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "rzpoint")
    {
      rzpointList.push_back(RZPointRead(child));
    }
  }

  G4int numRZPoints = rzpointList.size();

  G4double* r_array = new G4double[numRZPoints];
  G4double* z_array = new G4double[numRZPoints];

  for(G4int i = 0; i < numRZPoints; ++i)
  {
    r_array[i] = rzpointList[i].r * lunit;
    z_array[i] = rzpointList[i].z * lunit;
  }

  new G4Polyhedra(name, startphi, deltaphi, numsides, numRZPoints, r_array,
                  z_array);

  delete[] r_array;
  delete[] z_array;
}

// --------------------------------------------------------------------
G4QuadrangularFacet* G4GDMLReadSolids::QuadrangularRead(
  const xercesc::DOMElement* const quadrangularElement)
{
  G4ThreeVector vertex1;
  G4ThreeVector vertex2;
  G4ThreeVector vertex3;
  G4ThreeVector vertex4;
  G4FacetVertexType type = ABSOLUTE;
  G4double lunit         = 1.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    quadrangularElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::QuadrangularRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return nullptr;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "vertex1")
    {
      vertex1 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "vertex2")
    {
      vertex2 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "vertex3")
    {
      vertex3 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "vertex4")
    {
      vertex4 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::QuadrangularRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "type")
    {
      if(attValue == "RELATIVE")
      {
        type = RELATIVE;
      }
    }
  }

  return new G4QuadrangularFacet(vertex1 * lunit, vertex2 * lunit,
                                 vertex3 * lunit, vertex4 * lunit, type);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::ReflectedSolidRead(
  const xercesc::DOMElement* const reflectedSolidElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double aunit = 1.0;
  G4String solid;
  G4ThreeVector scale(1.0, 1.0, 1.0);
  G4ThreeVector rotation;
  G4ThreeVector position;

  const xercesc::DOMNamedNodeMap* const attributes =
    reflectedSolidElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::ReflectedSolidRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::ReflectedSolidRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::ReflectedSolidRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "solid")
    {
      solid = GenerateName(attValue);
    }
    else if(attName == "sx")
    {
      scale.setX(eval.Evaluate(attValue));
    }
    else if(attName == "sy")
    {
      scale.setY(eval.Evaluate(attValue));
    }
    else if(attName == "sz")
    {
      scale.setZ(eval.Evaluate(attValue));
    }
    else if(attName == "rx")
    {
      rotation.setX(eval.Evaluate(attValue));
    }
    else if(attName == "ry")
    {
      rotation.setY(eval.Evaluate(attValue));
    }
    else if(attName == "rz")
    {
      rotation.setZ(eval.Evaluate(attValue));
    }
    else if(attName == "dx")
    {
      position.setX(eval.Evaluate(attValue));
    }
    else if(attName == "dy")
    {
      position.setY(eval.Evaluate(attValue));
    }
    else if(attName == "dz")
    {
      position.setZ(eval.Evaluate(attValue));
    }
  }

  rotation *= aunit;
  position *= lunit;

  G4Transform3D transform(GetRotationMatrix(rotation), position);
  transform = transform * G4Scale3D(scale.x(), scale.y(), scale.z());

  new G4ReflectedSolid(name, GetSolid(solid), transform);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::ScaledSolidRead(
  const xercesc::DOMElement* const scaledSolidElement)
{
  G4String name;
  G4VSolid* solid = nullptr;
  G4ThreeVector scale(1.0, 1.0, 1.0);

  const xercesc::DOMNamedNodeMap* const attributes =
    scaledSolidElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::ScaledSolidRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
  }

  for(xercesc::DOMNode* iter = scaledSolidElement->getFirstChild();
                        iter != nullptr; iter = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::ScaledSolidRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "solidref")
    {
      solid = GetSolid(GenerateName(RefRead(child)));
    }
    else if(tag == "scale")
    {
      VectorRead(child, scale);
    }
    else if(tag == "scaleref")
    {
      scale = GetScale(GenerateName(RefRead(child)));
    }
    else
    {
      G4String error_msg = "Unknown tag in scaled solid: " + tag;
      G4Exception("G4GDMLReadSolids::ScaledSolidRead()", "ReadError",
                  FatalException, error_msg);
      return;
    }
  }

  G4Scale3D transform = G4Scale3D(scale.x(), scale.y(), scale.z());

  new G4ScaledSolid(name, solid, transform);
}

// --------------------------------------------------------------------
G4ExtrudedSolid::ZSection G4GDMLReadSolids::SectionRead(
  const xercesc::DOMElement* const sectionElement, G4double lunit)
{
  G4double zPosition = 0.0;
  G4TwoVector Offset;
  G4double scalingFactor = 1.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    sectionElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::SectionRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return G4ExtrudedSolid::ZSection(zPosition, Offset, scalingFactor);
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "zPosition")
    {
      zPosition = eval.Evaluate(attValue) * lunit;
    }
    else if(attName == "xOffset")
    {
      Offset.setX(eval.Evaluate(attValue) * lunit);
    }
    else if(attName == "yOffset")
    {
      Offset.setY(eval.Evaluate(attValue) * lunit);
    }
    else if(attName == "scalingFactor")
    {
      scalingFactor = eval.Evaluate(attValue);
    }
  }

  return G4ExtrudedSolid::ZSection(zPosition, Offset, scalingFactor);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::SphereRead(
  const xercesc::DOMElement* const sphereElement)
{
  G4String name;
  G4double lunit      = 1.0;
  G4double aunit      = 1.0;
  G4double rmin       = 0.0;
  G4double rmax       = 0.0;
  G4double startphi   = 0.0;
  G4double deltaphi   = 0.0;
  G4double starttheta = 0.0;
  G4double deltatheta = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    sphereElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::SphereRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::SphereRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::SphereRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "rmin")
    {
      rmin = eval.Evaluate(attValue);
    }
    else if(attName == "rmax")
    {
      rmax = eval.Evaluate(attValue);
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
    else if(attName == "starttheta")
    {
      starttheta = eval.Evaluate(attValue);
    }
    else if(attName == "deltatheta")
    {
      deltatheta = eval.Evaluate(attValue);
    }
  }

  rmin *= lunit;
  rmax *= lunit;
  startphi *= aunit;
  deltaphi *= aunit;
  starttheta *= aunit;
  deltatheta *= aunit;

  new G4Sphere(name, rmin, rmax, startphi, deltaphi, starttheta, deltatheta);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TessellatedRead(
  const xercesc::DOMElement* const tessellatedElement)
{
  G4String name;

  const xercesc::DOMNamedNodeMap* const attributes =
    tessellatedElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TessellatedRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
  }

  G4TessellatedSolid* tessellated = new G4TessellatedSolid(name);

  for(xercesc::DOMNode* iter = tessellatedElement->getFirstChild();
                        iter != nullptr; iter = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TessellatedRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "triangular")
    {
      tessellated->AddFacet(TriangularRead(child));
    }
    else if(tag == "quadrangular")
    {
      tessellated->AddFacet(QuadrangularRead(child));
    }
  }

  tessellated->SetSolidClosed(true);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TetRead(const xercesc::DOMElement* const tetElement)
{
  G4String name;
  G4ThreeVector vertex1;
  G4ThreeVector vertex2;
  G4ThreeVector vertex3;
  G4ThreeVector vertex4;
  G4double lunit = 1.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    tetElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TetRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TetRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "vertex1")
    {
      vertex1 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "vertex2")
    {
      vertex2 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "vertex3")
    {
      vertex3 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "vertex4")
    {
      vertex4 = GetPosition(GenerateName(attValue));
    }
  }

  new G4Tet(name, vertex1 * lunit, vertex2 * lunit, vertex3 * lunit,
            vertex4 * lunit);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TorusRead(const xercesc::DOMElement* const torusElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double rmin     = 0.0;
  G4double rmax     = 0.0;
  G4double rtor     = 0.0;
  G4double startphi = 0.0;
  G4double deltaphi = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    torusElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TorusRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TorusRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::TorusRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "rmin")
    {
      rmin = eval.Evaluate(attValue);
    }
    else if(attName == "rmax")
    {
      rmax = eval.Evaluate(attValue);
    }
    else if(attName == "rtor")
    {
      rtor = eval.Evaluate(attValue);
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
  }

  rmin *= lunit;
  rmax *= lunit;
  rtor *= lunit;
  startphi *= aunit;
  deltaphi *= aunit;

  new G4Torus(name, rmin, rmax, rtor, startphi, deltaphi);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::GenTrapRead(
  const xercesc::DOMElement* const gtrapElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double dz    = 0.0;
  G4double v1x = 0.0, v1y = 0.0, v2x = 0.0, v2y = 0.0, v3x = 0.0, v3y = 0.0,
           v4x = 0.0, v4y = 0.0, v5x = 0.0, v5y = 0.0, v6x = 0.0, v6y = 0.0,
           v7x = 0.0, v7y = 0.0, v8x = 0.0, v8y = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    gtrapElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::GenTrapRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::GenTrapRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "dz")
    {
      dz = eval.Evaluate(attValue);
    }
    else if(attName == "v1x")
    {
      v1x = eval.Evaluate(attValue);
    }
    else if(attName == "v1y")
    {
      v1y = eval.Evaluate(attValue);
    }
    else if(attName == "v2x")
    {
      v2x = eval.Evaluate(attValue);
    }
    else if(attName == "v2y")
    {
      v2y = eval.Evaluate(attValue);
    }
    else if(attName == "v3x")
    {
      v3x = eval.Evaluate(attValue);
    }
    else if(attName == "v3y")
    {
      v3y = eval.Evaluate(attValue);
    }
    else if(attName == "v4x")
    {
      v4x = eval.Evaluate(attValue);
    }
    else if(attName == "v4y")
    {
      v4y = eval.Evaluate(attValue);
    }
    else if(attName == "v5x")
    {
      v5x = eval.Evaluate(attValue);
    }
    else if(attName == "v5y")
    {
      v5y = eval.Evaluate(attValue);
    }
    else if(attName == "v6x")
    {
      v6x = eval.Evaluate(attValue);
    }
    else if(attName == "v6y")
    {
      v6y = eval.Evaluate(attValue);
    }
    else if(attName == "v7x")
    {
      v7x = eval.Evaluate(attValue);
    }
    else if(attName == "v7y")
    {
      v7y = eval.Evaluate(attValue);
    }
    else if(attName == "v8x")
    {
      v8x = eval.Evaluate(attValue);
    }
    else if(attName == "v8y")
    {
      v8y = eval.Evaluate(attValue);
    }
  }

  dz *= lunit;
  std::vector<G4TwoVector> vertices;
  vertices.push_back(G4TwoVector(v1x * lunit, v1y * lunit));
  vertices.push_back(G4TwoVector(v2x * lunit, v2y * lunit));
  vertices.push_back(G4TwoVector(v3x * lunit, v3y * lunit));
  vertices.push_back(G4TwoVector(v4x * lunit, v4y * lunit));
  vertices.push_back(G4TwoVector(v5x * lunit, v5y * lunit));
  vertices.push_back(G4TwoVector(v6x * lunit, v6y * lunit));
  vertices.push_back(G4TwoVector(v7x * lunit, v7y * lunit));
  vertices.push_back(G4TwoVector(v8x * lunit, v8y * lunit));
  new G4GenericTrap(name, dz, vertices);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TrapRead(const xercesc::DOMElement* const trapElement)
{
  G4String name;
  G4double lunit  = 1.0;
  G4double aunit  = 1.0;
  G4double z      = 0.0;
  G4double theta  = 0.0;
  G4double phi    = 0.0;
  G4double y1     = 0.0;
  G4double x1     = 0.0;
  G4double x2     = 0.0;
  G4double alpha1 = 0.0;
  G4double y2     = 0.0;
  G4double x3     = 0.0;
  G4double x4     = 0.0;
  G4double alpha2 = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    trapElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TrapRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TrapRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::TrapRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
    else if(attName == "theta")
    {
      theta = eval.Evaluate(attValue);
    }
    else if(attName == "phi")
    {
      phi = eval.Evaluate(attValue);
    }
    else if(attName == "y1")
    {
      y1 = eval.Evaluate(attValue);
    }
    else if(attName == "x1")
    {
      x1 = eval.Evaluate(attValue);
    }
    else if(attName == "x2")
    {
      x2 = eval.Evaluate(attValue);
    }
    else if(attName == "alpha1")
    {
      alpha1 = eval.Evaluate(attValue);
    }
    else if(attName == "y2")
    {
      y2 = eval.Evaluate(attValue);
    }
    else if(attName == "x3")
    {
      x3 = eval.Evaluate(attValue);
    }
    else if(attName == "x4")
    {
      x4 = eval.Evaluate(attValue);
    }
    else if(attName == "alpha2")
    {
      alpha2 = eval.Evaluate(attValue);
    }
  }

  z *= 0.5 * lunit;
  theta *= aunit;
  phi *= aunit;
  y1 *= 0.5 * lunit;
  x1 *= 0.5 * lunit;
  x2 *= 0.5 * lunit;
  alpha1 *= aunit;
  y2 *= 0.5 * lunit;
  x3 *= 0.5 * lunit;
  x4 *= 0.5 * lunit;
  alpha2 *= aunit;

  new G4Trap(name, z, theta, phi, y1, x1, x2, alpha1, y2, x3, x4, alpha2);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TrdRead(const xercesc::DOMElement* const trdElement)
{
  G4String name;
  G4double lunit = 1.0;
  G4double x1    = 0.0;
  G4double x2    = 0.0;
  G4double y1    = 0.0;
  G4double y2    = 0.0;
  G4double z     = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    trdElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TrdRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TrdRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "x1")
    {
      x1 = eval.Evaluate(attValue);
    }
    else if(attName == "x2")
    {
      x2 = eval.Evaluate(attValue);
    }
    else if(attName == "y1")
    {
      y1 = eval.Evaluate(attValue);
    }
    else if(attName == "y2")
    {
      y2 = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
  }

  x1 *= 0.5 * lunit;
  x2 *= 0.5 * lunit;
  y1 *= 0.5 * lunit;
  y2 *= 0.5 * lunit;
  z *= 0.5 * lunit;

  new G4Trd(name, x1, x2, y1, y2, z);
}

// --------------------------------------------------------------------
G4TriangularFacet* G4GDMLReadSolids::TriangularRead(
  const xercesc::DOMElement* const triangularElement)
{
  G4ThreeVector vertex1;
  G4ThreeVector vertex2;
  G4ThreeVector vertex3;
  G4FacetVertexType type = ABSOLUTE;
  G4double lunit         = 1.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    triangularElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TriangularRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return nullptr;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "vertex1")
    {
      vertex1 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "vertex2")
    {
      vertex2 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "vertex3")
    {
      vertex3 = GetPosition(GenerateName(attValue));
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TriangularRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "type")
    {
      if(attValue == "RELATIVE")
      {
        type = RELATIVE;
      }
    }
  }

  return new G4TriangularFacet(vertex1 * lunit, vertex2 * lunit,
                               vertex3 * lunit, type);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TubeRead(const xercesc::DOMElement* const tubeElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double rmin     = 0.0;
  G4double rmax     = 0.0;
  G4double z        = 0.0;
  G4double startphi = 0.0;
  G4double deltaphi = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    tubeElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TubeRead()", "InvalidRead", FatalException,
                  "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TubeRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::TubeRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "rmin")
    {
      rmin = eval.Evaluate(attValue);
    }
    else if(attName == "rmax")
    {
      rmax = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
  }

  rmin *= lunit;
  rmax *= lunit;
  z *= 0.5 * lunit;
  startphi *= aunit;
  deltaphi *= aunit;

  new G4Tubs(name, rmin, rmax, z, startphi, deltaphi);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::CutTubeRead(
  const xercesc::DOMElement* const cuttubeElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double rmin     = 0.0;
  G4double rmax     = 0.0;
  G4double z        = 0.0;
  G4double startphi = 0.0;
  G4double deltaphi = 0.0;
  G4ThreeVector lowNorm(0);
  G4ThreeVector highNorm(0);

  const xercesc::DOMNamedNodeMap* const attributes =
    cuttubeElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::CutTubeRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::CutTubeRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::CutTubeRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "rmin")
    {
      rmin = eval.Evaluate(attValue);
    }
    else if(attName == "rmax")
    {
      rmax = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
    else if(attName == "startphi")
    {
      startphi = eval.Evaluate(attValue);
    }
    else if(attName == "deltaphi")
    {
      deltaphi = eval.Evaluate(attValue);
    }
    else if(attName == "lowX")
    {
      lowNorm.setX(eval.Evaluate(attValue));
    }
    else if(attName == "lowY")
    {
      lowNorm.setY(eval.Evaluate(attValue));
    }
    else if(attName == "lowZ")
    {
      lowNorm.setZ(eval.Evaluate(attValue));
    }
    else if(attName == "highX")
    {
      highNorm.setX(eval.Evaluate(attValue));
    }
    else if(attName == "highY")
    {
      highNorm.setY(eval.Evaluate(attValue));
    }
    else if(attName == "highZ")
    {
      highNorm.setZ(eval.Evaluate(attValue));
    }
  }

  rmin *= lunit;
  rmax *= lunit;
  z *= 0.5 * lunit;
  startphi *= aunit;
  deltaphi *= aunit;

  new G4CutTubs(name, rmin, rmax, z, startphi, deltaphi, lowNorm, highNorm);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TwistedboxRead(
  const xercesc::DOMElement* const twistedboxElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double PhiTwist = 0.0;
  G4double x        = 0.0;
  G4double y        = 0.0;
  G4double z        = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    twistedboxElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TwistedboxRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TwistedBoxRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::TwistedboxRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "PhiTwist")
    {
      PhiTwist = eval.Evaluate(attValue);
    }
    else if(attName == "x")
    {
      x = eval.Evaluate(attValue);
    }
    else if(attName == "y")
    {
      y = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
  }

  PhiTwist *= aunit;
  x *= 0.5 * lunit;
  y *= 0.5 * lunit;
  z *= 0.5 * lunit;

  new G4TwistedBox(name, PhiTwist, x, y, z);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TwistedtrapRead(
  const xercesc::DOMElement* const twistedtrapElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double PhiTwist = 0.0;
  G4double z        = 0.0;
  G4double Theta    = 0.0;
  G4double Phi      = 0.0;
  G4double y1       = 0.0;
  G4double x1       = 0.0;
  G4double x2       = 0.0;
  G4double y2       = 0.0;
  G4double x3       = 0.0;
  G4double x4       = 0.0;
  G4double Alph     = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    twistedtrapElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TwistedtrapRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TwistedtrapRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::TwistedtrapRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "PhiTwist")
    {
      PhiTwist = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
    else if(attName == "Theta")
    {
      Theta = eval.Evaluate(attValue);
    }
    else if(attName == "Phi")
    {
      Phi = eval.Evaluate(attValue);
    }
    else if(attName == "y1")
    {
      y1 = eval.Evaluate(attValue);
    }
    else if(attName == "x1")
    {
      x1 = eval.Evaluate(attValue);
    }
    else if(attName == "x2")
    {
      x2 = eval.Evaluate(attValue);
    }
    else if(attName == "y2")
    {
      y2 = eval.Evaluate(attValue);
    }
    else if(attName == "x3")
    {
      x3 = eval.Evaluate(attValue);
    }
    else if(attName == "x4")
    {
      x4 = eval.Evaluate(attValue);
    }
    else if(attName == "Alph")
    {
      Alph = eval.Evaluate(attValue);
    }
  }

  PhiTwist *= aunit;
  z *= 0.5 * lunit;
  Theta *= aunit;
  Phi *= aunit;
  Alph *= aunit;
  y1 *= 0.5 * lunit;
  x1 *= 0.5 * lunit;
  x2 *= 0.5 * lunit;
  y2 *= 0.5 * lunit;
  x3 *= 0.5 * lunit;
  x4 *= 0.5 * lunit;

  new G4TwistedTrap(name, PhiTwist, z, Theta, Phi, y1, x1, x2, y2, x3, x4,
                    Alph);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TwistedtrdRead(
  const xercesc::DOMElement* const twistedtrdElement)
{
  G4String name;
  G4double lunit    = 1.0;
  G4double aunit    = 1.0;
  G4double x1       = 0.0;
  G4double x2       = 0.0;
  G4double y1       = 0.0;
  G4double y2       = 0.0;
  G4double z        = 0.0;
  G4double PhiTwist = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    twistedtrdElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TwistedtrdRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TwistedtrdRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::TwistedtrdRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "x1")
    {
      x1 = eval.Evaluate(attValue);
    }
    else if(attName == "x2")
    {
      x2 = eval.Evaluate(attValue);
    }
    else if(attName == "y1")
    {
      y1 = eval.Evaluate(attValue);
    }
    else if(attName == "y2")
    {
      y2 = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      z = eval.Evaluate(attValue);
    }
    else if(attName == "PhiTwist")
    {
      PhiTwist = eval.Evaluate(attValue);
    }
  }

  x1 *= 0.5 * lunit;
  x2 *= 0.5 * lunit;
  y1 *= 0.5 * lunit;
  y2 *= 0.5 * lunit;
  z *= 0.5 * lunit;
  PhiTwist *= aunit;

  new G4TwistedTrd(name, x1, x2, y1, y2, z, PhiTwist);
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::TwistedtubsRead(
  const xercesc::DOMElement* const twistedtubsElement)
{
  G4String name;
  G4double lunit        = 1.0;
  G4double aunit        = 1.0;
  G4double twistedangle = 0.0;
  G4double endinnerrad  = 0.0;
  G4double endouterrad  = 0.0;
  G4double zlen         = 0.0;
  G4double phi          = 0.0;
  G4double totphi       = 0.0;
  G4double midinnerrad  = 0.0;
  G4double midouterrad  = 0.0;
  G4double positiveEndz = 0.0;
  G4double negativeEndz = 0.0;
  G4int nseg            = 0;

  const xercesc::DOMNamedNodeMap* const attributes =
    twistedtubsElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TwistedtubsRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "lunit")
    {
      lunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Length")
      {
        G4Exception("G4GDMLReadSolids::TwistedtubsRead()", "InvalidRead",
                    FatalException, "Invalid unit for length!");
      }
    }
    else if(attName == "aunit")
    {
      aunit = G4UnitDefinition::GetValueOf(attValue);
      if(G4UnitDefinition::GetCategory(attValue) != "Angle")
      {
        G4Exception("G4GDMLReadSolids::TwistedtubsRead()", "InvalidRead",
                    FatalException, "Invalid unit for angle!");
      }
    }
    else if(attName == "twistedangle")
    {
      twistedangle = eval.Evaluate(attValue);
    }
    else if(attName == "endinnerrad")
    {
      endinnerrad = eval.Evaluate(attValue);
    }
    else if(attName == "endouterrad")
    {
      endouterrad = eval.Evaluate(attValue);
    }
    else if(attName == "zlen")
    {
      zlen = eval.Evaluate(attValue);
    }
    else if(attName == "midinnerrad")
    {
      midinnerrad = eval.Evaluate(attValue);
    }
    else if(attName == "midouterrad")
    {
      midouterrad = eval.Evaluate(attValue);
    }
    else if(attName == "negativeEndz")
    {
      negativeEndz = eval.Evaluate(attValue);
    }
    else if(attName == "positiveEndz")
    {
      positiveEndz = eval.Evaluate(attValue);
    }
    else if(attName == "nseg")
    {
      nseg = eval.Evaluate(attValue);
    }
    else if(attName == "totphi")
    {
      totphi = eval.Evaluate(attValue);
    }
    else if(attName == "phi")
    {
      phi = eval.Evaluate(attValue);
    }
  }

  twistedangle *= aunit;
  endinnerrad *= lunit;
  endouterrad *= lunit;
  zlen *= 0.5 * lunit;
  midinnerrad *= lunit;
  midouterrad *= lunit;
  positiveEndz *= lunit;
  negativeEndz *= lunit;
  phi *= aunit;
  totphi *= aunit;

  if(zlen != 0.0)
  {
    if(nseg != 0)
      new G4TwistedTubs(name, twistedangle, endinnerrad, endouterrad, zlen,
                        nseg, totphi);
    else
      new G4TwistedTubs(name, twistedangle, endinnerrad, endouterrad, zlen,
                        phi);
  }
  else
  {
    if(nseg != 0)
      new G4TwistedTubs(name, twistedangle, midinnerrad, midouterrad,
                        negativeEndz, positiveEndz, nseg, totphi);
    else
      new G4TwistedTubs(name, twistedangle, midinnerrad, midouterrad,
                        negativeEndz, positiveEndz, phi);
  }
}

// --------------------------------------------------------------------
G4TwoVector G4GDMLReadSolids::TwoDimVertexRead(
  const xercesc::DOMElement* const element, G4double lunit)
{
  G4TwoVector vec;

  const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
  XMLSize_t attributeCount                         = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::TwoDimVertexRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return vec;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "x")
    {
      vec.setX(eval.Evaluate(attValue) * lunit);
    }
    else if(attName == "y")
    {
      vec.setY(eval.Evaluate(attValue) * lunit);
    }
  }

  return vec;
}

// --------------------------------------------------------------------
G4GDMLReadSolids::zplaneType G4GDMLReadSolids::ZplaneRead(
  const xercesc::DOMElement* const zplaneElement)
{
  zplaneType zplane = { 0., 0., 0. };

  const xercesc::DOMNamedNodeMap* const attributes =
    zplaneElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* node = attributes->item(attribute_index);

    if(node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::ZplaneRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return zplane;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "rmin")
    {
      zplane.rmin = eval.Evaluate(attValue);
    }
    else if(attName == "rmax")
    {
      zplane.rmax = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      zplane.z = eval.Evaluate(attValue);
    }
  }

  return zplane;
}

// --------------------------------------------------------------------
G4GDMLReadSolids::rzPointType G4GDMLReadSolids::RZPointRead(
  const xercesc::DOMElement* const zplaneElement)
{
  rzPointType rzpoint = { 0., 0. };

  const xercesc::DOMNamedNodeMap* const attributes =
    zplaneElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* node = attributes->item(attribute_index);

    if(node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::RZPointRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return rzpoint;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "r")
    {
      rzpoint.r = eval.Evaluate(attValue);
    }
    else if(attName == "z")
    {
      rzpoint.z = eval.Evaluate(attValue);
    }
  }

  return rzpoint;
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::PropertyRead(
  const xercesc::DOMElement* const propertyElement,
  G4OpticalSurface* opticalsurface)
{
  G4String name;
  G4String ref;
  G4GDMLMatrix matrix;

  const xercesc::DOMNamedNodeMap* const attributes =
    propertyElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::PropertyRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "ref")
    {
      matrix = GetMatrix(ref = attValue);
    }
  }

  /*
  if (matrix.GetCols() != 2)
  {
    G4String error_msg = "Referenced matrix '" + ref
           + "' should have \n two columns as a property table for
  opticalsurface: "
           + opticalsurface->GetName();
    G4Exception("G4GDMLReadSolids::PropertyRead()", "InvalidRead",
                FatalException, error_msg);
  }
  */

  if(matrix.GetRows() == 0)
  {
    return;
  }

  G4MaterialPropertiesTable* matprop =
    opticalsurface->GetMaterialPropertiesTable();
  if(matprop == nullptr)
  {
    matprop = new G4MaterialPropertiesTable();
    opticalsurface->SetMaterialPropertiesTable(matprop);
  }
  if(matrix.GetCols() == 1)  // constant property assumed
  {
    matprop->AddConstProperty(Strip(name), matrix.Get(0, 0), true);
  }
  else  // build the material properties vector
  {
    G4MaterialPropertyVector* propvect;
    G4String temp = name + ref;
    // first check if it was already built
    if(mapOfMatPropVects.find(temp) == mapOfMatPropVects.end())
    {
      // if not create a new one
      propvect = new G4MaterialPropertyVector();
      for(size_t i = 0; i < matrix.GetRows(); i++)
      {
        propvect->InsertValues(matrix.Get(i, 0), matrix.Get(i, 1));
      }
      // and add it to the list for potential future reuse
      mapOfMatPropVects[temp] = propvect;
    }
    else
    {
      propvect = mapOfMatPropVects[temp];
    }

    matprop->AddProperty(Strip(name), propvect, true);
  }
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::OpticalSurfaceRead(
  const xercesc::DOMElement* const opticalsurfaceElement)
{
  G4String name;
  G4String smodel;
  G4String sfinish;
  G4String stype;
  G4double value = 0.0;

  const xercesc::DOMNamedNodeMap* const attributes =
    opticalsurfaceElement->getAttributes();
  XMLSize_t attributeCount = attributes->getLength();

  for(XMLSize_t attribute_index = 0; attribute_index < attributeCount;
      ++attribute_index)
  {
    xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

    if(attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
    {
      continue;
    }

    const xercesc::DOMAttr* const attribute =
      dynamic_cast<xercesc::DOMAttr*>(attribute_node);
    if(attribute == nullptr)
    {
      G4Exception("G4GDMLReadSolids::OpticalSurfaceRead()", "InvalidRead",
                  FatalException, "No attribute found!");
      return;
    }
    const G4String attName  = Transcode(attribute->getName());
    const G4String attValue = Transcode(attribute->getValue());

    if(attName == "name")
    {
      name = GenerateName(attValue);
    }
    else if(attName == "model")
    {
      smodel = attValue;
    }
    else if(attName == "finish")
    {
      sfinish = attValue;
    }
    else if(attName == "type")
    {
      stype = attValue;
    }
    else if(attName == "value")
    {
      value = eval.Evaluate(attValue);
    }
  }

  G4OpticalSurfaceModel model;
  G4OpticalSurfaceFinish finish;
  G4SurfaceType type;

  if((smodel == "glisur") || (smodel == "0"))
  {
    model = glisur;
  }
  else if((smodel == "unified") || (smodel == "1"))
  {
    model = unified;
  }
  else if((smodel == "LUT") || (smodel == "2"))
  {
    model = LUT;
  }
  else if((smodel == "DAVIS") || (smodel == "3"))
  {
    model = DAVIS;
  }
  else
  {
    model = dichroic;
  }

  if((sfinish == "polished") || (sfinish == "0"))
  {
    finish = polished;
  }
  else if((sfinish == "polishedfrontpainted") || (sfinish == "1"))
  {
    finish = polishedfrontpainted;
  }
  else if((sfinish == "polishedbackpainted") || (sfinish == "2"))
  {
    finish = polishedbackpainted;
  }
  else if((sfinish == "ground") || (sfinish == "3"))
  {
    finish = ground;
  }
  else if((sfinish == "groundfrontpainted") || (sfinish == "4"))
  {
    finish = groundfrontpainted;
  }
  else if((sfinish == "groundbackpainted") || (sfinish == "5"))
  {
    finish = groundbackpainted;
  }
  else if((sfinish == "polishedlumirrorair") || (sfinish == "6"))
  {
    finish = polishedlumirrorair;
  }
  else if((sfinish == "polishedlumirrorglue") || (sfinish == "7"))
  {
    finish = polishedlumirrorglue;
  }
  else if((sfinish == "polishedair") || (sfinish == "8"))
  {
    finish = polishedair;
  }
  else if((sfinish == "polishedteflonair") || (sfinish == "9"))
  {
    finish = polishedteflonair;
  }
  else if((sfinish == "polishedtioair") || (sfinish == "10"))
  {
    finish = polishedtioair;
  }
  else if((sfinish == "polishedtyvekair") || (sfinish == "11"))
  {
    finish = polishedtyvekair;
  }
  else if((sfinish == "polishedvm2000air") || (sfinish == "12"))
  {
    finish = polishedvm2000air;
  }
  else if((sfinish == "polishedvm2000glue") || (sfinish == "13"))
  {
    finish = polishedvm2000glue;
  }
  else if((sfinish == "etchedlumirrorair") || (sfinish == "14"))
  {
    finish = etchedlumirrorair;
  }
  else if((sfinish == "etchedlumirrorglue") || (sfinish == "15"))
  {
    finish = etchedlumirrorglue;
  }
  else if((sfinish == "etchedair") || (sfinish == "16"))
  {
    finish = etchedair;
  }
  else if((sfinish == "etchedteflonair") || (sfinish == "17"))
  {
    finish = etchedteflonair;
  }
  else if((sfinish == "etchedtioair") || (sfinish == "18"))
  {
    finish = etchedtioair;
  }
  else if((sfinish == "etchedtyvekair") || (sfinish == "19"))
  {
    finish = etchedtyvekair;
  }
  else if((sfinish == "etchedvm2000air") || (sfinish == "20"))
  {
    finish = etchedvm2000air;
  }
  else if((sfinish == "etchedvm2000glue") || (sfinish == "21"))
  {
    finish = etchedvm2000glue;
  }
  else if((sfinish == "groundlumirrorair") || (sfinish == "22"))
  {
    finish = groundlumirrorair;
  }
  else if((sfinish == "groundlumirrorglue") || (sfinish == "23"))
  {
    finish = groundlumirrorglue;
  }
  else if((sfinish == "groundair") || (sfinish == "24"))
  {
    finish = groundair;
  }
  else if((sfinish == "groundteflonair") || (sfinish == "25"))
  {
    finish = groundteflonair;
  }
  else if((sfinish == "groundtioair") || (sfinish == "26"))
  {
    finish = groundtioair;
  }
  else if((sfinish == "groundtyvekair") || (sfinish == "27"))
  {
    finish = groundtyvekair;
  }
  else if((sfinish == "groundvm2000air") || (sfinish == "28"))
  {
    finish = groundvm2000air;
  }
  else if((sfinish == "groundvm2000glue") || (sfinish == "29"))
  {
    finish = groundvm2000glue;
  }
  else if((sfinish == "Rough_LUT") || (sfinish == "30"))
  {
    finish = Rough_LUT;
  }
  else if((sfinish == "RoughTeflon_LUT") || (sfinish == "31"))
  {
    finish = RoughTeflon_LUT;
  }
  else if((sfinish == "RoughESR_LUT") || (sfinish == "32"))
  {
    finish = RoughESR_LUT;
  }
  else if((sfinish == "RoughESRGrease_LUT") || (sfinish == "33"))
  {
    finish = RoughESRGrease_LUT;
  }
  else if((sfinish == "Polished_LUT") || (sfinish == "34"))
  {
    finish = Polished_LUT;
  }
  else if((sfinish == "PolishedTeflon_LUT") || (sfinish == "35"))
  {
    finish = PolishedTeflon_LUT;
  }
  else if((sfinish == "PolishedESR_LUT") || (sfinish == "36"))
  {
    finish = PolishedESR_LUT;
  }
  else if((sfinish == "PolishedESRGrease_LUT") || (sfinish == "37"))
  {
    finish = PolishedESRGrease_LUT;
  }
  else
  {
    finish = Detector_LUT;
  }

  if((stype == "dielectric_metal") || (stype == "0"))
  {
    type = dielectric_metal;
  }
  else if((stype == "dielectric_dielectric") || (stype == "1"))
  {
    type = dielectric_dielectric;
  }
  else if((stype == "dielectric_LUT") || (stype == "2"))
  {
    type = dielectric_LUT;
  }
  else if((stype == "dielectric_LUTDAVIS") || (stype == "3"))
  {
    type = dielectric_LUTDAVIS;
  }
  else if((stype == "dielectric_dichroic") || (stype == "4"))
  {
    type = dielectric_dichroic;
  }
  else if((stype == "firsov") || (stype == "5"))
  {
    type = firsov;
  }
  else
  {
    type = x_ray;
  }

  G4OpticalSurface* opticalsurface =
    new G4OpticalSurface(name, model, finish, type, value);

  for(xercesc::DOMNode* iter = opticalsurfaceElement->getFirstChild();
      iter != nullptr; iter = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::OpticalSurfaceRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());

    if(tag == "property")
    {
      PropertyRead(child, opticalsurface);
    }
  }
}

// --------------------------------------------------------------------
void G4GDMLReadSolids::SolidsRead(
  const xercesc::DOMElement* const solidsElement)
{
#ifdef G4VERBOSE
  G4cout << "G4GDML: Reading solids..." << G4endl;
#endif
  for(xercesc::DOMNode* iter = solidsElement->getFirstChild(); iter != nullptr;
      iter                   = iter->getNextSibling())
  {
    if(iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
    {
      continue;
    }

    const xercesc::DOMElement* const child =
      dynamic_cast<xercesc::DOMElement*>(iter);
    if(child == nullptr)
    {
      G4Exception("G4GDMLReadSolids::SolidsRead()", "InvalidRead",
                  FatalException, "No child found!");
      return;
    }
    const G4String tag = Transcode(child->getTagName());
    if(tag == "define")
    {
      DefineRead(child);
    }
    else if(tag == "box")
    {
      BoxRead(child);
    }
    else if(tag == "cone")
    {
      ConeRead(child);
    }
    else if(tag == "elcone")
    {
      ElconeRead(child);
    }
    else if(tag == "ellipsoid")
    {
      EllipsoidRead(child);
    }
    else if(tag == "eltube")
    {
      EltubeRead(child);
    }
    else if(tag == "xtru")
    {
      XtruRead(child);
    }
    else if(tag == "hype")
    {
      HypeRead(child);
    }
    else if(tag == "intersection")
    {
      BooleanRead(child, INTERSECTION);
    }
    else if(tag == "multiUnion")
    {
      MultiUnionRead(child);
    }
    else if(tag == "orb")
    {
      OrbRead(child);
    }
    else if(tag == "para")
    {
      ParaRead(child);
    }
    else if(tag == "paraboloid")
    {
      ParaboloidRead(child);
    }
    else if(tag == "polycone")
    {
      PolyconeRead(child);
    }
    else if(tag == "genericPolycone")
    {
      GenericPolyconeRead(child);
    }
    else if(tag == "polyhedra")
    {
      PolyhedraRead(child);
    }
    else if(tag == "genericPolyhedra")
    {
      GenericPolyhedraRead(child);
    }
    else if(tag == "reflectedSolid")
    {
      ReflectedSolidRead(child);
    }
    else if(tag == "scaledSolid")
    {
      ScaledSolidRead(child);
    }
    else if(tag == "sphere")
    {
      SphereRead(child);
    }
    else if(tag == "subtraction")
    {
      BooleanRead(child, SUBTRACTION);
    }
    else if(tag == "tessellated")
    {
      TessellatedRead(child);
    }
    else if(tag == "tet")
    {
      TetRead(child);
    }
    else if(tag == "torus")
    {
      TorusRead(child);
    }
    else if(tag == "arb8")
    {
      GenTrapRead(child);
    }
    else if(tag == "trap")
    {
      TrapRead(child);
    }
    else if(tag == "trd")
    {
      TrdRead(child);
    }
    else if(tag == "tube")
    {
      TubeRead(child);
    }
    else if(tag == "cutTube")
    {
      CutTubeRead(child);
    }
    else if(tag == "twistedbox")
    {
      TwistedboxRead(child);
    }
    else if(tag == "twistedtrap")
    {
      TwistedtrapRead(child);
    }
    else if(tag == "twistedtrd")
    {
      TwistedtrdRead(child);
    }
    else if(tag == "twistedtubs")
    {
      TwistedtubsRead(child);
    }
    else if(tag == "union")
    {
      BooleanRead(child, UNION);
    }
    else if(tag == "opticalsurface")
    {
      OpticalSurfaceRead(child);
    }
    else if(tag == "loop")
    {
      LoopRead(child, &G4GDMLRead::SolidsRead);
    }
    else
    {
      G4String error_msg = "Unknown tag in solids: " + tag;
      G4Exception("G4GDMLReadSolids::SolidsRead()", "ReadError", FatalException,
                  error_msg);
    }
  }
}

// --------------------------------------------------------------------
G4VSolid* G4GDMLReadSolids::GetSolid(const G4String& ref) const
{
  G4VSolid* solidPtr = G4SolidStore::GetInstance()
                                    ->GetSolid(ref,false,reverseSearch);

  if(solidPtr == nullptr)
  {
    G4String error_msg = "Referenced solid '" + ref + "' was not found!";
    G4Exception("G4GDMLReadSolids::GetSolid()", "ReadError", FatalException,
                error_msg);
  }

  return solidPtr;
}

// --------------------------------------------------------------------
G4SurfaceProperty* G4GDMLReadSolids::GetSurfaceProperty(
  const G4String& ref) const
{
  const G4SurfacePropertyTable* surfaceList =
    G4SurfaceProperty::GetSurfacePropertyTable();
  const std::size_t surfaceCount = surfaceList->size();

  for(std::size_t i = 0; i < surfaceCount; ++i)
  {
    if((*surfaceList)[i]->GetName() == ref)
    {
      return (*surfaceList)[i];
    }
  }

  G4String error_msg =
    "Referenced optical surface '" + ref + "' was not found!";
  G4Exception("G4GDMLReadSolids::GetSurfaceProperty()", "ReadError",
              FatalException, error_msg);

  return nullptr;
}
