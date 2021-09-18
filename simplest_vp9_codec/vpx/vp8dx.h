#ifndef VPX_VPX_VP8DX_H_
#define VPX_VPX_VP8DX_H_

#include "vpx_codec.h"

#ifdef __cplusplus
extern "C" {
#endif

extern VpxCodecIface_t vpx_codec_vp8_dx_algo;
extern VpxCodecIface_t* vpx_codec_vp8_dx(void);

extern VpxCodecIface_t vpx_codec_vp9_dx_algo;
extern VpxCodecIface_t* vpx_codec_vp9_dx(void);


#ifdef __cplusplus
}// extern "C"
#endif


#endif // VPX_VPX_VP8DX_H_



