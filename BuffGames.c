// This code is intended to demonstrate buffering 
// a line oriented file, where line lengths vary
// The intent is to minimize reads using a buffer 
// size that is appropriate for system factors such
// as block size etc. 
// The module will be invoked as 
//    char * GimmeALine(   ) returning a pointer
// to a line from the file being processed or NULL 
// if no more data. All reads are done within this 
// module. An attempt at encapsulation is made by 
// declaring local names as static (and thus not 
// 'automatic' and temporary over calls to GimmeALine
// A simple main driver is used for testing
// Caveat Emptor: this is a work in progress
// Robert Trenary, 9/3/14
#include <stdio.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include "apue.h"
#define BUFFSIZE 2048
#define FALSE 0
#define TRUE -100
char * GimmeALine( int * ); 
// will send current file descriptor as a toggle for initializing 

int main(int argc, char *argv[])
{
    int fd = -1;
    char *LinePtr; // Return from GimmeALine
    int LineCount = 0;

    while ((LinePtr = GimmeALine(&fd)) != NULL)
      {  fprintf(stderr,"%s\n",LinePtr);LineCount++;}

    fprintf(stdout,"%u Lines\n",LineCount);
}

char * GimmeALine( int * FD)
{
    static char Buffer[BUFFSIZE+1];
    static int MyEof ;
    static int ReadCount; //??? why static?
    static int BufferPtr;
    static char * SavePtr;
    static char * TokenPtr;
    int offset;
    int MYLOCALFD;
    MYLOCALFD = *FD; // easier for system calls below 
                     // final value must be copied into *FD
//    fprintf(stderr,"%u is BufferPtr in Gimme\n",BufferPtr);
    if ( MYLOCALFD == -1 ) 
            // initialize the buffer 
    {   MYLOCALFD = open("AllCountries.dat",O_RDONLY);      
        if (MYLOCALFD <0) err_sys("Open of file failed");
       ReadCount = read( MYLOCALFD ,Buffer,BUFFSIZE);
       MyEof = (ReadCount == 0);
//       fprintf(stderr, "filled buffer with %u \n", ReadCount);
    if(ReadCount < 0) err_sys("Error on read");              
        Buffer[ReadCount] = (char) 0;
        *FD = MYLOCALFD; 
        BufferPtr = 0;       
    } // end of initialization
    TokenPtr = &Buffer[BufferPtr]; 
   
    while ((BufferPtr < ReadCount) && (Buffer[BufferPtr] != '\n'))
              BufferPtr++; // Move Ptr Forward To Eoln ? 
 //  assert( Buffer[BuffPtr] == '\n' );
    if (Buffer[BufferPtr] == '\n') // Yes 
     {  Buffer[BufferPtr]= (char) 0; // strtok_r(Buffer, "\n")
        BufferPtr++;
       fprintf(stderr,"token in Gimme is %s\n",TokenPtr);
      }
        else /* No .. reset the buffer ? */
            /* and File Pointer */
            {offset = (&Buffer[BufferPtr] - TokenPtr);
            lseek(MYLOCALFD, offset, SEEK_CUR);//bug?
            ReadCount = read(MYLOCALFD,Buffer,BUFFSIZE); 
            MyEof = (ReadCount == 0);
            BufferPtr=0;Buffer[ReadCount]= (char) 0;
   //         TokenPtr = strtok_r(Buffer,"\n",&SavePtr);  
            while ((BufferPtr < ReadCount) && (Buffer           [BufferPtr] != '\n'))
              BufferPtr++; // Move Ptr Forward To Eoln ? 
 //  assert( Buffer[BuffPtr] == '\n' );
    if (Buffer[BufferPtr] == '\n') // Yes 
     {  Buffer[BufferPtr]= (char) 0; // strtok_r(Buffer, "\n")
        BufferPtr++;
            }
            }
    if (MyEof) TokenPtr = NULL;
    return TokenPtr;
}












