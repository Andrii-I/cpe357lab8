
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
    byte* t = (byte*)malloc(size);
    pipe->pipebuffer = t;
    pipe->buffersize = size;
    pipe->start_occupied = pipe->end_occupied = 0;

    return;
}

//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe* pipe, byte* buffer, int size)
{
    //It returns how many bytes you have written into the pipe. That sounds redundant, but it could be less than the actual size if the pipe is smaller.     
    if (size > pipe->buffersize || size <= 0)
    {
        return 0;
    }
    else
    {
        int i = pipe->start_occupied;

        for (int j = 0; j < size; j++)
        {
            pipe->pipebuffer[i] = buffer[j];
            i++;
            
            if (i > size - 1)
            {
                i = 0;
                //MODIFY start & end occupied in a special way
            }
            else 
            {
                //standard start & end modification
            
            }
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
    mypipe pipeA;
    
    init_pipe(&pipeA, 32);
    mywrite(&pipeA, "hello world", 12);
    mywrite(&pipeA, "it's a nice day", 16);
    strcpy(text, pipeA.pipebuffer);
    printf("%s\n", text);
    
/*     myread(&pipeA, text, 12);
    printf("%s\n", text);
    
    myread(&pipeA, text, 16);
    printf("%s\n", text);
    
    mywrite(&pipeA, "and now we test the carryover", 30);
    myread(&pipeA, text, 16);
    printf("%s\n", text); */
}