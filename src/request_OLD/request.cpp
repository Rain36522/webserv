/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:29:51 by pudry             #+#    #+#             */
/*   Updated: 2024/02/15 16:56:14 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

// Fonction pour recevoir une requête HTTP du client
void receiveHTTPRequest(int client_fd, int RequestLength, HttpRequest &request) 
{
	const int 		bufferSize = 1024;
	char 			buffer[bufferSize];
	ssize_t 		bytesRead;
	std::string		buf2;

	bytesRead = bufferSize - 1;
	request.errorCode = 200;
	while (bytesRead == bufferSize - 1 || request.length < RequestLength)
	{
		bytesRead = read(client_fd, buffer, bufferSize - 1);
		request.length += bytesRead;
		if (bytesRead < 0)
		{
			DEBUG
			request.errorCode = 500;
			return ;
		}
		buf2 = std::string(buffer, bytesRead);
		request.body += buf2;
		for (int j = 0; j < bufferSize; j++)
			buffer[j] = '\0';
	}
}

static std::string	setHostPort(std::string httpRequest)
{
	size_t			i;
	size_t			j;

	i = httpRequest.find("Host: ") + 6;
	if (i == std::string::npos)
		return ("");
	j = httpRequest.find("\n", i);
	if (j == std::string::npos)
		return ("");
	return (httpRequest.substr(i, j - i - 1));
}

//TODO: make more rigourous (ex 127.0.0.1/GET)
static m_type	setMethod(std::string httpRequest)
{
	if (httpRequest.find("GET ") == 0)
		return (_GET);
	else if (httpRequest.find("DELETE ") == 0)
		return (_DEL);
	else if (httpRequest.find("POST") == 0)
		return (_POST);
	else
		return (_UNKNOW);
}

static void	getLogin(HttpRequest &request)
{
	size_t	i;
	size_t	j;

	if ((i = request.body.find("name=\"username\"")) == std::string::npos)
	{
		request.errorCode = 500;
		return;
	}
	i += 19;
	if ((j = request.body.find("\r\n", i)) == std::string::npos)
	{
		request.errorCode = 500;
		return;
	}
	request.parameters.push_back(request.body.substr(i, j - i));
	if ((i = request.body.find("name=\"pwd\"")) == std::string::npos)
	{
		request.errorCode = 500;
		return;
	}
	i += 14;
	if ((j = request.body.find("\r\n", i)) == std::string::npos)
	{
		request.errorCode = 500;
		return;
	}
	request.type = _LOGIN;
	request.parameters.push_back(request.body.substr(i, j - i));
}

static void	setPath(std::string httpRequest, HttpRequest &request)
{
	size_t			i;
	size_t			j;
	std::string	Ref;

	request.emptyPath = true;
	Ref = "/";
	i = httpRequest.find(Ref);
	if (i == std::string::npos)
		return ;
	i += Ref.size();
	j = httpRequest.find(" ", i);
	if (j == std::string::npos || j <= i + 1)
		return ;
	request.emptyPath = false;
	if (httpRequest[j - 1] == '/')
		j --;
	request.path = httpRequest.substr(i, j - i);
	request.htmlFile = (request.fileName.find(".html") != std::string::npos);
}

int	getRequestLength(std::string str)
{
	size_t			i;
	size_t			j;
	std::string	value;


	i = str.find("Content-Length: ");
	if (i == std::string::npos)
		return (-1);
	i += 16;
	j = i;
	for (; isdigit(str[j]); j++)
		continue;
	value = str.substr(i, j - i - 1);
	if (i == j)
		return (-1);
	return (stoi(value));
}

static void	printHttpRequest(HttpRequest request)
{
	std::cout << YELLOW;
	std::cout << "method    : " << request.method << "|" <<std::endl;
	// // std::cout << "path      : " << request.path << "|"  << std::endl;
	std::cout << "hostport  : " << request.hostPort << "|"  << std::endl;
	std::cout << "Html?     : " << request.htmlFile << "|"  << std::endl;
	std::cout << "Nopath    : " << request.emptyPath << "|"  << std::endl;
	std::cout << "body      : " << request.body << "|"  << std::endl;

	std::cout << "path      : " << request.path << "|\n";
	std::cout << "filename  : " << request.fileName << "|\n";
	std::cout << "extension : " << request.extension << "|\n";
	std::cout << "Change to file  : " << request.fileToChange << "|\n";
	std::cout << RESET;
	// (void) request;
}

HttpRequest	requestToStruct(int fd)
{
	HttpRequest	request;

	request.type = _STANDARD;
	receiveHTTPRequest(fd, 0, request);
	std::cout << RED << "Error code : " << request.errorCode << RESET << std::endl;
	if (request.errorCode == 500)
		return (request); 
	request.requestLength = request.body.length();
	request.length = request.requestLength;
	request.clientFd = fd;
	request.method = setMethod(request.body);
	if (request.method == _POST)
	{
		request.requestLength = getRequestLength(request.body);
		if (request.body.find("Content-Disposition: form-data; name=\"username\""))
			getLogin(request);
		else
			request.type = _UPLOAD;
	}
	request.hostPort = setHostPort(request.body);
	setPath(request.body, request);
	if (request.hostPort.find(":") == std::string::npos)
		request.hostPort += ":80";
	pathToData(request);
	printHttpRequest(request);
	std::cout << YELLOW << "Host port in function : " << request.hostPort << "|\n" << RESET << std::endl;
	return (request);
}
