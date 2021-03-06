/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Matthieu DARBOIS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../inc/private/yabmp_rtransforms.h"
#include "../inc/private/yabmp_struct.h"
#include "../inc/private/yabmp_stream.h"

#if defined(YABMP_BIG_ENDIAN)
YABMP_IAPI(void, yabmp_swap16u, (const yabmp* instance, yabmp_uint16* pSrcDst))
{
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrcDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width; ++x)
	{
		pSrcDst[x] = yabmp_bswap16(pSrcDst[x]);
	}
}
YABMP_IAPI(void, yabmp_swap32u, (const yabmp* instance, yabmp_uint32* pSrcDst))
{
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrcDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width; ++x)
	{
		pSrcDst[x] = yabmp_bswap32(pSrcDst[x]);
	}
}
#endif

YABMP_IAPI(void, yabmp_bitfield_get_shift_and_bits, (yabmp_uint32 mask, unsigned int* shift, unsigned int* bits))
{
	unsigned int l_shift, l_bits;
	
	assert(shift != NULL);
	assert(bits != NULL);
	
	l_shift = l_bits = 0U;
	
	if (mask != 0U) {
		/* get shift */
		while ((mask & 1U) == 0U) {
			mask >>= 1;
			l_shift++;
		}
		
		/* count bits */
		mask = mask - ((mask >> 1) & 0x55555555U);
		mask = (mask & 0x33333333U) + ((mask >> 2) & 0x33333333U);
		l_bits = (((mask + (mask >> 4)) & 0x0F0F0F0FU) * 0x01010101U) >> 24;
	}
	*shift = l_shift;
	*bits = l_bits;
}

YABMP_IAPI(void, yabmp_bf32u_to_bgr24, (const yabmp* instance, const yabmp_uint32* pSrc, yabmp_uint8* pDst ))
{
	unsigned int l_dummy_bits;
	unsigned int l_blue_shift, l_green_shift, l_red_shift;
	yabmp_uint32 l_blue_mask, l_green_mask, l_red_mask;
	
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	l_blue_mask  = instance->info2.mask_blue;
	l_green_mask = instance->info2.mask_green;
	l_red_mask   = instance->info2.mask_red;
	
	yabmp_bitfield_get_shift_and_bits(l_blue_mask, &l_blue_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_green_mask, &l_green_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_red_mask, &l_red_shift, &l_dummy_bits);
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint32 l_value = pSrc[x];
		
		pDst[3*x+0] = (l_value & l_blue_mask)  >> l_blue_shift;
		pDst[3*x+1] = (l_value & l_green_mask) >> l_green_shift;
		pDst[3*x+2] = (l_value & l_red_mask)   >> l_red_shift;
	}
}

YABMP_IAPI(void, yabmp_bf32u_to_bgr48, (const yabmp* instance, const yabmp_uint32* pSrc, yabmp_uint16* pDst ))
{
	unsigned int l_dummy_bits;
	unsigned int l_blue_shift, l_green_shift, l_red_shift;
	yabmp_uint32 l_blue_mask, l_green_mask, l_red_mask;
	
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	l_blue_mask  = instance->info2.mask_blue;
	l_green_mask = instance->info2.mask_green;
	l_red_mask   = instance->info2.mask_red;
	
	yabmp_bitfield_get_shift_and_bits(l_blue_mask, &l_blue_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_green_mask, &l_green_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_red_mask, &l_red_shift, &l_dummy_bits);
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint32 l_value = pSrc[x];
		
		pDst[3*x+0] = (l_value & l_blue_mask)  >> l_blue_shift;
		pDst[3*x+1] = (l_value & l_green_mask) >> l_green_shift;
		pDst[3*x+2] = (l_value & l_red_mask)   >> l_red_shift;
	}
}

