#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../../SL.h"
#include "argument.h"

void cmd_argSetAcceptedValues(cmd_arg* arg, array(cmd_arg_val) values) {
    if (arg->type == CMD_ARG_TOGGLE) SL_throwError("Argument of type TOGGLE cannot have default values assigned to it (it takes in no parameter)!");
    if (arg->type == CMD_ARG_BOOL)   SL_throwError("Argument of type BOOL cannot have default values assigned to it (they already are 'true' or 'false')!");
    arg->acceptedValues = values;
}
int cmd_argValueAsEnum(cmd_arg* arg) {
    if (arg->acceptedValues.count == 0 || arg->type == CMD_ARG_TOGGLE) return -1;
    if (arg->type == CMD_ARG_BOOL) return (int)arg->storage->bool_;
    
    int value = 0;
    switch (arg->type) {
        case CMD_ARG_UINT:   array_foreach(v IN arg->acceptedValues) if (v->uint_ != arg->storage->uint_) ++value; else break; break;
        case CMD_ARG_INT:    array_foreach(v IN arg->acceptedValues) if (v->int_ != arg->storage->int_) ++value; else break; break;
        case CMD_ARG_FLOAT:  array_foreach(v IN arg->acceptedValues) if (v->float_ != arg->storage->float_) ++value; else break; break;
        case CMD_ARG_STRING: array_foreach(v IN arg->acceptedValues) if (!SL_match(v->string, arg->storage->string)) ++value; else break; break;
    }
    return value == arg->acceptedValues.count ? -1 : value;
}
bool cmd_argAssigned(cmd_arg* arg) { return arg->assigned; }
void cmd_argPrintName(cmd_arg* arg, FILE* stream) {
    if (!arg->name_long) fprintf(stream, "-%s", arg->name_short);
    else if (arg->name_short) fprintf(stream, "--%s (-%s)", arg->name_long, arg->name_short);
    else fprintf(stream, "--%s", arg->name_long);
}
void cmd_argPrintDescription(cmd_arg* arg, FILE* stream) {
    printf("# Argument "); cmd_argPrintName(arg, stdout); printf(":");
    if (!arg->hasDefaultValue && arg->type != CMD_ARG_TOGGLE) printf(" [MANDATORY]\n");
    else switch (arg->type) {
        case CMD_ARG_BOOL:   printf(" [OPTIONNAL, DEFAULTS TO \'%s\']\n", arg->defaultValue.bool_ ? "true" : "false"); break;
        case CMD_ARG_UINT:   printf(" [OPTIONNAL, DEFAULTS TO \'%u\']\n", arg->defaultValue.uint_); break;
        case CMD_ARG_INT:    printf(" [OPTIONNAL, DEFAULTS TO \'%i\']\n", arg->defaultValue.int_); break;
        case CMD_ARG_FLOAT:  printf(" [OPTIONNAL, DEFAULTS TO \'%f\']\n", arg->defaultValue.float_); break;
        case CMD_ARG_STRING: printf(" [OPTIONNAL, DEFAULTS TO \'%s\']\n", arg->defaultValue.string); break;
        default: printf(" [OPTIONNAL]\n"); break;
    }
    switch (arg->type) {
        case CMD_ARG_BOOL:   printf("\tExpected type : boolean\n", arg->defaultValue.bool_ ? "true" : "false"); break;
        case CMD_ARG_UINT:   printf("\tExpected type : unsigned integer\n", arg->defaultValue.uint_); break;
        case CMD_ARG_INT:    printf("\tExpected type : signed integer\n", arg->defaultValue.int_); break;
        case CMD_ARG_FLOAT:  printf("\tExpected type : real number\n", arg->defaultValue.float_); break;
        case CMD_ARG_STRING: printf("\tExpected type : string\n", arg->defaultValue.string); break;
        default: printf("\tNo value expected (toggle type)\n"); break;
    }
    if (arg->acceptedValues.count > 0 || arg->type == CMD_ARG_BOOL) {
        printf("\tAccepted values");
        switch (arg->type) {
            case CMD_ARG_BOOL:   printf(": true, false"); break;
            case CMD_ARG_UINT:   array_foreach(value IN arg->acceptedValues) printf("%c %u", value == arg->acceptedValues.data ? ':' : ',', value->uint_); break;
            case CMD_ARG_INT:    array_foreach(value IN arg->acceptedValues) printf("%c %i", value == arg->acceptedValues.data ? ':' : ',', value->int_); break;
            case CMD_ARG_FLOAT:  array_foreach(value IN arg->acceptedValues) printf("%c %f", value == arg->acceptedValues.data ? ':' : ',', value->float_); break;
            case CMD_ARG_STRING: array_foreach(value IN arg->acceptedValues) printf("%c %s", value == arg->acceptedValues.data ? ':' : ',', value->string); break;
        }
        printf("\n");
    }
    printf("\tDesciption: %s\n\n", arg->description);
}


cmd createCMD(char* description) {
    return (cmd) {.description = description, .arguments = {0}};
}
void destroyCMD(cmd cmd) {
    array_foreach(arg IN cmd.arguments) destroyArray(arg->acceptedValues);
    destroyArray(cmd.arguments);
}

