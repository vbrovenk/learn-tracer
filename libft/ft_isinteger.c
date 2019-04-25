/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isinteger.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 18:10:59 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 18:11:01 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_isinteger(char *line)
{
	int		num;
	char	*itoa;
	int		res;

	num = ft_atoi(line);
	itoa = ft_itoa(num);
	if (ft_strequ(itoa, line))
		res = 1;
	else
		res = 0;
	ft_strdel(&itoa);
	return (res);
}
