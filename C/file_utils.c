#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "file_formats.h"

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