cmd_arg* cmd_addArg(cmd* cmd, char* name_short, char* name_long, cmd_arg_type type, char* description, void* storage) {
    if (name_short == NULL && name_long == NULL) {
        SL_throwError("Command line argument must have at least one name!");
        return NULL;
    }
    arrayAdd(cmd->arguments, ((cmd_arg) {
        .name_short = name_short, .name_long = name_long, .type = type,
        .acceptedValues = {0},
        .hasDefaultValue = false, .defaultValue = 0, .storage = storage, .assigned = false,
        .description = description
    }));
    return cmd->arguments.data + cmd->arguments.count - 1;
}
cmd_arg* cmd_addArg_default(cmd* cmd, char* name_short, char* name_long, cmd_arg_type type, char* description, void* storage, cmd_arg_val defaultValue) {
    if (name_short == NULL && name_long == NULL) {
        SL_throwError("Command line argument must have at least one name!");
        return NULL;
    }
    *(cmd_arg_val*)storage = defaultValue;
    arrayAdd(cmd->arguments, ((cmd_arg) {
        .name_short = name_short, .name_long = name_long, .type = type,
        .acceptedValues = {0},
        .hasDefaultValue = true, defaultValue = defaultValue, .storage = storage, .assigned = false,
        .description = description
    }));
    return cmd->arguments.data + cmd->arguments.count - 1;
}
void cmd_printUsage(const cmd* cmd, FILE* stream) {
    fprintf(stream, "Usage: %s ", cmd->name);
    for (uint i = 0; i < cmd->arguments.count; i++) {
        cmd_arg* arg = cmd->arguments.data + i;

        bool optionnal = arg->hasDefaultValue || arg->type == CMD_ARG_TOGGLE;
        if (optionnal) fprintf(stream, "[");

        if (!arg->name_long) fprintf(stream, "-%s", arg->name_short);
        else if (arg->name_short) fprintf(stream, "--%s|-%s", arg->name_long, arg->name_short);
        else fprintf(stream, "--%s", arg->name_long);

        switch (arg->type) {
            case CMD_ARG_TOGGLE: break;
            case CMD_ARG_BOOL: fprintf(stream, " : bool"); if (arg->hasDefaultValue) fprintf(stream, " = %s", arg->defaultValue.bool_ ? "true" : "false"); break;
            case CMD_ARG_UINT: fprintf(stream, " : uint"); if (arg->hasDefaultValue) fprintf(stream, " = %u", arg->defaultValue.uint_); break;
            case CMD_ARG_INT: fprintf(stream, " : int"); if (arg->hasDefaultValue) fprintf(stream, " = %i", arg->defaultValue.int_); break;
            case CMD_ARG_FLOAT: fprintf(stream, " : float"); if (arg->hasDefaultValue) fprintf(stream, " = %f", arg->defaultValue.float_); break;
            case CMD_ARG_STRING: fprintf(stream, " : string"); if (arg->hasDefaultValue) fprintf(stream, " = \"%s\"", arg->defaultValue.string); break;
        }

        if (optionnal) fprintf(stream, "]");

        printf("  ");
    }
    printf("\n");
}