YABMP_IAPI(void, yabmp_bf32u_to_bgra32, (const yabmp* instance, const yabmp_uint32* pSrc, yabmp_uint8* pDst ))
{
	unsigned int l_dummy_bits;
	unsigned int l_blue_shift, l_green_shift, l_red_shift, l_alpha_shift;
	yabmp_uint32 l_blue_mask, l_green_mask, l_red_mask, l_alpha_mask;
	
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	l_blue_mask  = instance->info2.mask_blue;
	l_green_mask = instance->info2.mask_green;
	l_red_mask   = instance->info2.mask_red;
	l_alpha_mask = instance->info2.mask_alpha;
	
	yabmp_bitfield_get_shift_and_bits(l_blue_mask, &l_blue_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_green_mask, &l_green_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_red_mask, &l_red_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_alpha_mask, &l_alpha_shift, &l_dummy_bits);
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint32 l_value = pSrc[x];
		
		pDst[4*x+0] = (l_value & l_blue_mask)  >> l_blue_shift;
		pDst[4*x+1] = (l_value & l_green_mask) >> l_green_shift;
		pDst[4*x+2] = (l_value & l_red_mask)   >> l_red_shift;
		pDst[4*x+3] = (l_value & l_alpha_mask) >> l_alpha_shift;
	}
}

YABMP_IAPI(void, yabmp_bf32u_to_bgra64, (const yabmp* instance, const yabmp_uint32* pSrc, yabmp_uint16* pDst ))
{
	unsigned int l_dummy_bits;
	unsigned int l_blue_shift, l_green_shift, l_red_shift, l_alpha_shift;
	yabmp_uint32 l_blue_mask, l_green_mask, l_red_mask, l_alpha_mask;
	
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	l_blue_mask  = instance->info2.mask_blue;
	l_green_mask = instance->info2.mask_green;
	l_red_mask   = instance->info2.mask_red;
	l_alpha_mask = instance->info2.mask_alpha;
	
	yabmp_bitfield_get_shift_and_bits(l_blue_mask, &l_blue_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_green_mask, &l_green_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_red_mask, &l_red_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits(l_alpha_mask, &l_alpha_shift, &l_dummy_bits);
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint32 l_value = pSrc[x];
		
		pDst[4*x+0] = (l_value & l_blue_mask)  >> l_blue_shift;
		pDst[4*x+1] = (l_value & l_green_mask) >> l_green_shift;
		pDst[4*x+2] = (l_value & l_red_mask)   >> l_red_shift;
		pDst[4*x+3] = (l_value & l_alpha_mask) >> l_alpha_shift;
	}
}

YABMP_IAPI(void, yabmp_bf16u_to_bgr24, (const yabmp* instance, const yabmp_uint16* pSrc, yabmp_uint8* pDst ))
{
	unsigned int l_dummy_bits;
	unsigned int l_blue_shift, l_green_shift, l_red_shift;
	yabmp_uint16 l_blue_mask, l_green_mask, l_red_mask;
	
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	l_blue_mask  = (yabmp_uint16)instance->info2.mask_blue;
	l_green_mask = (yabmp_uint16)instance->info2.mask_green;
	l_red_mask   = (yabmp_uint16)instance->info2.mask_red;
	
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_blue_mask, &l_blue_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_green_mask, &l_green_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_red_mask, &l_red_shift, &l_dummy_bits);
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint16 l_value = pSrc[x];
		
		pDst[3*x+0] = (l_value & l_blue_mask)  >> l_blue_shift;
		pDst[3*x+1] = (l_value & l_green_mask) >> l_green_shift;
		pDst[3*x+2] = (l_value & l_red_mask)   >> l_red_shift;
	}
}
YABMP_IAPI(void, yabmp_bf16u_to_bgr48, (const yabmp* instance, const yabmp_uint16* pSrc, yabmp_uint16* pDst ))
{
	unsigned int l_dummy_bits;
	unsigned int l_blue_shift, l_green_shift, l_red_shift;
	yabmp_uint16 l_blue_mask, l_green_mask, l_red_mask;
	
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	l_blue_mask  = (yabmp_uint16)instance->info2.mask_blue;
	l_green_mask = (yabmp_uint16)instance->info2.mask_green;
	l_red_mask   = (yabmp_uint16)instance->info2.mask_red;
	
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_blue_mask, &l_blue_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_green_mask, &l_green_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_red_mask, &l_red_shift, &l_dummy_bits);
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint16 l_value = pSrc[x];
		
		pDst[3*x+0] = (l_value & l_blue_mask)  >> l_blue_shift;
		pDst[3*x+1] = (l_value & l_green_mask) >> l_green_shift;
		pDst[3*x+2] = (l_value & l_red_mask)   >> l_red_shift;
	}
}
YABMP_IAPI(void, yabmp_bf16u_to_bgra32, (const yabmp* instance, const yabmp_uint16* pSrc, yabmp_uint8* pDst ))
{
	unsigned int l_dummy_bits;
	unsigned int l_blue_shift, l_green_shift, l_red_shift, l_alpha_shift;
	yabmp_uint16 l_blue_mask, l_green_mask, l_red_mask, l_alpha_mask;
	
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	l_blue_mask  = (yabmp_uint16)instance->info2.mask_blue;
	l_green_mask = (yabmp_uint16)instance->info2.mask_green;
	l_red_mask   = (yabmp_uint16)instance->info2.mask_red;
	l_alpha_mask = (yabmp_uint16)instance->info2.mask_alpha;
	
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_blue_mask, &l_blue_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_green_mask, &l_green_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_red_mask, &l_red_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_alpha_mask, &l_alpha_shift, &l_dummy_bits);
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint16 l_value = pSrc[x];
		
		pDst[4*x+0] = (l_value & l_blue_mask)  >> l_blue_shift;
		pDst[4*x+1] = (l_value & l_green_mask) >> l_green_shift;
		pDst[4*x+2] = (l_value & l_red_mask)   >> l_red_shift;
		pDst[4*x+3] = (l_value & l_alpha_mask) >> l_alpha_shift;
	}
}

