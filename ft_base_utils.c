#include "ft_printf.h"

// Safely get the length of a string (NULL protected)
size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str || !*str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

/**
 * @brief Prints a number in a given base
 * @param n The number to print
 * @param base The base to print the number in
 * @return `int` number of characters printed
 * @note The base must be a string with all the digits of the base in order
 */
int	ft_putnbr_base(long long n, const char *base)
{
	int		i;
	size_t	len;

	len = ft_strlen(base);
	i = 0;
	if (n < 0)
	{
		i += write(FD, "-", 1);
		n = -n;
	}
	if (n >= (long long)len)
		i += ft_putnbr_base(n / len, base);
	n = n % len;
	i += write(FD, &base[n], 1);
	return (i);
}

/**
 * @brief Prints an unsigned long number in a given base
 * @param n The number to print
 * @param base The base to print the number in
 * @return `int` number of characters printed
 */
int	ft_putunbr_base(unsigned long long n, const char *base)
{
	int		i;
	size_t	len;

	len = ft_strlen(base);
	i = 0;
	if (n >= (unsigned long long)len)
		i += ft_putunbr_base(n / len, base);
	n = n % len;
	i += write(FD, &base[n], 1);
	return (i);
}
