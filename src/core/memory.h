#ifndef __SERUM_CORE_MEMORY_H
#define __SERUM_CORE_MEMORY_H




#include "./common.h"




#ifdef __cplusplus
extern "C" {
#endif

void serum_erase(void *const target, const size_t size);
void serum_memdump(const void *const src, const size_t size);

#ifdef __cplusplus
}
#endif




#endif
