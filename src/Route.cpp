/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:23:18 by marvin            #+#    #+#             */
/*   Updated: 2024/02/01 17:44:24 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Route.hpp"

Route::Route(std::string const path, m_type type, std::string default_page)
{
	_path = path;
	_methods.insert(type);
	_default = default_page;
}


bool Route::match(HttpRequest req)
{
	return req.path == _path && _methods.find(req.method) != _methods.end();
}

HttpResponse Route::execute(HttpRequest request)
{
	sendHTMLResponse(request.clientFd, getHtmlPage(_default));
	return (HttpResponse){};
}