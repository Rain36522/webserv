/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getImg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:40:14 by pudry             #+#    #+#             */
/*   Updated: 2024/02/02 16:21:38 by pudry            ###   ########.fr       */
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
	if (i == std::string::npos)
		return ("");
	i += 11;
	j = request.find("\r\n", i);
	if (j == std::string::npos)
		return ("");
	return (request.substr(i, j - i));
}

static std::string	getFileContent(std::string header, std::string body)
{
	int	i;
	int	j;
	int	k;

	i = body.find("--" + header + "\r\n");
	if (i == std::string::npos)
		return ("");
	i = body.find("\r\n\r\n", i);
	if (i == std::string::npos)
	{
		std::cerr << "Wrong end header\n";
		return ("");
	}
	i += 4;
	j = i;
	k = 0;
	while (j != std::string::npos)
	{
		k = j;
		j = body.find("--" + header + "--\r\n", j + 1);
	}
	if (k == std::string::npos || k == i)
		return ("");
	return (body.substr(i, k - i - 2));
}

bool	putInBinary(std::string filename, std::string content)
{
	std::ofstream	outfile;

	outfile.open("test.png", std::ios::binary);
	if (outfile.fail())
	{
		std::cerr << "Opening fail error\n";
		return (false);
	}
	outfile << content;
	outfile.close();
	return (true);
}

HttpRequest	requestToFile(HttpRequest request)
{
	std::string	header;
	std::string	FileContent;

	FileContent = "";
	header = getHeader(request.body);
	request.FileName = getFileName(request.body);
	if (header != "" || request.FileName != "")
		FileContent = getFileContent(header, request.body);
	else
		std::cerr << "\033[94mHeader or filename invalide\033[39m\n";
	if (FileContent == "")
		std::cerr << "Post methode with invalide file content" << std::endl;
	else
		putInBinary(request.FileName, FileContent);
	return (request);
}