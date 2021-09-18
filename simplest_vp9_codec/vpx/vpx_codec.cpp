

#include "vpx_codec.h"
#include "vpx_codec_internal.h"


const char* vpx_codec_iface_name(VpxCodecIface_t *iface)
{
	return iface ? iface->cchName : "<invalid interface>";
}
