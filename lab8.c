
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

typedef unsigned char byte;

typedef struct mypipe
{
byte* pipebuffer;
int buffersize;
int start_occupied;
int end_occupied;
}mypipe;

//initializes (malloc) the pipe with a size of "size" and sets start and end.
void init_pipe(mypipe* pipe, int size)
{
    pipe->pipebuffer = (byte*)malloc(size);
    pipe->buffersize = size;
    pipe->start_occupied = pipe->end_occupied = 0;

    return;
}

//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe* pipe, byte* buffer, int size)
{
    //It returns how many bytes you have written into the pipe. That sounds redundant, but it could be less than the actual size if the pipe is smaller.
    if (size > pipe->buffersize)
    {
        return 0;
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            pipe->pipebuffer[pipe->start_occupied + i] = buffer[i];
            //add overflow stuff
        }
    }

    //change later to size written
    return 0;
}

//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty
int myread(mypipe* pipe, byte* buffer, int size)
{
    //In normal pipes, lets say you have "hello", 6 bytes, but you read 60. read still returns 6, because there was not more in the pipe
}

void main()
{

    //TESTING CODE:

    char text[100];
    mypipe* pipeA;
    
    init_pipe(pipeA, 32);
    printf("piped\n");
    mywrite(pipeA, "hello world", 12);
    mywrite(pipeA, "it's a nice day", 16);
    printf("written\n");

    myread(pipeA, text, 12);
    printf("%s\n", text);
    myread(pipeA, text, 16);
    printf("%s\n", text);

    /*mywrite(pipeA, "and now we test the carryover", 30);
    myread(pipeA, text, 30);
    printf("%s\n", text);*/
}