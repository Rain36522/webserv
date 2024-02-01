/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:29:51 by pudry             #+#    #+#             */
/*   Updated: 2024/02/01 15:48:46 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

// Fonction pour recevoir une requête HTTP du client
std::string receiveHTTPRequest(int client_fd, int RequestLength) 
{
	const int 		bufferSize = 1024;
	char 			buffer[bufferSize];
	std::string 	httpRequest;
	ssize_t 		bytesRead;
	ssize_t			i;
	
	i = 0;
	bytesRead = bufferSize - 1;
	DEBUG
	std::ofstream oui;
	oui.open("request2.txt");
	while (bytesRead == bufferSize - 1 || i < RequestLength)
	{
		bytesRead = read(client_fd, buffer, bufferSize - 1);
		i += bytesRead;
		if (bytesRead == -1) {
			perror("Error receiving HTTP request");
			// Gérer l'erreur appropriée, fermer la connexion, etc.
			return "";
		}
		std::cout << i << std::endl;
		httpRequest += buffer;
		oui << bytesRead;
	}
	oui.close();
	DEBUG
	std::cout << "read bytes : " << i << std::endl;

	return httpRequest;
}

static std::string	setHostPort(std::string httpRequest)
{
	int			i;
	int			j;

	i = httpRequest.find("Host: ") + 6;
	if (i == std::string::npos)
		return ("");
	j = httpRequest.find("\n", i);
	if (j == std::string::npos)
		return ("");
	return (httpRequest.substr(i, j - i - 1));
}

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

int	getRequestLength(std::string str)
{
	int			i;
	int			j;
	std::string	value;

	std::cout << "str : " << str << std::endl;

	i = str.find("Content-Length: ");
	std::cout << "i : " << i << std::endl;
	if (i == std::string::npos)
		return (-1);
	i += 16;
	for (int j = i; isdigit(str[j]); j++)
		continue;
	value = str.substr(i, j - i - 1);
	if (i == j)
		return (-1);
	return (stoi(value));
}

static void	printHttpRequest(HttpRequest request)
{
	std::cout << "method    : " << request.method << "|" <<std::endl;
	std::cout << "path      : " << request.path << "|"  << std::endl;
	std::cout << "hostport  : " << request.HostPort << "|"  << std::endl;
	std::cout << "Html?     : " << request.HtmlFile << "|"  << std::endl;
	std::cout << "Nopath    : " << request.emptyPath << "|"  << std::endl;
	std::cout << "body      : " << request.body << "|"  << std::endl;
}

HttpRequest	requestToStruct(int fd)
{
	std::string httpRequest;
	HttpRequest	request;

	httpRequest = receiveHTTPRequest(fd, 0); 
	request.clientFd = fd;
	request.body = httpRequest;
	request.HostPort = setHostPort(httpRequest);
	request.method = setMethod(httpRequest);
	request = setPath(httpRequest, request);
	request.length = httpRequest.length();
	request.HtmlFile = false;
	if (request.method == _POST)
		request.RequestLength = getRequestLength(httpRequest);
	if (request.path.find(".html") != std::string::npos)
		request.HtmlFile = true;
	if (request.HostPort.find(":") == std::string::npos)
		request.HostPort += ":80";
	printHttpRequest(request);
	return (request);
}