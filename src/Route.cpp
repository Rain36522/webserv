/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:23:18 by marvin            #+#    #+#             */
/*   Updated: 2024/02/20 12:35:27 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Route.hpp"
#include "../includes/Response.hpp"

Route::Route(){
	_listDir = false;
}

int	Route::match(Request req)
{
	std::cout << "compared route " << _path << " to request " << req._path;
	if (_path.size() == 0){
		std::cout << GREEN << "match" RESETN;
		return (0);
	}
	if (req._path.find(_path) == 0 && (req._path.size() == _path.size() || req._path[_path.size()] == '/'))
	{
		std::cout << GREEN << "match" RESETN;
		return (int)_path.size();
	}
	std::cout << RED << "no match" RESETN;
	return -1;
}

void Route::execute(Request request, Response &response)
{
	request.setUrlFile(_path, _uploadPath, _allowUpload);
	std::cout << "Incoming request method is " << request._method << std::endl;
		std::cout << "Incoming request type is " << request._type << std::endl;

	if (!_redirPath.empty())
	{
		response._redirLocation = _redirPath;
		response._errorCode = 302;
	}
	switch (request._type)
	{
		case _CGI:
			runCGI(request, response);break;
		case _COOKIES: // TODO
		case _LOGIN: // TODO
		case _DELETE:
			delMethod(request, response); break;
		case _UPLOAD:
		default:
			setHtml(request, _dir, response);
	}
}

void Route::delMethod(Request request, Response &response)
{
	DEBUG
	std::string fileFullPath = _uploadPath + "/" +request._bodyFileName;
	std::cout << GREEN << fileFullPath RESETN;
	if (remove(fileFullPath.c_str()))
		response._errorCode = 401;
	else
	{
		DEBUG
		setHtml(request, _dir, response);
	}
}

int	Route::doListDir(std::string &html, std::string path_dir) const
{
	DIR		*dir;
	dirent	*entry;
	std::string fullDir = _dir + "/" + path_dir;
	dir = opendir(fullDir.c_str());
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
	if (html == "")
		html = "<p>No file found on directory</p>";
	closedir(dir);
	return (200);
}

void	Route::setHtml(Request req, std::string dir, Response &response)
{
	if (req._fileName.empty())
	{
		std::cout << GREEN << "NO HTML PAGE IN PATH: ";

		if (!_default.empty())
		{
			std::cout << BLUE << "get default" RESETN;
			response._errorCode = getHtml(dir + "/" + _default, response._htmlContent);
		}
		else if (_listDir)
		{
			std::cout << BLUE << "get list" RESETN;
			response._errorCode = doListDir(response._htmlContent, "");
		}
		else
		{
			std::cout << BLUE << "no list or default" RESETN;
			response._errorCode = 404;
		}
	}
	else if (req._extension.empty())
	{
		std::cout << BLUE << "get list : " << req._fileName RESETN;
		response._errorCode = doListDir(response._htmlContent, req._fileName);
	}
	else
	{
		std::cout << GREEN << "GET HTML PAGE IN PATH" RESETN;
		response._errorCode = getHtml(dir + "/" + req._fileName, response._htmlContent);
	}
	addListBox(response._htmlContent);
}

void Route::runCGI(Request request, Response &response)
{
	response._errorCode = 200;
	if (std::find(_CGIs.begin(), _CGIs.end(), request._extension) == _CGIs.end())
	{
		response._errorCode = 404;
		return ;
	}
	int	fd[2];
	int	exev;
	std::string value = std::string(PATH_INFO) + "/" + request._fileName;
	char	py[] = "/usr/bin/python3";
	
	char	*a[3] = {py, (char *)value.c_str(), NULL};
	if (pipe(fd) == -1)
	{
		std::cerr << RED << "Pipe Error\n" << RESET;
		response._errorCode = 500;
		return;
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << RED << "Error Fork\n" << RESET;
		response._errorCode = 500;
		return;
	}
	else if (!pid)
	{
		std::vector<const char *>params;
		for (size_t i = 0; env[i]; i++)
			params.push_back(env[i]);
		for(size_t i = 0; i < request._Query.size(); i ++)
			params.push_back(request._Query[i].c_str());
		params.push_back(("uploadFolder=" + _uploadPath).c_str());
		params.push_back(NULL);
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		if (request._extension == ".py")
			execve(py, a, (char **)&params[0]);
		 if (request._extension == ".out")
			execve((char *)value.c_str(), 0, (char **)&params[0]);
		std::cerr << RED << "Error executing CGI : " << request._fileName << std::endl << RESET;
		exit(1);
	}
	else
	{
		close(fd[1]);
		waitpid(pid, &exev, 0);
		response._errorCode = getHtmlFd(fd[0], response._htmlContent);
		if (WEXITSTATUS(exev))
		{
			close(fd[0]);
			std::cout << RED << "Error executing CGI" RESETN;
			response._errorCode = 500;
			return;
		}
		close(fd[0]);
	}
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
