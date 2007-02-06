/**
 * \file
 * <!--
 * Copyright 2007 Develer S.r.l. (http://www.develer.com/)
 * This file is part of DevLib - See README.devlib for information.
 * -->
 *
 * \brief MD2 Message-Digest algorithm.
 *
 * \version $Id$
 * \author Daniele Basile <asterix@develer.com>
 */

/*#*
 *#* $Log$
 *#* Revision 1.14  2007/02/06 09:47:53  asterix
 *#* Typo.
 *#*
 *#* Revision 1.13  2007/02/05 18:44:42  asterix
 *#* Add md2_perm function.
 *#*
 *#* Revision 1.12  2007/02/05 16:52:44  asterix
 *#* Add define for harvard architecture.
 *#*
 *#* Revision 1.11  2007/02/02 18:15:31  asterix
 *#* Add function MD2_test. Fix bug in md2_update function.
 *#*
 *#* Revision 1.9  2007/02/02 13:10:01  asterix
 *#* Fix some bugs in md2_pad and md2_update fuction.
 *#*
 *#* Revision 1.8  2007/02/01 14:45:56  asterix
 *#* Rewrite md2_update function and fix some bug.
 *#*
 *#* Revision 1.7  2007/01/31 18:04:15  asterix
 *#* Write md2_end function
 *#*
 *#* Revision 1.4  2007/01/31 13:51:57  asterix
 *#* Write md2_compute function.
 *#*
 *#* Revision 1.2  2007/01/30 17:31:44  asterix
 *#* Add function prototypes.
 *#*
 *#* Revision 1.1  2007/01/30 15:53:26  batt
 *#* Add first md2 skel.
 *#*
 *#*/

#include "md2.h"

#include <string.h>           //memset(), memcpy();
#include <cfg/compiler.h>
#include <cfg/debug.h>        //ASSERT()
#include <cfg/macros.h>       //MIN(), countof();
#include <mware/pgm.h>   


#ifdef STD_PERMUTATION
	/*
	* Official array of 256 byte pemutation contructed from digits of pi, defined
	* in the RFC 1319.
	*/
	static const uint8_t PROGMEM md2_perm[256] = 
	{
	41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
	19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
	76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
	138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
	245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
	148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
	39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
	181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
	150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
	112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
	96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
	85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
	234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
	129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
	8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
	203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
	166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
	31, 26, 219, 153, 141, 51, 159, 17, 131, 20
	};
#else
	/**
	 * Md2_perm() function generate an array of 256 "casual" permutation.
	 */
	
	/**
	 * Costant define for computing an array of 256 "casual" permutation.
	 * \{
	 */
	#define K1 5
	#define K2 3
	#define R  2
	#define X  172
	/*\}*/

uint8_t md2_perm(uint8_t i)
{

	i = i * K1;
	i = (i >> R) ^ (i << R); //Rotate i for R times.
	i ^=  X;
	i = i * K2;

	return i;
}
	
#endif


#if CPU_HARVARD 
	#ifdef STD_PERMUTATION
		#define	MD2_PERM(x) pgm_read_char(&md2_permr[x]) //Read from program memory, if CPU is harvard
	#else
		#define	MD2_PERM(x) pgm_read_char(&md2_permr(x)) //Read from program memory, if CPU is harvard
	#endif
#else
	#ifdef STD_PERMUTATION
		#define	MD2_PERM(x) md2_perm[x]                  //
	#else
		#define	MD2_PERM(x) md2_perm(x)                  //
	#endif
#endif

/**
 * Pad function. Put len_pad unsigned char in
 * input block.
 */
static void md2_pad(void *_block, size_t len_pad)
{
	uint8_t *block;

	block = (uint8_t *)_block;

	ASSERT(len_pad <= CONFIG_MD2_BLOCK_LEN);

	/*
	 * Fill input block with len_pad char.
	 */
	memset(block, len_pad, len_pad);

}

