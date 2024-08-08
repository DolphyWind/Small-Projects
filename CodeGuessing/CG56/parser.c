#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_SIZE 1024

typedef struct Expression {
    char sym;
    struct Expression** subexpressions;
    size_t num_subexpressions;
    size_t size_arr;
    int state;
} Expression;

typedef struct
{
    char const* string;
    size_t len;
    size_t current;
    Expression* expression;
} Parser;

Expression* expression_init(char c, int state)
{
    Expression* e = malloc(sizeof(Expression));
    e->sym = c;
    e->size_arr = 1;
    e->num_subexpressions = 0;
    e->subexpressions = malloc(sizeof(Expression*));
    e->subexpressions[0] = NULL;
    e->state = state;
    return e;
}

void expression_free(Expression* e)
{
    for(size_t i = 0; i < e->num_subexpressions; ++i)
    {
        expression_free(e->subexpressions[i]);
    }
    free(e->subexpressions);
}

void expression_add_sub(Expression* e, Expression* sub)
{
    if(e->num_subexpressions + 1 == e->size_arr)
    {
        // Double the array
        Expression** new_arr = malloc(sizeof(Expression*) * e->size_arr * 2);
        for(size_t i = 0; i < e->num_subexpressions; ++i)
        {
            new_arr[i] = e->subexpressions[i];
        }
        free(e->subexpressions);
        e->subexpressions = new_arr;
        e->size_arr *= 2;
    }
    e->subexpressions[e->num_subexpressions] = sub;
    ++e->num_subexpressions;
}

void subexpression_remove(Expression* e, size_t index)
{
    if(index >= e->num_subexpressions) return;
    expression_free(e->subexpressions[index]);
    for(size_t i = index; i < e->num_subexpressions - 1; ++i)
    {
        e->subexpressions[i] = e->subexpressions[i + 1];
    }
    e->subexpressions[e->num_subexpressions - 1] = NULL;
    --e->num_subexpressions;
}

void expression_print(Expression* e, char* to)
{
    if(e->sym == '|')
    {
        if(e->num_subexpressions != 0) expression_print(e->subexpressions[0], to);
        return;
    }
    if(e->state == 2)
    {
        for(size_t i = 0; i < e->num_subexpressions; ++i)
        {
            expression_print(e->subexpressions[i], to);
        }
        return;
    }
    strncat(to, &e->sym, 1);
}

Expression* parse_pipe(Parser* p);

Parser* parser_init(char const* str)
{
    Parser* p = malloc(sizeof(Parser));
    p->string = str;
    p->current = 0;
    p->len = strlen(str);
    p->expression = NULL;

    return p;
}

void parser_free(Parser* p)
{
    if(p->expression)
    {
        expression_free(p->expression);
    }
    free(p);
}

void parser_panic(Parser* p, char const* msg)
{
    fprintf(stderr, "%s", msg);
    parser_free(p);
    exit(1);
}

int parser_at_end(Parser* p)
{
    return (p->current >= p->len);
}

char parser_current(Parser* p)
{
    if(parser_at_end(p)) return '\0';
    return p->string[p->current];
}

int parser_check(Parser* p, char c)
{
    if(parser_at_end(p)) return 0;
    return (c == parser_current(p));
}

int parser_check_multiple(Parser* p, char const* chars)
{
    int len = strlen(chars);
    for(int i = 0; i < len; ++i)
    {
        if(parser_check(p, chars[i]))
        {
            return 1;
        }
    }
    return 0;
}

char parser_prev(Parser* p)
{
    if(p->current == 0 || p->current > p->len) return '\0';
    return p->string[p->current - 1];
}

char parser_advance(Parser* p)
{
    char c = parser_current(p);
    ++p->current;
    return c;
}

int parser_match(Parser* p, char const* chars)
{
    int check = parser_check_multiple(p, chars);
    if(check)
    {
        parser_advance(p);
    }
    return check;
}

