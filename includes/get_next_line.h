/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 10:12:05 by pudry             #+#    #+#             */
/*   Updated: 2024/01/31 12:51:04 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>

int		ft_strchr_endl(char *ptr);
void	*ft_calloc_gnl(size_t count, size_t size);
char	*ft_strdup_gnl(const char *str);
char	*ft_get_ptr_line(char *ptr, char *file, int fd);
char	*ft_give_line(char *str);
char	*ft_remove_line(char *ptr);
char	*get_next_line(int fd);

#endif