#pragma once

#include <array>

#include "m25gpu.h"

#define BUFFER_SIZE 8

namespace ram {
    int read(int id);

    void write(int id, int value);
} // namespace ram