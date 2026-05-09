#include <stdio.h>
#include <stdlib.h>
#include "rand_malloc.h"

char *getLine();
char *expandBuffer(char *buffer, size_t *capacity);
int isValidOctalNumber(const char *str);
char *removeWhitespaces(const char *str);

int main() 
{
   char *line;
   while ((line = getLine()) != NULL)
   {   
        if (isValidOctalNumber(line)) 
        {
            char *cleaned_line = removeWhitespaces(line);

        }


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

int isValidOctalNumber(const char *str) 
{
    size_t index = 0;
    int found_at_least_one_digit = 0;
    
    while (str[index] != '\0') 
    {
       char current_character = str[index];
       if (current_character == ' ' || current_character == '\t') 
       {
           index++;
           continue;
       }
       else if (current_character >= '0' && current_character <= '7') 
       {
           found_at_least_one_digit = 1;
           index++;
       } 
       else 
       {
            printf("Invalid octal number: %s\n", str);
            return 0; 
       }
    }
    return found_at_least_one_digit;
}

char *removeWhitespaces(const char *str) 
{
    size_t length = 0;
    for (int character = 0; str[character] != '\0'; character++) 
    {
        if (str[character] >= '0' && str[character] <= '7') 
        {
            length++;
        }
    }

    char *cleaned = malloc((length + 1) * sizeof(char));
    if (cleaned == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    size_t index = 0;
    for (int character = 0; str[character] != '\0'; character++)
    {
        if (str[character] >= '0' && str[character] <= '7') 
        {
            cleaned[index] = str[character];
            index++;
        }
    }
    cleaned[index] = '\0';
    return cleaned;
}