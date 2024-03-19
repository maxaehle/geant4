#include "CommonHeader.h"

/*
# <<BEGIN-copyright>>
# <<END-copyright>>
*/

#ifndef ptwX_h_included
#define ptwX_h_included

#include <stdio.h>
#include <stdint.h>

#include <nf_utilities.h>

#if defined __cplusplus
    extern "C" {
    namespace GIDI {
#endif

#define ptwX_minimumSize 10

enum ptwX_sort_order { ptwX_sort_order_descending, ptwX_sort_order_ascending };

typedef
    struct ptwXPoints_s {
        nfu_status status;
        int64_t length;
        int64_t allocatedSize;
        int64_t mallocFailedSize;
        G4double *points;
    } ptwXPoints;

/*
* Routines in ptwX_core.c
*/
ptwXPoints *ptwX_new( int64_t size, nfu_status *status );
nfu_status ptwX_setup( ptwXPoints *ptwX, int64_t size );
ptwXPoints *ptwX_create( int64_t size, int64_t length, G4double const *xs, nfu_status *status );
ptwXPoints *ptwX_createLine( int64_t size, int64_t length, G4double slope, G4double offset, nfu_status *status );
nfu_status ptwX_copy( ptwXPoints *dest, ptwXPoints *src );
ptwXPoints *ptwX_clone( ptwXPoints *ptwX, nfu_status *status );
ptwXPoints *ptwX_slice( ptwXPoints *ptwX, int64_t index1, int64_t index2, nfu_status *status );
nfu_status ptwX_reallocatePoints( ptwXPoints *ptwX, int64_t size, int forceSmallerResize );
nfu_status ptwX_clear( ptwXPoints *ptwX );
nfu_status ptwX_release( ptwXPoints *ptwX );
ptwXPoints *ptwX_free( ptwXPoints *ptwX );

int64_t ptwX_length( ptwXPoints *ptwX );
nfu_status ptwX_setData( ptwXPoints *ptwX, int64_t length, G4double const *xs );
nfu_status ptwX_deletePoints( ptwXPoints *ptwX, int64_t i1, int64_t i2 );
G4double *ptwX_getPointAtIndex( ptwXPoints *ptwX, int64_t index );
G4double ptwX_getPointAtIndex_Unsafely( ptwXPoints *ptwX, int64_t index );
nfu_status ptwX_setPointAtIndex( ptwXPoints *ptwX, int64_t index, G4double x );
nfu_status ptwX_insertPointsAtIndex( ptwXPoints *ptwX, int64_t index, int64_t n1, G4double const *xs );
int ptwX_ascendingOrder( ptwXPoints *ptwX );
ptwXPoints *ptwX_fromString( char const *str, char **endCharacter, nfu_status *status );
nfu_status ptwX_countOccurrences( ptwXPoints *ptwX, G4double value, int *count );
nfu_status ptwX_reverse( ptwXPoints *ptwX );
nfu_status ptwX_sort( ptwXPoints *ptwX, enum ptwX_sort_order order );
nfu_status ptwX_closesDifference( ptwXPoints *ptwX, G4double value, int64_t *index, G4double *difference );
nfu_status ptwX_closesDifferenceInRange( ptwXPoints *ptwX, int64_t i1, int64_t i2, G4double value, int64_t *index, G4double *difference );
ptwXPoints *ptwX_unique( ptwXPoints *ptwX, int order, nfu_status *status );

nfu_status ptwX_abs( ptwXPoints *ptwX );
nfu_status ptwX_neg( ptwXPoints *ptwX );
nfu_status ptwX_add_G4double( ptwXPoints *ptwX, G4double value );
nfu_status ptwX_mul_G4double( ptwXPoints *ptwX, G4double value );
nfu_status ptwX_slopeOffset( ptwXPoints *ptwX, G4double slope, G4double offset );
nfu_status ptwX_add_ptwX( ptwXPoints *ptwX1, ptwXPoints *ptwX2 );
nfu_status ptwX_sub_ptwX( ptwXPoints *ptwX1, ptwXPoints *ptwX2 );

nfu_status ptwX_xMinMax( ptwXPoints *ptwX, G4double *xMin, G4double *xMax );

nfu_status ptwX_compare( ptwXPoints *ptwX1, ptwXPoints *ptwX2, int *comparison );
int ptwX_close( ptwXPoints *ptwX1, ptwXPoints *ptwX2, int epsilonFactor, G4double epsilon, nfu_status *status );

/*
* Routines in ptwX_misc.c
*/
void ptwX_simpleWrite( ptwXPoints const *ptwX, FILE *f, char const *format );
void ptwX_simplePrint( ptwXPoints const *ptwX, char const *format );

#if defined __cplusplus
    }
    }
#endif

#endif          /* End of ptwX_h_included. */
