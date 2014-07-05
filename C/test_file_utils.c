#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "file_formats.h"
#include "unit_test.h"

static char *result;
static char  tsbrom_file_name  [50];

static char *readTsbRom_ShouldReturnAPointerToATsbRomObject_GivenAFilename()
{
     assertNotNull( readTsbRom( tsbrom_file_name ) );

     return NULL;
}

static char *writeTsbRom_ShouldCreateATsbRom_GivenATsbRomObjectAndFilename()
{
     tsbrom_s tsbrom                  = { 0 };
     char     temp_file_name[999 + 1] = { 0 };

     sprintf( temp_file_name, "%s.tmp", tsbrom_file_name );

     assertEquals( bl_True, writeTsbRom( temp_file_name, &tsbrom ) );

     assertEquals( 0, access( temp_file_name, F_OK ) );

     unlink( temp_file_name );

     return NULL;
}

static char *lowercase_ShouldReturnACopyOfTheStringInAllLowercase_GivenAString()
{
     assertEqualsStr( "abcdefghijklmnopqrstuvwxyz", lowercase( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ) );

     return NULL;
}

static char *uppercase_ShouldReturnACopyOfTheStringInAllUppercase_GivenAString()
{
     assertEqualsStr( "ABCDEFGHIJKLMNOPQRSTUVWXYZ", uppercase( "abcdefghijklmnopqrstuvwxyz" ) );

     return NULL;
}

static char *pointer2int_ShouldReturnTheConvertedIntValue_GivenANesPointer()
{
     nes_pointer_s pointer1 = {    0,    0 };
     nes_pointer_s pointer2 = {   12,   32 };
     nes_pointer_s pointer3 = { 0x57, 0x84 };
     nes_pointer_s pointer4 = { 0x22, 0xfc };
     nes_pointer_s pointer5 = { 0xa3, 0xd8 };

     assertEquals( 0x0000, pointer2int( &pointer1 ) );
     assertEquals( 0x200c, pointer2int( &pointer2 ) );
     assertEquals( 0x8457, pointer2int( &pointer3 ) );
     assertEquals( 0xfc22, pointer2int( &pointer4 ) );
     assertEquals( 0xd8a3, pointer2int( &pointer5 ) );

     return NULL;
}

static char *int2pointer_ShouldSetTheNesPointerValue_GivenANesPointerAndAnInToConvert()
{
     nes_pointer_s pointer = { 0, 0 };

     int2pointer( 0xa852, &pointer );

     assertEquals( 0x52, pointer.value[0] );
     assertEquals( 0xa8, pointer.value[1] );

     return NULL;
}

static void check_file_utils_error()
{
     printf( "file utils error: %s\n", getFileUtilsError() );
}

static void run_all_tests()
{
     run_test( readTsbRom_ShouldReturnAPointerToATsbRomObject_GivenAFilename, check_file_utils_error );
     run_test( writeTsbRom_ShouldCreateATsbRom_GivenATsbRomObjectAndFilename, check_file_utils_error );

     run_test( lowercase_ShouldReturnACopyOfTheStringInAllLowercase_GivenAString, null );
     run_test( uppercase_ShouldReturnACopyOfTheStringInAllUppercase_GivenAString, null );

     run_test( pointer2int_ShouldReturnTheConvertedIntValue_GivenANesPointer,            null );
     run_test( int2pointer_ShouldSetTheNesPointerValue_GivenANesPointerAndAnInToConvert, null );
}


int main( int argc, char *argv[] )
{
     if ( argc < 2 )
     {
          printf( "Must supply a path.\n" );

          return EXIT_FAILURE;
     }

     sprintf( tsbrom_file_name, "%s/%s", argv[1], "tsb_test.nes" );

     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
