/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:23:18 by marvin            #+#    #+#             */
/*   Updated: 2024/02/06 16:10:44 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Route.hpp"

Route::Route(){
	_listDir = false;
}

Route::Route(std::string const path, m_type type, std::string default_page)
{
	_path = path;
	_methods.insert(type);
	_default = default_page;
	(void)_listDir, (void)_allowUpload;
}


bool Route::match(HttpRequest req)
{
	return req.path == _path && _methods.find(req.method) != _methods.end();
}

int Route::execute(HttpRequest request)
{
	int code = 404;
	std::string html;
	if (request.method == _GET)
		code = getMethod(request, html);
	if (request.method == _POST)
		code = postMethod(request, html);
	if (request.method == _DEL)
		code = delMethod(request);
	sendHTMLResponse(request.clientFd, html);
	return code;
}

int Route::delMethod(HttpRequest request)
{
	// figure it out
	(void) request;
	return (404);
}

int Route::postMethod(HttpRequest request, std::string &html)
{
	if (std::find(_CGIs.begin(), _CGIs.end(), request.extension) != _CGIs.end())
		return runCGI(request, html);
	else // move download here
		return (uploadClientFile(request, html));
	return 404;// Is this correct?
}

int Route::getMethod(HttpRequest request, std::string &html)
{
	if (request.fileName.empty())
	{
		if (!_default.empty())
			return getHtml(_dir + "/" + _default, html);
		else if (_listDir)
			;// list dir
		return 404;
	}
	if (std::find(_CGIs.begin(), _CGIs.end(), request.extension) != _CGIs.end())
		return runCGI(request, html);
	if (request.htmlFile)
		return getHtml(_dir + "/" +request.fileName, html);
	return 404;
}

int Route::runCGI(HttpRequest request, std::string &html)
{
	pid_t pid = fork();
	(void) request, (void) html;
	if (pid == -1)
	{

	}
	else if (pid > 0)
	{

	}
	else
	{
		//execve with request.fileName and request.parameters
	}
	return 200;
}

int	Route::uploadClientFile(HttpRequest request, std::string &html)
{
	(void)  html;
	if (request.length < request.requestLength)
	{
		request = receiveHTTPRequest(request.clientFd, request.requestLength, request);
	}
	request = requestToFile(request, _uploadPath);
	if (!request.PostFile)
		return (413);
	else
		return (200);
}

