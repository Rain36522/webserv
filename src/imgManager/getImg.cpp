/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getImg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:40:14 by pudry             #+#    #+#             */
/*   Updated: 2024/02/01 13:46:25 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

std::string	getFileName(std::string str)
{
	int	i;
	int	j;

	i = str.find("filename=\"");
	if (i == std::string::npos)
		return ("");
	i += 10;
	j = str.find("\"\n", i);
	if (j == std::string::npos || i + 1 == j)
		return ("");
	return (str.substr(i, j - 3 - i));
}

bool downloadImage(int sockfd, HttpRequest HttpStruct)
{
	std::string	request;
	int			fd;
	int			i = 0;

	DEBUG
	processFileUpload(HttpStruct.body);
	// fd = open("test.png", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	// i += write(fd, request.c_str(), HttpStruct.RequestLength);
	// DEBUG
	// std::cout << "Write : " << i << ", of : " << HttpStruct.RequestLength << " char\n";
	// close(fd);
	return true;
}
