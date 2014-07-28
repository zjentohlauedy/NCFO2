#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "file_formats.h"
#include "save_state.h"
#include "schedule.h"
#include "unit_test.h"

static char *result;
static char  tsbrom_file_name        [50];
static char  schedule_file_name      [50];
static char  save_state_file_name    [50];
static char  compressed_ss_file_name [50];

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

static char *number2hex_ShouldReturnTheHexValueEquivalent_GivenANumber()
{
     assertEquals( 0x01, number2hex(  1 ) );
     assertEquals( 0x32, number2hex( 32 ) );
     assertEquals( 0x57, number2hex( 57 ) );
     assertEquals( 0x64, number2hex( 64 ) );
     assertEquals( 0x80, number2hex( 80 ) );
     assertEquals( 0x99, number2hex( 99 ) );

     return NULL;
}

static char *number2hex_ShouldReturnLessThanZero_GivenAnInvalidNumber()
{
     assertEquals( -1, number2hex( 100 ) );

     return NULL;
}

static char *hex2number_ShouldReturnTheNumberValue_GivenAHexValue()
{
     assertEquals(  1, hex2number( 0x01 ) );
     assertEquals( 32, hex2number( 0x32 ) );
     assertEquals( 57, hex2number( 0x57 ) );
     assertEquals( 64, hex2number( 0x64 ) );
     assertEquals( 80, hex2number( 0x80 ) );
     assertEquals( 99, hex2number( 0x99 ) );

     return NULL;
}

static char *writeSchedule_ShouldCreateAScheduleCSVFile_GivenAScheduleAndAFilename()
{
     schedule_s *schedule               = generateSchedule( NULL, NULL );
     char       temp_file_name[999 + 1] = { 0 };

     sprintf( temp_file_name, "%s.tmp", schedule_file_name );

     assertEquals( bl_True, writeSchedule( temp_file_name, schedule ) );

     assertEquals( 0, access( temp_file_name, F_OK ) );

     unlink( temp_file_name );

     return NULL;
}

static char *readNstSaveState_ShouldReturnAPointerToABufferContainingTheSaveState_GivenAFilename()
{
     int len;

     assertNotNull( readNstSaveState( save_state_file_name, &len ) );

     assertEquals( 13165, len );

     return NULL;
}

static char *readNstSaveState_ShouldUncompressASaveStateThatIsCompressed_GivenAFilenameContainingCompressedSaveState()
{
     int len;

     assertNotNull( readNstSaveState( compressed_ss_file_name, &len ) );

     assertEquals( 13165, len );

     return NULL;
}

static char *writeNstSaveState_ShouldCreateASaveState_GivenASaveStateALengthAndFilename()
{
     unsigned char save_state[13165];
     /**/     char temp_file_name[999 + 1] = { 0 };

     sprintf( temp_file_name, "%s.tmp", save_state_file_name );

     assertEquals( bl_True, writeNstSaveState( temp_file_name, save_state, 13165 ) );

     assertEquals( 0, access( temp_file_name, F_OK ) );

     unlink( temp_file_name );

     return NULL;
}

static char *getSaveStateStats_ShouldReturnANstSaveState_GivenASaveStateBuffer()
{
     int len;

     unsigned char *save_state = readNstSaveState( save_state_file_name, &len );

     assertNotNull( save_state );

     nst_save_state_s *nst_save_state = getSaveStateStats( save_state, len );

     assertNotNull( nst_save_state );

     assertEquals( 0x2000, sizeof(nst_save_state_s) );

     free( save_state );

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

     run_test( number2hex_ShouldReturnTheHexValueEquivalent_GivenANumber, null );
     run_test( number2hex_ShouldReturnLessThanZero_GivenAnInvalidNumber,  null );
     run_test( hex2number_ShouldReturnTheNumberValue_GivenAHexValue,      null );

     run_test( writeSchedule_ShouldCreateAScheduleCSVFile_GivenAScheduleAndAFilename, check_file_utils_error );

     run_test( readNstSaveState_ShouldReturnAPointerToABufferContainingTheSaveState_GivenAFilename,                     check_file_utils_error );
     run_test( readNstSaveState_ShouldUncompressASaveStateThatIsCompressed_GivenAFilenameContainingCompressedSaveState, check_file_utils_error );
     run_test( writeNstSaveState_ShouldCreateASaveState_GivenASaveStateALengthAndFilename,                              check_file_utils_error );
     run_test( getSaveStateStats_ShouldReturnANstSaveState_GivenASaveStateBuffer,                                       check_file_utils_error );
}


int main( int argc, char *argv[] )
{
     if ( argc < 2 )
     {
          printf( "Must supply a path.\n" );

          return EXIT_FAILURE;
     }

     sprintf( tsbrom_file_name,        "%s/%s", argv[1], "tsb_test.nes"            );
     sprintf( schedule_file_name,      "%s/%s", argv[1], "tsb_schedule.csv"        );
     sprintf( save_state_file_name,    "%s/%s", argv[1], "tsb_test.nst"            );
     sprintf( compressed_ss_file_name, "%s/%s", argv[1], "tsb_test_compressed.nst" );

     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