int cmd_eval(cmd* cmd, int argc, char** argv) {
    cmd->name = argv[0];

    #define cmd_error_msg(msg, ...) fprintf(stderr, "[ERROR]: "msg"\n", ##__VA_ARGS__)
    #define cmd_error_msg_(msg1, msg2, ...) do { fprintf(stderr, "[ERROR]: "msg1" \'", ##__VA_ARGS__); cmd_argPrintName(arg, stderr); fprintf(stderr, "\' "msg2"\n"); } while(0)

    if (argc > 1 && SL_match(argv[1], "--help")) {
        if (argc == 2 || SL_match(argv[2], "*")) { // Printf program help
            printf("# Program %s:\n", cmd->name);
            printf("\tDescription: %s\n\t", cmd->description);
            cmd_printUsage(cmd, stdout);
        }
        if (argc > 2) array_foreach(arg IN cmd->arguments) cmd_argPrintDescription(arg, stdout);
        else { // print command help
            for (uint i = 2; i < argc; i++) {
                char* cur = argv[i];
                while (*cur == '-') ++cur; // Keep only name
                
                cmd_arg* corresponding = NULL; 
                array_foreach(arg IN cmd->arguments) {
                    if (SL_match(cur, arg->name_short) || SL_match(cur, arg->name_long)) {
                        corresponding = arg;
                        break;
                    }
                }

                if (!corresponding) { cmd_error_msg("Expected only command(s) after command \'--help\'!"); goto ERROR; }
                cmd_argPrintDescription(corresponding, stdout);
            }
        }

        return 2; // Exit help!
    }

    for (uint i = 1; i < argc; i++) {
        char* command = argv[i];
        char* value = NULL;

        if (*command != '-') { cmd_error_msg("Unexpected \"%s\", expected argument!", argv[i]); goto ERROR; }
        
        bool useLongName = *++command == '-'; // Wether the argument starts with '--' 
        if (useLongName) ++command;

        cmd_arg* arg = NULL;
        array_foreach(arg_ IN cmd->arguments) if (arg_->names[useLongName] && SL_match(command, arg_->names[useLongName])) { arg = arg_; break; } 
        if (!arg) { cmd_error_msg("Unexpected \"%s\", unrecognized argument!", argv[i]); goto ERROR; }

        #define cmd_error(msg, type, ...) do { cmd_error_msg_(msg" after "type"-type command", "!", ##__VA_ARGS__); goto ERROR; } while (0)
        
        #define cmd_test_equal(full_type, type) do { \
            if (++i < argc) value = argv[i]; \
            if (!arg->hasDefaultValue && !value) cmd_error("Expected "full_type, type); \
        } while (0)

        #define cmd_test_array(full_type, type, p, t, immidiate) do { \
            if (arg->acceptedValues.count > 0) { \
                if (immidiate) { for (; k < arg->acceptedValues.count; k++) if (arg->storage-> t == arg->acceptedValues.data[k]. t) break; } \
                else { for (; k < arg->acceptedValues.count; k++) if (SL_match(value, arg->acceptedValues.data[k].string)) break; } \
                if (k == arg->acceptedValues.count) { \
                    cmd_error_msg_("Unexpected \""p"\" after "type"-type command", "!", arg->storage-> t); \
                    fprintf(stderr, "\t Should be one of: [ \'"p"\'", arg->acceptedValues.data[0]. t); for (k = 1; k < arg->acceptedValues.count; k++) printf(", \'"p"\'", arg->acceptedValues.data[k]. t); fprintf(stderr, " ]\n"); \
                    goto ERROR; \
                } \
            } \
        } while (0)

        #define cmd_test_full(full_type, type, p, t, ...) do { \
            cmd_test_equal(full_type, type); \
            if (value) { \
                if (sscanf(value, p, arg->storage) != 1) cmd_error("Unexpected \"%s\"", type, value); \
                cmd_test_array(full_type, type, p, t, true, ##__VA_ARGS__); \
            } \
        } while (0)
        
        uint k = 0;
        switch (arg->type) {
            case CMD_ARG_TOGGLE: {
                arg->storage->toggle_ = !arg->storage->toggle_;
                if (++i < argc && argv[i][0] != '-') cmd_error("Unexpected \"%s\"", "toggle", value);
            } break;
            
            case CMD_ARG_BOOL: {
                cmd_test_equal("boolean", "bool");

                if (value) {
                    if (SL_match_NoCase(value, "true")) arg->storage->bool_ = true;
                    else if (SL_match_NoCase(value, "false")) arg->storage->bool_ = false;
                    else cmd_error("Unexpected \"%s\"", "bool");
                }
            } break;

            case CMD_ARG_UINT:  cmd_test_full("unsigned integer", "uint", "%u", uint_); break;
            case CMD_ARG_INT:   cmd_test_full("integer", "int", "%i", int_); break;
            case CMD_ARG_FLOAT: cmd_test_full("float", "float", "%f", float_); break;

            case CMD_ARG_STRING: {
                cmd_test_equal("string", "string");
                if (value) {
                    arg->storage->string = value;
                    cmd_test_array("string", "string", "%s", string, false);
                }
                else printf("No string\n");
            } break;

            default: SL_throwError("Unexpected \"%s\", unrecognized argument!", argv[i]); goto ERROR;                        
        }

        arg->assigned = true;
    }

    for (uint j = 0; j < cmd->arguments.count; j++) {
        cmd_arg* arg = cmd->arguments.data + j;
        if (!arg->assigned && !arg->hasDefaultValue && arg->type != CMD_ARG_TOGGLE) {
            cmd_error_msg_("Mandatory argument ", " missing!");
            goto ERROR;
        }
    }

    return 0; // Exit success!

    ERROR:
    fprintf(stderr, "\t "); cmd_printUsage(cmd, stderr);
    return 1; // Exit error!
}

#include <time.h>
struct CMD_Progress {
    uint l;
    uint p;
    uint max_val;
    clock_t timer;
};
cmd_progress cmd_progressInit(uint bar_length, uint max_value) {
	cmd_progress p = {
        .l = bar_length,
        .p = 0,
        .max_val = max_value,
        .timer = clock()
    };
	printf("\nProgress: ["); for (uint i = 0; i < p.l; i++) putc('-', stdout); printf("]");
    return p;
}
bool cmd_progressTest(cmd_progress* p, uint value) {
	if (value * p->l < (p->p + 1) * p->max_val) return false;
	p->p++;
	
	if (value >= p->max_val) {
		printf("\r\033[2KDone! [%fms]", 1000.0 * (clock() - p->timer) / (double)CLOCKS_PER_SEC);
		return true;
	}
	
	float progress = value / (float)p->max_val * p->l;
	printf("\rProgress: ["); for (uint i = 0; i < p->l; i++) putc(i > progress ? '-' : '#', stdout); printf("]");
    return value >= p->max_val;
}