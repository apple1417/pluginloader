#ifndef LOADER_H
#define LOADER_H

namespace pluginloader::loader {

/**
 * @brief Loads all plugins.
 */
void load(void);

/**
 * @brief Frees all plugins.
 */
void free(void);

}  // namespace pluginloader::loader

#endif /* LOADER_H */
