/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getImg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:40:14 by pudry             #+#    #+#             */
/*   Updated: 2024/02/16 13:44:25 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"
#include "../../Request.hpp"

static std::string	getFileName(std::string str)
{
	size_t	i;
	size_t	j;

	i = str.find("filename=\"");
	if (i == std::string::npos)
		return ("");
	i += 10;
	j = str.find("\"", i);
	if (j == std::string::npos || i + 1 == j)
		return ("");
	return (str.substr(i, j - i));
}

static std::string	getHeader(std::string request)
{
	size_t	i;
	size_t	j;

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
	size_t	i;
	size_t	j;
	size_t	k;

	i = body.find("--" + header + "\r\n");
	if (i == std::string::npos)
		return ("");
	i = body.find("\r\n\r\n", i);
	if (i == std::string::npos)
	{
		std::cerr << RED << "Wrong end header\n" << RESET;
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
	validateFd(filename, EVFILT_WRITE);

	outfile.open(filename, std::ios::binary);
	if (outfile.fail())
	{
		std::cerr << ORANGE << "Opening fail error\n" << RESET;
		return (false);
	}
	outfile << content;
	outfile.close();
	return (true);
}

void	requestToFile(Request &request, std::string uploadPath)
{
	std::string	header;
	std::string	FileContent;
	std::string	FileName;

	FileContent = "";
	request.PostFile = false;
	header = getHeader(request.body);
	FileName = getFileName(request.body);
	if (header != "" || FileName != "")
		FileContent = getFileContent(header, request.body);
	else
		std::cerr << RED << "Header or filename invalide\n" << RESET;
	if (FileContent == "")
		std::cerr << RED << "Post methode with invalide file content" << RESET << std::endl;
	else
	{
		std::cout << GREEN << "Download image on : " << uploadPath + "/"+ FileName << std::endl << RESET;
		if (putInBinary(uploadPath + "/" + FileName, FileContent))
			request.PostFile = true;
	}
}
