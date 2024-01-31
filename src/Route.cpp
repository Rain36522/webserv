/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:23:18 by marvin            #+#    #+#             */
/*   Updated: 2024/01/31 11:26:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Route.hpp"

bool Route::match(HttpRequest req)
{
	return req.path == _path && _methods.find(req.method) != _methods.end();
}

