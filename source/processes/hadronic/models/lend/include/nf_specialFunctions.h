/*
# <<BEGIN-copyright>>
# <<END-copyright>>
*/

#ifndef specialFunctions_h_included
#define specialFunctions_h_included

#include <math.h>
#include <G4float.h>
#include <nf_utilities.h>

#ifdef WIN32
#define isfinite _finite
#define M_PI 3.141592653589793238463
/*#define INFINITY (DBL_MAX+DBL_MAX)*/
#endif

#if defined __cplusplus
    extern "C" {
    namespace GIDI {
#endif

G4double nf_polevl( G4double x, G4double coef[], int N );
G4double nf_p1evl( G4double x, G4double coef[], int N );
G4double nf_exponentialIntegral( int n, G4double x, nfu_status *status );
G4double nf_gammaFunction( G4double x, nfu_status *status );
G4double nf_logGammaFunction( G4double x, nfu_status *status );
G4double nf_incompleteGammaFunction( G4double a, G4double x, nfu_status *status );
G4double nf_incompleteGammaFunctionComplementary( G4double a, G4double x, nfu_status *status );

G4double  nf_amc_log_factorial( int );
G4double  nf_amc_factorial( int );
G4double  nf_amc_wigner_3j( int, int, int, int, int, int );
G4double  nf_amc_wigner_6j( int, int, int, int, int, int );
G4double  nf_amc_wigner_9j( int, int, int, int, int, int, int, int, int );
G4double  nf_amc_racah( int, int, int, int, int, int );
G4double  nf_amc_clebsh_gordan( int, int, int, int, int );
G4double  nf_amc_z_coefficient( int, int, int, int, int, int );
G4double  nf_amc_zbar_coefficient( int, int, int, int, int, int );
G4double  nf_amc_reduced_matrix_element( int, int, int, int, int, int, int );

#if defined __cplusplus
    }
    }
#endif

#endif          /* End of ptwXY_h_included. */
