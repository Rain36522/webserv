/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:51 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/20 12:36:40 by dvandenb         ###   ########.fr       */
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

enum m_type
{
	_GET,
	_POST,
	_DEL,
	_UNKNOW
};

enum m_dataType
{
	_UPLOAD,
	_LOGIN,
	_CGI,
	_STANDARD,
	_COOKIES,
	_DELETE
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
