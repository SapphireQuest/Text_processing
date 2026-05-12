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
char *manageBufferCapacity(char *buffer, size_t length, size_t *capacity);
char *moveOctal(char *result, int write_index, int max_size);
char **readAllNumbers(size_t *final_count);
void freeStoredNumbers(char **stored_numbers, size_t stored_count);
char *calculateTotal(char **stored_numbers, size_t stored_count);

int main()
{
    size_t stored_count = 0;
    char **stored_numbers = readAllNumbers(&stored_count); 
    if (stored_numbers == NULL)
    {
        return 1;
    }

    char *total_sum = calculateTotal(stored_numbers, stored_count);
    if (total_sum == NULL)
    {
        freeStoredNumbers(stored_numbers, stored_count);
        return 1;
    }
    
    printf("Sum: \n");
    printf("%s\n\n", total_sum);
    printf("Input numbers: \n");
    for (size_t num = 0; num < stored_count; num++)
    {
        printf("%s\n", stored_numbers[num]);
    }
    printf("\n");
    free(total_sum);
    freeStoredNumbers(stored_numbers, stored_count);
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
        printf("Memory allocation failed getLine\n");
        return NULL;
    }

    while (1)
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

        buffer = manageBufferCapacity(buffer, length, &capacity);
        if (buffer == NULL)
        {
            return NULL;
        }
        buffer[length] = current_character;
        length++;
    }
    buffer[length] = '\0';
    return buffer;
}

char *manageBufferCapacity(char *buffer, size_t length, size_t *capacity)
{
    if (length + 1 < *capacity)
    {
        return buffer;
    }

    char *new_buffer = expandBuffer(buffer, capacity);
    
    if (new_buffer == NULL)
    {
        printf("Failed to expand buffer manageBufferCapacity\n");
        free(buffer);
        return NULL;
    }
    return new_buffer;
}

char *expandBuffer(char *buffer, size_t *capacity)
{
    size_t new_capacity = 2 * (*capacity);
    char *temporary_buffer = realloc(buffer, new_capacity * sizeof(char));
    if (temporary_buffer == NULL)
    {
        printf("Memory reallocation failed expandBuffer\n");
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
        if (current_character == ' ' || current_character == '\t' || current_character == '\r')
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
        printf("Memory allocation failed removeWhitespaces\n");
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
    size_t new_capacity = 2 * (*stored_capacity);
    char **temporary_buffer = realloc(stored_numbers, new_capacity * sizeof(char *));
    if (temporary_buffer == NULL)
    {
        printf("Memory reallocation failed expandStoredNumbers\n");
        return NULL;
    }
    *stored_capacity = new_capacity;
    return temporary_buffer;
}

char *addOctal(const char *octal1, const char *octal2)
{
    int len1 = strlen(octal1);
    int len2 = strlen(octal2);
    int size1 = len1 - 1;
    int size2 = len2 - 1;
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
        printf("Memory allocation failed addOctal\n");
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

    return moveOctal(result, write_index, max_size);
}

char *moveOctal(char *result, int write_index, int max_size)
{
    char *final_result = malloc((max_size - write_index) * sizeof(char));
    if (final_result == NULL)
    {
        printf("Memory allocation failed moveOctal\n");
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

void freeStoredNumbers(char **stored_numbers, size_t stored_count)
{
    for (size_t i = 0; i < stored_count; i++)
    {
        free(stored_numbers[i]);
    }
    free(stored_numbers);
}

char **readAllNumbers(size_t *final_count)
{
    char *line;
    size_t stored_count = 0;
    size_t stored_capacity = 10;
    
    char **stored_numbers = malloc(stored_capacity * sizeof(char*));
    if (stored_numbers == NULL)
    {
        printf("Memory allocation failed readAllNUms\n");
        return NULL;
    }
    
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
                        free(line);
                        freeStoredNumbers(stored_numbers, stored_count); 
                        return NULL;
                    }
                    stored_numbers = new_stored_numbers;
                }
                stored_numbers[stored_count] = cleaned_line;
                stored_count++;
            }
            else
            {
                printf("Failed to remove whitespaces\n");
                free(line);
                freeStoredNumbers(stored_numbers, stored_count); 
                return NULL;
            }
        }
        free(line);
    }

    *final_count = stored_count; 
    return stored_numbers;
}

char *calculateTotal(char **stored_numbers, size_t stored_count)
{
    char *total_sum = malloc(2 * sizeof(char));
    if (total_sum == NULL)
    {
        return NULL;
    }
    total_sum[0] = '0';
    total_sum[1] = '\0';

    for (size_t num = 0; num < stored_count; num++)
    {
        char *new_sum = addOctal(total_sum, stored_numbers[num]);
        if (new_sum == NULL)
        {
            free(total_sum);
            return NULL; 
        }
        free(total_sum);
        total_sum = new_sum;
    }

    return total_sum;
}