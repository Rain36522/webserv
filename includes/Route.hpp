/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:50:01 by dvandenb          #+#    #+#             */
/*   Updated: 2024/01/26 16:07:07 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "AbstractCGI.hpp"

class Route
{
	//set_methode();
	private:
		std::string			_path; // part in the url after the port/hostname if I understand correctly
		std::string			_dir; // path on server
		std::string			_default; // default page (index.html)
		std::set<m_type>	_methods; // a set elements are unique and also has the contains() method
		bool				listDir; // enable or disable directory listing
		AbstractCGI			&_cgi; // Not entirely sure if this is the correct place or not.
		std::string			_redirPath; // Redirect from this route to another?
	public:
		Route(std::string _path, m_type _type, AbstractCGI &cgi);
		bool match(HttpRequest req)
		{
			return req.path == _path && _methods.find(req.method) != _methods.end();
		};
		HttpResponse execute(HttpRequest req)
		{
			return _cgi.execute(req);
		}
};

// Helper class for creating Http Reponses?