/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csil <csil@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:01:18 by pudry             #+#    #+#             */
/*   Updated: 2023/12/17 13:29:54 by csil             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr_endl(char *ptr)
{
	while (*ptr)
	{
		if (*ptr == '\n')
			return (1);
		ptr ++;
	}
	return (0);
}

char	*ft_strdup_gnl(const char *str)
{
	int		isize;
	int		i;
	char	*dup;

	isize = 0;
	while (str[isize])
		isize ++;
	dup = (char *) ft_calloc_gnl(isize + 1, sizeof(char));
	if (! dup)
		return (NULL);
	dup[isize] = '\0';
	i = 0;
	while (str[i])
	{
		dup[i] = str[i];
		i ++;
	}
	return (dup);
}

void	*ft_calloc_gnl(size_t count, size_t size)
{
	char	*str;
	size_t	i;

	str = (char *) malloc(count * size);
	if (!str)
		return (NULL);
	i = 0;
	while (i < count * size)
		str[i ++] = 0;
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*ptr;
	char		*line;
	char		*file;

	if (!ptr)
		ptr = 0;
	file = (char *) ft_calloc_gnl(BUFFER_SIZE + 1, sizeof(char));
	if (!file)
		return (NULL);
	file[BUFFER_SIZE] = '\0';
	ptr = ft_get_ptr_line(ptr, file, fd);
	free(file);
	if (!ptr)
		return (NULL);
	line = ft_give_line(ptr);
	ptr = ft_remove_line(ptr);
	if (!ft_strchr_endl(line))
		free(ptr);
	return (line);
}
/*
int	main(void)
{
	int		fd;
	int		i;
	char	*line;
	
	fd = open("test.txt", O_RDWR);
	i = 0;
	printf("________________________________________________\n");
	printf("\\                                              /\n");
	printf("|=================================TEST=========|\n");
	printf("/----------------------------------------------\\\n");
	while(i ++ <= 2)
	{
		line = get_next_line(fd);
		printf("%i. line : %s\n", i, line);
		printf("-----------------------------------------\n");
	}
}
*/