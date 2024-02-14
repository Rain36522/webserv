/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:51 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/14 10:36:41 by dvandenb         ###   ########.fr       */
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


#define OK					200
#define CREATED				201
#define NO_CONTENT			204
#define FOUND				302
// Bad request includes bad syntax, size too large, weird routing
#define BAD_REQUEST			400
// attempting something without permission (bad file permission?)
#define FORBIDDEN			 403
#define NOT_FOUND			 404
#define METHOD_NOT_ALLOWED	 405
// attempting to brew coffee with a teapot
#define IM_A_TEAPOT			 418
#define WINDOWS_PARENT_BLOCK 450
#define UNAVALIABLE_LEGAL	 451
// generic error message
#define INTERNAL_ERROR		500
#define INSUFFICIENT_STORAGE 507


enum m_type
{
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

extern char **env;

