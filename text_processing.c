#include <stdio.h>
#include <stdlib.h>
#include "rand_malloc.h"

char *getLine();
char *expandBuffer(char *buffer, size_t *capacity);

int main() 
{
   char *line;
   while ((line = getLine()) != NULL)
   {   
       printf("You entered: %s\n", line);
       free(line);
   }

    return 0;
}

char *getLine() 
{
    size_t capacity = 10;
    size_t length = 0;
    int current_character;

    char *buffer = malloc(capacity * sizeof(char));

    if (buffer == NULL) 
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    while(1)
    {
        current_character = getchar();

        if (current_character == EOF && length == 0) 
        {
            free(buffer);
            return NULL;
        }

        if (current_character == EOF || current_character == '\n') 
        {
            break;
        }

        if (length + 1 >= capacity)
        {
           char* new_buffer = expandBuffer(buffer, &capacity);
           if (new_buffer == NULL)
           {
               printf("Failed to expand buffer\n");
               free(buffer);
               return NULL;
           }
           buffer = new_buffer;
        }

        buffer[length] = current_character;
        length++;
    }
    buffer[length] = '\0';
    
    return buffer; 
}

char *expandBuffer(char *buffer, size_t *capacity)
{
    size_t new_capacity = 2*(*capacity);
    char *temporary_buffer = realloc(buffer, new_capacity * sizeof(char));
    if (temporary_buffer == NULL)
    {
        printf("Memory reallocation failed\n");
        return NULL;
    }
    *capacity = new_capacity;
    return temporary_buffer;
}