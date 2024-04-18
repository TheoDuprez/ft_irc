/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:44:47 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/19 00:24:00 by shellks          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<std::string>    split(const std::string& str, char delim);
void                        sendMessage(int fd, std::string msg);
size_t						strtost(const std::string& str);

std::string					ullToString(size_t valueToConvert);
std::string                 iToString(int valueToConvert);
std::string	const	        getCurrentTimeStamp(void);

#endif