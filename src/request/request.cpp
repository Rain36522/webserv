/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:29:51 by pudry             #+#    #+#             */
/*   Updated: 2024/02/02 17:43:34 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

// Fonction pour recevoir une requête HTTP du client
HttpRequest receiveHTTPRequest(int client_fd, int RequestLength, HttpRequest request) 
{
	const int 		bufferSize = 1024;
	char 			buffer[bufferSize];
	ssize_t 		bytesRead;
	std::string		buf2;

	bytesRead = bufferSize - 1;
	DEBUG
	while (bytesRead == bufferSize - 1 || request.length < RequestLength)
	{
		bytesRead = read(client_fd, buffer, bufferSize - 1);
		request.length += bytesRead;
		if (bytesRead == -1) {
			perror("Error receiving HTTP request");
			// Gérer l'erreur appropriée, fermer la connexion, etc.
			return request;
		}
		buf2 = std::string(buffer, bytesRead);
		request.body += buf2;
		for (int j = 0; j < bufferSize; j++)
			buffer[j] = '\0';
	}
	return request;
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
	if (httpRequest.find("GET") != std::string::npos)
		return (_GET);
	else if (httpRequest.find("POST") != std::string::npos)
		return (_POST);
	else if (httpRequest.find("DELETE") != std::string::npos)
		return (_DEL);
	else
		return (_UNKNOW);
}

/* Old:
static HttpRequest	setPath(std::string httpRequest, HttpRequest request)
{
	int			i;
	int			j;
	std::string	Ref;

	request.emptyPath = true;
	Ref = "Referer: http://" + request.HostPort + "/";
	i = httpRequest.find(Ref);
	if (i == std::string::npos)
		return (request);
	i += Ref.size();
	j = httpRequest.find("\n", i);
	if (j == std::string::npos || j <= i + 1)
		return (request);
	request.emptyPath = false;
	request.path = httpRequest.substr(i, j - i - 1);
	return (request);

}
*/

static HttpRequest	setPath(std::string httpRequest, HttpRequest request)
{
	size_t			i;
	size_t			j;
	std::string	Ref;

	request.emptyPath = true;
	Ref = "/";
	i = httpRequest.find(Ref);
	DEBUG
	if (i == std::string::npos)
		return (request);
	i += Ref.size();
	j = httpRequest.find(" ", i);
	DEBUG
	if (j == std::string::npos || j <= i + 1)
		return (request);
	request.emptyPath = false;
	request.path = httpRequest.substr(i, j - i);
	return (request);

}

int	getRequestLength(std::string str)
{
	size_t			i;
	size_t			j;
	std::string	value;

	std::cout << "str : " << str << std::endl;

	i = str.find("Content-Length: ");
	std::cout << "i : " << i << std::endl;
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

// static void	printHttpRequest(HttpRequest request)
// {
// 	std::cout << "method    : " << request.method << "|" <<std::endl;
// 	std::cout << "path      : " << request.path << "|"  << std::endl;
// 	std::cout << "hostport  : " << request.hostPort << "|"  << std::endl;
// 	std::cout << "Html?     : " << request.htmlFile << "|"  << std::endl;
// 	std::cout << "Nopath    : " << request.emptyPath << "|"  << std::endl;
// 	std::cout << "body      : " << request.body << "|"  << std::endl;
// }

HttpRequest	requestToStruct(int fd)
{
	HttpRequest	request;

	request.length = 0;
	request = receiveHTTPRequest(fd, 0, request);
	request.clientFd = fd;
	request.method = setMethod(request.body);
	if (request.method == _POST)
	{
		request.requestLength = getRequestLength(request.body);
		if (request.length < request.requestLength)
		{
			request = receiveHTTPRequest(fd, request.requestLength, request);
		}
		request = requestToFile(request);
		// std::cout << "\033[94mBody :\n";
		// std::cout << request.body << std::endl;
		// std::cout << "\033[92mFile content\n";
		// std::cout << request.FileName << std::endl << std::endl;
		// std::cout << request.FileContent << std::endl;
		// std::cout << "\033[39m";
	}
	request.hostPort = setHostPort(request.body);
	request = setPath(request.body, request);
	request.htmlFile = false;
	if (request.path.find(".html") != std::string::npos)
		request.htmlFile = true;
	if (request.hostPort.find(":") == std::string::npos)
		request.hostPort += ":80";
	// printHttpRequest(request);
	return (request);
}
