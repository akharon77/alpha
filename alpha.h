#ifndef ALPHA_H
#define ALPHA_H

#include <SFML/Graphics.hpp>

#include <stdint.h>

extern const char *DEFAULT_BACKGROUND;
extern const char *DEFAULT_FOREGROUND;

const uint32_t BYTES_PER_PIXEL = 4;

struct AlphaConfig
{
    sf::Image  back_img;
    sf::Image front_img;

    uint32_t   width;
    uint32_t   height;

    uint8_t  *res;
};

void AlphaConfigCtor (AlphaConfig *conf, const char *back_path, const char *front_path);
void AlphaConfigDtor (AlphaConfig *conf);

#endif  // ALPHA_H
