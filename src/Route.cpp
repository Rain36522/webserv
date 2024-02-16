/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:23:18 by marvin            #+#    #+#             */
/*   Updated: 2024/02/16 17:30:53 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Route.hpp"
#include "../includes/Response.hpp"

Route::Route(){
	_listDir = false;
}

int	Route::match(Request req)
{
	if (req._path.find(_path) == 0 && (req._path.size() == _path.size() || req._path[_path.size()] == '/'))
		return (int)_path.size();
	return -1;
}

void Route::execute(Request request, Response &response)
{
	request.setUrlFile(_path);
	if (!_redirPath.empty())
	{
		response._redirLocation = _redirPath;
		response._errorCode = 302;
	}
	switch (request._type)
	{
		case _CGI:
			runCGI(request, response);break;
		case _UPLOAD:
			uploadClientFile(request, response);break;
		case _COOKIES: // TODO
		case _LOGIN: // TODO
		case _DELETE:
			delMethod(request, response);
		default:
			setHtml(request._fileName, _dir, response);
	}
}

void Route::delMethod(Request request, Response &response)
{
	std::string fileFullPath = _uploadPath + "/" +request._bodyFileName;
	if (remove(fileFullPath.c_str()))
		response._errorCode = 401;
	else
	{
		response._errorCode = 200;
		setHtml(request._fileName, _dir, response);
	}
}

// int Route::postMethod(HttpRequest request, std::string &html)
// {
// 	if (std::find(_CGIs.begin(), _CGIs.end(), request.extension) != _CGIs.end())
// 		return runCGI(request, html);
// 	else // move download here
// 		return (uploadClientFile(request, html));
// 	return 404;// Is this correct?
// }

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

void	Route::setHtml(std::string file, std::string dir, Response &response)
{
	if (file.empty())
	{
		if (!_default.empty())
			getHtml(dir + "/" + _default, response._htmlContent);
		else if (_listDir)
			doListDir(response._htmlContent);
		else
			response._errorCode = 404;
	}
	else
		getHtml(dir + "/" + file, response._htmlContent);
}

// int Route::getMethod(HttpRequest request, std::string &html)
// {
// 	if (!_redirPath.empty())
// 		return 302;
// 	if (request.fileName.empty())
// 	{
// 		if (!_default.empty())
// 			return getHtml(_dir + "/" + _default, html);
// 		else if (_listDir)
// 			return (doListDir(html));
// 		return 404;
// 	}
// 	if (std::find(_CGIs.begin(), _CGIs.end(), request.extension) != _CGIs.end())
// 		return runCGI(request, html);
// 	if (request.extension == ".html")
// 		return getHtml(_dir + "/" +request.fileName, html);
// 	return 404;
// }

void Route::runCGI(Request request, Response &response)
{
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
			response._errorCode = 500;
			return;
		}
		
		close(fd[0]);
	}
	response._errorCode = exev;
}

void	Route::uploadClientFile(Request request, Response &response)
{
	(void) request, (void) response;
	// requestToFile(request, _uploadPath);
	// // if (!request.PostFile)
	// // {
	// // 	response._errorCode = 413;
	// // 	return;
	// // } TODO make sure this is already done
	// if (request.fileName == "")
	// 	request.fileName = _default;
	// std::cout << RED << "DEFAULT PAGE : " <<this->_dir << request.fileName << RESET << std::endl;
	// return (getHtml(this->_dir + request.fileName, html));
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
