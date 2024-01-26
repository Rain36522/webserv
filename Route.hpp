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
		std::string _path;
		m_type _type;
		AbstractCGI &_cgi;
	public:
		Route(std::string _path, m_type _type, AbstractCGI &cgi);
		bool match(HttpRequest req)
		{
			return req.path == _path && req.method == _type;
		};
		HttpResponse execute(HttpRequest req)
		{
			return _cgi.execute(req);
		}
};

// Helper class for creating Http Reponses?