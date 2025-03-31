#include "m25cpu.h"

namespace cpu {
    int compute(const int nums) {
        int result = 0;
        for (int i = 0; i < nums; ++i) {
            result += ram::read(i);
        }
        return result;
    }
}