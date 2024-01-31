/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csil <csil@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:22:12 by pudry             #+#    #+#             */
/*   Updated: 2023/12/17 13:28:56 by csil             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strcat(char *s1, char *s2, int is2)
{
	int		i;
	int		is1;
	char	*ptr;

	is1 = 0;
	i = 0;
	while (s1[is1])
		is1 ++;
	ptr = (char *) ft_calloc_gnl(is1 + is2 + 1, sizeof(char));
	if (!ptr)
		return (NULL);
	ptr[is1 + is2] = '\0';
	while (i < is1)
	{
		ptr[i] = s1[i];
		i ++;
	}
	while (i < is1 + is2)
	{
		ptr[i] = s2[i - is1];
		i ++;
	}
	free (s1);
	return (ptr);
}

char	*ft_get_ptr_line(char *ptr, char *file, int fd)
{
	int	i;

	i = BUFFER_SIZE;
	if (!ptr)
	{
		ptr = (char *) ft_calloc_gnl(1, sizeof(char));
		ptr[0] = '\0';
	}
	while (i >= BUFFER_SIZE && !ft_strchr_endl(file))
	{
		i = read(fd, file, BUFFER_SIZE);
		if (i > 0)
			ptr = ft_strcat(ptr, file, i);
		if (!ptr)
			return (NULL);
		else if (!*ptr || i < 0)
		{
			free(ptr);
			return (NULL);
		}
	}
	return (ptr);
}

char	*ft_give_line(char *str)
{
	int		isize;
	int		i;
	char	*dup;

	isize = 0;
	while (str[isize] && str[isize] != '\n')
		isize ++;
	if (str[isize] == '\n')
		isize ++;
	dup = (char *) ft_calloc_gnl(isize + 1, sizeof(char));
	if (! dup)
		return (NULL);
	dup[isize] = '\0';
	i = 0;
	while (i < isize)
	{
		dup[i] = str[i];
		i ++;
	}
	return (dup);
}

char	*ft_remove_line(char *s)
{
	char	*ptr;
	char	*mem_s;

	mem_s = s;
	while (*s && *s != '\n')
		s ++;
	if (!*s)
	{
		free(mem_s);
		return (NULL);
	}
	if (*s == '\n')
		s ++;
	ptr = ft_strdup_gnl(s);
	free(mem_s);
	return (ptr);
}
