/*********************************************************************************
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
* 																			     *
* This program is free software; you can redistribute it and/or                  *
* modify it under the terms of the GNU General Public License                    *
* as published by the Free Software Foundation; either version 2				 *
* of the License, or (char *argumentat your option) any later version.			 *
*																				 *
* This program is distributed in the hope that it will be useful,				 *
* but WITHout ANY WARRANTY; without even the implied warranty of                 *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#ifndef __SHA2_H__
#define __SHA2_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Import u_intXX_t size_t type definitions from system headers.  You
 * may need to change this, or define these things yourself in this
 * file.
 */
#include <sys/types.h>

#define BYTE_ORDER LITTLE_ENDIAN

#define SHA2_USE_INTTYPES_H
#ifdef SHA2_USE_INTTYPES_H

#include <inttypes.h>

#endif /* SHA2_USE_INTTYPES_H */


/*** SHA-256/384/512 Various Length Definitions ***********************/
#define SHA256_BLOCK_LENGTH		64
#define SHA256_DIGEST_LENGTH		32
#define SHA256_DIGEST_STRING_LENGTH	(SHA256_DIGEST_LENGTH * 2 + 1)
#define SHA384_BLOCK_LENGTH		128
#define SHA384_DIGEST_LENGTH		48
#define SHA384_DIGEST_STRING_LENGTH	(SHA384_DIGEST_LENGTH * 2 + 1)
#define SHA512_BLOCK_LENGTH		128
#define SHA512_DIGEST_LENGTH		64
#define SHA512_DIGEST_STRING_LENGTH	(SHA512_DIGEST_LENGTH * 2 + 1)


/*** SHA-256/384/512 Context Structures *******************************/
/* NOTE: If your architecture does not define either u_intXX_t types or
 * uintXX_t (from inttypes.h), you may need to define things by hand
 * for your system:
 */
#if 0
typedef unsigned char u_int8_t;		/* 1-byte  (8-bits)  */
typedef unsigned int u_int32_t;		/* 4-bytes (32-bits) */
typedef unsigned long long u_int64_t;	/* 8-bytes (64-bits) */
#endif
/*
 * Most BSD systems already define u_intXX_t types, as does Linux.
 * Some systems, however, like Compaq's Tru64 Unix instead can use
 * uintXX_t types defined by very recent ANSI C standards and included
 * in the file:
 *
 *   #include <inttypes.h>
 *
 * If you choose to use <inttypes.h> then please define:
 *
 *   #define SHA2_USE_INTTYPES_H
 *
 * Or on the command line during compile:
 *
 *   cc -DSHA2_USE_INTTYPES_H ...
 */
#ifdef SHA2_USE_INTTYPES_H

typedef struct _SHA256_CTX {
	uint32_t	state[8];
	uint64_t	bitcount;
	uint8_t	buffer[SHA256_BLOCK_LENGTH];
} SHA256_CTX;
typedef struct _SHA512_CTX {
	uint64_t	state[8];
	uint64_t	bitcount[2];
	uint8_t	buffer[SHA512_BLOCK_LENGTH];
} SHA512_CTX;

#else /* SHA2_USE_INTTYPES_H */

typedef struct _SHA256_CTX {
	u_int32_t	state[8];
	u_int64_t	bitcount;
	u_int8_t	buffer[SHA256_BLOCK_LENGTH];
} SHA256_CTX;
typedef struct _SHA512_CTX {
	u_int64_t	state[8];
	u_int64_t	bitcount[2];
	u_int8_t	buffer[SHA512_BLOCK_LENGTH];
} SHA512_CTX;

#endif /* SHA2_USE_INTTYPES_H */

typedef SHA512_CTX SHA384_CTX;


/*** SHA-256/384/512 Function Prototypes ******************************/
#ifndef NOPROTO
#ifdef SHA2_USE_INTTYPES_H

void SHA256_Init(SHA256_CTX *);
void SHA256_Update(SHA256_CTX*, const uint8_t*, size_t);
void SHA256_Final(uint8_t[SHA256_DIGEST_LENGTH], SHA256_CTX*);
char* SHA256_End(SHA256_CTX*, char[SHA256_DIGEST_STRING_LENGTH]);
char* SHA256_Data(const uint8_t*, size_t, char[SHA256_DIGEST_STRING_LENGTH]);

void SHA384_Init(SHA384_CTX*);
void SHA384_Update(SHA384_CTX*, const uint8_t*, size_t);
void SHA384_Final(uint8_t[SHA384_DIGEST_LENGTH], SHA384_CTX*);
char* SHA384_End(SHA384_CTX*, char[SHA384_DIGEST_STRING_LENGTH]);
char* SHA384_Data(const uint8_t*, size_t, char[SHA384_DIGEST_STRING_LENGTH]);

void SHA512_Init(SHA512_CTX*);
void SHA512_Update(SHA512_CTX*, const uint8_t*, size_t);
void SHA512_Final(uint8_t[SHA512_DIGEST_LENGTH], SHA512_CTX*);
char* SHA512_End(SHA512_CTX*, char[SHA512_DIGEST_STRING_LENGTH]);
char* SHA512_Data(const uint8_t*, size_t, char[SHA512_DIGEST_STRING_LENGTH]);

#else /* SHA2_USE_INTTYPES_H */

void SHA256_Init(SHA256_CTX *);
void SHA256_Update(SHA256_CTX*, const u_int8_t*, size_t);
void SHA256_Final(u_int8_t[SHA256_DIGEST_LENGTH], SHA256_CTX*);
char* SHA256_End(SHA256_CTX*, char[SHA256_DIGEST_STRING_LENGTH]);
char* SHA256_Data(const u_int8_t*, size_t, char[SHA256_DIGEST_STRING_LENGTH]);

void SHA384_Init(SHA384_CTX*);
void SHA384_Update(SHA384_CTX*, const u_int8_t*, size_t);
void SHA384_Final(u_int8_t[SHA384_DIGEST_LENGTH], SHA384_CTX*);
char* SHA384_End(SHA384_CTX*, char[SHA384_DIGEST_STRING_LENGTH]);
char* SHA384_Data(const u_int8_t*, size_t, char[SHA384_DIGEST_STRING_LENGTH]);

void SHA512_Init(SHA512_CTX*);
void SHA512_Update(SHA512_CTX*, const u_int8_t*, size_t);
void SHA512_Final(u_int8_t[SHA512_DIGEST_LENGTH], SHA512_CTX*);
char* SHA512_End(SHA512_CTX*, char[SHA512_DIGEST_STRING_LENGTH]);
char* SHA512_Data(const u_int8_t*, size_t, char[SHA512_DIGEST_STRING_LENGTH]);

#endif /* SHA2_USE_INTTYPES_H */

#else /* NOPROTO */

void SHA256_Init();
void SHA256_Update();
void SHA256_Final();
char* SHA256_End();
char* SHA256_Data();

void SHA384_Init();
void SHA384_Update();
void SHA384_Final();
char* SHA384_End();
char* SHA384_Data();

void SHA512_Init();
void SHA512_Update();
void SHA512_Final();
char* SHA512_End();
char* SHA512_Data();

#endif /* NOPROTO */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* __SHA2_H__ */
