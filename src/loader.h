#ifndef LOADER_H
#define LOADER_H

#include "pch.h"

namespace pluginloader::loader {

/**
 * @brief Loads all plugins.

 * @param this_dll Handle to this dll's module.
 */
void load(HMODULE this_dll);

/**
 * @brief Frees all plugins.
 */
void free(void);

}  // namespace pluginloader::loader

#endif /* LOADER_H */
