#ifndef FINDROOT_H
#define FINDROOT_H

#include "postfix.h"

#define MAX_ITER 1000    // Số lần lặp tối đa
#define EPSILON 1e-4     // Ngưỡng sai số chấp nhận được

float newtonRaphson(Token *postfix);
float bisectionMethod(Token *postfix);
float secantMethod(Token *postfix,float a, float b);

#endif // FINDROOT_H
