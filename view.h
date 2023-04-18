#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>

#include "alpha.h"

const int32_t MAX_FRAME_CNT = 1000;

#ifdef _MEASURE
const int32_t ACCURANCY     = 100;
#else
const int32_t ACCURANCY     = 1;
#endif

void  AlphaGetImage (const AlphaConfig *conf, sf::Image *img);
void  AlphaRun      (      AlphaConfig *conf);

float GetFPS       (sf::Clock *clock, int32_t *frame_cnt);

#endif  // VIEW_H
