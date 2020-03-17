
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

int empty = 1;

int min(int x, int y) 
{ 
    return y ^ ((x ^ y) & -(x < y)); 
} 

//initializes (malloc) the pipe with a size of "size" and sets start and end.
void init_pipe(mypipe* pipe, int size)
{
    byte* t = (byte*)malloc(size);
    pipe->pipebuffer = t;
    pipe->buffersize = size;
    pipe->start_occupied = pipe->end_occupied = -1;

    return;
}

//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe* pipe, byte* buffer, int size)
{
    //It returns how many bytes you have written into the pipe. That sounds redundant, but it could be less than the actual size if the pipe is smaller.     
    int move_start = 0;

    if (pipe->start_occupied == -1 && pipe->end_occupied == -1 && size > 0)
    {
        empty = 0;
        move_start = 1;
    }  


    int j = 0;
    for (; j < size; j++)
    {
        if (pipe->end_occupied + 1 == pipe->start_occupied)
        {
            break;
        }        
        else if (pipe->end_occupied + 1 == pipe->buffersize)
        {
            if (pipe->start_occupied > 0)
            {
                pipe->end_occupied = -1;
            }
            else if (pipe->start_occupied == 0)
            {
                break;
            }
            else
            {
                printf("ERROR ERROR\n");
            }
        }

        if (empty == 0)
        {
            pipe->end_occupied++;
        }
        
        //pipe->end_occupied++;
        pipe->pipebuffer[pipe->end_occupied] = buffer[j];
        empty = 0;
    }

    if (move_start)
    {
        pipe->start_occupied = 0;
    }
    
    return j;
}

//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty
int myread(mypipe* pipe, byte* buffer, int size)
{
    //In normal pipes, lets say you have "hello", 6 bytes, but you read 60. read still returns 6, because there was not more in the pipe
    if (pipe->start_occupied == -1 && pipe->end_occupied == -1)
    {
        return 0;
    }  


    int j = 0;
    for (; j < size; j++)
    {
/*         else if (pipe->start_occupied + 1 > pipe->end_occupied)
        {
            break;
        }  */

        buffer[j] = pipe->pipebuffer[pipe->start_occupied];

        if (pipe->start_occupied + 1 == pipe->buffersize)
        {
            pipe->start_occupied = -1;
        } 

        if (pipe->start_occupied == pipe->end_occupied)
        {
            empty = 1;
            break;
        } 


        pipe->start_occupied++;
    }

    return j;
}

void main()
{
    //TESTING CODE:

    char text[100];
    mypipe pipeA;
    init_pipe(&pipeA, 32);
    mywrite(&pipeA, "hello world", 12);
    mywrite(&pipeA, "it's a nice day", 16);
    myread(&pipeA, text, 12);
    printf("%s\n", text);
    myread(&pipeA, text, 16);
    printf("%s\n", text);
    mywrite(&pipeA, "and now we test the carryover", 30);
    myread(&pipeA, text, 30);
    printf("%s\n", text);
}