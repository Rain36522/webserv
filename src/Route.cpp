/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:23:18 by marvin            #+#    #+#             */
/*   Updated: 2024/02/12 18:23:20 by pudry            ###   ########.fr       */
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
	else if (request.method == _POST)
		code = postMethod(request, html);
	else if (request.method == _DEL)
		code = delMethod(request);
	if (code == 200 && _methods.find(_DEL) != _methods.end())
		code = addListBox(html);
	sendHTMLResponse(request.clientFd, html);
	std::cout << "Html code :" + std::to_string(code) << std::endl;
	return code;
}

int Route::delMethod(HttpRequest request)
{
	// figure it out
	(void) request;
	//std::cout << request.body.find("delete_file:") ;
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
	int	fd[2];
	int	exev;
	char	value[] = "/Users/pudry/Documents/git_webserv/Html_code/cgi.py";
	char	py[] = "/usr/bin/python3";
	char	*a[3];
	a[0] = py;
	a[1] = value;
	a[2] = NULL;

	if (pipe(fd) == -1)
	{
		std::cerr << "Pipe Error\n";
		return 500;
	}
	pid_t pid = fork();
	
	(void) request, (void) html;
	if (pid == -1)
	{
		std::cerr << "Error Fork\n";
		return 500;
	}
	else if (pid > 0)
	{
		std::vector<const char *>params;
		for (size_t i = 0; env[i]; i++)
			params.push_back(env[i]);
		for(size_t i = 0; i < request.parameters.size(); i ++)
		{
			std::cout << BLUE << "add env variable : " << request.parameters[i] << RESET << std::endl;
			//putenv(&request.parameters[i]);
			params.push_back(request.parameters[i].c_str());
		}
		params.push_back(NULL);
		DEBUG
		for (int i = 0; env[i]; i ++)
			std::cout << GREEN << "env : " << env[i] << RESET << std::endl;
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		execve(py, a, (char **)&params[0]);
		std::cerr << "Error executing CGI : " << request.fileName << std::endl;
		exit(1);
	}
	else
	{
		close(fd[1]);
		waitpid(pid, &exev, 0);
		if (WEXITSTATUS(exev))
			return 500;
		exev = getHtmlFd(fd[0], html);
		close(fd[0]);
	}
	return exev;
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
	{
		html = getErrorHtml("./Html_code/400.html", 413);
		return (413);
	}
	return (getHtml(this->_default, html));
}

int	Route::addListBox(std::string &html)
{
	DIR			*dir;
	bool		file;
	size_t		i;
	dirent		*entry;
	std::string	listBox;

	if ((i = html.find("{Delete_list}")) == std::string::npos)
		return (200);
	dir = opendir(_uploadPath.c_str());
	html.erase(i, 13);
	if (dir == nullptr)
	{
		std::cerr << "Could not acces to upload path\n";
		return (500);
	}
	file = false;
	//listBox = "<form action=\"\" method=\"DEL\">\n";
	listBox = "<input type=\"hidden\" name=\"_method\" value=\"DELETE\">\n";
	listBox += "<select name=\"DeleteFile\" id=\"DeleteFile\">\n";
	DEBUG
	while ((entry = readdir(dir)) != nullptr)
	{
		if (entry->d_name[0] != '.')
		{
			file = true;
			listBox += "<option value=\"" + std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</option>\n";
		}
	}
	DEBUG
	closedir(dir);
	if (file)
	{
		listBox += "</select>\n";
		//listBox += "<input type=\"submit\" value=\"Delete\">\n";
		//listBox += "</form>";
		listBox += "<button onclick=\"deleteFile()\">Delete Selected File</button>";
		html.insert(i, listBox);
	}
	return (200);
}
