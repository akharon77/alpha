#include "calc.h"

#include <immintrin.h>
#include <stdint.h>

void AlphaCalcAVX512(AlphaConfig *conf)
{
    uint32_t pos = 0;

    const uint8_t *front_ptr = conf->front_img.getPixelsPtr();
    const uint8_t  *back_ptr = conf-> back_img.getPixelsPtr();

    for (uint32_t row = 0; row < conf->height; ++row)
    {
        for (uint32_t col = 0; col < conf->width; col += PIXELS_PER_ITER, pos += PIXELS_PER_ITER * BYTES_PER_PIXEL)
        {
            __m512i front = _mm512_loadu_si512(front_ptr + pos);

            __m256i front_low_8pixels  = _mm512_extracti32x8_epi32(front, 0);
            __m256i front_high_8pixels = _mm512_extracti32x8_epi32(front, 1);

            __m512i  front_low_8pixels_expanded = _mm512_cvtepu8_epi16( front_low_8pixels);
            __m512i front_high_8pixels_expanded = _mm512_cvtepu8_epi16(front_high_8pixels);

            __m512i  move_alpha = _mm512_set_epi8(
                                                1, 14, 1, 14, 1, 14, 1, 14, 1, 6, 1, 6, 1, 6, 1, 6, 
                                                1, 14, 1, 14, 1, 14, 1, 14, 1, 6, 1, 6, 1, 6, 1, 6, 
                                                1, 14, 1, 14, 1, 14, 1, 14, 1, 6, 1, 6, 1, 6, 1, 6, 
                                                1, 14, 1, 14, 1, 14, 1, 14, 1, 6, 1, 6, 1, 6, 1, 6
                                                );

            __m512i front_alpha_low_8pixels  = _mm512_shuffle_epi8( front_low_8pixels_expanded, move_alpha);
            __m512i front_alpha_high_8pixels = _mm512_shuffle_epi8(front_high_8pixels_expanded, move_alpha);

            __m512i max_alpha               = _mm512_set1_epi16(255);
            __m512i back_alpha_low_8pixels  = _mm512_subs_epu16(max_alpha, front_alpha_low_8pixels);
            __m512i back_alpha_high_8pixels = _mm512_subs_epu16(max_alpha, front_alpha_high_8pixels);

             front_low_8pixels_expanded = _mm512_mullo_epi16( front_low_8pixels_expanded, front_alpha_low_8pixels);
            front_high_8pixels_expanded = _mm512_mullo_epi16(front_high_8pixels_expanded, front_alpha_high_8pixels);

            __m512i back = _mm512_loadu_si512(back_ptr + pos);

            __m256i back_low_8pixels  = _mm512_extracti32x8_epi32(back, 0);
            __m256i back_high_8pixels = _mm512_extracti32x8_epi32(back, 1);

            __m512i  back_low_8pixels_expanded = _mm512_cvtepu8_epi16( back_low_8pixels);
            __m512i back_high_8pixels_expanded = _mm512_cvtepu8_epi16(back_high_8pixels);

             back_low_8pixels_expanded = _mm512_mullo_epi16( back_low_8pixels_expanded,  back_alpha_low_8pixels);
            back_high_8pixels_expanded = _mm512_mullo_epi16(back_high_8pixels_expanded, back_alpha_high_8pixels);

            __m256i sum_low  = _mm512_cvtepi16_epi8(_mm512_srli_epi16(_mm512_add_epi16( front_low_8pixels_expanded,  back_low_8pixels_expanded), 8));
            __m256i sum_high = _mm512_cvtepi16_epi8(_mm512_srli_epi16(_mm512_add_epi16(front_high_8pixels_expanded, back_high_8pixels_expanded), 8));

            __m512i sum = _mm512_inserti64x4(_mm512_castsi256_si512(sum_low), sum_high, 1);
            
            _mm512_storeu_si512(conf->res + pos, sum);
        }
    }
}

