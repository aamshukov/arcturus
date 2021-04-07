/* Part of CRC-32C library: https://crc32c.machinezoo.com/ */
#ifndef CRC32C_H
#define CRC32C_H

#if defined(__GNUC__) || defined(__GNUG__)
#define CRC32C_GCC
#elif defined(_MSC_VER)
#define CRC32C_MSC
#endif

#ifndef CRC32C_STATIC
#ifdef CRC32C_EXPORTS
#ifdef CRC32C_GCC
#define CRC32C_API __attribute__ ((dllexport))
#else
#define CRC32C_API __declspec(dllexport)
#endif
#else
#ifdef CRC32C_GCC
#define CRC32C_API __attribute__ ((dllimport))
#else
#define CRC32C_API __declspec(dllimport)
#endif
#endif
#else
#define CRC32C_API
#endif

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
    Computes CRC-32C (Castagnoli) checksum. Uses Intel's CRC32 instruction if it is available.
    Otherwise it uses a very fast software fallback.
*/
CRC32C_API uint32_t crc32c_append(
    uint32_t crc,               /* Initial CRC value. Typically it's 0.                            */
                                /* You can supply non-trivial initial value here.                  */
                                /* Initial value can be used to chain CRC from multiple buffers.   */
    const uint8_t *input,       /* Data to be put through the CRC algorithm.                       */
    size_t length);             /* Length of the data in the input buffer.                         */


/*
	Software fallback version of CRC-32C (Castagnoli) checksum.
*/
CRC32C_API uint32_t crc32c_append_sw(uint32_t crc, const uint8_t *input, size_t length);

/*
	Hardware version of CRC-32C (Castagnoli) checksum. Will fail, if CPU does not support related instructions. Use a crc32c_append version instead of.
*/
CRC32C_API uint32_t crc32c_append_hw(uint32_t crc, const uint8_t *input, size_t length);

/*
	Checks is hardware version of CRC-32C is available.
*/
CRC32C_API int crc32c_hw_available();

#ifndef __cplusplus
/*
    Initializes the CRC-32C library. Should be called only by C users to enable hardware version for crc32c_append.
*/
CRC32C_API void crc32c_init();
#endif


#ifdef __cplusplus
}
#endif

#endif