YABMP_IAPI(void, yabmp_bf16u_to_bgra64, (const yabmp* instance, const yabmp_uint16* pSrc, yabmp_uint16* pDst ))
{
	unsigned int l_dummy_bits;
	unsigned int l_blue_shift, l_green_shift, l_red_shift, l_alpha_shift;
	yabmp_uint16 l_blue_mask, l_green_mask, l_red_mask, l_alpha_mask;
	
	yabmp_uint32 x, l_width;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_width      = (yabmp_uint32)instance->info2.width;
	l_blue_mask  = (yabmp_uint16)instance->info2.mask_blue;
	l_green_mask = (yabmp_uint16)instance->info2.mask_green;
	l_red_mask   = (yabmp_uint16)instance->info2.mask_red;
	l_alpha_mask = (yabmp_uint16)instance->info2.mask_alpha;
	
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_blue_mask, &l_blue_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_green_mask, &l_green_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_red_mask, &l_red_shift, &l_dummy_bits);
	yabmp_bitfield_get_shift_and_bits((yabmp_uint32)l_alpha_mask, &l_alpha_shift, &l_dummy_bits);
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint16 l_value = pSrc[x];
		
		pDst[4*x+0] = (l_value & l_blue_mask)  >> l_blue_shift;
		pDst[4*x+1] = (l_value & l_green_mask) >> l_green_shift;
		pDst[4*x+2] = (l_value & l_red_mask)   >> l_red_shift;
		pDst[4*x+3] = (l_value & l_alpha_mask) >> l_alpha_shift;
	}
}

