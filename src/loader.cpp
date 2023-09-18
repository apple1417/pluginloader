#include "pch.h"
#include "loader.h"
#include "util.h"

namespace pluginloader::loader {

const constexpr auto PLUGINS_DIR_NAME = "Plugins";

std::vector<HMODULE> loaded_modules{};
DLL_DIRECTORY_COOKIE plugins_cookie = nullptr;

void load(void) {
    auto plugins_dir = std::filesystem::current_path() / PLUGINS_DIR_NAME;
    if (!std::filesystem::exists(plugins_dir)) {
        std::filesystem::create_directories(plugins_dir);
        return;
    }

    // While path does have a `.c_str()`, it's type isn't necessarily stable.
    // Use the explicit wstring version to be safe
    plugins_cookie = AddDllDirectory(plugins_dir.wstring().c_str());
    if (plugins_cookie == nullptr) {
        std::cerr << "Failed to add plugins folder as dll directory: "
                  << util::format_last_win_error() << '\n'
                  << std::flush;

        // Continue to try load plugins anyway
    }

    for (const auto& entry : std::filesystem::directory_iterator{plugins_dir}) {
        const auto& path = entry.path();
        if (path.extension() != ".dll") {
            continue;
        }

        auto handle = LoadLibraryW(path.wstring().c_str());
        if (handle == nullptr) {
            std::cerr << "Failed to load plugin '" << path << "': " << util::format_last_win_error()
                      << '\n'
                      << std::flush;
        } else {
            loaded_modules.emplace_back(handle);
        }
    }
}

void free(void) {
    if (plugins_cookie != nullptr) {
        RemoveDllDirectory(plugins_cookie);
        plugins_cookie = nullptr;
    }

    for (const auto& module : loaded_modules) {
        FreeLibrary(module);
    }
    loaded_modules.clear();
}

}  // namespace pluginloader::loader
