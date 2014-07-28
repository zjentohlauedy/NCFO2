#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>
#include "file_formats.h"
#include "save_state.h"
#include "schedule.h"

#define MEMCMP( A, OP, B, L ) (memcmp( (A), (B), (L) ) OP 0)


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


int dword2int( const unsigned char *dword )
{
     return (dword[3] << 24) + (dword[2] << 16) + (dword[1] << 8) + dword[0];
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

     clearErrorMessage();

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


static char *zerror( const int error )
{
     switch ( error )
     {
     case Z_OK:         return "Success";
     case Z_MEM_ERROR:  return "Not enough memory";
     case Z_BUF_ERROR:  return "Destination buffer not large enough to inflated data";
     case Z_DATA_ERROR: return "Data cannot be uncompressed possibly corrupted or incomplete";
     default:           return "Unknown error";
     }
}

static boolean_e loadSaveState( unsigned char *save_state, int *save_state_size, const unsigned char *data, const size_t datasize )
{
     const unsigned char      *p_data             = data;
     /**/  unsigned char      *p_save_state       = save_state;
     const unsigned char      *section_header     = NULL;
     /**/           int        section_length     = 0;
     /**/           boolean_e  inside_img_section = bl_False;

     while ( (p_data - data) < datasize )
     {
          section_header = p_data;
          section_length = dword2int( p_data + 4 );

          memcpy( p_save_state, p_data, 8 );

          p_data       += 8;
          p_save_state += 8;

          if      ( MEMCMP( section_header, ==, "NST\x1A", 4 ) )
          {
               // do nothing - file header
          }
          else if ( MEMCMP( section_header, ==, "CPU", 3 ) )
          {
               // do nothing - parent section header
               inside_img_section = bl_False;
          }
          else if ( MEMCMP( section_header, ==, "APU", 3 ) )
          {
               // do nothing - parent section header
               inside_img_section = bl_False;
          }
          else if ( MEMCMP( section_header, ==, "PPU", 3 ) )
          {
               // do nothing - parent section header
               inside_img_section = bl_False;
          }
          else if ( MEMCMP( section_header, ==, "IMG", 3 ) )
          {
               // do nothing - parent section header
               inside_img_section = bl_True;
          }
          else if ( MEMCMP( section_header, ==, "SQ0", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "SQ1", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "TRI", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "NOI", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "DMC", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "MPR", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "PRG", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "CHR", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "WRK", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "POW", 3 ) )
          {
               // do nothing - does not contain data - seems to be just a trigger
          }
          else if ( MEMCMP( section_header, ==, "NFO", 3 ) )
          {
               inside_img_section = bl_False;

               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "REG", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "FRM", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "CLK", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "IRQ", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "EXT", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "LEN", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "ENV", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "VSS", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "ACC", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "BNK", 3 ) )
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "PRT", 3 ) )
          {
               inside_img_section = bl_False;

               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
          else if ( MEMCMP( section_header, ==, "RAM", 3 ) )
          {
               uLongf dest_len = 50000 - (p_save_state - save_state);
               int    ret;

               if ( *p_data )
               {
                    *p_save_state = *p_data;
                    p_save_state++;

                    if ( (ret = uncompress( p_save_state, &dest_len, p_data + 1, section_length - 1)) != Z_OK )
                    {
                         sprintf( error_message, "Cannot uncompress RAM data: %s", zerror(ret) );

                         return bl_False;
                    }

                    p_data       += section_length;
                    p_save_state += dest_len;
               }
               else
               {
                    p_data       += section_length;
                    p_save_state += section_length;
               }
          }
          else if ( MEMCMP( section_header, ==, "PAL", 3 ) )
          {
               uLongf dest_len = 50000 - (p_save_state - save_state);
               int    ret;

               if ( *p_data )
               {
                    *p_save_state = *p_data;
                    p_save_state++;

                    if ( (ret = uncompress( p_save_state, &dest_len, p_data + 1, section_length - 1)) != Z_OK )
                    {
                         sprintf( error_message, "Cannot uncompress PAL data: %s", zerror(ret) );

                         return bl_False;
                    }

                    p_data       += section_length;
                    p_save_state += dest_len;
               }
               else
               {
                    p_data       += section_length;
                    p_save_state += section_length;
               }
          }
          else if ( MEMCMP( section_header, ==, "OAM", 3 ) )
          {
               uLongf dest_len = 50000 - (p_save_state - save_state);
               int    ret;

               if ( *p_data )
               {
                    *p_save_state = *p_data;
                    p_save_state++;

                    if ( (ret = uncompress( p_save_state, &dest_len, p_data + 1, section_length - 1)) != Z_OK )
                    {
                         sprintf( error_message, "Cannot uncompress OAM data: %s", zerror(ret) );

                         return bl_False;
                    }

                    p_data       += section_length;
                    p_save_state += dest_len;
               }
               else
               {
                    p_data       += section_length;
                    p_save_state += section_length;
               }
          }
          else if ( MEMCMP( section_header, ==, "NMT", 3 ) )
          {
               //********************* MAY BE A PARENT SECTION HEADER OR DATA HEADER ********************
               if ( ! inside_img_section )
               {
                    uLongf dest_len = 50000 - (p_save_state - save_state);
                    int    ret;

                    if ( *p_data )
                    {
                         *p_save_state = *p_data;
                         p_save_state++;

                         if ( (ret = uncompress( p_save_state, &dest_len, p_data + 1, section_length - 1)) != Z_OK )
                         {
                              sprintf( error_message, "Cannot uncompress NMT data: %s", zerror(ret) );

                              return bl_False;
                         }

                         p_data       += section_length;
                         p_save_state += dest_len;
                    }
                    else
                    {
                         p_data       += section_length;
                         p_save_state += section_length;
                    }
               }
          }
          else if ( MEMCMP( section_header, ==, "WRM", 3 ) )
          {
               uLongf dest_len = 50000 - (p_save_state - save_state);
               int    ret;

               if ( *p_data )
               {
                    *p_save_state = *p_data;
                    p_save_state++;

                    if ( (ret = uncompress( p_save_state, &dest_len, p_data + 1, section_length - 1)) != Z_OK )
                    {
                         sprintf( error_message, "Cannot uncompress WRM data: %s", zerror(ret) );

                         return bl_False;
                    }

                    p_data       += section_length;
                    p_save_state += dest_len;
               }
               else
               {
                    p_data       += section_length;
                    p_save_state += section_length;
               }
          }
          else if ( MEMCMP( section_header, ==, "VRM", 3 ) )
          {
               uLongf dest_len = 50000 - (p_save_state - save_state);
               int    ret;

               if ( *p_data )
               {
                    *p_save_state = *p_data;
                    p_save_state++;

                    if ( (ret = uncompress( p_save_state, &dest_len, p_data + 1, section_length - 1)) != Z_OK )
                    {
                         sprintf( error_message, "Cannot uncompress VRM data: %s", zerror(ret) );

                         return bl_False;
                    }

                    p_data       += section_length;
                    p_save_state += dest_len;
               }
               else
               {
                    p_data       += section_length;
                    p_save_state += dest_len;
               }
          }
          else
          {
               memcpy( p_save_state, p_data, section_length );

               p_data       += section_length;
               p_save_state += section_length;
          }
     }

     *save_state_size = p_save_state - save_state;
}

