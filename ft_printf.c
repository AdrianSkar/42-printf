#include "ft_printf.h"

/**
 * @brief Processes the format specifier and calls the corresponding function
 * 	within the handlers array.
 * @param format_char The character selector to handle
 * @param args The `va_list` of arguments
 * @param handlers The array of handlers holding specifier functions
 * @return Number of characters printed
 * @note This function is called when a '%' is found in the format string. If
 * 	a valid specifier is found, the corresponding function is called. Otherwise,
 * 	'%' is printed followed by the next character (undefined behavior but
 * 	mimics the behavior of printf(3)).
 */
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

/**
 * @brief Produce output according to a format as described in the format string
 * @param format string that contains the text to be written to stdout
 * @param ... additional arguments
 * @return Number of characters printed
 * @note This is a simplified version of the printf function (cspdiuxX%)
 * @note A pointer to `args` is passed to each function to process the args list
 * 	itself and not a copy of the list. This way, the list is processed in order
 * 	and we can avoid undefined behavior and/or va_copy.
 */
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
