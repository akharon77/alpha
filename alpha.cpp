#include <assert.h>
#include <stdlib.h>

#include "alpha.h"
#include "calc.h"

void AlphaConfigCtor(AlphaConfig *conf, const char *back_path, const char *front_path)
{
    assert(conf && "conf is NULL");

    assert( back_path &&  "back_path is NULL");
    assert(front_path && "front_path is NULL");

    conf-> back_img.loadFromFile (back_path);
    conf->front_img.loadFromFile(front_path);

    assert(conf->back_img.getSize() == conf->front_img.getSize() && "Front and back images don't have equal sizes");

    sf::Vector2u size = conf->back_img.getSize();

    conf->width        = size.x;
    conf->height       = size.y;

    conf->res          = (uint8_t*) calloc(conf->width * conf->height, sizeof(uint8_t) * BYTES_PER_PIXEL);

    assert(conf->res && "Result image allocation failed");
}

void AlphaConfigDtor(AlphaConfig *conf)
{
    conf->width  = 0;
    conf->height = 0;

    free(conf->res);
    conf->res = NULL;
}

