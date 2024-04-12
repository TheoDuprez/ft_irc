/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:44:47 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/12 14:45:00 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>

std::vector<std::string>    split(const std::string& str, char delim);
void                        sendMessage(int fd, std::string msg);
size_t						strtost(const std::string& str);

std::string					ullToString(size_t valueToConvert);
std::string                 iToString(int valueToConvert);
// bool                        checkExcludedChar(std::string name);

#endif