/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:23:58 by mberila           #+#    #+#             */
/*   Updated: 2024/11/26 10:36:42 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <unistd.h>

int	ft_putchar(char c);
int	ft_printf(const char *str, ...);
int	ft_putstr(char *str);
int	ft_puthex_adr(size_t n, char sp);
int	ft_putnbr(int n, int is_signed);

#endif