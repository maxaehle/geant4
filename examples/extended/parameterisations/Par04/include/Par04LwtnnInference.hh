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

#ifdef USE_INFERENCE_LWTNN
#ifndef PAR04LWTNNINFERENCE_HH
#define PAR04LWTNNINFERENCE_HH

#include "Par04InferenceInterface.hh"
#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/parse_json.hh"
#include <fstream>

/**
 * @brief Inference using the LWTNN library.
 *
 * Initializes a computation object or a light weight graph using the ML model saved
 * as a JSON file.
 * Runs the inference by evaluating the graph on the input vector from Par04InferenceSetup.
 *
 **/

using namespace std;

class Par04LwtnnInference : public Par04InferenceInterface
{
 public:
  Par04LwtnnInference(G4String);
  Par04LwtnnInference();

  /// Run inference
  /// @param[in] aGenVector Input latent space and conditions
  /// @param[out] aEnergies Model output = generated shower energies
  /// @param[in] aSize Size of the output
  void RunInference(vector<G4float> aGenVector, std::vector<G4double>& aEnergies, int aSize);

 private:
  /// LWTNN graph , Network input and output vectors
  std::unique_ptr<lwt::LightweightGraph> fGraph;
  typedef std::map<std::string, std::map<std::string, G4double>> fNetworkInputs;
  typedef std::map<std::string, G4double> fNetworkOutputs;
};

#endif /* PAR04LWTNNINFERENCE_HH */
#endif
