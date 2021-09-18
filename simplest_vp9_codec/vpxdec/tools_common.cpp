
#include <stdio.h>// NULL
#include <stdarg.h>// va_start()
#include <stdlib.h>
#include <string.h>// strcmp()


#include "tools_common.h"
#include "vpxdec.h"

#if CONFIG_VP8_DECODER || CONFIG_VP9_DECODER
#include "../vpx/vp8dx.h"
#endif



#define LOG_ERROR(label)               \
  do {                                 \
    const char *l = label;             \
    va_list ap;                        \
    va_start(ap, fmt);                 \
    if (l) fprintf(stderr, "%s: ", l); \
    vfprintf(stderr, fmt, ap);         \
    fprintf(stderr, "\n");             \
    va_end(ap);                        \
  } while (0)

void die(const char *fmt, ...)
{
	LOG_ERROR(NULL);
	usage_exit();
}




#if CONFIG_DECODERS
static const VpxInterface_t vpx_decoders[] =
{
#if CONFIG_VP8_DECODER
    {"vp8", VP8_FOURCC, &vpx_codec_vp8_dx},

#endif


#if CONFIG_VP9_DECODER

    {"vp9", VP9_FOURCC, &vpx_codec_vp9_dx},

#endif


};

int get_vpx_decoder_count(void)
{
    return sizeof(vpx_decoders) / sizeof(vpx_decoders[0]);
}


const VpxInterface_t *get_vpx_decoder_by_index(int i)
{
    return &vpx_decoders[i];
}

const VpxInterface_t* get_vpx_decoder_by_name(const char* name)
{
    int i;
    for (i = 0; i < get_vpx_decoder_count(); ++i)
    {
        const VpxInterface_t* const decoder = get_vpx_decoder_by_index(i);
        if (strcmp(decoder->name, name) == 0)
            return decoder;
    }
    return NULL;
}



#endif

