#include "monty.h"

/**
 * main - Entry point
 * @argc: Number of command-line arguments
 * @argv: Array containing the command-line arguments
 *
 * Description: Main function of the Monty interpreter.
 * It reads bytecode instructions from a file, parses each instruction,
 * and executes the corresponding operation.
 *
 * Return: Always 0 on success
 */
int main(int argc, char *argv[])
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    unsigned int line_number = 0;

    /* Check if filename is provided */
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    /* Open file */
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* Read file line by line */
    while ((read = getline(&line, &len, file)) != -1)
    {
        char *opcode, *arg;

        line_number++;

        /* Tokenize the line */
        opcode = strtok(line, " \n");
        if (opcode == NULL || *opcode == '#')
            continue; /* Skip empty lines and comments */

        /* Handle push opcode */
        if (strcmp(opcode, "push") == 0)
        {
            arg = strtok(NULL, " \n");
            if (arg == NULL || !is_number(arg))
            {
                fprintf(stderr, "L%d: usage: push integer\n", line_number);
                free(line);
                free_stack(&stack);
                fclose(file);
                exit(EXIT_FAILURE);
            }
            push(&stack, line_number);
        }
        /* Handle pall opcode */
        else if (strcmp(opcode, "pall") == 0)
        {
            pall(&stack, line_number);
        }
        else
        {
            fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
            free(line);
            free_stack(&stack);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    /* Clean up */
    free(line);
    free_stack(&stack);
    fclose(file);
    return 0;
}

/**
 * push - Pushes an element onto the stack
 * @stack: Pointer to the stack
 * @line_number: Current line number in the file
 *
 * Description: The function pushes an element onto the stack.
 * If the argument is not an integer, it prints an error message.
 */
void push(stack_t **stack, unsigned int line_number)
{
    stack_t *new_node;
    char *arg;
    int value;

    arg = strtok(NULL, " \n");
    value = atoi(arg);

    new_node = malloc(sizeof(stack_t));
    if (new_node == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        free_stack(stack);
        exit(EXIT_FAILURE);
    }

    new_node->n = value;
    new_node->prev = NULL;
    new_node->next = *stack;

    if (*stack != NULL)
        (*stack)->prev = new_node;

    *stack = new_node;
}

/**
 * pall - Prints all elements of the stack
 * @stack: Pointer to the stack
 * @line_number: Current line number in the file
 *
 * Description: The function prints all elements of the stack.
 * If the stack is empty, it doesn't print anything.
 */
void pall(stack_t **stack, unsigned int line_number)
{
    stack_t *current = *stack;

    while (current != NULL)
    {
        printf("%d\n", current->n);
        current = current->next;
    }
}

/**
 * free_stack - Frees the entire stack
 * @stack: Pointer to the stack
 *
 * Description: The function frees the entire stack.
 */
void free_stack(stack_t **stack)
{
    stack_t *current = *stack, *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    *stack = NULL;
}

/**
 * is_number - Checks if a string is a number
 * @str: String to be checked
 *
 * Return: 1 if string is a number, 0 otherwise
 */
int is_number(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;

    while (*str)
    {
        if (*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

