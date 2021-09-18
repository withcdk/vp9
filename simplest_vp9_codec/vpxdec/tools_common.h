#ifndef VPX_TOOLS_COMMON_H_
#define VPX_TOOLS_COMMON_H_
#include <stdint.h>


#include "../vpx/vpx_config.h" //if tools_common.h will use ../vpx/vpx_config.h, include it in tools_common.h
#include "../vpx/vpx_codec.h"

#define VP8_FOURCC 0x30385056
#define VP9_FOURCC 0x30395056





#ifdef __cplusplus
extern "C" {
#endif

#if defined(__GNUC__)
#define VPX_NO_RETURN __attribute__((noreturn))

#else
#define VPX_NO_RETURN

#endif


void die(const char* fmt, ...) VPX_NO_RETURN;
void usage_exit(void) VPX_NO_RETURN;


#undef VPX_NO_RETURN






typedef struct VpxInterface
{
	const char* const name;
	const uint32_t fourcc;
	VpxCodecIface_t* (*const codec_interface)();// function pointer, pointer to a function 
	                                            // that returns a pointer(pointer function) 
} VpxInterface_t;


int get_vpx_decoder_count(void);
const VpxInterface_t* get_vpx_decoder_by_index(int i);
const VpxInterface_t* get_vpx_decoder_by_name(const char *name);



#ifdef __cplusplus
} // extern "C" 
#endif

#endif // VPX_TOOLS_COMMON_H_
