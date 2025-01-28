#ifndef __SL_UTILS_CMD_H__
#define __SL_UTILS_CMD_H__

#include "../../SL.h"

/// @brief Type of a command line argument
typedef enum CMD_ArgType {
    CMD_ARG_TOGGLE = 0,
    CMD_ARG_BOOL,
    CMD_ARG_INT,
    CMD_ARG_UINT,
    CMD_ARG_FLOAT,
    CMD_ARG_STRING
} cmd_arg_type;

/// @brief A comand line argument value
typedef union CMD_ArgValue {
    bool toggle_;
    bool bool_;
    int int_;
    uint uint_;
    float float_;
    char* string;
} cmd_arg_val;
SL_DEFINE_ARRAY(cmd_arg_val);

/// @brief A command line argument
typedef struct CMD_Argument {
    union {
        struct { char *name_short, *name_long; };
        char* names[2];
    };
    cmd_arg_type type;

    array(cmd_arg_val) acceptedValues;

    bool hasDefaultValue;
    cmd_arg_val defaultValue;
    cmd_arg_val* storage;
    bool assigned;

    char *description;
} cmd_arg;
SL_DEFINE_ARRAY(cmd_arg);

/// @brief Set the accepted values for a command line argument
/// @param arg The argument
/// @param values The accepted values
void cmd_argSetAcceptedValues(cmd_arg* arg, array(cmd_arg_val) values);
/// @brief Get index of read value in "accepted values" array
/// @param arg The argument
/// @return The index of the read value or -1 if not assigned or has no accepted values
int cmd_argValueAsEnum(cmd_arg* arg);
/// @brief Check wether a command line argument has been assigned
/// @param arg The argument
/// @return Wether the command line argument has been assigned
bool cmd_argAssigned(cmd_arg* arg);

/// @brief The command line program
typedef struct CMD_Program {
    array(cmd_arg) arguments;
    char* name;
    char* description;
} cmd;

/// @brief Create a command line program
/// @param description The description to print when calling "--help"
/// @return The newly created command line program
cmd createCMD(char* description);
/// @brief Destroy a command line program
/// @param toDestroy The command line program to destroy
void destroyCMD(cmd toDestroy);
/// @brief Add an argument in a cmd program
/// @param cmd The cmd program
/// @param name_short The long name of the argument (can be NULL)
/// @param name_long The short name of the argument (can be NULL)
/// @param type The type expected by the argument
/// @param description The decription to print when calling "--help"
/// @param storage Pointer to the variable in which to store the value if assigned
/// @return The newly added argument
cmd_arg* cmd_addArg(cmd* cmd, char* name_short, char* name_long, cmd_arg_type type, char* description, void* storage);
/// @brief Add an argument in a cmd program with a default value
/// @param cmd The cmd program
/// @param name_short The long name of the argument (can be NULL)
/// @param name_long The short name of the argument (can be NULL)
/// @param type The type expected by the argument
/// @param description The decription to print when calling "--help"
/// @param storage Pointer to the variable in which to store the value if assigned
/// @param defaultValue The value to use by default if no assignement is made
/// @return The newly added argument
cmd_arg* cmd_addArg_default(cmd* prog, char* name_short, char* name_long, cmd_arg_type type, char* description, void* storage, cmd_arg_val defaultValue);

/// @brief Evaluate the command line arguments based on a cmd program
/// @param cmd The cmd program
/// @param argc The main argc
/// @param argv The main argv
/// @return 0 if evaluated successfully, 1 if encountered an error, 2 if read "--help" argument
int cmd_eval(cmd* cmd, int argc, char** argv);

/// @brief Progress bar
typedef struct CMD_Progress cmd_progress;
/// @brief Initialize a progress bar
/// @param bar_length The number of characters used to render the bar
/// @param max_value The maximum value the iteration an take
/// @return The newly created prograss bar
cmd_progress cmd_progressInit(uint bar_length, uint max_value);
/// @brief Test and print a progress bar
/// @param p The progress bar
/// @param value The value to check against
/// @return Wether the bar is full
bool cmd_progressTest(cmd_progress* p, uint value);

#endif