Expression* parse_atom(Parser* p)
{
    if(parser_at_end(p))
    {
        return NULL; // Return an expression that has no leaves and the char is parser_current(p)
    }
    if(parser_check_multiple(p, "\\*|()[]") == 0)
    {
        char c = parser_advance(p);
        if(c == '\0') return NULL;
        return expression_init(c, 0); // Same here
    }

    char current_char = parser_advance(p);
    switch (current_char)
    {
        case '\\':
        {
            return expression_init(parser_advance(p), 0); // Similar here: char is parser_advance(p); 
        }
        case '[':
        {
            if(parser_advance(p) != ']')
            {
                parser_panic(p, "Character classes must be empty!");
            }
            return expression_init('G', 1);
        }
        case '(':
        {
            Expression* e = parse_pipe(p);
            if(parser_advance(p) != ')')
            {
                parser_panic(p, "Unclosed \'(\'!");
            }
            return e;
        }
        default:
        {
            if(current_char != ')' && current_char != '|')
            {
                parser_panic(p, "Unexpected character");
            }
            p->current -= 1;
            return NULL;
        }
    }

    return NULL;
}

Expression* parse_kleene(Parser* p)
{
    Expression* atoms = expression_init('_', 2); // This should be a list
    Expression* atom = NULL;

    while ((atom = parse_atom(p)) != NULL)
    {
        while(parser_match(p, "*") == 1)
        {
            atom->sym = '*';
        }
        // Append atom to atoms
        expression_add_sub(atoms, atom);
    }
    return atoms;
}

Expression* parse_pipe(Parser* p)
{
    Expression* expr = expression_init('|', 0);
    Expression* left = parse_kleene(p);
    expression_add_sub(expr, left);

    while(parser_match(p, "|"))
    {
        Expression* right = parse_kleene(p);
        expression_add_sub(expr, right);
    }

    return expr;
}

Expression* parse_regex(Parser* p)
{
    p->expression = parse_pipe(p);
    return p->expression;
}

Expression* expression_filter(Expression* e)
{
    if(e->sym != '|') return e;

    for(size_t i = 0; i < e->num_subexpressions;)
    {
        Expression* current = e->subexpressions[i];
        // printf("Debug: %c\n", current->sym);
        if(current->sym == '|') expression_filter(current->subexpressions[i]);
        else if(current->state == 2)
        {
            int removed = 0;
            for(size_t j = 0; j < current->num_subexpressions; ++j)
            {
                Expression* cc = current->subexpressions[j];
                if(cc->state == 1)
                {
                    subexpression_remove(e, i);
                    removed = 1;
                    break;
                }
                if(cc->sym == '*')
                {
                    expression_free(cc);
                    current->subexpressions[j] = expression_init('_', 2);
                }
            }
            if(removed) continue;
        }

        if(current->sym == '*')
        {
            printf("d\n");
            expression_free(e->subexpressions[i]);
            e->subexpressions[i] = expression_init('_', 2);
        }
        ++i;
    }
    return e;
}

char* entry(char const* regex)
{
    Parser* p = parser_init(regex);
    parse_regex(p);
    expression_filter(p->expression);
    
    char* output = malloc(sizeof(char)*STR_SIZE);
    memset(output, 0, STR_SIZE);
    expression_print(p->expression, output);
    parser_free(p);
    return output;
}

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    if(!file)
    {
        fprintf(stderr, "Cannot open \"%s\"\n", argv[1]);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // Allocate memory to hold the file contents
    char* buffer = (char*)malloc(file_size * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return 1;
    }
    // Read the entire file into the buffer
    if (fread(buffer, 1, file_size, file) != file_size) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        free(buffer);
        return 1;
    }
    fclose(file);
    if(buffer[file_size - 1] == '\n') buffer[file_size - 1] = '\0';

    char* result = entry(buffer);
    printf("%s\n", result);

    free(result);
    free(buffer);
    return 0;
}
