# `ft_printf`

- [📔 Project overview](#-project-overview)
- [📚 Concept guide](#-concept-guide)
- [🔧 Struct array implementation](#-struct-array-implementation)
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

1. **Variadic functions**: A variadic function can accept a variable number of arguments. The `stdarg.h` library provides macros (`va_start`, `va_arg`, `va_end`) to access these arguments sequentially.
2. **Format string parsing**: The format string contains both literal characters and format specifiers (starting with `%`). Parsing involves iterating through the string and identifying when to print literal characters versus when to process format specifiers.
3. **Function pointers**: Instead of using lengthy if/else chains, function pointers allow for cleaner code by creating a dispatch table that maps format specifiers to their corresponding handler functions.
4. **Type promotion in variadic functions**: When passed to variadic functions, `char` and `short` are promoted to `int`, and `float` is promoted to `double`. This affects how arguments are retrieved with `va_arg`.
5. **Base conversion algorithms**: Converting numbers to different bases (decimal, hexadecimal) requires understanding positional notation and character mapping.
6. **Memory safety**: Proper handling of NULL pointers, invalid format specifiers, and write system call failures.
7. **Direct output approach**: Unlike the standard `printf` which uses internal buffering, this implementation writes directly to the file descriptor for simplicity.
8. **Edge case handling**: Managing scenarios like NULL string pointers, NULL address pointers, negative numbers, and malformed format strings.

## 🔧 Struct array implementation

This project was implemented using a struct array (dispatch table) to handle format specifiers instead of the conventional if/else chain approach. The struct array implementation not only provides cleaner, more maintainable code but also offers better performance through direct function pointer lookup and makes the code easily extensible for additional format specifiers.

### Struct array structure

```c
typedef struct s_format
{
	char	selector;
	int		(*f)(va_list *args, const char selector);
}			t_format;
```
`selector`: The format specifier character (`c`, `s`, `p`, `d`, `i`, `u`, `x`, `X`, `%`).
`f`: Function pointer to the handler function for this specific format specifier.

### Main function
The function parses the format string character by character. When a `%` is encountered, it looks up the next character in the dispatch table and calls the corresponding handler function.
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

### Dispatch table functions

##### `int ft_sel_strs(va_list *args, const char selector);`
Handles character (`%c`) and string (`%s`) format specifiers. Promotes chars to unsigned `char` as per `stdarg.h` specifications.

##### `int ft_sel_ptr(va_list *args, const char selector);`
Handles pointer (`%p`) format specifier. Converts memory addresses to hexadecimal representation with "0x" prefix, or prints "(nil)" for NULL pointers.

##### `int ft_sel_nums(va_list *args, const char selector);`
Handles signed decimal (`%d`, `%i`) and unsigned decimal (`%u`) format specifiers. Uses appropriate casting and base conversion functions.

##### `int ft_sel_hexs(va_list *args, const char selector);`
Handles hexadecimal format specifiers (`%x` for lowercase, `%X` for uppercase). Uses base conversion with appropriate character sets.

##### `int ft_sel_percent(va_list *args, const char selector);`
Handles the literal percent (`%%`) format specifier by simply printing a `%` character.

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
