
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
        int occupied;
        if (pipe->start_occupied == pipe->end_occupied)
        {
            occupied = 0;
        }
        else if (pipe->start_occupied < pipe->end_occupied)
        {
            occupied = pipe->end_occupied - pipe->start_occupied + 1;
        }
        else
        {
            occupied = pipe->buffersize - pipe->start_occupied + pipe->end_occupied;
        }
        

        int i; 
        if ( occupied == 0)
        {
            i = 0;
            pipe->start_occupied = i;
        }
        else if (occupied + size > pipe->buffersize) 
        {
            i = pipe->start_occupied = pipe->end_occupied + 1;
        }
        else 
        {
            i = pipe->end_occupied + 1;
        }

        

        for (int j = 0; j < size; j++)
        {
            pipe->pipebuffer[i] = buffer[j];
            pipe->end_occupied = i;
            i++;
            if (i == pipe->buffersize)
            {
                i = 0;
            }
        }

        return size;
    }
}

//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty
int myread(mypipe* pipe, byte* buffer, int size)
{
    //In normal pipes, lets say you have "hello", 6 bytes, but you read 60. read still returns 6, because there was not more in the pipe
    
    int to_read;

    if (pipe->start_occupied == pipe->end_occupied) return 0;
    else if (pipe->end_occupied > pipe->start_occupied)
    {
        to_read = min(size, pipe->end_occupied - pipe->start_occupied + 1);      
    }
    else
    {
        to_read = min(size, pipe->buffersize - pipe->start_occupied + pipe->end_occupied);
    }

    int i = pipe->start_occupied;
    for (int j = 0; j < to_read; j++)
    {
        buffer[j] = pipe->pipebuffer[i];
        i++;
    }

    return to_read;

}

void main()
{
    //TESTING CODE:

    char text[100];
    mypipe pipeA;
    
    init_pipe(&pipeA, 32);
    mywrite(&pipeA, "hello world", 12);

     for (int i = 0; i < 32; i++)
    {
        printf("%c\n", pipeA.pipebuffer[i]);
    }

    printf("**********************\n"); 


    mywrite(&pipeA, "it's a nice day", 16);

    for (int i = 0; i < 32; i++)
    {
        printf("%c\n", pipeA.pipebuffer[i]);
    } 

    myread(&pipeA, text, 12);
    printf("1: %s\n", text);
    
    myread(&pipeA, text, 16);
    printf("2: %s\n", text);
    
    mywrite(&pipeA, "and now we test the carryover", 30);
    myread(&pipeA, text, 16);
    printf("3: %s\n", text);

    for (int i = 0; i < 32; i++)
    {
        printf("%c\n", pipeA.pipebuffer[i]);
    } 
}