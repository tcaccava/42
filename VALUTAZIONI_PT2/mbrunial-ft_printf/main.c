#include "ft_printf.h"
#include <limits.h>
#include <stdio.h>
int	main(void)
{
	char			c = 'A';
	char			*null = " esaminatori";
	int				i = 0;
	unsigned int	u = UINT_MAX;
	int				x = 48879;

	// printf("===== %%c =====\n");
	// printf("printf    : [%c]\n", c);
	// ft_printf("ft_printf : [%c]\n\n", c);


	// printf("===== %%s (NULL) =====\n");
	// printf("printf    : [%a]\n");
	ft_printf("%a\n");


	// printf("===== %%p =====\n");
	// printf("printf    : [%p]\n", NULL);
	// ft_printf("ft_printf : [%p]\n\n", NULL);

	// printf("===== %%d =====\n");
	// printf("printf    : [%d]\n", i);
	// ft_printf("ft_printf : [%d]\n\n", i);

	// printf("printf    : [%d]\n", INT_MIN);
	// ft_printf("ft_printf : [%d]\n\n", INT_MIN);

	// printf("printf    : [%d]\n", INT_MAX);
	// ft_printf("ft_printf : [%d]\n\n", INT_MAX);

	// printf("===== %%i =====\n");
	// printf("printf    : [%i]\n", 12345);
	// ft_printf("ft_printf : [%i]\n\n", 12345);

	// printf("===== %%u =====\n");
	// printf("printf    : [%u]\n", u);
	// ft_printf("ft_printf : [%u]\n\n", u);

	// printf("===== %%x =====\n");
	// printf("printf    : [%x]\n", x);
	// ft_printf("ft_printf : [%x]\n\n", x);

	// printf("printf    : [%x]\n", UINT_MAX);
	// ft_printf("ft_printf : [%x]\n\n", UINT_MAX);

	// printf("===== %%X =====\n");
	// printf("printf    : [%X]\n", x);
	// ft_printf("ft_printf : [%X]\n\n", x);

	// printf("===== %%%% =====\n");
	// printf("printf    : [%%]\n");
	// ft_printf("ft_printf : [%%]\n");

	return (0);
}
