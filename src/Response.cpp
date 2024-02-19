/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:14:34 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/19 08:57:00 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

Response::Response(int clientFd)
{
	_clientFd = clientFd;
	_servName = "master";
	_errorCode = 200;
}

void Response::sendHTML()
{
	std::string responseHeaders = "HTTP/1.1 " + getCodeText() +  " \r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(_htmlContent.size()) + "\r\n";
	responseHeaders += "Server: " + _servName +"\r\n";
	responseHeaders += "\r\n";
	ssize_t sent = send(_clientFd, responseHeaders.c_str(), responseHeaders.size(), 0);
	if (sent == -1)
	{
		perror("Error sending response headers");
	}
	sent = send(_clientFd, _htmlContent.c_str(), _htmlContent.size(), 0);
	if (sent == -1) {
		perror("Error sending HTML content");
	}
}


void Response::sendRedirect()
{
    std::string responseHeaders = "HTTP/1.1 302 Found\r\n";
    responseHeaders += "Location: " + _redirLocation + "\r\n";
    responseHeaders += "Server: " + _servName + "\r\n";
    responseHeaders += "\r\n";

    ssize_t sent = send(_clientFd, responseHeaders.c_str(), responseHeaders.size(), 0);
    if (sent == -1) {
        perror("Error sending redirect response");
    }
}

std::string	Response::getCodeText()
{
	switch (_errorCode)
	{
	case 400:
		return ("400 Bad Request");
	case 401:
		return ("401 Unauthorized");
	case 404:
		return ("404 Not Found");
	case 405:
		return ("405 Method Not Allowed");
	case 413:
		return ("413 Payload to large");
	case 418:
		return ("418 I'm a tea pot");
	case 200:
		return ("200 Ok");
	case 302:
		return ("302 Found");
	default:
		return ("500 Internal server error");
	}
}

void Response::sendResponse()
{
	// if (_errorCode == 302)
	// sendRedirect();
	if (_errorCode != 302)
		sendHTML();
}
