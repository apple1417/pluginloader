#include "pch.h"

// Empty proxy for dlls where we don't need to forward anything.

namespace pluginloader::proxy {

void init(HMODULE /*this_dll*/) {}

}  // namespace pluginloader::proxy
