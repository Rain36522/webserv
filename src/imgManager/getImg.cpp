/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getImg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:40:14 by pudry             #+#    #+#             */
/*   Updated: 2024/02/07 16:20:35 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

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
	std::cout << "File " << filename << std::endl;
	validateFd(filename, EVFILT_WRITE);

	outfile.open(filename, std::ios::binary);
	if (outfile.fail())
	{
		std::cerr << "Opening fail error\n";
		return (false);
	}
	outfile << content;
	outfile.close();
	return (true);
}

HttpRequest	requestToFile(HttpRequest request, std::string uploadPath)
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
		std::cerr << "\033[94mHeader or filename invalide\033[39m\n";
	if (FileContent == "")
		std::cerr << "\033[94mPost methode with invalide file content\033[39m" << std::endl;
	else
	{
		std::cout << "\033[94mDownload image on : " << uploadPath + FileName << "\033[39m\n";
		if (putInBinary(uploadPath + "/" + FileName, FileContent))
			request.PostFile = true;
	}
	return (request);
}
