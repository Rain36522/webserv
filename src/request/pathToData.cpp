/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathToData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 12:54:08 by pudry             #+#    #+#             */
/*   Updated: 2024/02/07 12:40:59 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

static HttpRequest	setParameters(HttpRequest request, size_t start)
{
	size_t	i;
	std::cout << "\033[35m\n";
	i = start;
	while (request.path.find("&", start) != std::string::npos)
	{
		i = request.path.find("&", start);
		request.parameters.push_back(request.path.substr(start, i - start));
		std::cout << "parametres : " << request.path.substr(start, i - start) << "|\n";
		start = i + 1;
	}
	if (start != request.path.length())
	{
		request.parameters.push_back(request.path.substr(start, i - start - 1));
		std::cout << "parametres = " << request.path.substr(start, i - start - 1) << "|\n";
	}
	std::cout << "\033[39m\n";
	return (request);
}

static HttpRequest	pathFilename(HttpRequest request, size_t istart)
{
	size_t	j;
	size_t	k;

	k = request.path.find("?", istart);
	if (k == std::string::npos)
		k = request.path.length();
	j = istart;
	while (request.path.find(".", j + 1) != std::string::npos && request.path.find(".", j + 1) <= k)
		j = request.path.find(".", j + 1);
	if (j == istart || j == std::string::npos || j >= k)
		return (request);
	request.extension = request.path.substr(j, k - j);
	request.fileName = request.path.substr(istart, k - istart);
	return (request);	
}

void 	pathToData(HttpRequest &request)
{
	size_t	i;
	size_t	j;

	i = 0;
	request.fileName = "";
	while (request.path.find("/", i + 1) != std::string::npos)
		i = request.path.find("/", i + 1);
	if ((i == 0 && request.path.find("/", i) == std::string::npos) || i + 1 == request.path.length())
		return ;
	i ++;
	request = pathFilename(request, i);
	j = request.path.find("?", i);
	if (j != std::string::npos && request.fileName != "")
		request = setParameters(request, j + 1);
	if (request.fileName != "")
		request.path = request.path.substr(0, i - 1);
	std::cout << "word : " << request.fileName << ", origine : " << request.path <<std::endl;

}
