/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:42:30 by mpierant          #+#    #+#             */
/*   Updated: 2025/01/27 19:43:03 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_printf.h"

int	ft_print_format(const char specifier, va_list ap)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count += ft_putchar(va_arg(ap, int));
	else if (specifier == 's')
		count += ft_putstr(va_arg(ap, char *));
	else if (specifier == 'd' || specifier == 'i')
		count += ft_putnbr(va_arg(ap, int));
	else if (specifier == 'u')
		count += ft_putnbr_u(va_arg(ap, unsigned int));
	else if (specifier == 'x')
		count += ft_putnbrhex_x(va_arg(ap, unsigned int));
	else if (specifier == 'X')
		count += ft_putnbrhex_xcapital(va_arg(ap, unsigned int));
	else if (specifier == '%')
		count += ft_putchar('%');
	else if (specifier == 'p')
		count += ft_putptr(va_arg(ap, void *));
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		count;
	int		i;

	va_start(ap, format);
	count = 0;
	i = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
			count += ft_print_format(format[++i], ap);
		else
			count += ft_putchar(format[i]);
		i++;
	}
	va_end(ap);
	return (count);
}

/*int	main(void)
{
	int	count;

	count = printf("Ciao %s sei all'89%% di %x hex, %X HEX, %u unsigned,
			%i signed\n", "Marco", 2759, 255, 4252, -2564);
	printf("%i caratteri stampati (incluso a capo)\n", count);
	count = ft_printf("Ciao %s sei all'89%% di %x hex, %X HEX, %u unsigned,
			%i signed\n", "Marco", 2759, 255, 4252, -2564);
		ft_printf("%i caratteri stampati (incluso a capo)\n", count);
	printf("\n\n");

	count = printf("indirizzo di count = %p\n", &count);
	printf("%i caratteri\n", count);
	count = ft_printf("indirizzo di count = %p\n", &count);
	ft_printf("%i caratteri\n", count);
	count = printf("NULL Pointer = %p\n", NULL);
	printf("%i caratteri\n", count);
	count = ft_printf("NULL Pointer = %p\n", NULL);
	ft_printf("%i caratteri\n", count);
	count = printf("stringa vuota = %s\n", "\0");
	printf("%i caratteri\n", count);
	count = ft_printf("stringa vuota = %s\n", "\0");
	ft_printf("%i caratteri\n", count);
	//printf("stringa NULL = %s\n", NULL);
	//ft_printf("stringa NULL = %s\n", NULL);
	count = printf("carattere 'e' = %c\n", 'e');
	printf("%i caratteri\n", count);
	count = ft_printf("carattere 'e' = %c\n", 'e');
	ft_printf("%i caratteri\n", count);
}*/
