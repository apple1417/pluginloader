#include "pch.h"
#include "loader.h"
#include "util.h"

namespace pluginloader::loader {

namespace {

const constexpr auto PLUGINS_DIR_NAME = "Plugins";

std::vector<HMODULE> loaded_modules{};

}  // namespace

void load(HMODULE this_dll) {
    wchar_t buf[MAX_PATH];
    if (GetModuleFileNameW(this_dll, &buf[0], ARRAYSIZE(buf)) == 0) {
        buf[0] = '\0';
    }

    auto plugins_dir = std::filesystem::path{buf}.parent_path() / PLUGINS_DIR_NAME;
    if (!std::filesystem::exists(plugins_dir)) {
        std::filesystem::create_directories(plugins_dir);
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator{plugins_dir}) {
        const auto& path = entry.path();
        if (path.extension() != ".dll") {
            continue;
        }

        // While path does have a `.c_str()`, it's type isn't necessarily stable.
        // Use the explicit wstring version to be safe
        // Include the DLL dir in the search path, so `Plugins/a.dll` can depend on `Plugins/b.dll`
        auto handle =
            LoadLibraryExW(path.wstring().c_str(), nullptr,
                           LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
        if (handle == nullptr) {
            std::cerr << "Failed to load plugin " << path << ": " << util::format_last_win_error()
                      << std::flush;
        } else {
            loaded_modules.emplace_back(handle);
        }
    }
}

void free(void) {
    for (const auto& module : loaded_modules) {
        FreeLibrary(module);
    }
    loaded_modules.clear();
}

}  // namespace pluginloader::loader
