#ifndef __ERATOSTHENES_H__
#define __ERATOSTHENES_H__

#include "base.h"

EXTERN_C_BEGIN

/*
 *  Sieve of Eratosthenes
 *  @return value: 
 */
unsigned long* Eratosthenes(unsigned long N, int* count);

EXTERN_C_END

#endif