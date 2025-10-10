# js-obfuscator

A small C++ JavaScript obfuscator that tokenizes and parses a JavaScript source file into an AST, applies simple obfuscation passes (identifier renaming, string table extraction, numeric obfuscation), and emits obfuscated JavaScript code.

This repository contains a lightweight, educational obfuscator implemented in modern C++ with a minimal lexer, parser, AST representation, and an `Obfuscator` class that transforms the AST and generates code.

The project is meant for collaborative development: improving parsing accuracy, adding obfuscation strategies, improving code generation, and hardening correctness and performance.

## Goals

- Provide a clear, maintainable implementation of a JavaScript lexer, parser, AST, and simple obfuscation passes.
- Be easy to extend with new obfuscation techniques (control-flow flattening, dead code insertion, constant folding/encoding, etc.).
- Serve as a learning codebase for compiler and obfuscation techniques.

## Table of contents

- Features
- Quick start (build & run)
- Command-line usage
- Project layout
- Contributing
- Development notes & next steps
- License

## Features

- Tokenize JavaScript source into tokens (lexer)
- Parse tokens into a simplified AST (parser)
- Obfuscate identifiers and extract string table (obfuscator)
- Generate obfuscated JavaScript code

## Quick start (build & run)

This project uses CMake and targets Visual Studio on Windows, but it should build on any platform with a C++17-compatible compiler and CMake.

Prerequisites

- CMake 3.10 or newer
- A C++17 compiler (MSVC, clang, or gcc)
- Ninja or Visual Studio (recommended on Windows)

Build (Windows / Visual Studio)

1. Open the generated solution in `build/CursiObfuscator.sln` with Visual Studio and build the `Release` or `Debug` configuration.

Or using CMake from the command line (PowerShell):

```powershell
cd .\build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

After a successful build, the executable will be available under `build/bin/Release` (or the Visual Studio output directory configured by your generator).

Run the obfuscator

1. Place a JavaScript file to obfuscate in `test/input.js` (a sample `test/input.js` exists).
2. Run the executable with the input file path as argument. From `build/bin/Release` (or from the project root) run:

```powershell
.\cursiobfuscator.exe ..\test\input.js
```

By default the current implementation writes the obfuscated output to `test/output.js` (the path is currently hard-coded in `src/main.cc`). The program also prints a representation of the obfuscated AST to stdout.

## Command-line usage

Current usage is minimal. The executable expects a single argument:

```
Usage: cursiobfuscator <input.js>
```

Notes:

- The output filename and additional options are not yet available as CLI flags. See "Development notes & next steps" for suggestions and tasks to implement richer CLI behavior.

## Project layout

Top-level files and directories (high-level):

- `CMakeLists.txt` — CMake build script
- `src/` — C++ source files (lexer, parser, obfuscator, main)
- `include/` — public headers for core components
- `test/` — example input and expected output files
- `build/` — build artifacts and generated Visual Studio solution (ignored in VCS for collaborators)

Key source files

- `src/main.cc` — CLI entrypoint: reads input file, runs lexer, parser, obfuscator, writes `test/output.js`.
- `include/lexer.h`, `src/lexer.cc` — lexical analysis (tokenizer)
- `include/parser.h`, `src/parser.cc` — parser building AST nodes
- `include/obfuscator.h`, `src/obfuscator.cc` — obfuscation passes and codegen

## Contributing

Contributions are welcome. Below are guidelines to help you get started and make contributions easily reviewable.

How to contribute

1. Fork the repository and create a topic branch for your change.
2. Add tests or example scripts if you modify behavior. Keep changes small and focused.
3. Follow the existing code style (clear, commented C++17). Add header comments for new public functions/types.
4. Open a pull request with a clear description of what you changed and why. Reference any related issues.

Code review checklist (what maintainers will look for)

- Correctness: the obfuscation must preserve semantics for supported constructs.
- Tests: new features should include basic tests or example input/output.
- Clarity: prefer simple, well-documented implementations over clever one-liners.
- Build: ensure the project builds with CMake on at least one platform and configuration.

Areas to help with (good first issues)

- Add CLI flags for output path and obfuscation options.
- Improve parser coverage for more JavaScript constructs.
- Implement more obfuscation passes: control-flow flattening, string encoding, dead code insertion.
- Add a test harness to run `test/input.js` -> `test/output.js` and compare semantics or behavior.

## Development notes & next steps

Recommended improvements and considerations:

- Make the output path configurable via CLI.
- Add unit tests around the lexer, parser, and obfuscator.
- Add a small test runner script (PowerShell or cross-platform) to automate build + run + compare.
- Improve error handling and messaging when parsing/obfuscation fail.
- Consider using an established JavaScript front-end library if the goal moves towards production-grade obfuscation.

Security and legal

- This project is intended for educational purposes. Be mindful of legal and ethical considerations when using obfuscation on third-party code.

## License

This project includes a `LICANSE` file in the repository root; please review and follow the license terms when contributing or reusing code.

## Contact and maintainers

Open issues and pull requests on the repository. Include a short description, reproduction steps, and any logs or error messages when filing issues.

---

