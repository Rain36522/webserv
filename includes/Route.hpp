/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:50:01 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/06 13:51:19 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "data.hpp"
#include "ParseConfig.hpp"

class ParseConfig;

class Route
{
	//set_methode();
	private:
		std::string					_path; // part in the url after the port/hostname if I understand correctly
		// required, can be empty
		std::string					_dir; // path on server
		// required, can be empty
		std::string					_default; // default page (index.html)
		std::set<m_type>			_methods; // a set elements are unique and also has the contains() method
		bool						_listDir; // enable or disable directory listing
		std::string					_redirPath; // Redirect from this route to another?
		bool						_allowUpload;
		std::string					_uploadPath;
		std::vector<std::string>	_CGIs;
		
	public:
		Route();
		Route(std::string const path, m_type type, std::string default_page);
		bool match(HttpRequest req);
		int execute(HttpRequest req);
		int runCGI(HttpRequest request, std::string &html);
		int getMethod(HttpRequest request, std::string &html);
		int postMethod(HttpRequest request, std::string &html);
		int delMethod(HttpRequest request);
		int	uploadClientFile(HttpRequest request, std::string &html);
		friend class ParseConfig;
};

// Helper class for creating Http Reponses?
