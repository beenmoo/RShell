# RShell

A small POSIX-style shell supporting `&&`, `||`, `;`, `(...)`, and `test`/`[ ]`.

## Building

Requires [CMake](https://cmake.org/) 3.21+ and [vcpkg](https://vcpkg.io/); set `VCPKG_ROOT`
to your vcpkg checkout.

```sh
# Windows (MSVC, auto-detected)
cmake --preset windows
cmake --build --preset windows-debug

# Linux
cmake --preset linux-debug
cmake --build --preset linux-debug
```

The binary is written to `Build/<preset>/rshell` (`Build/windows/Debug/rshell.exe` on Windows).
