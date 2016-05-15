/** @file
    Interface of parser.
    @author Miłosz Rzeźnikowski
    @copyright Uniwersytet Warszawski
    @date 2016-05-15
 */

#ifndef PARSE_H
#define PARSE_H

/**
    Structure that contains parsed content
    */
typedef struct def_command {
    char name[16];  ///< Contains name of a command.
    int data[7];    ///< Conatins parsed integer aguments of a command.
} command_t;


/** Reads a command_t.
    Returns command_t with data points using 'command_t' structure.
    */
command_t* parse_command();

#endif /* PARSE_H */
