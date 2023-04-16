#include <SFML/Graphics.hpp>

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
    sf::Image back_img = {};
    back_img.loadFromFile("Table.bmp");

    sf::Image front_img = {};
    front_img.loadFromFile("AskhatCat1.bmp");

    uint8_t *res = (uint8_t*) calloc(800 * 600 * 4, sizeof(uint8_t));
    sf::Image res_img = {};
    res_img.create(800, 600);

    for (int32_t row = 0; row < 600; ++row)
    {
        for (int32_t col = 0; col < 800; col += 16)
        {
            __m512i front = _mm512_loadu_si512(front_img.getPixelsPtr() + (row * 800 + col) * 4);

            __m256i front_low_8pixels  = _mm512_extracti32x8_epi32(front, 0);
            __m256i front_high_8pixels = _mm512_extracti32x8_epi32(front, 1);

            __m512i  front_low_8pixels_expanded = _mm512_cvtepu8_epi16( front_low_8pixels);
            __m512i front_high_8pixels_expanded = _mm512_cvtepu8_epi16(front_high_8pixels);

            const uint8_t ZERO = 1;

            __m512i  move_alpha = _mm512_set_epi8(
                                                ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 6, ZERO, 6, ZERO, 6, ZERO, 6, 
                                                ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 6, ZERO, 6, ZERO, 6, ZERO, 6, 
                                                ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 6, ZERO, 6, ZERO, 6, ZERO, 6, 
                                                ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 6, ZERO, 6, ZERO, 6, ZERO, 6
                                                );

            __m512i front_alpha_low_8pixels  = _mm512_shuffle_epi8( front_low_8pixels_expanded, move_alpha);
            __m512i front_alpha_high_8pixels = _mm512_shuffle_epi8(front_high_8pixels_expanded, move_alpha);

            __m512i back_alpha_low_8pixels  = _mm512_subs_epu16(_mm512_set1_epi16(255), front_alpha_low_8pixels);
            __m512i back_alpha_high_8pixels = _mm512_subs_epu16(_mm512_set1_epi16(255), front_alpha_high_8pixels);

             front_low_8pixels_expanded = _mm512_mullo_epi16( front_low_8pixels_expanded, front_alpha_low_8pixels);
            front_high_8pixels_expanded = _mm512_mullo_epi16(front_high_8pixels_expanded, front_alpha_high_8pixels);

            __m512i back = _mm512_loadu_si512(back_img.getPixelsPtr() + (row * 800 + col) * 4);

            __m256i back_low_8pixels  = _mm512_extracti32x8_epi32(back, 0);
            __m256i back_high_8pixels = _mm512_extracti32x8_epi32(back, 1);

            __m512i  back_low_8pixels_expanded = _mm512_cvtepu8_epi16( back_low_8pixels);
            __m512i back_high_8pixels_expanded = _mm512_cvtepu8_epi16(back_high_8pixels);

             back_low_8pixels_expanded = _mm512_mullo_epi16( back_low_8pixels_expanded,  back_alpha_low_8pixels);
            back_high_8pixels_expanded = _mm512_mullo_epi16(back_high_8pixels_expanded, back_alpha_high_8pixels);

            __m256i sum_low  = _mm512_cvtepi16_epi8(_mm512_srli_epi16(_mm512_add_epi16( front_low_8pixels_expanded,  back_low_8pixels_expanded), 8));
            __m256i sum_high = _mm512_cvtepi16_epi8(_mm512_srli_epi16(_mm512_add_epi16(front_high_8pixels_expanded, back_high_8pixels_expanded), 8));

            __m512i sum = _mm512_inserti64x4(_mm512_castsi256_si512(sum_low), sum_high, 1);
            
            _mm512_storeu_si512(res + (row * 800 + col) * 4, sum);
        }
    }

    for (int32_t i = 0; i < 600; ++i)
    {
        for (int32_t j = 0; j < 800; ++j)
        {
            res_img.setPixel(j, i, sf::Color(
                                            res[(i * 800 + j) * 4],
                                            res[(i * 800 + j) * 4 + 1],
                                            res[(i * 800 + j) * 4 + 2],
                                            res[(i * 800 + j) * 4 + 3]
                                            ));
        }
    }

    res_img.saveToFile("ans.bmp");

    return 0;
}

