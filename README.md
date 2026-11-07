# new

A small interpreted language I'm writing in C++ to learn how programming languages work.

I started with a lexer, then added a parser that builds an AST, and an evaluator that runs it. I mainly wanted to understand these parts by implementing them myself.

Some of the keywords are Nepali.

## What works

- Variables, numbers, and strings
- Arithmetic and comparisons
- `yedi`, `navaye`, and `natra` (if / else if / else)
- `jabasamma` (while loops)
- `chapa` (printing)
- Local scopes with parent environments

Function definitions (`kaam`) and calls (`kaam_gar`) are being worked on. Calls aren't fully evaluated yet, and `firta` (return) isn't implemented.

## Example

```text
manam count := 0;

jabasamma count < 5 {
    chapa count;
    chapa "\n";
    count := count + 1;
}
```

## Building

Requires C++20 and CMake 3.20+.

```bash
cmake -S . -B build
cmake --build build
```

For now, the program reads code from `test.txt` in the current working directory.

```bash
./build/new
```

## Tests

There are separate test executables for the lexer, parser, environment, and interpreter:

```bash
./build/lexer_tests
./build/parser_tests
./build/environment_tests
./build/interpreter_tests
```

The tests don't cover everything yet, especially the newer function code.

## A few notes

The lexer turns text into tokens. The parser turns those tokens into AST nodes. The evaluator runs the AST and uses an Environment to store and look up variables.

This is a learning project, so there are still bugs and missing features. I plan to finish the function work, test what I have, and leave the language small rather than keep adding features.
