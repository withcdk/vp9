


#ifndef VPX_ARGS_H_
#define VPX_ARGS_H_


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


struct ArgEnumList
{
    const char* name;
    int val;
};


typedef struct ArgDef {
    const char* chShortName;
    const char* chLongName;
    int iHasVal;// whether there is a value after the parameter, -o ...    --limit=...
    const char* chDesc;
    const struct ArgEnumList* Enums;
} ArgDef_t;

#define ARG_DEF(s, l, v, d) \
    { s, l, v, d, NULL }



char** argv_dup(int argc, const char** argv);
int arg_match(struct Arg *arg, const struct ArgDef *def, char **argv);
struct Arg arg_init(char **argv);
unsigned int arg_parse_uint(const struct Arg *arg);


#ifdef __cplusplus

}// extern "C"
#endif

#endif // VPX_ARGS_H_





