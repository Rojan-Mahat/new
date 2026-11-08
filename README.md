# new

A small interpreted language I'm writing in C++ to learn how programming languages work.

I started with a lexer, then added a parser that builds an AST, and an evaluator that runs it. I mainly wanted to understand these parts by implementing them myself.

Most of the keywords in the language are Nepali.

## What works

- Variables, numbers, and strings
- Arithmetic and comparisons
- `yedi`, `navaye`, and `natra` (if / else if / else)
- `jabasamma` (while loops)
- `chapa` (printing)
- Local scopes with parent environments
- Functions with parameters
- Functions call and recursion
- Return values (`firta`) with local scopes

The language is still experimental, and there are features I haven't implemented.

## Example

Here's a recursive factorial function written in `new`:

```text
kaam factorial(n) {
    yedi n <= 1 {
        firta 1;
    }

    firta n * kaam_gar(factorial(n - 1));
}

chapa kaam_gar(factorial(5));
```

Output:

```text
120
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

The automated tests were written with AI assistance and reviewed and run by me. They cover the lexer, parser, environment, and interpreter, including function calls and recursion.

To run all tests:

```bash
ctest --test-dir build --output-on-failure
```

## A few notes

This is an independent self-learning project. I built the lexer, parser, AST, evaluator, and environment to understand how an interpreted language works internally.

I used AI as a learning resource to discuss concepts, review code, and help with testing, but the interpreter itself was implemented by me.

There are still limitations and probably bugs I haven't discovered. My goal was to understand the process of building a language, not to make a production-ready one.