static void md2_compute(void *_state, void *_checksum, void *_block)
{
	int i = 0;
	uint16_t t = 0;
	uint8_t compute_array[COMPUTE_ARRAY_LEN];
	uint8_t *state;
	uint8_t *checksum;
	uint8_t *block;

	state = (uint8_t *)_state;
	checksum  = (uint8_t *)_checksum;
	block = (uint8_t *)_block;

	/*
	 * Copy state and checksum context in compute array.
	 */
	memcpy(compute_array, state, CONFIG_MD2_BLOCK_LEN);
	memcpy(compute_array + CONFIG_MD2_BLOCK_LEN, block, CONFIG_MD2_BLOCK_LEN);

	/*
	 * Fill compute array with state XOR block
	 */
	for(i = 0; i < CONFIG_MD2_BLOCK_LEN; i++)
		compute_array[i + (CONFIG_MD2_BLOCK_LEN * 2)] = state[i] ^ block[i];

	/*
	 * Encryt block.
	 */
	for(i = 0; i < NUM_COMPUTE_ROUNDS; i++)
	{
		for(int j = 0; j < COMPUTE_ARRAY_LEN; j++)
		{
			compute_array[j] ^= MD2_PERM(t);
			t = compute_array[j];
		}

		t = (t + i) & 0xff; //modulo 256.
	}
	/*
	 * Update checksum.
	 */
	t = checksum[CONFIG_MD2_BLOCK_LEN - 1];

	for(i = 0; i < CONFIG_MD2_BLOCK_LEN; i++)
	{
		checksum[i]  ^= MD2_PERM(block[i] ^ t);
		t = checksum[i];
	}

	/*
	 * Update state and clean compute array.
	 */
	memcpy(state, compute_array, CONFIG_MD2_BLOCK_LEN);
	memset(compute_array, 0, sizeof(compute_array));
}

/**
 * Algorithm initialization.
 *
 * \param empty context.
 */
void md2_init(Md2Context *context)
{

	memset(context, 0, sizeof(Md2Context));

}

/**
 * Update block.
 */
void md2_update(Md2Context *context, const void *_block_in, size_t block_len)
{

	const uint8_t *block_in;
	size_t cpy_len;


	block_in = (const uint8_t *)_block_in;

	while(block_len > 0)
	{
		/*
		 * Choose a number of block that fill input context buffer.
		 */
		cpy_len = MIN(block_len, CONFIG_MD2_BLOCK_LEN - context->counter);

		
		/*
		 * Copy in the buffer input block.
		 */
		memcpy(&context->buffer[context->counter], block_in, cpy_len);

		/*
		 * Update a context counter, input block length and remaning 
		 * context buffer block lenght.
		 */
		context->counter += cpy_len;
		block_len -= cpy_len;
		block_in += cpy_len;

		/*
		 * If buffer is full, compute it.
		 */
		if (context->counter >= CONFIG_MD2_BLOCK_LEN)
		{
			md2_compute(context->state, context->checksum, context->buffer);
			context->counter = 0;
		}
	}


}
/**
 * Ends an MD2 message digest operation.
 * This fuction take an context and return a pointer
 * to context state.
 *
 * \param context in input.
 * \return a pointer to context state (message digest).
 */
uint8_t  *md2_end(Md2Context *context)
{

	uint8_t buf[CONFIG_MD2_BLOCK_LEN];

	/*
	 * Fill remaning empty context buffer.
	 */
	md2_pad(buf, CONFIG_MD2_BLOCK_LEN - context->counter);

	/*
	 * Update context buffer and compute it.
	 */
	md2_update(context, buf, CONFIG_MD2_BLOCK_LEN - context->counter);

	/*
	 * Add context checksum to message input.
	 */
	md2_update(context, context->checksum, CONFIG_MD2_BLOCK_LEN);


	return context->state; //return a pointer to message digest.
}
/**
 * MD2 test fuction.
 * This function test MD2 algorithm with a standard string specified
 * in RFC 1319.
 *
 * \note This test work with official array of 256 byte pemutation
 * contructed from digits of pi, defined in the RFC 1319.
 *
 */
bool md2_test(void)
{

	Md2Context context;

	const char *test[] =
	{
		"",
		"message digest",
		"abcdefghijklmnopqrstuvwxyz",
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890"
	};


	const uint8_t *result[] = {
		"\x83\x50\xe5\xa3\xe2\x4c\x15\x3d\xf2\x27\x5c\x9f\x80\x69\x27\x73",
		"\xab\x4f\x49\x6b\xfb\x2a\x53\x0b\x21\x9f\xf3\x30\x31\xfe\x06\xb0",
		"\x4e\x8d\xdf\xf3\x65\x02\x92\xab\x5a\x41\x08\xc3\xaa\x47\x94\x0b",
		"\xd5\x97\x6f\x79\xd8\x3d\x3a\x0d\xc9\x80\x6c\x3c\x66\xf3\xef\xd8",
	};


	for (int i = 0; i < countof(test); i++)
	{
		md2_init(&context);
		md2_update(&context, test[i], strlen(test[i]));

		if(memcmp(result[i], md2_end(&context), CONFIG_MD2_BLOCK_LEN))
			return false;
	}

	return true;
}

#if 0

#include <stdio.h>
int main(int argc, char * argv[])
{

	if(md2_test())
		printf("MD2 algorithm work well!\n");
	else
		printf("MD2 algorithm doesn't work well.\n");

}

#endif

