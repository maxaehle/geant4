#include "CommonHeader.h"

/*
# <<BEGIN-copyright>>
# <<END-copyright>>
*/
#ifndef PoPs_mass_h_included
#define PoPs_mass_h_included

#include <statusMessageReporting.h>

#if defined __cplusplus
    extern "C" {
    namespace GIDI {
#endif
    
G4double PoPs_particleMass_AMU( statusMessageReporting *smr, char const *name );

#if defined __cplusplus
    }
    }
#endif

#endif          /* End of PoPs_mass_h_included. */
