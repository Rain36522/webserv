/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:51 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/05 17:33:49 by pudry            ###   ########.fr       */
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
	std::string					hostPort;
	std::string					path; // route
	std::string					fileName;
	std::vector<std::string>	parameters;
	std::string					body;
	std::string					extension;
	int							requestLength; // total request length of download file
	int							length; // actual request length
	bool						htmlFile;
	bool						emptyPath;
	bool						PostFile;
	int							clientFd;
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
