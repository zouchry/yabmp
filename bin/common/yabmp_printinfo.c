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

#include <assert.h>
#include "yabmp_printinfo.h"

void yabmp_printinfo(FILE* outstream, yabmp* bmp_reader, const yabmp_info* info)
{
	yabmp_uint32 l_width, l_height, l_compression_type, l_res_x, l_res_y;
	unsigned int l_bit_depth, l_scan_direction, l_color_type;
	unsigned int l_blue_bits, l_green_bits, l_red_bits, l_alpha_bits;
	unsigned int l_color_profile_type, l_color_profile_intent;
	
	assert(outstream != NULL);
	assert(bmp_reader != NULL);
	assert(info != NULL);

	/* Those calls can't fail with proper arguments */
	(void)yabmp_get_dimensions(bmp_reader, info, &l_width, &l_height);
	(void)yabmp_get_pixels_per_meter(bmp_reader, info, &l_res_x, &l_res_y);
	(void)yabmp_get_bit_depth(bmp_reader, info, &l_bit_depth);
	(void)yabmp_get_color_type(bmp_reader, info, &l_color_type);
	(void)yabmp_get_compression_type(bmp_reader, info, &l_compression_type);
	(void)yabmp_get_scan_direction(bmp_reader, info, &l_scan_direction);
	(void)yabmp_get_bits(bmp_reader, info, &l_blue_bits, &l_green_bits, &l_red_bits, &l_alpha_bits);
	(void)yabmp_get_color_profile_type(bmp_reader, info, &l_color_profile_type);
	(void)yabmp_get_color_profile_intent(bmp_reader, info, &l_color_profile_intent);
	
	fprintf(outstream, "Dimensions (WxH): %" YABMP_PRIu32 "x%" YABMP_PRIu32 "\n", l_width, l_height);
	if ((l_res_x != 0U) || (l_res_y != 0U)) {
		fprintf(outstream, "Pixels Per Meter (XxY): %" YABMP_PRIu32 "x%" YABMP_PRIu32 "\n", l_res_x, l_res_y);
	}
	fprintf(outstream, "Bit Depth: %u\n", l_bit_depth);
	fprintf(outstream, "Bits Per Channel: B%u.G%u.R%u.A%u\n", l_blue_bits, l_green_bits, l_red_bits, l_alpha_bits);
	fputs("Color mask: ", outstream);
	if ((l_color_type & YABMP_COLOR_MASK_PALETTE) != 0U) {
		fputs(" PALETTE", outstream);
	}
	if ((l_color_type & YABMP_COLOR_MASK_BITFIELDS) != 0U) {
		fputs(" BITFIELDS", outstream);
	}
	if ((l_color_type & YABMP_COLOR_MASK_COLOR) != 0U) {
		fputs(" COLOR", outstream);
	}
	if ((l_color_type & YABMP_COLOR_MASK_ALPHA) != 0U) {
		fputs(" ALPHA", outstream);
	}
	fputc('\n', outstream);
	
	if ((l_color_type & YABMP_COLOR_MASK_BITFIELDS) != 0U) {
		yabmp_uint32 blue_mask, green_mask, red_mask, alpha_mask;
		(void)yabmp_get_bitfields(bmp_reader, info, &blue_mask, &green_mask, &red_mask, &alpha_mask);
		fprintf(outstream, "Bitfields: B:0x%08" YABMP_PRIX32 " G:0x%08" YABMP_PRIX32 " R:0x%08" YABMP_PRIX32 " A:0x%08" YABMP_PRIX32 "\n", blue_mask, green_mask, red_mask, alpha_mask);
	}
		
	switch (l_compression_type) {
		case YABMP_COMPRESSION_NONE:
			fputs("Compression: NONE\n", outstream);
			break;
		case YABMP_COMPRESSION_RLE4:
			fputs("Compression: RLE4\n", outstream);
			break;
		case YABMP_COMPRESSION_RLE8:
			fputs("Compression: RLE8\n", outstream);
			break;
		default:
			fputs("Compression: UNKNOWN\n", outstream);
			break;
	}
	switch (l_scan_direction) {
		case YABMP_SCAN_BOTTOM_UP:
			fputs("Scan direction: BOTTOM-UP\n", outstream);
			break;
		case YABMP_SCAN_TOP_DOWN:
			fputs("Scan direction: TOP-DOWN\n", outstream);
			break;
		default:
			fputs("Scan direction: UNKNOWN\n", outstream);
			break;
	}
	switch (l_color_profile_type) {
		case YABMP_COLOR_PROFILE_NONE:
			fputs("Color profile type: NONE\n", outstream);
			break;
		case YABMP_COLOR_PROFILE_sRGB:
			fputs("Color profile type: sRGB\n", outstream);
			break;
		case YABMP_COLOR_PROFILE_CALIBRATED_RGB:
			fputs("Color profile type: CALIBRATED RGB\n", outstream);
			break;
		case YABMP_COLOR_PROFILE_ICC_EMBEDDED:
			fputs("Color profile type: ICC EMBEDDED\n", outstream);
			break;
		case YABMP_COLOR_PROFILE_ICC_LINKED:
			fputs("Color profile type: ICC LINKED\n", outstream);
			break;
		default:
			fputs("Color profile type: UNKNOWN\n", outstream);
			break;
	}
	switch (l_color_profile_intent) {
		case YABMP_COLOR_PROFILE_INTENT_NONE:
			fputs("Color profile intent: NONE\n", outstream);
			break;
		case YABMP_COLOR_PROFILE_INTENT_PERCEPTUAL:
			fputs("Color profile intent: PERCEPTUAL\n", outstream);
			break;
		case YABMP_COLOR_PROFILE_INTENT_RELCOL:
			fputs("Color profile intent: RELATIVE COLORIMETRIC\n", outstream);
			break;
		case YABMP_COLOR_PROFILE_INTENT_SATURATION:
			fputs("Color profile intent: SATURATION\n", outstream);
			break;
		case YABMP_COLOR_PROFILE_INTENT_ABSCOL:
			fputs("Color profile intent: ABSOLUTE COLORIMETRIC\n", outstream);
			break;
		default:
			fputs("Color profile intent: UNKNOWN\n", outstream);
			break;
	}
}
