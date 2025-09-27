# include "ft_printf.h"

// Tests
# include <stdio.h> // printf
# include <limits.h> // limits

# define GRAY "\033[1;30m"
# define RESET "\033[0m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"

void print_test_result(long ret, long ret2);

int main()
{
	char *str = "Boo";
	ssize_t ret;
	ssize_t ret2;

	ret = ft_printf("Characters: %c %c \n", 'A', 65);
	ret2 = printf("Characters: %c %c \n", 'A', 65);
	print_test_result(ret, ret2);

	ret = ft_printf("Multibyte char: %c%c\n", 0xC3, 0xA9);
	ret2 = printf("Multibyte char: %c%c\n", 0xC3, 0xA9);
	print_test_result(ret, ret2);

	ret = ft_printf("String: %s\n", "Hello, World!");
	ret2 = printf("String: %s\n", "Hello, World!");
	print_test_result(ret, ret2);

	// ret = ft_printf("Empty string: %s\n", "");
	// ret2 = printf("Empty string: %s\n", "");
	// print_test_result(ret, ret2);

	// str = NULL;
	// ret = ft_printf("Null string: %s\n", str);
	// ret2 = printf("Null string: %s\n", str);
	// print_test_result(ret, ret2);

	// str = NULL;
	// ret = ft_printf("Null pointer: %p\n", str);
	// ret2 = printf("Null pointer: %p\n", str);
	// print_test_result(ret, ret2);

	// ret = ft_printf("Decimals: %d\n", 1977);
	// ret2 = printf("Decimals: %d\n", 1977);
	// print_test_result(ret, ret2);

	// ret = ft_printf("Integers: %i\n", -1977);
	// ret2 = printf("Integers: %i\n", -1977);
	// print_test_result(ret, ret2);

	// ret = ft_printf("Unsigned: %u %u\n", 33000, -42);
	// ret2 = printf("Unsigned: %u %u\n", 33000, -42);
	// printf("Printed %ld characters\n", ret);
	// print_test_result(ret, ret2);

	// ret = ft_printf("uintmax: %u\n", UINT_MAX);
	// ret2 = printf("uintmax: %u\n", UINT_MAX);
	// print_test_result(ret, ret2);

	// ret = ft_printf("Hex: %x %X\n", 495, -495);
	// ret2 = printf("Hex: %x %X\n", 495, -495);
	// printf("Printed %ld characters\n", ret);
	// print_test_result(ret, ret2);

	// ret = ft_printf("Percent sign: %%\n");
	// ret2 = printf("Percent sign: %%\n");
	// print_test_result(ret, ret2);

	// ret = ft_printf("Mixed: %c %d %s %x %u\n", 'X', 10, "Test", 255, 256);
	// ret2 = printf("Mixed: %c %d %s %x %u\n", 'X', 10, "Test", 255, 256);
	// print_test_result(ret, ret2);

	// ret = ft_printf("Long string: %s\n", "This is a very long string to test the buffer system of the ft_printf function. It should handle it correctly and print everything.");
	// ret2 = printf("Long string: %s\n", "This is a very long string to test the buffer system of the ft_printf function. It should handle it correctly and print everything.");
	// print_test_result(ret, ret2);

	// ret = ft_printf("Multiple pointers: %p %p\n", &str, &ret);
	// ret2 = printf("Multiple pointers: %p %p\n", &str, &ret);
	// print_test_result(ret, ret2);

	// ret = ft_printf("Negative ptr: %p \n", (void *)-1);
	// ret2 = printf("Negative ptr: %p \n", (void *)-1);
	// print_test_result(ret, ret2);

	// ret = ft_printf("Max ptr: %p \n", (void *)UINTPTR_MAX);
	// ret2 = printf("Max ptr: %p \n", (void *)UINTPTR_MAX);
	// print_test_result(ret, ret2);

	// void *ptr = &ptr;
	// ret = ft_printf("Recursive ptr: %p \n", ptr);
	// ret2 = printf("Recursive ptr: %p \n", ptr);
	// print_test_result(ret, ret2);

	//! Warning: Unknown specifier, printf's behavior is undefined, flag needed
	//! to compile.
	// ret = ft_printf("%b %q %k %m %k\n");
	// ret2 = printf("%b %q %k %m %k\n");
	// print_test_result(ret, ret2);

	(void)str;
	(void)ret;
	(void)ret2;

	return 0;
}

/**
 * @brief Prints the test result in color
 * @param ret The return value of ft_printf
 * @param ret2 The return value of printf
 * @note If the return values are the same, it prints in green, otherwise in red
 */
void print_test_result(long ret, long ret2) {
	const char *color = (ret == ret2) ? GREEN : RED;
	printf("%sft: %ld, printf: %ld\n%s", color, ret, ret2, RESET);
	printf("%s --------------------%s\n", GRAY, RESET);
}

// Sanitize
// gcc -Wall -Wextra -Werror -g -fsanitize=address -I. -o a.out main.c ft_printf.c ft_base_utils.c ft_arg_utils.c
// ./a.out

// Valgrind
// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./a.out
// make re && gcc -Wall -Wextra -Werror -g main.c libftprintf.a -o a.out
