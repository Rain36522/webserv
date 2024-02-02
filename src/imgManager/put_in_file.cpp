/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_in_file.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 10:08:32 by pudry             #+#    #+#             */
/*   Updated: 2024/02/02 17:38:40 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

int	put_in_file(std::string filename, std::string content, int length)
{
	int	fd;
	int	i;

	i = 0;
	fd = open(filename.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	i += write(fd, content.c_str(), length);
	DEBUG
	std::cout << "Write : " << i << ", of : " << length << " char\n";
	close(fd);
	return (i);
}
