


#ifndef VPX_ARGS_H_
#define VPX_ARGS_H_
#include <stdio.h>//FILE

#ifdef __cplusplus
extern "C" {
#endif

struct Arg
{
    char** pArgv;
    const char* pName;
    const char* pVal;
    unsigned int uArgvStep;// the step to next command line argument
    const struct ArgDef* Def;

};



typedef struct ArgDef {
    const char* chShortName;
    const char* chLongName;
    int iHasVal;// whether there is a value after the parameter, -o ...    --limit=...
    const char* chDesc;//description
} ArgDef_t;

#define ARG_DEF(s, l, v, d) \
    { s, l, v, d }



char** argv_dup(int argc, const char** argv);
int arg_match(struct Arg *arg, const struct ArgDef *def, char **argv);
struct Arg arg_init(char **argv);
unsigned int arg_parse_uint(const struct Arg *arg);
void arg_show_usage(FILE *fp, const struct ArgDef* const* defs);


#ifdef __cplusplus
}// extern "C"
#endif

#endif // VPX_ARGS_H_





