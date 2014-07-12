#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "file_formats.h"
#include "schedule.h"

static char error_message[999 + 1] = { 0 };


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}


static void *readFile( const char *filename, const size_t filesize )
{
     void *filedata;
     int   fd;

     clearErrorMessage();

     if ( (filedata = malloc( filesize )) == NULL )
     {
	  sprintf( error_message, "Cannot allocate memory for reading file <%s>.", filename );

	  return NULL;
     }

     if ( (fd = open( filename, O_RDONLY )) < 0 )
     {
	  sprintf( error_message, "Cannot open file <%s>: %s", filename, strerror(errno) );

	  free( filedata );

	  return NULL;
     }

     if ( read( fd, filedata, filesize ) < filesize )
     {
	  sprintf( error_message, "Unexpected end of file <%s>.", filename );

	  free( filedata );

	  if ( close( fd ) < 0 )
	  {
	       sprintf( error_message, "Error closing file <%s>: %s", filename, strerror(errno) );
	  }

	  return NULL;
     }

     if ( close( fd ) < 0 )
     {
	  sprintf( error_message, "Error closing file <%s>: %s", filename, strerror(errno) );

          free( filedata );

	  return NULL;
     }

     return filedata;
}


static boolean_e writeFile( const char *filename, const void *filedata, const size_t filesize )
{
     int bytes_written;
     int fd;

     clearErrorMessage();

     if ( (fd = creat( filename, S_IRUSR | S_IWUSR )) < 0 )
     {
	  sprintf( error_message, "Cannot open file <%s>: %s", filename, strerror(errno) );

	  return bl_False;
     }

     if ( (bytes_written = write( fd, filedata, filesize )) < filesize )
     {
          if ( bytes_written < 0 )
          {
               sprintf( error_message, "Error writing to output file: %s", strerror( errno ) );

               if ( close( fd ) < 0 )
               {
                    sprintf( error_message, "Error closing output file: %s", strerror(errno) );
               }

               return bl_False;
          }

          sprintf( error_message, "Warning: incomplete buffer written to output file!" );
     }

     if ( close( fd ) < 0 )
     {
	  sprintf( error_message, "Error closing file <%s>: %s", filename, strerror(errno) );

	  return bl_False;
     }

     return bl_True;
}


char *getFileUtilsError( void )
{
     return error_message;
}


tsbrom_s *readTsbRom( const char *filename )
{
     return readFile( filename, sizeof(tsbrom_s) );
}


boolean_e writeTsbRom( const char *filename, const tsbrom_s *tsbrom_file )
{
     return writeFile( filename, tsbrom_file, sizeof(tsbrom_s) );
}


char *lowercase( const char *s )
{
     static char buf[1024];

     int i;

     for ( i = 0; s[i] != '\0'; ++i )
     {
          buf[i] = tolower( s[i] );
     }

     buf[i] = '\0';

     return buf;
}

char *uppercase( const char *s )
{
     static char buf[1024];

     int i;

     for ( i = 0; s[i] != '\0'; ++i )
     {
          buf[i] = toupper( s[i] );
     }

     buf[i] = '\0';

     return buf;
}


int  pointer2int( const nes_pointer_s *ptr )
{
     return (ptr->value[1] << 8) + ptr->value[0];
}


void int2pointer( const int value, nes_pointer_s *ptr )
{
     ptr->value[0] = (value & 0x000000ff);
     ptr->value[1] = (value & 0x0000ff00)>>8;
}


int number2hex( const int number )
{
     if ( number > 99 ) return -1;

     return ((number / 10) << 4) + (number % 10);
}


int hex2number( const int hex )
{
     if ( hex > 0x99 ) return -1;

     int tens = hex >>    4;
     int ones = hex  & 0x0f;

     if ( ones > 9  ||  tens > 9 ) return -1;

     return (tens * 10) + ones;
}


static char *getTeamName( const int team_number )
{
     switch ( team_number )
     {
     case  0: return "Connecticut";
     case  1: return "Maine";
     case  2: return "Massachusetts";
     case  3: return "New Hampshire";
     case  4: return "Rhode Island";
     case  5: return "Vermont";
     case  6: return "Delaware";
     case  7: return "Maryland";
     case  8: return "New Jersey";
     case  9: return "New York";
     case 10: return "Virginia";
     case 11: return "West Virginia";
     case 12: return "Alabama";
     case 13: return "Florida";
     case 14: return "Georgia";
     case 15: return "North Carolina";
     case 16: return "South Carolina";
     case 17: return "Tennessee";
     case 18: return "Illinois";
     case 19: return "Indiana";
     case 20: return "Kentucky";
     case 21: return "Michigan";
     case 22: return "Ohio";
     case 23: return "Pennsylvania";
     case 24: return "Arizona";
     case 25: return "California";
     case 26: return "Colorado";
     case 27: return "Nevada";
     case 28: return "New Mexico";
     case 29: return "Utah";
     case 30: return "Idaho";
     case 31: return "Montana";
     case 32: return "Nebraska";
     case 33: return "Oregon";
     case 34: return "Washington";
     case 35: return "Wyoming";
     case 36: return "Iowa";
     case 37: return "Kansas";
     case 38: return "Minnesota";
     case 39: return "North Dakota";
     case 40: return "South Dakota";
     case 41: return "Wisconsin";
     case 42: return "Arkansas";
     case 43: return "Louisiana";
     case 44: return "Mississippi";
     case 45: return "Missouri";
     case 46: return "Oklahoma";
     case 47: return "Texas";
     }
}

boolean_e writeSchedule( const char *filename, const schedule_s *schedule )
{
     FILE *schedule_file;

     if ( (schedule_file = fopen( filename, "w+" )) == NULL )
     {
          sprintf( error_message, "Cannot open file <%s>: %s", filename, strerror(errno) );

          return bl_False;
     }

     fprintf( schedule_file, ",01,,02,,03,,04,,05,,06,,07,,08,,09,,10,,11,,12,,13,,14,,15,,16,,17,,18,,19,,20,,21,,22,,23,,24,\n" );

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               fprintf( schedule_file, ",%s,", getTeamName( games[j].road ) );
          }

          fprintf( schedule_file, "\n" );

          fprintf( schedule_file, "%d", i + 1 );

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               fprintf( schedule_file, ",%s,", getTeamName( games[j].home ) );
          }

          fprintf( schedule_file, "\n" );
          fprintf( schedule_file, ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\n" );
     }

     if ( fclose( schedule_file ) == EOF )
     {
          sprintf( error_message, "Error closing file pointed to by %x: %s", schedule_file, strerror(errno) );

          return bl_False;
     }

     return bl_True;
}
