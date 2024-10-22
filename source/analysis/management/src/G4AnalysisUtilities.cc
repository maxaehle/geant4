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

// Author: Ivana Hrivnacova, 22/08/2013  (ivana@ipno.in2p3.fr)

#include "G4AnalysisUtilities.hh"
#include "G4BinScheme.hh"
#include "G4Exception.hh"
#include "G4UnitsTable.hh"
#include "G4String.hh"
#include "G4Threading.hh"
#include "G4Filesystem.hh"

using std::to_string;

namespace {

//_____________________________________________________________________________
G4bool GetToken(const G4String& line, G4String& token,
                std::string::size_type begIdx, std::string::size_type& endIdx)
{
  while ( line[begIdx] == ' ') ++begIdx; // Loop checking, 23.06.2015, I. Hrivnacova
  if ( line[begIdx] == '"' ) {
    endIdx = line.find('"', begIdx+1);
    if ( endIdx == std::string::npos ) endIdx = line.length();
    token = line.substr(begIdx+1, (endIdx-1)-begIdx);
    ++endIdx;
  }
  else {
    endIdx = line.find(' ', begIdx);
    if ( endIdx == std::string::npos ) endIdx = line.length();
    token = line.substr(begIdx, endIdx-begIdx);
  }
  return ( token.length() > 0 );
}

}

