#include "ft_printf.h"

/**
 * @brief Prints a string or a char depending on flag (selector)
 * @param args The arguments list to print
 * @param selector The flag to print a string or a char
 * @return Number of characters printed
 * @line 9 cast int to unsigned char as stdarg.h spec states va_arg only takes
 * fully promoted types and printf(3) to do it as unsigned char
 */
int	ft_sel_strs(va_list *args, const char selector)
{
	int				i;
	char			*str;
	unsigned char	c;

	i = 0;
	str = NULL;
	if (selector == SEL_CHAR)
	{
		c = (unsigned char)va_arg(*args, int);
		i += write(FD, &c, 1);
	}
	else if (selector == SEL_STR)
	{
		str = va_arg(*args, char *);
		if (!str)
			return (i += write(FD, STR_NULL, 6));
		i += write(FD, str, ft_strlen(str));
	}
	return (i);
}

/**
 * @brief Prints a pointer address in hexadecimal
 * @param args The arguments list to print
 * @param selector The flag to print a pointer address
 * @return Number of characters printed
 * @note If the pointer is NULL, it prints "(nil)" as is the standard behavior
 * in printf on Linux
 * @line 5 check SEL_PTR to avoid unused variable warning
 */
int	ft_sel_ptr(va_list *args, const char selector)
{
	int		i;
	void	*ptr;

	ptr = va_arg(*args, void *);
	if (!ptr && selector == SEL_PTR)
		return (write(FD, STR_NIL, 5));
	i = 0;
	i += write(FD, "0x", 2);
	i += ft_putunbr_base((uintptr_t)ptr, BASE_HEX);
	return (i);
}

/**
 * @brief Prints hexadecimal numbers in lower or upper case
 * @param args The arguments list to print
 * @param selector The flag to print in lower or upper case
 * @return Number of characters printed
 * @note `u_int` to retrieve (printf), `ull` for putunbr_base
 */
int	ft_sel_hexs(va_list *args, const char selector)
{
	unsigned int	hex_value;

	hex_value = va_arg(*args, unsigned int);
	if (selector == SEL_HEX)
		return (ft_putunbr_base(hex_value, BASE_HEX));
	else
		return (ft_putunbr_base(hex_value, BASE_HEXUP));
}

/**
 * @brief Prints unsigned integers or decimals (d, i)
 * @param args The arguments list to print
 * @param selector The flag to print a decimal or an unsigned integer
 * @return Number of characters printed
 */
int	ft_sel_nums(va_list *args, const char selector)
{
	if (selector == SEL_UINT)
		return (ft_putunbr_base(va_arg(*args, unsigned int), BASE_DEC));
	else
		return (ft_putnbr_base(va_arg(*args, int), BASE_DEC));
}

/**
 * @brief Prints a percent sign
 * @param args The arguments list to print
 * @param selector The flag to print a percent sign
 * @return Number of characters printed
 * @note The args and selector are not used in this function but kept due to
 * the function signature/general logic of the program.
 */
int	ft_sel_percent(va_list *args, const char selector)
{
	return (write(FD, "%", 1));
	(void)args;
	(void)selector;
}