YABMP_IAPI(void, yabmp_pal1_to_bgr24, (const yabmp* instance, const yabmp_uint8* pSrc, yabmp_uint8* pDst ))
{
	yabmp_uint32 x, l_width;
	const yabmp_color *l_palette;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_palette = instance->info2.palette;
	l_width = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width / 8U; ++x)
	{
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_current;
		
		l_current = (l_value >> 7) & 0x01;
		pDst[24*x+0] = l_palette[l_current].blue;
		pDst[24*x+1] = l_palette[l_current].green;
		pDst[24*x+2] = l_palette[l_current].red;
		
		l_current = (l_value >> 6) & 0x01;
		pDst[24*x+3] = l_palette[l_current].blue;
		pDst[24*x+4] = l_palette[l_current].green;
		pDst[24*x+5] = l_palette[l_current].red;
		
		l_current = (l_value >> 5) & 0x01;
		pDst[24*x+6] = l_palette[l_current].blue;
		pDst[24*x+7] = l_palette[l_current].green;
		pDst[24*x+8] = l_palette[l_current].red;
		
		l_current = (l_value >> 4) & 0x01;
		pDst[24*x+9] = l_palette[l_current].blue;
		pDst[24*x+10] = l_palette[l_current].green;
		pDst[24*x+11] = l_palette[l_current].red;
		
		l_current = (l_value >> 3) & 0x01;
		pDst[24*x+12] = l_palette[l_current].blue;
		pDst[24*x+13] = l_palette[l_current].green;
		pDst[24*x+14] = l_palette[l_current].red;
		
		l_current = (l_value >> 2) & 0x01;
		pDst[24*x+15] = l_palette[l_current].blue;
		pDst[24*x+16] = l_palette[l_current].green;
		pDst[24*x+17] = l_palette[l_current].red;
		
		l_current = (l_value >> 1) & 0x01;
		pDst[24*x+18] = l_palette[l_current].blue;
		pDst[24*x+19] = l_palette[l_current].green;
		pDst[24*x+20] = l_palette[l_current].red;
		
		l_current = (l_value >> 0) & 0x01;
		pDst[24*x+21] = l_palette[l_current].blue;
		pDst[24*x+22] = l_palette[l_current].green;
		pDst[24*x+23] = l_palette[l_current].red;
	}
	
	if (l_width & 7U) {
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_current;
		int l_remaining = (int)l_width & 7U;
		
		l_current = (l_value >> 7) & 0x01;
		pDst[24*x+0] = l_palette[l_current].blue;
		pDst[24*x+1] = l_palette[l_current].green;
		pDst[24*x+2] = l_palette[l_current].red;
		
		if (l_remaining > 1) {
			l_current = (l_value >> 6) & 0x01;
			pDst[24*x+3] = l_palette[l_current].blue;
			pDst[24*x+4] = l_palette[l_current].green;
			pDst[24*x+5] = l_palette[l_current].red;
		}
		if (l_remaining > 2) {
			l_current = (l_value >> 5) & 0x01;
			pDst[24*x+6] = l_palette[l_current].blue;
			pDst[24*x+7] = l_palette[l_current].green;
			pDst[24*x+8] = l_palette[l_current].red;
		}
		if (l_remaining > 3) {
			l_current = (l_value >> 4) & 0x01;
			pDst[24*x+9] = l_palette[l_current].blue;
			pDst[24*x+10] = l_palette[l_current].green;
			pDst[24*x+11] = l_palette[l_current].red;
		}
		if (l_remaining > 4) {
			l_current = (l_value >> 3) & 0x01;
			pDst[24*x+12] = l_palette[l_current].blue;
			pDst[24*x+13] = l_palette[l_current].green;
			pDst[24*x+14] = l_palette[l_current].red;
		}
		if (l_remaining > 5) {
			l_current = (l_value >> 2) & 0x01;
			pDst[24*x+15] = l_palette[l_current].blue;
			pDst[24*x+16] = l_palette[l_current].green;
			pDst[24*x+17] = l_palette[l_current].red;
		}
		if (l_remaining > 6) {
			l_current = (l_value >> 1) & 0x01;
			pDst[24*x+18] = l_palette[l_current].blue;
			pDst[24*x+19] = l_palette[l_current].green;
			pDst[24*x+20] = l_palette[l_current].red;
		}
	}
}

