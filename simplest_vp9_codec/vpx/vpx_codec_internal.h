#ifndef VPX_VPX_INTERNAL_VPX_CODEC_INTERNAL_H_
#define VPX_VPX_INTERNAL_VPX_CODEC_INTERNAL_H_

#include "vpx_codec.h"


#ifdef __cplusplus
extern "C" {
#endif

// decoder algorithm interface 
// all decoders \ref MUST expose a variable of this type
struct VpxCodecIface
{
	
	const char* cchName;
	/*
	int iAbiVersion;
	VpxCodecCaps_t caps;
	*/
	    
};


#define CODEC_INTERFACE(id)                          \
  VpxCodecIface_t *id(void) { return &id##_algo; }   \
  VpxCodecIface_t id##_algo                          

                        

#ifdef __cplusplus
}// extern "C"
#endif

#endif // VPX_VPX_INTERNAL_VPX_CODEC_INTERNAL_H_

