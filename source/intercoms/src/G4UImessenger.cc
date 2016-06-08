// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4UImessenger.cc,v 1.2 1999/12/15 14:50:42 gunter Exp $
// GEANT4 tag $Name: geant4-03-01 $
//

#include "G4UImessenger.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include <string.h>
#include "G4ios.hh"

#include "g4std/strstream"


G4UImessenger::G4UImessenger() { }

G4UImessenger::~G4UImessenger() { }

G4String G4UImessenger::GetCurrentValue(G4UIcommand * command) 
{ 
  G4String nullString;
  return nullString;
}

void G4UImessenger::SetNewValue(G4UIcommand * command,G4String newValue) 
{ ; }

G4bool G4UImessenger::operator == (const G4UImessenger& messenger) const {
  return this == &messenger;
}

G4String G4UImessenger::ItoS(G4int i)
{
  char defVal[20];
  G4std::ostrstream os(defVal,20);
  os << i << '\0';
  return G4String(defVal);
}

G4String G4UImessenger::DtoS(G4double a)
{
  char defVal[40];
  G4std::ostrstream os(defVal,40);
  os << a << '\0';
  return G4String(defVal);
}

G4String G4UImessenger::BtoS(G4bool b)
{
  G4String vl = "0";
  if(b) vl = "true";
  return vl;
}

G4int G4UImessenger::StoI(G4String s)
{
  G4int vl;
  const char* t = s;
  G4std::istrstream is((char*)t);
  is >> vl;
  return vl;
}

G4double G4UImessenger::StoD(G4String s)
{
  G4double vl;
  const char* t = s;
  G4std::istrstream is((char*)t);
  is >> vl;
  return vl;
}

G4bool G4UImessenger::StoB(G4String s)
{
  G4String v = s;
  v.toUpper();
  G4bool vl = false;
  if( v=="Y" || v=="YES" || v=="1" || v=="T" || v=="TRUE" )
  { vl = true; }
  return vl;
}


void G4UImessenger::AddUIcommand(G4UIcommand * newCommand)
{
  G4cerr << "Warning : Old style definition of G4UIcommand <" 
         << newCommand->GetCommandPath() << ">." << G4endl;
}
