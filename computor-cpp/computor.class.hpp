// ************************************************************************** //
//                                                                            //
//                                                        ::::::::            //
//   computor.class.hpp                                 :+:    :+:            //
//                                                     +:+                    //
//   By: pacovali <pacovali@student.codam.nl>         +#+                     //
//                                                   +#+                      //
//   Created: 2020/06/04 12:37:04 by pacovali      #+#    #+#                 //
//   Updated: 2020/06/04 12:37:04 by pacovali      ########   odam.nl         //
//                                                                            //
// ************************************************************************** //

#pragma once

#include "header.h"

class Computor {
public:
	Computor();
	~Computor();
	int					parseArgument();
	int 				parseArgument(std::stringstream& arg);
	void				calculate();
	void				clear();
private:
	bool								add_values(float& k, float& p, char& s);
	std::map<int, std::vector<float>>	values_;
	std::vector<float>					result_;
};
