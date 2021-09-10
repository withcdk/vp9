
#include <stdio.h>
#include <string.h>
#include <stdlib.h>// free()

#include "../vpx/test1.h"

#include "args.h"



static const ArgDef_t looparg = ARG_DEF(NULL, "loops", 1, "Number of times to decode the file");




int main(int argc, const char **argv)
{
	char** tempArgv, ** argi, ** argj;
	struct Arg arg;
	unsigned int loops = 1, i;
	int error = 0;

	tempArgv = argv_dup(argc - 1, argv + 1);

	
	for (argi = argj = tempArgv; (*argj = *argi); argi += arg.uArgvStep)// (*argj = *argi) true or not and decide to quit
	{
		memset(&arg, 0, sizeof(arg));
		arg.uArgvStep = 1;

		// match "loops" command line argument
		if (arg_match(&arg, &looparg, argi))
		{
			loops = arg_parse_uint(&arg);
			break;
		}
		
	}
	free(tempArgv);

	/*
	for (i = 0; !error && i < loops; i++)
	{
		error = main_loop(argc, argv);
	}
	*/
	
	
	show_code(5);
	printf("test_cdk ok!\n");
}