YABMP_IAPI(void, yabmp_pal2_to_bgr24, (const yabmp* instance, const yabmp_uint8* pSrc, yabmp_uint8* pDst ))
{
	yabmp_uint32 x, l_width;
	const yabmp_color *l_palette;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_palette = instance->info2.palette;
	l_width = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width / 4U; ++x)
	{
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_current;
		
		l_current = (l_value >> 6) & 0x03;
		pDst[12*x+0] = l_palette[l_current].blue;
		pDst[12*x+1] = l_palette[l_current].green;
		pDst[12*x+2] = l_palette[l_current].red;
		
		l_current = (l_value >> 4) & 0x03;
		pDst[12*x+3] = l_palette[l_current].blue;
		pDst[12*x+4] = l_palette[l_current].green;
		pDst[12*x+5] = l_palette[l_current].red;
		
		l_current = (l_value >> 2) & 0x03;
		pDst[12*x+6] = l_palette[l_current].blue;
		pDst[12*x+7] = l_palette[l_current].green;
		pDst[12*x+8] = l_palette[l_current].red;
		
		l_current = (l_value >> 0) & 0x03;
		pDst[12*x+9] = l_palette[l_current].blue;
		pDst[12*x+10] = l_palette[l_current].green;
		pDst[12*x+11] = l_palette[l_current].red;
	}
	
	if (l_width & 3U) {
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_current;
		int l_remaining = (int)l_width & 3U;
		
		l_current = (l_value >> 6) & 0x03;
		pDst[12*x+0] = l_palette[l_current].blue;
		pDst[12*x+1] = l_palette[l_current].green;
		pDst[12*x+2] = l_palette[l_current].red;
		
		if (l_remaining > 1) {
			l_current = (l_value >> 4) & 0x03;
			pDst[12*x+3] = l_palette[l_current].blue;
			pDst[12*x+4] = l_palette[l_current].green;
			pDst[12*x+5] = l_palette[l_current].red;
		}
		if (l_remaining > 2) {
			l_current = (l_value >> 2) & 0x03;
			pDst[12*x+6] = l_palette[l_current].blue;
			pDst[12*x+7] = l_palette[l_current].green;
			pDst[12*x+8] = l_palette[l_current].red;
		}
	}
}

YABMP_IAPI(void, yabmp_pal4_to_bgr24, (const yabmp* instance, const yabmp_uint8* pSrc, yabmp_uint8* pDst ))
{
	yabmp_uint32 x, l_width;
	const yabmp_color *l_palette;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_palette = instance->info2.palette;
	l_width = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width / 2U; ++x)
	{
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_value_lo = l_value & 0x0F;
		
		l_value >>= 4;
		
		pDst[6*x+0] = l_palette[l_value].blue;
		pDst[6*x+1] = l_palette[l_value].green;
		pDst[6*x+2] = l_palette[l_value].red;
		
		pDst[6*x+3] = l_palette[l_value_lo].blue;
		pDst[6*x+4] = l_palette[l_value_lo].green;
		pDst[6*x+5] = l_palette[l_value_lo].red;
	}
	
	if (l_width & 1U) {
		yabmp_uint8 l_value = pSrc[x] >> 4;
		
		pDst[6*x+0] = l_palette[l_value].blue;
		pDst[6*x+1] = l_palette[l_value].green;
		pDst[6*x+2] = l_palette[l_value].red;
	}
}

YABMP_IAPI(void, yabmp_pal8_to_bgr24, (const yabmp* instance, const yabmp_uint8* pSrc, yabmp_uint8* pDst ))
{
	yabmp_uint32 x, l_width;
	const yabmp_color *l_palette;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_palette = instance->info2.palette;
	l_width      = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint8 l_value = pSrc[x];
		
		pDst[3*x+0] = l_palette[l_value].blue;
		pDst[3*x+1] = l_palette[l_value].green;
		pDst[3*x+2] = l_palette[l_value].red;
	}
}

