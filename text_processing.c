#include <stdio.h>
#include <stdlib.h>
#include "rand_malloc.h"
#include <string.h>

char *getLine();
char *expandBuffer(char *buffer, size_t *capacity);
int isValidOctalNumber(const char *str);
char *removeWhitespaces(const char *str);
char **expandStoredNumbers(char **stored_numbers, size_t *stored_capacity);
char *addOctal(const char *octal1, const char *octal2);

int main() 
{
    char *line;
    char **stored_numbers = NULL;
    size_t stored_count = 0;
    size_t stored_capacity = 10;
    while ((line = getLine()) != NULL)
    {   
        if (isValidOctalNumber(line)) 
        {
            char *cleaned_line = removeWhitespaces(line);
            if (cleaned_line != NULL)
            {
                if (stored_count >= stored_capacity)
                {
                    char **new_stored_numbers = expandStoredNumbers(stored_numbers, &stored_capacity);
                    if (new_stored_numbers == NULL)
                    {
                        printf("Failed to expand stored numbers\n");
                        free(cleaned_line);
                        free(stored_numbers);
                        free(line);
                        return 1;
                    }
                    stored_numbers = new_stored_numbers;
                }
                stored_numbers[stored_count] = cleaned_line;
                stored_count++;
            }


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

char **expandStoredNumbers(char **stored_numbers, size_t *stored_capacity) 
{
    size_t new_capacity = 2*(*stored_capacity);
    char **temporary_buffer = realloc(stored_numbers, new_capacity * sizeof(char*));
    if (temporary_buffer == NULL)
    {
        printf("Memory reallocation failed\n");
        return NULL;
    }
    *stored_capacity = new_capacity;
    return temporary_buffer;
}

char *addOctal(const char *octal1, const char *octal2) 
{
    int len1 = strlen(octal1);
    int len2 = strlen(octal2);
    int size1 = len1 -1;
    int size2 = len2 -1;
    int carry = 0;
    int max_size;
    if (len1 > len2)
    {
        max_size = len1 + 2;
    }
    else
    {
        max_size = len2 + 2;
    }
    char *result = malloc(max_size * sizeof(char));
    if (result == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int write_index = max_size - 1;
    while (size1 >= 0 || size2 >= 0 || carry > 0) 
    {
        int sum = carry;
        if (size1 >= 0)
        {
            sum += octal1[size1] - '0';
            size1--;
        }
        if (size2 >= 0)
        {
            sum += octal2[size2] - '0';
            size2--;
        }
        carry = sum / 8;
        result[write_index] = (sum % 8) + '0';
        write_index--;
    }
    char *final_result = malloc((max_size - write_index) * sizeof(char));
    if (final_result == NULL)
    {
        printf("Memory allocation failed\n");
        free(result);
        return NULL;
    }
    int final_index = 0;
    for (int real_digits = write_index + 1; real_digits < max_size; real_digits++)
    {
        final_result[final_index] = result[real_digits];
        final_index++;
    }
    final_result[final_index] = '\0';
    free(result);
    return final_result;
}
