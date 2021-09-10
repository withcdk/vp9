#ifndef VPX_TOOLS_COMMON_H_
#define VPX_TOOLS_COMMON_H_


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


#ifdef __cplusplus
} // extern "C" 
#endif

#endif // VPX_TOOLS_COMMON_H_
