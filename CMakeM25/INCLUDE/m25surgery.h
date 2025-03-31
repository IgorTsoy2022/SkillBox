#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "m25common.h"

struct COORD {
	double x = 0;
	double y = 0;
};

void incision(const COORD& begin, const COORD& end);

void hemostat(const COORD& point);

void tweezers(const COORD& point);

bool suture(const COORD& incision_begin, const COORD& incision_end,
	        const COORD& suture_begin, const COORD& suture_end);

void surgery();