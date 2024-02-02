/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:51 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/02 10:39:41 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>

enum m_type{
	_GET,
	_POST,
	_DEL,
	_UNKNOW
};

struct HttpRequest
{
	m_type method;
	std::string path;
	std::string body;
	std::string	HostPort;
	std::string	FileContent;
	std::string	FileName;
	int			RequestLength; // total request length of download file
	int			length; // actual request length
	bool		HtmlFile;
	bool		emptyPath;
	int			clientFd;
};

struct HttpResponse
{
	// status line
	int code;
	std::string reason_phrase;
	// headers
	std::string content_type;
	size_t		content_length;
	// ... ?
	// Reponse body
	std::string body;
};

struct	ServConf
{
	std::string	name;
	int			port;
	std::string	host;
	int			bodyLimit;
	int			idirectorylListing;
	std::string	cgi;
	bool		bdirectoryListing;
	std::string	ErrPage;
	std::string	DefPage;
	bool		Get;
	bool		Post;
	bool		Delete;
};