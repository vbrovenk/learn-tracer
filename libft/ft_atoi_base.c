/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 12:44:09 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/08/25 12:44:10 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_pow(int num, int exp)
{
	int i;
	int result;

	if (exp == 0)
		return (1);
	if (exp > 0)
	{
		result = 1;
		i = -1;
		while (++i < exp)
			result *= num;
		return (result);
	}
	return (-1);
}

static	int	char2nb(char symbol)
{
	if (symbol >= '0' && symbol <= '9')
		return (symbol - '0');
	else if (symbol >= 'A' && symbol <= 'F')
		return (symbol - 'A' + 10);
	else if (symbol >= 'a' && symbol <= 'f')
		return (symbol - 'a' + 10);
	else
		return (-1);
}

int			ft_atoi_base(char *number, int base)
{
	int result;
	int length;
	int i;
	int j;

	result = 0;
	length = ft_strlen(number);
	j = -1;
	i = length - 1;
	while (++j < length)
	{
		result += char2nb(number[j]) * ft_pow(base, i);
		i--;
	}
	return (result);
}
