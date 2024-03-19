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
# <<BEGIN-copyright>>
# <<END-copyright>>
*/
#ifndef G4GIDI_target_h_included
#define G4GIDI_target_h_included 1

#include <vector>
#include <string>

//using namespace std;

#include <statusMessageReporting.h>

#include <MCGIDI.h>

typedef struct crossSectionData_s crossSectionData;
typedef struct G4GIDI_Product_s G4GIDI_Product;

struct crossSectionData_s {
    int start, end;
    std::vector<G4double> crossSection;
};

#define channelID std::string

struct G4GIDI_Product_s {
    int A, Z, m;
    G4double kineticEnergy, px, py, pz;
    G4double birthTimeSec;
};

class G4GIDI_target {

    public:
        void init( const char *fileName );
        std::string equalProbableBinSampleMethod;
        int nElasticIndices, nCaptureIndices, nFissionIndices, nOthersIndices;
        int *elasticIndices, *captureIndices, *fissionIndices, *othersIndices;

    public:
        GIDI::statusMessageReporting smr;
        int projectilesPOPID;
        std::string name;
        std::string sourceFilename;
        G4double mass;
        GIDI::MCGIDI_target *target;

        G4GIDI_target( const char *fileName );
        G4GIDI_target( std::string const &fileName );       
        ~G4GIDI_target( );

        std::string *getName( void );
        std::string *getFilename( void );
        int getZ( void );
        int getA( void );
        int getM( void );
        G4double getMass( void );
        int getTemperatures( G4double *temperatures );
        int readTemperature( int index );
        std::string getEqualProbableBinSampleMethod( void );
        int setEqualProbableBinSampleMethod( std::string method );

        int getNumberOfChannels( void );
        int getNumberOfProductionChannels( void );
        channelID getChannelsID( int channelIndex );
        std::vector<channelID> *getChannelIDs( void );
        std::vector<channelID> *getProductionChannelIDs( void );

        std::vector<G4double> *getEnergyGridAtTIndex( int index );

        G4double getTotalCrossSectionAtE( G4double e_in, G4double temperature );
        G4double getElasticCrossSectionAtE( G4double e_in, G4double temperature );
        G4double getCaptureCrossSectionAtE( G4double e_in, G4double temperature );
        G4double getFissionCrossSectionAtE( G4double e_in, G4double temperature );
        G4double getOthersCrossSectionAtE( G4double e_in, G4double temperature );
        G4double sumChannelCrossSectionAtE( int nIndices, int *indices, G4double e_in, G4double temperature );
        int sampleChannelCrossSectionAtE( int nIndices, int *indices, G4double e_in, G4double temperature, G4double (*rng)( void * ), void *rngState );

        G4double getElasticFinalState( G4double e_in, G4double temperature, G4double (*rng)( void * ), void *rngState );
        std::vector<G4GIDI_Product> *getCaptureFinalState( G4double e_in, G4double temperature, G4double (*rng)( void * ), void *rngState );
        std::vector<G4GIDI_Product> *getFissionFinalState( G4double e_in, G4double temperature, G4double (*rng)( void * ), void *rngState );
        std::vector<G4GIDI_Product> *getOthersFinalState( G4double e_in, G4double temperature, G4double (*rng)( void * ), void *rngState );
        std::vector<G4GIDI_Product> *getFinalState( int nIndices, int *indices, G4double e_in, G4double temperature, G4double (*rng)( void * ), void *rngState );

        G4double getReactionsThreshold( int index );
        G4double getReactionsDomain( int index, G4double *EMin, G4double *EMax );
};

#endif      // End of G4GIDI_target_h_included
