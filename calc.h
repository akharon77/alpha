#ifndef CALC_H
#define CALC_H

#include "alpha.h"

const uint32_t PIXELS_PER_ITER = 16;

void AlphaCalcNoOpts (AlphaConfig *conf);
void AlphaCalcAVX512 (AlphaConfig *conf);

#endif
