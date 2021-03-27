#pragma once

#define EXPORT __declspec( dllexport )
#define IMPORT __declspec( dllimport )

#ifdef JOYFULSTICKMAKEDLL
#define JOYFULSTICKEXPORT EXPORT
#else
#define JOYFULSTICKEXPORT IMPORT
#endif


#include <vector>
#include <array>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>
