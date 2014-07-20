#include <stdio.h>
#include <stdlib.h>
#include "file_formats.h"
#include "save_state.h"

int main( const int argc, const char *argv[] )
{
     unsigned char *save_state  = NULL;
     const    char *source_file = NULL;
     const    char *dest_file   = NULL;
     /**/     int   ss_size     = 0;

     if ( argc < 3 )
     {
          printf( "Usage: %s <src> <dest>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     source_file = argv[1];
     dest_file   = argv[2];

     if ( (save_state = readNstSaveState( source_file, &ss_size )) == NULL )
     {
          printf( "Unable to load save state: %s\n", getFileUtilsError() );

          return EXIT_FAILURE;
     }

     if ( ! writeNstSaveState( dest_file, save_state, ss_size ) )
     {
          printf( "Unable to write save state file: %s\n", getFileUtilsError() );

          free( save_state );

          return EXIT_FAILURE;
     }

     free( save_state );

     return EXIT_SUCCESS;
}
