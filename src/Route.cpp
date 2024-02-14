/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:23:18 by marvin            #+#    #+#             */
/*   Updated: 2024/02/14 10:58:12 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Route.hpp"

Route::Route(){
	_listDir = false;
}

int	Route::match(HttpRequest req)
{
	if (req.path.find(_path) == 0 && (req.path.size() == _path.size() || req.path[_path.size()] == '/'))
		return (int)_path.size();
	return -1;
}

int Route::execute(HttpRequest request)
{
	int code = 404;
	std::string html;
	std::cout << CYAN << request.path << " ROUTED TO PATH: " << _path << RESET << "\n";
	std::cout << GREEN << "filename:<" << BLUE << request.fileName << GREEN << ">" << RESET << "\n";
	if (!request.fileName.empty())
		request.fileName = request.path.substr(_path.size(), request.path.size() - _path.size()) + "/" + request.fileName;
	std::cout << GREEN << "filename:<" << BLUE << request.fileName << GREEN << ">" << RESET << "\n";
	if (_methods.find(request.method) == _methods.end())
		code = 405;
	else if (request.method == _GET)
		code = getMethod(request, html);
	else if (request.method == _POST)
		code = postMethod(request, html);
	else if (request.method == _DEL)
		code = delMethod(request);
	if (code == 200 && _methods.find(_DEL) != _methods.end())
		code = addListBox(html);
	if (code == 302)
		sendRedirectResponse(request.clientFd, _redirPath, request.servName);
	else if (code < 400)
		sendHTMLResponse(request.clientFd, html, code, request.servName);
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

int	Route::doListDir(std::string &html) const
{
	DIR		*dir;
	dirent	*entry;

	dir = opendir(_dir.c_str());
	if (dir == nullptr)
	{
		std::cerr << RED << "Could not access path for list directory\n" << RESET;
		return (500);
	}
	html = "";
	while ((entry = readdir(dir)) != nullptr)
	{
		if (std::string(entry->d_name)[0] != '.')
			html += "<p>" + std::string(entry->d_name) + "</p>";
	}
	closedir(dir);
	return (200);
}

int Route::getMethod(HttpRequest request, std::string &html)
{
	if (!_redirPath.empty())
		return 302;
	if (request.fileName.empty())
	{
		if (!_default.empty())
			return getHtml(_dir + "/" + _default, html);
		else if (_listDir)
			return (doListDir(html));
		return 404;
	}
	if (std::find(_CGIs.begin(), _CGIs.end(), request.extension) != _CGIs.end())
		return runCGI(request, html);
	if (request.extension == ".html")
		return getHtml(_dir + "/" +request.fileName, html);
	return 404;
}

int Route::runCGI(HttpRequest request, std::string &html)
{
	DEBUG
	int	fd[2];
	int	exev;
	std::string value = std::string(PATH_INFO) + "/" + request.fileName;
	char	py[] = "/usr/bin/python3";
	
	char	*a[3] = {py, (char *)value.c_str(), NULL};
	if (pipe(fd) == -1)
	{
		std::cerr << RED << "Pipe Error\n" << RESET;
		return 500;
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << RED << "Error Fork\n" << RESET;
		return 500;
	}
	else if (!pid)
	{
		std::vector<const char *>params;
		for (size_t i = 0; env[i]; i++)
			params.push_back(env[i]);
		for(size_t i = 0; i < request.parameters.size(); i ++)
			params.push_back(request.parameters[i].c_str());
		params.push_back(("uploadFolder=" + _uploadPath).c_str());
		params.push_back(NULL);
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		if (request.extension == ".py")
			execve(py, a, (char **)&params[0]);
		 if (request.extension == ".out")
			execve((char *)value.c_str(), 0, (char **)&params[0]);
		std::cerr << RED << "Error executing CGI : " << request.fileName << std::endl << RESET;
		exit(1);
	}
	else
	{
		close(fd[1]);
		waitpid(pid, &exev, 0);
		exev = getHtmlFd(fd[0], html);
		if (WEXITSTATUS(exev))
		{
			close(fd[0]);
			return 500;
		}
		
		close(fd[0]);
	}
	return exev;
}

int	Route::uploadClientFile(HttpRequest request, std::string &html)
{
	(void)  html;
	if (request.length < request.requestLength)
	{
		receiveHTTPRequest(request.clientFd, request.requestLength, request);
	}
	if (request.errorCode == 500)
		return (request.errorCode);
	requestToFile(request, _uploadPath);
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
		std::cerr << RED << "Could not acces to upload path\n" << RESET;
		return (500);
	}
	file = false;
	//listBox = "<form action=\"\" method=\"DEL\">\n";
	listBox = "<input type=\"hidden\" name=\"_method\" value=\"DELETE\">\n";
	listBox += "<select name=\"DeleteFile\" id=\"DeleteFile\">\n";
	while ((entry = readdir(dir)) != nullptr)
	{
		if (entry->d_name[0] != '.')
		{
			file = true;
			listBox += "<option value=\"" + std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</option>\n";
		}
	}
	closedir(dir);
	if (file)
	{
		listBox += "</select>\n";
		listBox += "<button onclick=\"deleteFile()\">Delete Selected File</button>";
		html.insert(i, listBox);
	}
	return (200);
}
