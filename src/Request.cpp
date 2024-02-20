/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:05:33 by pudry             #+#    #+#             */
/*   Updated: 2024/02/20 16:39:14 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

// This function read the request. She stop when the length is good and there is a null char.
int Request::receiveHTTPRequest(const int client_fd, const int length)
{
	const int 		bufferSize = 1024;
	char 			buffer[bufferSize];
	ssize_t 		bytesRead;

	bytesRead = bufferSize - 1;
	while (bytesRead == bufferSize - 1 || _length < length)
	{
		bytesRead = read(client_fd, buffer, bufferSize - 1);
		if (bytesRead < 0 || (!length && !bytesRead))
		{
			perror("read failed :( )");
			return 500;
		}
		_length += bytesRead;
		_body += std::string(buffer, bytesRead);
		for (int j = 0; j < bufferSize; j++)
			buffer[j] = '\0';
	}
	// std::cout << _body RESETN;
	return (200);
}

bool	Request::getMethode(void)
{
	if (_body.find("GET ") == 0)
		_method = _GET;
	else if (_body.find("DELETE ") == 0)
		_method = _DEL;
	else if (_body.find("POST") == 0)
		_method = _POST;
	else
	{
		_method = _UNKNOW;
		return false;
	}
	return true;
}

bool	Request::getHostPort(void)
{
	size_t			i;
	size_t			j;

	i = _body.find("Host: ") + 6;
	if (i == std::string::npos)
		return false;
	j = _body.find("\r\n", i);
	if (j == std::string::npos)
		return false;
	_hostPort = _body.substr(i, j - i);
	if (_hostPort.find(":") == std::string::npos)
		_hostPort += ":80";
	std::cout << GREEN << "Host and port <" << BLUE << _hostPort << GREEN << ">" RESETN;
	return true;
}

bool	Request::getPath(void)
{
	size_t	i;
	size_t	j;
	size_t	k;

	switch (_method)
	{
	case _GET:
		i = 5;
		break;
	case _POST:
		i = 6;
		break;
	default:
		i = 8;
	}
	if ((k = _body.find(" HTTP")) == std::string::npos)
		return (false);
	if ((j = _body.find("?")) == std::string::npos || j > k)
		j = k;
	if (j == i || j - 1 == i)
	{
		_path = "";
		std::cout << GREEN << "Path <" << GREEN << ">" RESETN;
		return true;
	}
	if (_body[j - 1] == '/')
		j --;
	_path = _body.substr(i, j - i);
	std::cout << GREEN << "Path <" << BLUE << _path << GREEN << ">" RESETN;
	return true;
}

bool	Request::getExtension(void)
{
	
	int	i;
	int	j;

	i = 0;
	while (_path.find("/", i + 1) != std::string::npos)
		i = _path.find("/", i + 1);
	j = i;
	while (_path.find(".", i + 1) != std::string::npos)
		i = _path.find(".", i + 1);
	if (j == i)
		_extension = "";
	else
		_extension = _path.substr(i, _path.length() - i);
	std::cout << GREEN << "Extension <" << BLUE << _extension << GREEN << ">" RESETN;
	return true;
}

// TODO Remove coment if useless
bool	Request::getType(void)
{
	if (_method == _DEL)
		_type = _DELETE;	
	else if (_extension != ".html" && _extension != "" && _method != _DEL)
		_type = _CGI;
	// else if (_body.find("\r\nContent-Disposition: form-data; name=\"username\"") != std::string::npos)
	// 	_type = _LOGIN;
	// else if (_body.find("\r\nContent-Disposition: form-data; name=\"fileUpload\"; filename=") != std::string::npos)
	// 	_type = _UPLOAD;
	else
		_type = _STANDARD;
	return true;
}

bool	Request::getSpecialType()
{
	if (_body.find("\r\nContent-Disposition: form-data; name=\"username\"") != std::string::npos)
		_type = _LOGIN;
	else if (_body.find("\r\nContent-Disposition: form-data; name=\"fileUpload\"; filename=") != std::string::npos)
		_type = _UPLOAD;
	else
		return false;
	return true;
}

int	Request::getTotalLength(int error)
{
	
	size_t			i;
	size_t			j;
	std::string	value;

	if ((i = _body.find("\r\nContent-Length: ")) == std::string::npos)
		return 500;
	i+= 18;
	j = i;
	while (_body[j] && _body[j] != '\r')
		j ++;
	if (_body[j] != '\r')
		return 500;
	value = _body.substr(i, j -i);
	try
	{
		_totaLength = std::stoi(value);
	}
	catch(const std::exception& e)
	{
		return 500;
	}
	std::cout << GREEN << "total length <" << BLUE << _totaLength << GREEN << ">" RESETN;
	return error;
}

void	Request::getQuery(void)
{
	size_t	i;
	size_t	max;
	size_t	start;

	if ((max = _body.find(" HTTP")) == std::string::npos || max > _body.find("\n"))
		return;
	
	if ((start = _body.find("?")) == std::string::npos || start >= max)
		return;
	start ++;
	while ((i = _body.find("&", start)) != std::string::npos && i < max)
	{
		_Query.push_back(_body.substr(start, i - start));
		start = i + 1;
	}
	if (start < max - 1)
		_Query.push_back(_body.substr(start, max - start));
	std::cout << GREEN << "Query <" << BLUE << " ";
	for (size_t i = 0; i < _Query.size(); i ++)
		std::cout << _Query[i] << ", ";
	std::cout << GREEN << ">" RESETN;
}

bool	Request::getBoundary(int &error)
{
	size_t	i;
	size_t	j;
	// Content-Type: multipart/form-data; boundary=
	if ((i = _body.find("\r\nContent-Type: multipart/form-data; boundary=")) == std::string::npos)
	{
		error = 500;
		return false;
	}
	i += 46;
	j = i;
	while (_body[j] && _body[j] != '\r')
		j++;
	if (!_body[j])
	{
		error = 500;
		return false;
	}
	_boundary = _body.substr(i, j - i);
	std::cout << GREEN << "Boundary <" << BLUE << _boundary << GREEN << ">" RESETN;
	return true;
}

int	Request::getUploadName(int error)
{
	std::string	search;
	size_t			i;
	size_t			j;

	
	search = "Content-Disposition: form-data; name=\"fileUpload\"; filename=\"";
	if ((i = _body.find(search)) == std::string::npos)
		return 500;
	i += search.size();
	j = i;
	while (_body[j] && _body[j] != '\"')
		j ++;
	if (!_body[j])
		return 500;
	_bodyFileName = _body.substr(i, j - i);
	std::cout << GREEN << "body file name <" << BLUE << _bodyFileName << GREEN << ">" RESETN;
	return error;
}

int	Request::getLogin(int error)
{
	std::string	search;
	size_t			i;
	size_t			j;

	DEBUG 
	search = "Content-Disposition: form-data; name=\"username\"\r\n\r\n";
	if ((i = _body.find(search)) == std::string::npos)
		return 500;
	DEBUG
	i += search.length();
	j = i;
	while (_body[j] && _body[j] != '\r')
		j ++;
	if (!_body[j])
		return 500;
	DEBUG
	_usr = _body.substr(i, j - i);
	search = "Content-Disposition: form-data; name=\"pwd\"\r\n\r\n";
	if ((i = _body.find(search)) == std::string::npos)
		return 500;
	i += search.length();
	j = i;
	while (_body[j] && _body[j] != '\r')
		j ++;
	if (!_body[j])
		return 500;
	DEBUG
	_pass = _body.substr(i, j - i);
	std::cout << GREEN << "login <" << BLUE << _usr << ", " << _pass << GREEN << ">" RESETN;
	return error;
}

int	Request::getDelete(int error)
{
	size_t	i;

	if ((i = _body.find("delete_file:")) == std::string::npos)
	{
		std::cerr << ORANGE << "Delete methode without filename\n" << RESET;
		return 500;
	}
	i += 12;
	_bodyFileName = _body.substr(i, _body.size() - i);
	std::cout << GREEN << "body file name <" << BLUE << _bodyFileName << GREEN << ">" RESETN;
	return error;
}

int	Request::getBodyContent(int error)
{
	size_t	i;
	DEBUG
	i = 0;
	if ((i = _body.find(("--" + _boundary))) == std::string::npos)
		i = 0;
	DEBUG
	std::cout << "Total : " << _totaLength << ", length : " << _length RESETN;
	if (_totaLength - int(i) > _length)
		error = receiveHTTPRequest(_clientFd, _totaLength - int(i));
	DEBUG
	return error;
}

int	Request::setUrlFile(std::string route_path, std::string uploadDir, bool allowUpload)
{
	std::cout << "Route path : " << route_path << std::endl;
	if (route_path[route_path.size() - 1] != '/')
		_fileName = _path.substr(route_path.size(), _path.size() - route_path.size());
	else
		_fileName = _path.substr(route_path.size() + 1, _path.size() - route_path.size() - 1);
	if (_fileName[0] == '/')
		_fileName = _fileName.substr(1, _fileName.size() - 1);
	std::cout << GREEN << "Filename <" << BLUE << _fileName << GREEN << ">" RESETN;
	if (_type == _UPLOAD && allowUpload && doUpload(200, uploadDir) == 500)
	{
		std::cout << RED << "Error while uploading file." RESETN;
		return 500;
	}
	if (!allowUpload && _type == _UPLOAD)
		return 405;
	return 200;
}

int	Request::setBody(int bodySize)
{
	int	error;

	DEBUG
	std::cout << "BODY : " << RED << bodySize << RESET << " TOTAL " << RED << _totaLength RESETN;
	error = 200;
	if (_totaLength > bodySize)
		return 413;
	DEBUG
	if (_method == _POST)
	{
		
		if (!getBoundary(error))
			return 500;
		else if (getBodyContent(error) == 500)
			return 500;
		else if (_type == _STANDARD && !getSpecialType())
			return 500;
		std::cout << "Actual body : " << YELLOW << _body RESETN;
		if (_type == _LOGIN)
			return getLogin(error);
		else if (_type == _UPLOAD)
			error = getUploadName(error);
	}
	else if (_method == _DEL)
		error = getDelete(error);
	DEBUG
	if (error == 500)
		std::cout << RED << "Error getting body request" RESETN;
	return error;
}

Request::Request(const int client_Fd, int &error)
{
	_length = 0;
	_clientFd = client_Fd;
	if (receiveHTTPRequest(_clientFd, 0) && getMethode() && getHostPort() \
		&& getPath() && getExtension() && getType())
	{
		_totaLength = _length;
		std::cout << MAGENTA << "ACTUAL LENGTH : " << _length RESETN;
		if (_method == _POST && _type != _CGI)
			error = getTotalLength(error);
		else if (_type == _CGI)
			getQuery();
	}
	else
	{
		error = 500;
		std::cout << RED << "Error getting request" RESETN;
	}
}

int	Request::doUpload(int error, std::string upload_dir)
{
	size_t			start;
	size_t			stop;
	std::string		file;
	std::ofstream	outfile;

	file = upload_dir + "/" + _bodyFileName;
	validateFd(file, EVFILT_WRITE);
	start = _body.find(("--" + _boundary + "\r\n"));
	start += ("--" + _boundary + "\r\n").size();
	start = _body.find("\r\n", start);
	start = _body.find("\r\n", start + 1);
	start = _body.find("\r\n", start + 1);
	if (start == std::string::npos)
		return 500;
	start += 2;
	stop = _body.find((_boundary + "--"), start);
	std::cout << ORANGE << "Boundary : " << _boundary RESETN;
	std::cout << BLUE << "File : " << file RESETN;
	
	if (stop == std::string::npos)
		return 500;
	stop -= 4;
	outfile.open(file, std::ios::binary);
	if (outfile.fail())
	{
		std::cerr << ORANGE << "Opening fail error\n" << RESET;
		return (500);
	}
	outfile << _body.substr(start, stop - start);
	outfile.close();
	return error;
}
