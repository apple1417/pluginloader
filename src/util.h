#ifndef UTIL_H
#define UTIL_H

namespace pluginloader::util {

/**
 * @brief Converts a windows error code into it's string.
 *
 * @param err The error code.
 * @return The error string.
 */
std::string format_win_error(DWORD err);
std::string format_last_win_error(void);

/**
 * @brief RAII class which suspends all other threads for it's lifespan.
 */
class ThreadSuspender {
   public:
    ThreadSuspender(void);
    ~ThreadSuspender();

    ThreadSuspender(const ThreadSuspender&) = delete;
    ThreadSuspender(ThreadSuspender&&) = delete;
    ThreadSuspender& operator=(const ThreadSuspender&) = delete;
    ThreadSuspender& operator=(ThreadSuspender&&) = delete;
};

}  // namespace pluginloader::util

#endif /* UTIL_H */
