
#include <stdio.h>
#include <string.h>
#include <stdlib.h>// free()

#include "../vpx/test1.h"
#include "../vpx/vpx_config.h"
#include "../vpx/vpx_decoder.h"

#include "args.h"
#include "vpxdec.h"
#include "tools_common.h"


static const char* exec_name;



static const ArgDef_t help = ARG_DEF(NULL, "help", 0, "Show usage options and exit");
static const ArgDef_t looparg = ARG_DEF(NULL, "loops", 1, "Number of times to decode the file");
static const ArgDef_t codecarg = ARG_DEF(NULL, "codec", 1, "Codec to use");
static const ArgDef_t use_yv12 = ARG_DEF(NULL, "yv12", 0, "Output raw YV12 frames");
static const ArgDef_t use_i420 = ARG_DEF(NULL, "i420", 0, "Output raw I420 frames");
static const ArgDef_t flipuvarg = ARG_DEF(NULL, "flipuv", 0, "Flip the chroma planes in the output");
static const ArgDef_t rawvideo = ARG_DEF(NULL, "rawvideo", 0, "Output raw YUV frames");// equal to i420
static const ArgDef_t noblitarg = ARG_DEF(NULL, "noblit", 0, "Don't process the decoded frames");
static const ArgDef_t progressarg = ARG_DEF(NULL, "progress", 0, "Show progress after each frame decodes");
static const ArgDef_t limitarg = ARG_DEF(NULL, "limit", 1, "Stop decoding after n frames");
static const ArgDef_t skiparg = ARG_DEF(NULL, "skip", 1, "Skip the first n input frames");
static const ArgDef_t postprocarg = ARG_DEF(NULL, "postproc", 0, "Postprocess decoded frames");
static const ArgDef_t summaryarg = ARG_DEF(NULL, "summary", 0, "Show timing summary");
static const ArgDef_t outputfile = ARG_DEF("o", "output", 1, "Output file name pattern (see below)");
static const ArgDef_t threadsarg = ARG_DEF("t", "threads", 1, "Max threads to use");
static const ArgDef_t frameparallelarg = ARG_DEF(NULL, "frame-parallel", 0, "Frame parallel decode (ignored)");
static const ArgDef_t verbosearg = ARG_DEF("v", "verbose", 0, "Show version string");
static const ArgDef_t error_concealment = ARG_DEF(NULL, "error-concealment", 0, "Enable decoder error-concealment");
static const ArgDef_t scalearg = ARG_DEF("S", "scale", 0, "Scale output frames uniformly");
static const ArgDef_t continuearg = ARG_DEF("k", "keep-going", 0, "(debug) Continue decoding after error");
static const ArgDef_t fb_arg = ARG_DEF(NULL, "frame-buffers", 1, "Number of frame buffers to use");
static const ArgDef_t md5arg = ARG_DEF(NULL, "md5", 0, "Compute the MD5 sum of the decoded frame");
#if CONFIG_VP9_HIGHBITDEPTH
static const ArgDef_t outbitdeptharg = ARG_DEF(NULL, "output-bit-depth", 1, "Output bit-depth for decoded frames");
#endif
static const ArgDef_t svcdecodingarg = ARG_DEF(NULL, "svc-decode-layer", 1, "Decode SVC stream up to given spatial layer");
static const ArgDef_t framestatsarg = ARG_DEF(NULL, "framestats", 1, "Output per-frame stats (.csv format)");
static const ArgDef_t rowmtarg = ARG_DEF(NULL, "row-mt", 1, "Enable multi-threading to run row-wise in VP9");
static const ArgDef_t lpfoptarg = ARG_DEF(NULL, "lpf-opt", 1, "Do loopfilter without waiting for all threads to sync.");

static const ArgDef_t* all_args[] = {  &help,
									   &codecarg,
									   &use_yv12,
									   &use_i420,
									   &flipuvarg,
									   &rawvideo,
									   &noblitarg,
									   &progressarg,
									   &limitarg,
									   &skiparg,
									   &postprocarg,
									   &summaryarg,
									   &outputfile,
									   &threadsarg,
									   &frameparallelarg,
									   &verbosearg,
									   &scalearg,
									   &fb_arg,
									   &md5arg,
									   &error_concealment,
									   &continuearg,
#if CONFIG_VP9_HIGHBITDEPTH
									   & outbitdeptharg,
#endif
									   & svcdecodingarg,
									   &framestatsarg,
									   &rowmtarg,
									   &lpfoptarg,
									   NULL };

#if CONFIG_VP8_DECODER
static const ArgDef_t addnoise_level = ARG_DEF(NULL, "noise-level", 1, "Enable VP8 postproc add noise");
static const ArgDef_t deblock = ARG_DEF(NULL, "deblock", 1, "Enable VP8 deblocking");
static const ArgDef_t demacroblock_level = ARG_DEF(NULL, "demacroblock-level", 1, "Enable VP8 demacroblocking, w/ level");
static const ArgDef_t mfqe = ARG_DEF(NULL, "mfqe", 1, "Enable multiframe quality enhancement");
static const ArgDef_t* vp8_pp_args[] = { &addnoise_level, &deblock, &demacroblock_level, &mfqe, NULL};


#endif




/****************************************************
Description: 
Input para:
Return value: 
Author£ºDe-Kai Chen    <cdk5@foxmail.com>
*****************************************************/
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

	
	for (i = 0; !error && i < loops; i++)
	{
		error = main_loop(argc, argv);
	}
	
	show_code(5);
	printf("test_cdk ok!\n");
}



