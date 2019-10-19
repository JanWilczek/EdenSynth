//
// pch.h
// Header for standard system include files.
//

#pragma once

#define NOMINMAX	// to omit the max macro definition in windows.h

#include "gtest/gtest.h"
#include <array>
#include <memory>
#include <utility/MathConstants.h>

// Workaround for C++ 17 compilation failure.
// Source: https://github.com/Microsoft/TestAdapterForGoogleTest/issues/119