YABMP_IAPI(void, yabmp_pal1_to_y8, (const yabmp* instance, const yabmp_uint8* pSrc, yabmp_uint8* pDst ))
{
	yabmp_uint32 x, l_width;
	const yabmp_color *l_palette;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_palette = instance->info2.palette;
	l_width = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width / 8U; ++x)
	{
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_current;
		
		l_current = (l_value >> 7) & 0x01;
		pDst[8*x+0] = l_palette[l_current].blue;
		
		l_current = (l_value >> 6) & 0x01;
		pDst[8*x+1] = l_palette[l_current].blue;
		
		l_current = (l_value >> 5) & 0x01;
		pDst[8*x+2] = l_palette[l_current].blue;
		
		l_current = (l_value >> 4) & 0x01;
		pDst[8*x+3] = l_palette[l_current].blue;
		
		l_current = (l_value >> 3) & 0x01;
		pDst[8*x+4] = l_palette[l_current].blue;
		
		l_current = (l_value >> 2) & 0x01;
		pDst[8*x+5] = l_palette[l_current].blue;
		
		l_current = (l_value >> 1) & 0x01;
		pDst[8*x+6] = l_palette[l_current].blue;
		
		l_current = (l_value >> 0) & 0x01;
		pDst[8*x+7] = l_palette[l_current].blue;
	}
	
	if (l_width & 7U) {
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_current;
		int l_remaining = (int)l_width & 7U;
		
		l_current = (l_value >> 7) & 0x01;
		pDst[8*x+0] = l_palette[l_current].blue;
		
		if (l_remaining > 1) {
			l_current = (l_value >> 6) & 0x01;
			pDst[8*x+1] = l_palette[l_current].blue;
		}
		if (l_remaining > 2) {
			l_current = (l_value >> 5) & 0x01;
			pDst[8*x+2] = l_palette[l_current].blue;
		}
		if (l_remaining > 3) {
			l_current = (l_value >> 4) & 0x01;
			pDst[8*x+3] = l_palette[l_current].blue;
		}
		if (l_remaining > 4) {
			l_current = (l_value >> 3) & 0x01;
			pDst[8*x+4] = l_palette[l_current].blue;
		}
		if (l_remaining > 5) {
			l_current = (l_value >> 2) & 0x01;
			pDst[8*x+5] = l_palette[l_current].blue;
		}
		if (l_remaining > 6) {
			l_current = (l_value >> 1) & 0x01;
			pDst[8*x+6] = l_palette[l_current].blue;
		}
	}
}

YABMP_IAPI(void, yabmp_pal2_to_y8, (const yabmp* instance, const yabmp_uint8* pSrc, yabmp_uint8* pDst ))
{
	yabmp_uint32 x, l_width;
	const yabmp_color *l_palette;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_palette = instance->info2.palette;
	l_width = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width / 4U; ++x)
	{
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_current;
		
		l_current = (l_value >> 6) & 0x03;
		pDst[4*x+0] = l_palette[l_current].blue;
		
		l_current = (l_value >> 4) & 0x03;
		pDst[4*x+1] = l_palette[l_current].blue;
		
		l_current = (l_value >> 2) & 0x03;
		pDst[4*x+2] = l_palette[l_current].blue;
		
		l_current = (l_value >> 0) & 0x03;
		pDst[4*x+3] = l_palette[l_current].blue;
	}
	
	if (l_width & 3U) {
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_current;
		int l_remaining = (int)l_width & 3U;
		
		l_current = (l_value >> 6) & 0x03;
		pDst[4*x+0] = l_palette[l_current].blue;
		
		if (l_remaining > 1) {
			l_current = (l_value >> 4) & 0x03;
			pDst[4*x+1] = l_palette[l_current].blue;
		}
		if (l_remaining > 2) {
			l_current = (l_value >> 2) & 0x03;
			pDst[4*x+2] = l_palette[l_current].blue;
		}
	}
}

YABMP_IAPI(void, yabmp_pal4_to_y8, (const yabmp* instance, const yabmp_uint8* pSrc, yabmp_uint8* pDst ))
{
	yabmp_uint32 x, l_width;
	const yabmp_color *l_palette;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_palette = instance->info2.palette;
	l_width = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width / 2U; ++x)
	{
		yabmp_uint8 l_value = pSrc[x];
		yabmp_uint8 l_value_lo = l_value & 0x0F;
		
		l_value >>= 4;
		pDst[2*x+0] = l_palette[l_value].blue;
		pDst[2*x+1] = l_palette[l_value_lo].blue;
	}
	
	if (l_width & 1U) {
		yabmp_uint8 l_value = pSrc[x] >> 4;
		
		pDst[2*x+0] = l_palette[l_value].blue;
	}
}

YABMP_IAPI(void, yabmp_pal8_to_y8, (const yabmp* instance, const yabmp_uint8* pSrc, yabmp_uint8* pDst ))
{
	yabmp_uint32 x, l_width;
	const yabmp_color *l_palette;
	
	assert(instance != NULL);
	assert(pSrc != NULL);
	assert(pDst != NULL);
	
	l_palette = instance->info2.palette;
	l_width = (yabmp_uint32)instance->info2.width;
	
	for(x = 0U; x < l_width; ++x)
	{
		yabmp_uint8 l_value = pSrc[x];
		
		pDst[x] = l_palette[l_value].blue;
	}
}