namespace G4Analysis
{

//_____________________________________________________________________________
void Warn(const G4String& message,
          const std::string_view inClass,
          const std::string_view inFunction)
{
  auto source = std::string(inClass) + "::" + std::string(inFunction);
  G4Exception(source.data(), "Analysis_W001", JustWarning, message);
}

//_____________________________________________________________________________
G4bool CheckNbins(G4int nbins)
{
  if ( nbins <= 0 ) {
    Warn("Illegal value of number of bins: nbins <= 0",
      kNamespaceName, "CheckNbins");
    return false;
  }
  else
    return true;
}


//_____________________________________________________________________________
G4bool CheckMinMax(G4double xmin, G4double xmax,
                   const G4String& fcnName, const G4String& binSchemeName)
{
  auto result = true;

  if ( xmax <= xmin ) {
    Warn("Illegal value of number of (xmin >= xmax)",
      kNamespaceName, "CheckMinMax");
    result = false;
  }

  if ( ( fcnName != "none" ) && ( binSchemeName != "linear" ) ) {
    Warn("Combining Function and Binning scheme is not supported.",
      kNamespaceName, "CheckMinMax");
    result = false;
  }

  if ( ( GetBinScheme(binSchemeName) == G4BinScheme::kLog ||
         fcnName == "log" || fcnName == "log10" ) && ( xmin == 0 ) ) {
    Warn("Illegal value of (xmin = 0) with logarithmic function or binning",
      kNamespaceName, "CheckMinMax");
    result = false;
  }

  return result;
}

//_____________________________________________________________________________
G4bool CheckEdges(const std::vector<G4double>& edges)
{
  if ( edges.size() <= 1 ) {
    Warn("Illegal edges vector (size <= 1)",
      kNamespaceName, "CheckEdges");
    return false;
  }
  else
    return true;

}

//_____________________________________________________________________________
G4bool CheckName(const G4String& name, const G4String& objectType)
{
  if ( ! name.size() ) {
    Warn("Empty " + objectType + " name is not allowed.\n" +
         objectType + " was not created.", kNamespaceName, "CheckEdges");
    return false;
  }
  else
    return true;
}

//_____________________________________________________________________________
G4double GetUnitValue(const G4String& unit)
{
   G4double value = 1.;
   if ( unit != "none" ) {
     value = G4UnitDefinition::GetValueOf(unit);
     if ( value == 0. ) value = 1.;
   }
   return value;
}

//_____________________________________________________________________________
void UpdateTitle(G4String& title,
                 const G4String& unitName,
                 const G4String& fcnName)
{
  if ( fcnName != "none" )  { title += " "; title += fcnName; title += "("; }
  if ( unitName != "none" ) { title += " ["; title += unitName; title += "]";}
  if ( fcnName != "none" )  { title += ")"; }
}

//_____________________________________________________________________________
void  Tokenize(const G4String& line, std::vector<G4String>& tokens)
{
  // Define start values
  std::string::size_type begIdx = 0;
  std::string::size_type endIdx = 0;
  G4String token;

  do {
    if ( GetToken(line, token, begIdx, endIdx) ) {
      //G4cout << "got token: '" << token << "'" << G4endl;
      //G4cout << "beg, end: " << begIdx << ", " << endIdx << G4endl;
      tokens.push_back(token);
    }
    begIdx = endIdx + 1;
  }
  while ( endIdx < line.length() ); // Loop checking, 23.06.2015, I. Hrivnacova
}

//_____________________________________________________________________________
G4AnalysisOutput GetOutput(const G4String& outputName, G4bool warn) {
  if      ( outputName == "csv"  )  { return G4AnalysisOutput::kCsv;  }
  else if ( outputName == "hdf5" )  { return G4AnalysisOutput::kHdf5; }
  else if ( outputName == "root" )  { return G4AnalysisOutput::kRoot; }
  else if ( outputName == "xml"  )  { return G4AnalysisOutput::kXml;  }
  else if ( outputName == "none" )  { return G4AnalysisOutput::kNone; }
  else {
    if (warn) {
      Warn("\"" + outputName + "\" output type is not supported.",
        kNamespaceName, "GetOutput");
    }
    return G4AnalysisOutput::kNone;
  }
}

//_____________________________________________________________________________
G4String GetOutputName(G4AnalysisOutput output) {
  switch ( output ) {
    case G4AnalysisOutput::kCsv:
      return "csv";
      break;
    case G4AnalysisOutput::kHdf5:
      return "hdf5";
      break;
    case G4AnalysisOutput::kRoot:
      return "root";
      break;
    case G4AnalysisOutput::kXml:
      return "xml";
      break;
    case G4AnalysisOutput::kNone:
      return "none";
      break;
  }
  // should never reach this line
  Warn("\"" + to_string(static_cast<int>(output)) +
       "\" output type is not supported.",
       kNamespaceName, "CheckOutputName");
  return "none";
}

//_____________________________________________________________________________
G4String GetBaseName(const G4String& fileName)
{
// Get file base name (without dot)

  G4fs::path filePath(fileName.data());
  if ( filePath.has_parent_path()) {
    return  filePath.parent_path().string() + "/" + filePath.stem().string();
  }

  return filePath.stem().string();
}

//_____________________________________________________________________________
G4String GetExtension(const G4String& fileName,
                      const G4String& defaultExtension)
{
// Get file base extension (without dot)
// If fileName is provided without extension, return defaultExtension

  G4fs::path filePath(fileName.data());
  if ( filePath.has_extension() ) {
    auto extension = filePath.extension().string();
    // remove "."
    return extension.substr(1, extension.length());
  }

  return defaultExtension;
}

//_____________________________________________________________________________
G4String GetHnFileName(
            const G4String& fileName,
            const G4String& fileType,
            const G4String& hnType,
            const G4String& hnName)
{
// Compose and return the histogram or profile specific file name:
// - add _hn_hnName suffix to the file base name
// - add file extension if not present

  auto name = GetBaseName(fileName);

  // Add _hnType_hnName
  name.append("_");
  name.append(hnType);
  name.append("_");
  name.append(hnName);

  // Add file extension
  auto extension = GetExtension(fileName, fileType);
  if ( extension.size() ) {
    name.append(".");
    name.append(extension);
  }

  return name;
}

//_____________________________________________________________________________
G4String GetNtupleFileName(
            const G4String& fileName,
            const G4String& fileType,
            const G4String& ntupleName)
{
// Compose and return the ntuple specific file name:
// - add _nt_ntupleName suffix to the file base name
// - add _tN suffix if called on thread worker
// - add file extension if not present

  auto name = GetBaseName(fileName);

  // Add ntupleName
  name.append("_nt_");
  name.append(ntupleName);

  // Add thread Id to a file name if MT processing
  if ( ! G4Threading::IsMasterThread() ) {
    std::ostringstream os;
    os << G4Threading::G4GetThreadId();
    name.append("_t");
    name.append(os.str());
  }

  // Add file extension
  auto extension = GetExtension(fileName, fileType);
  if ( extension.size() ) {
    name.append(".");
    name.append(extension);
  }

  return name;
}

//_____________________________________________________________________________
G4String GetNtupleFileName(
            const G4String& fileName,
            const G4String& fileType,
            G4int ntupleFileNumber)
{
// Compose and return the ntuple specific file name:
// - add _mFN suffix to the file base name where FN = ntupleFileNumber
// - add file extension if not present

  auto name = GetBaseName(fileName);

  // Add _M followed by ntupleFileNumber
  std::ostringstream os;
  os << ntupleFileNumber;
  name.append("_m");
  name.append(os.str());

  // Add file extension
  auto extension = GetExtension(fileName, fileType);
  if ( extension.size() ) {
    name.append(".");
    name.append(extension);
  }

  return name;
}

//_____________________________________________________________________________
G4String GetTnFileName(
            const G4String& fileName,
            const G4String& fileType)
{
// Update file base name with the thread suffix:
// - add _tN suffix if called on thread worker
// - add file extension if not present

  auto name = GetBaseName(fileName);

  // Add thread Id to a file name if MT processing
  if ( !  G4Threading::IsMasterThread() ) {
    std::ostringstream os;
    os << G4Threading::G4GetThreadId();
    name.append("_t");
    name.append(os.str());
  }

  // Add file extension
  auto extension = GetExtension(fileName, fileType);
  if ( extension.size() ) {
    name.append(".");
    name.append(extension);
  }

  return name;
}

//_____________________________________________________________________________
G4String GetPlotFileName(const G4String& fileName)
{
// Generate plot file name for an output file name

  auto name = GetBaseName(fileName);

  // Add .ps extension
  name.append(".ps");

  return name;
}

}
