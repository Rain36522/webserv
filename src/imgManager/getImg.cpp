/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getImg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:40:14 by pudry             #+#    #+#             */
/*   Updated: 2024/01/31 13:17:32 by pudry            ###   ########.fr       */
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
	if (i == std::string::npos || i + 1 == j)
		return ("");
	return (str.substr(i, j - i - 1));
}


bool downloadImage(int sockfd)
{
	std::string 	line;
	std::ofstream	outFile;
	if (!get_next_line(sockfd))
	{
		std::cerr << "Error geht file\n";
		return false;
	}
	line = get_next_line(sockfd);
	if (getFileName(line) == "")
		return false;
	outFile.open(getFileName(line));
	line = get_next_line(sockfd);
	line = get_next_line(sockfd);
	while (line != "")
	{
		outFile << line;
		line = get_next_line(sockfd);
	}
	outFile.close();
	return true;
}