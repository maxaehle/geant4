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
 * =============================================================================
 *
 *       Filename:  CexmcEnergyDepositDigitizerMessenger.hh
 *
 *    Description:  energy deposit digitizer messenger
 *
 *        Version:  1.0
 *        Created:  29.11.2009 18:54:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_ENERGY_DEPOSIT_DIGITIZER_MESSENGER_HH
#define CEXMC_ENERGY_DEPOSIT_DIGITIZER_MESSENGER_HH

#include <G4UImessenger.hh>

class  CexmcEnergyDepositDigitizer;
class  G4UIcommand;
class  G4UIcmdWithADouble;
class  G4UIcmdWithADoubleAndUnit;
class  G4UIcmdWithAString;
class  G4UIcmdWithABool;
class  G4UIcmdWith3Vector;
class  G4UIcmdWithoutParameter;


class  CexmcEnergyDepositDigitizerMessenger : public G4UImessenger
{
    public:
        explicit CexmcEnergyDepositDigitizerMessenger(
                        CexmcEnergyDepositDigitizer *  energyDepositDigitiser );

        ~CexmcEnergyDepositDigitizerMessenger();

    public:
        void  SetNewValue( G4UIcommand *  cmd, G4String  value );

    private:
        CexmcEnergyDepositDigitizer *  energyDepositDigitizer;

        G4UIcmdWithADoubleAndUnit *    setMonitorThreshold;

        G4UIcmdWithADoubleAndUnit *    setVetoCountersThreshold;

        G4UIcmdWithADoubleAndUnit *    setLeftVetoCounterThreshold;

        G4UIcmdWithADoubleAndUnit *    setRightVetoCounterThreshold;

        G4UIcmdWithADoubleAndUnit *    setCalorimetersThreshold;

        G4UIcmdWithADoubleAndUnit *    setLeftCalorimeterThreshold;

        G4UIcmdWithADoubleAndUnit *    setRightCalorimeterThreshold;

        G4UIcmdWithAString *           setCalorimeterTriggerAlgorithm;

        G4UIcmdWithAString *           setOuterCrystalsVetoAlgorithm;

        G4UIcmdWithADouble *           setOuterCrystalsVetoFraction;

        G4UIcmdWithABool *             applyFiniteCrystalResolution;

        G4UIcmdWith3Vector *           addCrystalResolutionRange;

        G4UIcmdWithoutParameter *      clearCrystalResolutionData;
};


#endif

