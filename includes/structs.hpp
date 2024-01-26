/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:51 by dvandenb          #+#    #+#             */
/*   Updated: 2024/01/26 16:07:59 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

enum m_type{
	_GET,
	_POST,
	_DEL
};

struct HttpRequest
{
	m_type method;
	std::string path;
	std::string body;
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
