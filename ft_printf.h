#ifndef FT_PRINTF_H
# define FT_PRINTF_H

// Constants
# define FD 1
# define SEL_CHAR 'c'
# define SEL_STR 's'
# define SEL_PTR 'p'
# define SEL_DEC 'd'
# define SEL_INT 'i'
# define SEL_UINT 'u'
# define SEL_HEX 'x'
# define SEL_HEXUP 'X'
# define SEL_PERCENT '%'

// Bases
# define BASE_DEC "0123456789"
# define BASE_HEX "0123456789abcdef"
# define BASE_HEXUP "0123456789ABCDEF"
// Strings
# define STR_NULL "(null)"
# define STR_NIL "(nil)"

// Libraries
# include <stdarg.h> // variadic functions
# include <unistd.h> // write
# include <stdint.h> // uintptr_t

// Selectors struct
typedef struct s_format
{
	char	selector;
	int		(*f)(va_list *args, const char selector);
}			t_format;

int		ft_printf(const char *format, ...);
// Basic utils
size_t	ft_strlen(const char *str);
int		ft_putnbr_base(long long n, const char *base);
int		ft_putunbr_base(unsigned long long n, const char *base);

// Functions on args
int		ft_sel_ptr(va_list *args, const char selector); // p
int		ft_sel_strs(va_list *args, const char selector); // c, s
int		ft_sel_nums(va_list *args, const char selector); // d, i, u
int		ft_sel_hexs(va_list *args, const char selector); // X, x
int		ft_sel_percent(va_list *args, const char selector); // %

#endif
