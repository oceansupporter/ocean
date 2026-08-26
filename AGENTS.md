# Ocean coding instructions

These rules apply to all code below `impl/`.

## General

- Use C++20.
- Match surrounding code when this file does not specify a convention.
- Consult `doc/style-guide.md` when more detail is required.
- Do not modify code outside the requested scope.

## Formatting

- Indent code with tabs. Do not indent namespaces.
- Put opening and closing braces on separate lines.
- Put one expression per line.
- Put spaces around binary operators, but not unary operators.
- Put a space after flow-control keywords: `if (...)`, `for (...)`.
- Prefer lines of at most 160 characters. Longer straightforward declarations are permitted.
- Bind `*` and `&` to the type: `Type* pointer`, `Type& reference`.

## Naming

- Classes and enums use upper camel case.
- Functions and variables use lower camel case.
- Non-static and static data members have a trailing underscore.
- Template type parameters start with `T`; template value parameters start with `t`.
- Enum values use uppercase names with the enum-specific prefix.
- Do not encode types in variable names.
- Avoid abbreviations unless they are established domain terminology.

## Types and declarations

- Use `nullptr`, never `NULL` or `0`, for null pointers.
- Use `using Name = Type`, not `typedef`, in new code.
- Use explicit variable types. Do not use `auto` unless an explicit type cannot reasonably be written.
- Prefer references for required non-null objects and pointers for optional objects.
- Express ownership with an owning type; raw pointers and references do not express ownership.
- Make values `const` when they do not change.
- Use `override` for every override and `final` where further overriding is prohibited.
- Use `static inline`, never `inline static`.
- Use `constexpr`, `noexcept`, and `[[nodiscard]]` where their semantic guarantees apply.

## Functions and classes

- Prefer named functions or function objects over lambdas.
- Use a lambda only when required as a short, local, self-explanatory callable.
- Lambda captures must be explicit and minimal. Do not use `[&]` or `[=]`.
- Do not put complex control flow or independently documentable behavior in a lambda.
- Keep declarations and definitions separate except for templates and inline functions.
- Do not expose public data members.
- Setter names start with `set`; getter names normally omit `get`.
- Declare functions `static` when they do not depend on object state.

## Documentation and validation

- Document public declarations in headers with Doxygen.
- Use `@param`, `@return`, and `@see`.
- Document valid ranges for parameters, return values, and variables whenever meaningful.
- Use explicit intervals such as `[0, 255]`, `[0, infinity)`, and `(0, infinity)`.
- Document sentinel values, nullability, ownership, and lifetime requirements.
- Ensure documented ranges and preconditions have corresponding checks where practical.
- Use `ocean_assert` for internal programming errors.
- Use runtime `if` checks for external or recoverable invalid input.
- Assertions must not replace checks needed for correctness in release builds.
- Use `ocean_assert_and_suppress_unused(condition, variable)` only when `variable` would otherwise be unused because the assertion disappears in release builds.
- Assertion expressions must not contain required side effects.

## Includes and portability

- Use conventional include guards, not `#pragma once`.
- Include Ocean headers with quotation marks and external headers with angle brackets.
- Group includes by source and sort each group alphabetically.
- Put library dependencies in build files, not source-code pragmas.
- Isolate unavoidable platform-specific code behind appropriate preprocessor checks.

## Before finishing

- Review edited declarations for documentation and accurate ranges.
- Verify that implementation checks match documented inclusive and exclusive boundaries.
- Build or test the smallest relevant target when practical.
- Do not leave compiler warnings in changed code.
