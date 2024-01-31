/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:29:51 by pudry             #+#    #+#             */
/*   Updated: 2024/01/31 09:30:29 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

// Fonction pour recevoir une requête HTTP du client
static std::string receiveHTTPRequest(int client_fd) {
	const int bufferSize = 1024;
	char buffer[bufferSize];
	std::string httpRequest;

	ssize_t bytesRead;
	do {
		bytesRead = recv(client_fd, buffer, bufferSize - 1, 0);
		if (bytesRead == -1) {
			perror("Error receiving HTTP request");
			// Gérer l'erreur appropriée, fermer la connexion, etc.
			return "";
		}

		buffer[bytesRead] = '\0';
		httpRequest += buffer;
	} while (bytesRead == bufferSize - 1);

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

	httpRequest = receiveHTTPRequest(fd);
	request.HostPort = setHostPort(httpRequest);
	request.method = setMethod(httpRequest);
	request = setPath(httpRequest, request);
	request.HtmlFile = false;
	if (request.path.find(".html") != std::string::npos)
		request.HtmlFile = true;
	request.body = httpRequest;
	if (request.HostPort.find(":") == std::string::npos)
		request.HostPort += ":80";
	printHttpRequest(request);
	return (request);
}