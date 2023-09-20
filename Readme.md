# DLL Plugin Loader
[![Developer Discord](https://img.shields.io/static/v1?label=&message=Developer%20Discord&logo=discord&color=222)](https://discord.gg/VJXtHvh)

A simple pluginloader. Comes with multiple different system dll proxies.

To install:
1. Download one of the releases, and pick the relevant proxy dll for your game.
2. Place that dll in the same folder as the game executable.
3. Place any plugin dlls you want to load in the `Plugins` folder. This folder is also auto created
   when you launch the game.

# Development
To build:

1. Clone the repo (including submodules).
   ```sh
   git clone --recursive https://github.com/bl-sdk/pluginloader.git
   ```

2. Choose a preset, and run CMake. Most IDEs will be able to do this for you,
   ```
   cmake . --preset msvc-x64-debug
   cmake --build out/build/msvc-x64-debug
   ```
