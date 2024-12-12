#pragma once

#define NOMINMAX
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include "SimpleIni.h"

namespace logs = SKSE::log;
using namespace std::literals;