/****************************************************
Description: main loop
Input para:
Return value: 
Author£ºDe-Kai Chen    <cdk5@foxmail.com>
*****************************************************/
static int main_loop(int argc, const char** argv)
{
	int ret = EXIT_FAILURE;
	char** tempArgv, ** argi, ** argj;
	struct Arg arg;
	const VpxInterface_t* interface = NULL;
	const char* outfilePattern = NULL;
	int useY4m = 1;
	int flipUv = 0, noBlit = 0;
	int progress = 0, doMd5 = 0;
	int stopAfter = 0, postproc = 0, summary = 0, quiet = 1;
	int argSkip = 0;
	int optI420 = 0;
	VpxCodecDecCfg_t cfg = {0, 0, 0};
	int doScale = 0;
	int numExternalFrameBuffers = 0;
	int keepGoing = 0;



	exec_name = argv[0];
	tempArgv = argv_dup(argc - 1, argv + 1);

	for (argi = argj = tempArgv; (*argj = *argi); argi += arg.uArgvStep)
	{
		memset(&arg, 0, sizeof(arg));
		arg.uArgvStep = 1;

		if (arg_match(&arg, &help, argi))
		{
			show_help(stdout, 0);
			exit(EXIT_SUCCESS);
		}
		else if (arg_match(&arg, &codecarg, argi))
		{
			interface = get_vpx_decoder_by_name(arg.pVal);
			if (!interface)
			{
				die("Error: Unrecognized argument (%s) to --codec\n", arg.pVal);
			}
		}
		else if (arg_match(&arg, &looparg, argi))
		{
			// no-op
		}
		else if (arg_match(&arg, &outputfile, argi))
		{
			outfilePattern = arg.pVal;
		}
		else if (arg_match(&arg, &use_yv12, argi))
		{			
			useY4m = 0;
			flipUv = 1;
			optI420 = 1;
		}
		else if (arg_match(&arg, &use_i420, argi))
		{
			useY4m = 0;
			flipUv = 0;
			optI420 = 1;
		}
		else if (arg_match(&arg, &rawvideo, argi))
		{
			useY4m = 0;
		}
		else if (arg_match(&arg, &flipuvarg, argi))
		{
			flipUv = 1;
		}
		else if (arg_match(&arg, &noblitarg, argi))
		{
			noBlit = 1;
		}
		else if (arg_match(&arg, &progressarg, argi))
		{
			progress = 1;
		}
		else if (arg_match(&arg, &limitarg, argi))
		{
			stopAfter = arg_parse_uint(&arg);
		}
		else if (arg_match(&arg, &skiparg, argi))
		{
			argSkip = arg_parse_uint(&arg);
		}
		else if (arg_match(&arg, &postprocarg, argi))
		{
			postproc = 1;
		}
		else if (arg_match(&arg, &md5arg, argi))
		{
			doMd5 = 1;
		}
		else if (arg_match(&arg, &summaryarg, argi))
		{
			summary = 1;
		}
		else if (arg_match(&arg, &threadsarg, argi))
		{
			cfg.threads = arg_parse_uint(&arg);
		}
#if CONFIG_VP9_DECODER
		else if (arg_match(&arg, &frameparallelarg, argi))
		{
			// ignored for compatibility
		}

#endif
		else if (arg_match(&arg, &verbosearg, argi))
		{
			quiet = 0;
		}
		else if (arg_match(&arg, &scalearg, argi))
		{
			doScale = 1;
		}
		else if (arg_match(&arg, &fb_arg, argi))
		{
			numExternalFrameBuffers = arg_parse_uint(&arg);
		}
		else if (arg_match(&arg, &continuearg, argi))
		{
			keepGoing = 1;
		}

	}

	free(tempArgv);
	return ret;
}


/****************************************************
Description: show help list, "static" indicates that  
             it can only be used in this file
Input para:
Return value:
Author£ºDe-Kai Chen    <cdk5@foxmail.com>
*****************************************************/
static void show_help(FILE* fout, int shortHelp)
{
	int i;
	fprintf(fout, "Usage: %s <options> filename\n\n", exec_name);
	if (shortHelp)
	{
		fprintf(fout, "Use --help to see the full list of options.\n");
		return;
	}

	fprintf(fout, "Options:\n");
	arg_show_usage(fout, all_args);
#if CONFIG_VP8_DECODER
	fprintf(fout, "\nVP8 Postprocessing Options:\n");
	arg_show_usage(fout, vp8_pp_args);
#endif
	fprintf(fout,
			"\nOutput File Patterns:\n\n"
			"  The -o argument specifies the name of the file(s) to "
			"write to. If the\n  argument does not include any escape "
			"characters, the output will be\n  written to a single file. "
			"Otherwise, the filename will be calculated by\n  expanding "
			"the following escape characters:\n");
	fprintf(fout,
			"\n\t%%w   - Frame width"
			"\n\t%%h   - Frame height"
			"\n\t%%<n> - Frame number, zero padded to <n> places (1..9)"
			"\n\n  Pattern arguments are only supported in conjunction "
			"with the --yv12 and\n  --i420 options. If the -o option is "
			"not specified, the output will be\n  directed to stdout.\n");
	fprintf(fout, "\nInclude decoders:\n\n");


	
	for (i = 0; i < get_vpx_decoder_count(); ++i)
	{
		const VpxInterface_t* const decoder = get_vpx_decoder_by_index(i);
		
		
		fprintf(fout, "    %-6s - %s\n", decoder->name,
			vpx_codec_iface_name(decoder->codec_interface()));// function name call
		
	}
	
}

void usage_exit(void)
{
	show_help(stderr, 1);
	exit(EXIT_FAILURE);
}


