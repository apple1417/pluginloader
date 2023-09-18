#ifndef CONSOLE_H
#define CONSOLE_H

namespace pluginloader::console {

/**
 * @brief If the `--debug` arg is specified, creates a new console window, and redirects the
 *        standard streams to point to it.
 */
void create_if_needed(void);

}  // namespace pluginloader::console

#endif /* CONSOLE_H */
