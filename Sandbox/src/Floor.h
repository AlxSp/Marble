#pragma once
#include "Nucleus.h"

static inline int32_t fastfloor1(float fp) {
    int32_t i = static_cast<int32_t>(fp);
    return (fp < i) ? (i - 1) : (i);
}

static inline int32_t fastfloor2(float fp) {
    int32_t i = static_cast<int32_t>(fp);
    if (i > fp) i--;
    return i;
}

static inline int32_t fastfloor3(float fp) {
    return static_cast<int32_t>(fp + 32768.) - 32768;
}