# new

**new** is a small, experimental interpreted programming language built from scratch in **C++20**. It is a learning project focused on understanding how source code becomes tokens, syntax trees, and executable behavior.

The language uses several Nepali-inspired keywords and intentionally keeps its feature set small. The main goal is to explore interpreter architecture, recursive-descent parsing, runtime values, and lexical scope, not to compete with production programming languages.

> **Project status:** Experimental / work in progress. Variables, expressions, strings, output, conditionals, loops, and nested scopes are implemented. Function definitions can be registered and function calls can be parsed, but function-call execution and `firta` (return) are not yet complete on the current version of `main`.

## Example

```text
manam count := 0;

jabasamma count < 5 {
    chapa count;
    chapa "\n";
    count := count + 1;
}
```

Expected output:

```text
0
1
2
3
4
```

A conditional example:

```text
manam health := 75;

yedi health >= 80 {
    chapa "Healthy";
}
navaye health >= 50 {
    chapa "Recovering";
}
natra {
    chapa "Critical";
}
```

## Language overview

| Syntax | Meaning |
| --- | --- |
| `manam x := 10;` | Declare a variable |
| `x := 20;` | Assign to an existing variable |
| `chapa x;` | Print a value (no automatic newline) |
| `yedi condition { ... }` | Conditional block (`if`) |
| `navaye condition { ... }` | Additional conditional branch (`else if`) |
| `natra { ... }` | Fallback branch (`else`) |
| `jabasamma condition { ... }` | While loop |
| `kaam name(a, b) { ... }` | Function definition (registered, not yet fully callable) |
| `kaam_gar(name(1, 2))` | Function-call syntax (parser support; evaluator unfinished) |
| `firta` | Reserved return keyword (not yet implemented) |
| `~ comment` | Comment through end of line |

**Current runtime values:** numbers (`double`), strings (`std::string`), and internal function references (`FunctionValue`).

**Implemented expressions:** `+`, `-`, `*`, `/`, unary minus, parentheses, and comparisons `==`, `!=`, `<`, `<=`, `>`, `>=`. Strings support concatenation with `+` and equality/inequality comparisons.

Statements use semicolons; blocks use braces. The lexer also recognizes some additional operators and punctuation that the interpreter does not currently execute.

## Architecture

```text
Source code (test.txt)
        |
        v
Lexer       -> vector<Token>
        |
        v
Parser      -> AST (unique_ptr<Expr>)
        |
        v
Evaluator   -> Value
        |
        +---- Environment (name -> Value)
                   |
                   +---- optional parent Environment
```

- **Lexer:** Scans characters into tokens, tracks source lines, handles strings, comments, operators, and keywords.
- **Parser:** Uses recursive descent to build an abstract syntax tree (AST), with separate parsing stages for precedence and statements.
- **AST:** Represents literals, identifiers, operators, declarations, assignments, output, conditions, loops, and function syntax using polymorphic nodes owned by `std::unique_ptr`.
- **Evaluator:** Recursively evaluates AST nodes and performs runtime operations.
- **Environment:** Stores named `Value` objects and searches enclosing scopes through a parent pointer. Block statements execute in child environments.
- **FunctionValue:** Holds non-owning references to a function's definition and its defining Environment. Function execution and lifetime handling are still under development.

This separation is intentional: the lexer does not execute code, the parser does not resolve variable values, and the Environment does not need to understand individual AST node types.

## Build and run

Requirements:

- C++20 compiler
- CMake 3.20 or newer

```bash
git clone https://github.com/Rojan-Mahat/new.git
cd new
cmake -S . -B build
cmake --build build
```

The current executable reads the file named **`test.txt` in its working directory**. Edit that file with your program, then run from the repository root:

```bash
./build/new
```

The commands above use a Unix-like shell, such as macOS or Linux. Other platforms may place the executable in a different build directory.

## Tests

CMake defines four test executables:

```bash
./build/lexer_tests
./build/parser_tests
./build/environment_tests
./build/interpreter_tests
```

The existing tests cover portions of tokenization, expression parsing, environments, and evaluation. They are **not** yet comprehensive coverage of loops, function calls, recursion, or return propagation. A successful test run should not be treated as proof that those unfinished features work.

## Known limitations

- Function calls are parsed, but their evaluator path is incomplete.
- `firta` return statements are not implemented.
- Nested functions and escaping closures are not safe to assume: `FunctionValue` stores non-owning pointers to Environments.
- Arrays, classes, modules, and a standard library are not implemented.
- `chapa` does not automatically add a newline.
- Execution currently uses a fixed `test.txt` input file rather than command-line source-file arguments.
- This project is for learning, not security-sensitive or production workloads.

## Why I built it

I wanted to go beyond *using* a programming language and understand how one works internally: how text becomes tokens, how recursive parsing creates a tree, how runtime values are resolved, and how scopes interact.

The emphasis is on learning through implementation, maintaining clear component boundaries, and being able to explain the code rather than continuously adding features.

## License

No license has been specified yet. Until one is added, do not assume permission to reuse or redistribute the source under an open-source license.
