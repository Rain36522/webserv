/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getImg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:40:14 by pudry             #+#    #+#             */
/*   Updated: 2024/02/02 15:31:28 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

static std::string	getFileName(std::string str)
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

static std::string	getHeader(std::string request)
{
	int	i;
	int	j;

	i = request.find("; boundary=----");
	DEBUG
	if (i == std::string::npos)
		return ("");
	i += 11;
	j = request.find("\r\n", i);
	if (j == std::string::npos)
		return ("");
	return (request.substr(i, j - i - 1));
}

static std::string	getFileContent(std::string header, std::string body)
{
	int	i;
	int	j;
	int	k;

	i = body.find("--" + header + "\n");
	DEBUG
	if (i == std::string::npos)
		return ("");
	DEBUG
	i = body.find("\n\n", i);
	if (i == std::string::npos)
		return ("");
	DEBUG
	i += 2;
	j = i;
	k = 0;
	while (j != std::string::npos)
	{
		k = j;
		j = body.find("--" + header + "\r--", j);
	}
	if (k == std::string::npos || k == i)
		return ("");
	DEBUG
	return (body.substr(i, j - i - 1));
}

HttpRequest	requestToFile(HttpRequest request)
{
	std::string	header;

	// std::cout << "\033[94mBody :\n";
	// std::cout << request.body << std::endl;
	// std::cout << "\033[92mFile content\n";
	// std::cout << request.FileName << std::endl << std::endl;
	// std::cout << request.FileContent << std::endl;
	// std::cout << "\033[39m";
	DEBUG
	request.FileContent = "";
	header = getHeader(request.body);
	// std::cout << ""
	request.FileName = getFileName(request.body);
	if (header != "" || request.FileName != "")
		request.FileContent = getFileContent(header, request.body);
	else
		std::cout << "\033[94mHeader or filename invalide\033[39m\n";
	DEBUG
	std::cout << "\033[94m" << request.FileContent << "\033[39m\n";
	if (request.FileContent == "")
		std::cerr << "Post methode with invalide file content" << std::endl;
	return (request);
}