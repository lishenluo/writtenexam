#pragma once
/**
* 类型定义
*
* @author   lishenluo
* @date     2019-01-12
*/
#ifndef TYPE_H
#define TYPE_H

#ifdef __cplusplus
extern "C" {
#endif
    typedef unsigned int uint;

#ifdef _MSC_VER
    typedef __int8  r_int8;
    typedef __int16 r_int16;
    typedef __int32 r_int32;
    typedef __int64 r_int64;

    typedef unsigned __int8  r_uint8;
    typedef unsigned __int16 r_uint16;
    typedef unsigned __int32 r_uint32;
    typedef unsigned __int64 r_uint64;
#else
#include <stdint.h>
    typedef int8_t  r_int8;
    typedef int16_t r_int16;
    typedef int32_t r_int32;
    typedef long long int r_int64;

    typedef uint8_t  r_uint8;
    typedef uint16_t r_uint16;
    typedef uint32_t r_uint32;
    typedef unsigned long long int r_uint64;
#endif
#ifdef __cplusplus
}
#endif

#endif


