# RShell

A small POSIX-style shell.

## Supported syntax

- `cmd arg1 arg2 ...` — run an external command
- `cmd1 && cmd2` — run `cmd2` only if `cmd1` succeeds
- `cmd1 || cmd2` — run `cmd2` only if `cmd1` fails
- `cmd1 ; cmd2` — run both unconditionally, in order
- `( ... )` — group a sub-expression, e.g. `( a && b ) ; c`
- `test -e|-f|-d path` and `[ -e|-f|-d path ]` — check whether a path exists / is a regular
  file / is a directory
- `exit` — quit the shell

`&&` and `||` bind tighter than `;`, matching bash: `a ; b && c` runs `a` unconditionally, then
separately runs `b && c`.

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

The binary is written to `Build/<preset>/bin/rshell` (`Build/windows/bin/Debug/rshell.exe` on Windows).

## Testing

Tests use [GoogleTest](https://github.com/google/googletest) (fetched automatically via vcpkg).

```sh
ctest --preset linux-debug      # or windows-debug / windows-release / linux-release
```

or run the test binary directly (`Build/<preset>/bin/rshell_tests`,
`Build\windows\bin\Debug\rshell_tests.exe` on Windows) for GoogleTest's own output. Set
`-DRSHELL_BUILD_TESTS=OFF` at configure time to skip building the test suite.

## Running

```sh
$ ./Build/linux-debug/bin/rshell    # launch (the $ here is your OS shell)
$ echo hi && test -d /tmp       # this $ is RShell's own prompt
hi
(True)
$ exit
```
