# `ft_printf`

![42 School](https://img.shields.io/badge/42-Madrid-000000?style=flat&logo=42&logoColor=white)
![Score](https://img.shields.io/badge/Score-100%2F100-success)
![Language](https://img.shields.io/badge/Language-C-blue)
![Norminette](https://img.shields.io/badge/Norminette-passing-success)
![License](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)

Printf reimplementation in C using dispatch tables with function pointers for clean format specifier handling.

---

- [📔 Project overview](#-project-overview)
- [📚 Concept guide](#-concept-guide)
- [🔧 Struct array implementation](#-struct-array-implementation)
- [⚙️ Utility functions](#️-utility-functions)
- [💡 Potential improvements](#-potential-improvements)
- [📝 Notes](#-notes)
- [🛠️ Compilation and usage](#️-compilation-and-usage)
- [⚖️ License](#️-license)

## 📔 Project overview

- **Objective**: Implement a function, `ft_printf`, that recreates the behavior of the standard C library's `printf` function. The function must handle variadic arguments and format specifiers to produce formatted output.
- **Function signature**: `int ft_printf(const char *format, ...);` where `format` is the format string containing text and format specifiers.
- **Authorized functions**: `malloc`, `free`, `write`, `va_start`, `va_arg`, `va_copy`, `va_end`.
- **Returns**: 
	- ✔️ The number of characters printed to stdout.
	- ❌ Negative value if an error occurs during writing.
- **Supported specifiers**: `%c`, `%s`, `%p`, `%d`, `%i`, `%u`, `%x`, `%X`, `%%`.

### Learning objectives
- Master variadic functions and understand how `stdarg.h` works under the hood.
- Implement efficient format string parsing and argument type handling.
- Develop clean, maintainable code architecture using function pointers and dispatch tables.

#### Directives
- The function must handle the conversion specifiers: `c`, `s`, `p`, `d`, `i`, `u`, `x`, `X`, `%`.
- The function should mimic the behavior of the real `printf` function as closely as possible.
- No need to implement buffer management; direct `write` system calls are acceptable.
- Return value must be the number of characters printed, or -1 on error.
- `cc -Wall -Wextra -Werror ft_printf.c ft_arg_utils.c ft_base_utils.c`.
- Handle NULL pointers gracefully (print `(null)` for strings, `(nil)` for pointers).
- Undefined behavior for invalid format specifiers is acceptable but should be handled consistently.

## 📚 Concept guide

1. **Variadic functions**: Handle variable numbers of arguments using `stdarg.h` macros (`va_start`, `va_arg`, `va_end`).
2. **Dispatch tables**: Use function pointers instead of if/else chains for cleaner, more maintainable code.
3. **Format string parsing**: Iterate through format strings, distinguishing literal characters from `%` specifiers.
4. **Type promotion**: Handle automatic promotion of `char`/`short` to `int` in variadic functions.
5. **Base conversion**: Convert numbers between decimal and hexadecimal representations.
6. **Memory safety**: Robust handling of NULL pointers and edge cases.
7. **Direct output**: Write directly to file descriptors without internal buffering.

## 🔧 Struct array implementation

This project uses a **dispatch table** approach with function pointers instead of traditional if/else chains. This provides cleaner, more maintainable code and better performance through direct function pointer lookup.

### Dispatch table structure

```c
typedef struct s_format
{
	char	selector;
	int		(*f)(va_list *args, const char selector);
}			t_format;
```

### Core algorithm
The function parses the format string character by character. When a `%` is encountered, it looks up the next character in the dispatch table and calls the corresponding [handler function](#-utility-functions).
```c
int	ft_printf(const char *format, ...)
{
	va_list			args;
	int				ret;
	const t_format	handlers[9] = {
		{SEL_CHAR, &ft_sel_strs}, {SEL_STR, &ft_sel_strs},
		{SEL_PTR, &ft_sel_ptr},
		{SEL_DEC, &ft_sel_nums}, {SEL_INT, &ft_sel_nums}, {SEL_UINT, &ft_sel_nums},
		{SEL_HEX, &ft_sel_hexs}, {SEL_HEXUP, &ft_sel_hexs},
		{SEL_PERCENT, &ft_sel_percent}
	};

	ret = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == SEL_PERCENT)
		{
			format++;
			ret += ft_handle_specifier(*format, &args, handlers);
		}
		else
			ret += write(FD, format, 1);
		format++;
	}
	va_end(args);
	return (ret);
}
```

### Format specifier lookup
The `ft_handle_specifier` function iterates through the dispatch table to find the matching format specifier:
```c
static int	ft_handle_specifier(const char format_char, va_list *args,
						const t_format *handlers)
{
	int	ret;

	ret = 0;
	if (!format_char)
		return (-1);
	while (handlers->selector && handlers->selector != format_char)
		handlers++;
	if (handlers->selector == format_char)
		ret += handlers->f(args, handlers->selector);
	else
	{
		if (format_char != SEL_PERCENT)
			ret += write(FD, "%", 1);
		ret += write(FD, &format_char, 1);
	}
	return (ret);
}
```

## ⚙️ Utility functions

The project is organized into modular utility files for better code organization and maintainability:

### Handler functions (format specifier processors)

| Function | File | Format specifiers | Description |
|----------|------|-------------------|-------------|
| `ft_sel_strs` | `ft_arg_utils.c` | `%c`, `%s` | Handles character and string output. Promotes chars to unsigned char per stdarg.h specs |
| `ft_sel_ptr` | `ft_arg_utils.c` | `%p` | Handles pointer addresses. Converts to hexadecimal with "0x" prefix or prints "(nil)" for NULL |
| `ft_sel_nums` | `ft_arg_utils.c` | `%d`, `%i`, `%u` | Handles signed and unsigned decimal integers with appropriate type casting |
| `ft_sel_hexs` | `ft_arg_utils.c` | `%x`, `%X` | Handles hexadecimal conversion (lowercase and uppercase) |
| `ft_sel_percent` | `ft_arg_utils.c` | `%%` | Handles literal percent sign output |

### Base conversion utilities

| Function | File | Purpose |
|----------|------|---------|
| `ft_putnbr_base` | `ft_base_utils.c` | Converts numbers to specified base representation |
| `ft_putunbr_base` | `ft_base_utils.c` | Converts unsigned numbers to specified base representation |
| `ft_strlen` | `ft_base_utils.c` | Calculates string length for internal operations |

### Core functionality

| Function | File | Purpose |
|----------|------|---------|
| `ft_printf` | `ft_printf.c` | Main entry point. Parses format string and manages variadic arguments |
| `ft_handle_specifier` | `ft_printf.c` | Dispatch table lookup and handler function invocation |

## 💡 Potential improvements

- **Extended format specifier support**: The current struct array implementation could easily be extended to support additional format specifiers like `%f`, `%e`, `%g` for floating-point numbers, or field width and precision modifiers.

- **Buffered output optimization**: While the current implementation uses direct `write` system calls for simplicity, implementing an internal buffer could reduce the number of system calls and improve performance for large outputs.

- **Enhanced error reporting**: The current implementation returns -1 for write errors, but could be enhanced to provide more detailed error information, such as distinguishing between different types of failures (invalid file descriptor, disk full, etc.).


## 📝 Notes

### Design decisions
- **Struct array over if/else chains**: The dispatch table approach provides cleaner code organization, better maintainability, and easier extension compared to traditional cascading if/else statements.
- **Modular handler functions**: Each format type (strings, numbers, hexadecimal, etc.) is handled by specialized functions, promoting code reusability and easier debugging.
- **Consistent error handling**: All handler functions follow the same pattern for error detection and return value management.

### Performance considerations
- **Function pointer overhead**: While minimal, function pointer calls have slight overhead compared to direct function calls, but this is offset by the improved code organization and maintainability.

### 42 Norm compliance
- **Function length**: Each function stays within the 25-line limit through careful modularization.
- **Variable declarations**: All variables are declared at the beginning of their scope as required by the 42 Norm.
- **Function parameters**: No function exceeds the 4-parameter limit, with complex data passed via structures where needed.

## 🛠️ Compilation and usage

A comprehensive test file (`main.c`) is provided that compares the implementation against the standard `printf` function with extensive test cases.

### Compilation

```bash
# Compile the library
make

# Compile with the provided test main
cc -Wall -Wextra -Werror ft_printf.c ft_arg_utils.c ft_base_utils.c main.c
```

### Testing

```bash
# Run the comprehensive test suite
./a.out
```

The test file includes:
- **Basic format specifiers**: Characters, strings, pointers, decimals, integers, unsigned, hexadecimal, and percent signs
- **Edge cases**: NULL strings, NULL pointers, multibyte characters, negative values
- **Boundary conditions**: Maximum values, recursive pointers, long strings
- **Return value validation**: Compares character count with standard `printf`
- **Color-coded results**: Green for matching outputs, red for discrepancies

### Test categories available

The `main.c` file contains commented test sections that can be uncommented to test specific functionality:
- Character and multibyte character handling
- String operations including NULL and empty strings  
- Pointer address formatting with various edge cases
- Numeric conversions (decimal, integer, unsigned, hexadecimal)
- Mixed format specifier combinations
- Buffer and memory management with long strings
- Invalid format specifier handling (requires compiler flags)

## ⚖️ License

This project is licensed under the [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](LICENSE).
You're free to study, modify, and share this code for educational purposes, but commercial use is prohibited.
