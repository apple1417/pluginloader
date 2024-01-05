#include "pch.h"
#include "util.h"

namespace pluginloader::util {

std::string format_win_error(DWORD err) {
    LPSTR buf = nullptr;
    const size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&buf), 0,
        nullptr);

    std::string out{buf, size};

    LocalFree(buf);

    return out;
}
std::string format_last_win_error(void) {
    return format_win_error(GetLastError());
}

namespace {

/**
 * @brief Suspends or resumes all other threads in the process.
 *
 * @param resume True if to resume, false if to suspend them.
 */
void adjust_running_status(bool resume) {
    HANDLE thread_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (thread_snapshot == nullptr) {
        CloseHandle(thread_snapshot);
        return;
    }

    THREADENTRY32 te32{};
    te32.dwSize = sizeof(THREADENTRY32);

    if (Thread32First(thread_snapshot, &te32) == 0) {
        CloseHandle(thread_snapshot);
        return;
    }

    do {
        if (te32.th32OwnerProcessID != GetCurrentProcessId()
            || te32.th32ThreadID == GetCurrentThreadId()) {
            continue;
        }

        HANDLE thread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME,
                                   0, te32.th32ThreadID);
        if (thread != nullptr) {
            if (resume) {
                ResumeThread(thread);
            } else {
                SuspendThread(thread);
            }

            CloseHandle(thread);
        }
    } while (Thread32Next(thread_snapshot, &te32) != 0);

    CloseHandle(thread_snapshot);
}

}  // namespace

ThreadSuspender::ThreadSuspender(void) {
    adjust_running_status(false);
}
ThreadSuspender::~ThreadSuspender() {
    adjust_running_status(true);
}

}  // namespace pluginloader::util
