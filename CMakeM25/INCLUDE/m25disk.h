#pragma once

#include <fstream>
#include <string>

#include "m25common.h"
#include "m25ram.h"

namespace disk {
    void load(const std::string& filename, const int nums);

    void save(const std::string& filename, const int nums);
} // namespace disk