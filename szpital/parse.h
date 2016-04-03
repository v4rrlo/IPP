#ifndef PARSE_H_   /* Include guard */
#define PARSE_H_
typedef struct ParsedInput{
    char *command;
    char *name1;
    char *name2;
    char *diseaseDescription;
    long diseaseIndex;
}parsed_t;

void parse(parsed_t *, char *);
void freeParsedObject(parsed_t *);
void printParsedObject(parsed_t *);
void fillParsedWithNull(parsed_t *);
#endif // PARSE_H_
