#include "CommonHeader.h"

/*
# <<BEGIN-copyright>>
# <<END-copyright>>
*/
#ifndef MCGIDI_mass_h_included
#define MCGIDI_mass_h_included

#include "G4Types.hh"

#if defined __cplusplus
    extern "C" {
    namespace GIDI {
#endif
    
G4double MCGIDI_particleMass_AMU( statusMessageReporting *smr, const char *name );

#if defined __cplusplus
    }
    }
#endif

#endif          /* End of MCGIDI_mass_h_included. */
