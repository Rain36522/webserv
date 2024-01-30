/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:51 by dvandenb          #+#    #+#             */
/*   Updated: 2024/01/30 16:59:56 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

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
	bool		HtmlFile;
	bool		emptyPath;
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
	int			port = 0;
	int			host;
	int			bodyLimit;
	int			idirectorylListing;
	std::string	cgi;
	bool		bdirectoryListing;
	std::string	ErrPage;
	bool		Get;
	bool		Post;
	bool		Delete;
};