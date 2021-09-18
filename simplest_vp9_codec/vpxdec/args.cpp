
#include <stdlib.h>//malloc()
#include <string.h>//memcpy()
#include <stdint.h>


#include "args.h"

#include "tools_common.h"




/****************************************************
Description: skip argv[0] .exe 
Input para: 
Return value: new_argv point to the first argument	  
Author£ºDe-Kai Chen    <cdk5@foxmail.com>
*****************************************************/
char** argv_dup(int argc, const char** argv)
{
    char** new_argv = (char **)malloc((argc + 1) * sizeof(*argv));

    memcpy(new_argv, argv, argc * sizeof(*argv));
    new_argv[argc] = NULL;
    return new_argv;
}


struct Arg arg_init(char **argv)
{
    struct Arg a;
    a.pArgv = argv;
    a.uArgvStep = 1;
    a.pName = NULL;
    a.pVal = NULL;
    a.Def = NULL;
    return a;

}



/****************************************************
Description: match command line arguments with 
             arguments defined in def
Input para:
Return value: matched: 1, not matched: 0
Author£ºDe-Kai Chen    <cdk5@foxmail.com>
*****************************************************/
int arg_match(struct Arg* arg, const struct ArgDef* def, char** argv)
{
    struct Arg tempArg;
    if (!argv[0] || argv[0][0] != '-')
    {
        return 0;
    }

    tempArg = arg_init(argv);

    // handle short name -o -i and so on
    if (def->chShortName && strlen(tempArg.pArgv[0]) == strlen(def->chShortName) + 1 &&
        !strcmp(tempArg.pArgv[0] + 1, def->chShortName))
    {
        tempArg.pName = tempArg.pArgv[0] + 1;
        tempArg.pVal = def->iHasVal ? tempArg.pArgv[1] : NULL;
        tempArg.uArgvStep = def->iHasVal ? 2 : 1;
    }
    else if (def->chLongName)
    {//handle long name --output=...
        const size_t nameLen = strlen(def->chLongName);

        if (strlen(tempArg.pArgv[0]) >= nameLen + 2 && tempArg.pArgv[0][1] == '-' &&
            !strncmp(tempArg.pArgv[0] + 2, def->chLongName, nameLen) &&
            (tempArg.pArgv[0][nameLen + 2] == '=' || 
             tempArg.pArgv[0][nameLen + 2] == '\0'))
        {
            tempArg.pName = tempArg.pArgv[0] + 2;
            tempArg.pVal = tempArg.pName[nameLen] == '=' ? tempArg.pName + nameLen + 1 : NULL;
            tempArg.uArgvStep = 1;
        }

    }

    if (tempArg.pName && !tempArg.pVal && def->iHasVal)
    {
        die("Error: option %s requires argument.\n", tempArg.pName);
    }

    if (tempArg.pName && tempArg.pVal && !def->iHasVal)
    {
        die("Error: option %s requires no argument.\n", tempArg.pName);
    }

    if (tempArg.pName && (tempArg.pVal || !def->iHasVal))
    {
        tempArg.Def = def;
        *arg = tempArg;
        return 1;
    }

    return 0;
}


/*******************************************************
Description: parse unsigned int argument behind "--xx="             
Input para:
Return value: unsigned int argument behind "--xx="
Author£ºDe-Kai Chen    <cdk5@foxmail.com>
*********************************************************/
unsigned int arg_parse_uint(const struct Arg *arg)
{
    uint32_t rawVal;
    char* endPtr;
    rawVal = (uint32_t)strtoul(arg->pVal, &endPtr, 10);
    if (arg->pVal[0] != '\0' && endPtr[0] == '\0')
    {
        if (rawVal <= UINT_MAX)
        {
            return rawVal;
        }
        die("Option %s: Value %ld out of range for unsigned int\n", arg->pName, 
            rawVal);// x86: ld -> unsigned long int
    }
    die("Option %s: Invalid character '%c'\n", arg->pName, *endPtr);

    return 0;

}


/*******************************************************
Description: show the usage of options
Input para:
Return value: 
Author£ºDe-Kai Chen    <cdk5@foxmail.com>
*********************************************************/
void arg_show_usage(FILE* fp, const struct ArgDef* const* defs)
{
    char optionText[40] = { 0 };
    for (; *defs; defs++)
    {
        const struct ArgDef* def = *defs;
        char* shortVal = def->iHasVal ? " <arg>" : "";
        char* longVal = def->iHasVal ? "=<arg>" : "";

        if (def->chShortName && def->chLongName)
        {
            char* comma = def->iHasVal ? "," : ",      ";
            snprintf(optionText, 37, "-%s%s%s --%s%6s", def->chShortName, shortVal, comma,
                def->chLongName, longVal);
        }
        else if (def->chShortName)
            snprintf(optionText, 37, "-%s%s", def->chShortName, shortVal);
        else if (def->chLongName)
            snprintf(optionText, 37, "          --%s%s", def->chLongName, longVal);

        fprintf(fp, "  %-37s\t%s\n", optionText, def->chDesc);
       
    }

}