unsigned char *readNstSaveState( const char *filename, int *save_state_size )
{
     unsigned char *save_state = NULL;
     unsigned char *filedata   = NULL;
     struct   stat  filestats  = { 0 };
     /**/     int   filesize   = 0;

     clearErrorMessage();

     if ( stat( filename, &filestats ) < 0 )
     {
          sprintf( error_message, "Cannot stat file <%s>: %s", filename, strerror(errno) );

          return NULL;
     }

     filesize = filestats.st_size;

     if ( (filedata = readFile( filename, filesize )) == NULL ) return NULL;

     if ( (save_state = malloc( 50000 )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for save state data" );

          free( filedata );

          return NULL;
     }

     if ( ! loadSaveState( save_state, save_state_size, filedata, filesize ) )
     {
          free( filedata   );
          free( save_state );

          return NULL;
     }

     free( filedata );

     return save_state;
}

boolean_e writeNstSaveState( const char *filename, const unsigned char *save_state, const size_t size )
{
     return writeFile( filename, save_state, size );
}


nst_save_state_s *getSaveStateStats( const unsigned char *save_state, const size_t length )
{
     if ( save_state == NULL ) return NULL;

     // NST -> IMG -> MPR -> WRM
     const unsigned char *p_state        = save_state;
     const unsigned char *section_header = NULL;
     /**/           int   section_length = 0;

     while ( (p_state - save_state) <= length )
     {
          section_header = p_state;
          section_length = dword2int( p_state + 4 );

          p_state += 8;

          if      ( MEMCMP( section_header, ==, "NST\x1A", 4 ) )
          {
               // do nothing - file header
          }
          else if ( MEMCMP( section_header, ==, "IMG", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "MPR", 3 ) )
          {
               // do nothing - parent section header
          }
          else if ( MEMCMP( section_header, ==, "WRM", 3 ) )
          {
               return (nst_save_state_s *)p_state + 1; // skip the single byte compression flag
          }
          else
          {
               p_state += section_length;
          }
     }

     return NULL;
}
