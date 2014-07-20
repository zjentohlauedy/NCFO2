#ifndef _INC_SAVE_STATE_H_
#define _INC_SAVE_STATE_H_

unsigned char *readNstSaveState(  const char *filename, int *save_state_size );
boolean_e      writeNstSaveState( const char *filename, const unsigned char *save_state, const size_t size );

#endif
