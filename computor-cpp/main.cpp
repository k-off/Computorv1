// ************************************************************************** //
//                                                                            //
//                                                        ::::::::            //
//   main.cpp                                           :+:    :+:            //
//                                                     +:+                    //
//   By: pacovali <pacovali@student.codam.nl>         +#+                     //
//                                                   +#+                      //
//   Created: 2020/06/04 12:37:04 by pacovali      #+#    #+#                 //
//   Updated: 2020/06/04 12:37:04 by pacovali      ########   odam.nl         //
//                                                                            //
// ************************************************************************** //

#include "computor.class.hpp"

using namespace std;

int     main(int ac, char** av) {
	Computor c;

	if (ac > 2) {
		cout << "Usage: computor [\"expression\"]" << endl;
		return (0);
	} else if (ac == 2) {
		string	s(av[1]);
		for (auto i = 0u; i < s.size(); i++) {
			if (isdigit(s[i]) && !isdigit(s[i + 1]) && s[i + 1] != '.') { s.insert(i + 1, " "); }
		}
		stringstream ss(s);
		if (!c.parseArgument(ss)) {
			cout << "Error: expression not well formatted" << endl;
		} else {
			c.calculate();
		}
	} else if (ac < 2) {
		cout << "Computor started without arguments. Entering interactive mode." << endl;
		while (1) {
			int res = c.parseArgument();
			if (!res) {
				cout << "Error: expression not well formatted" << endl;
			} else if (res > 0) {
				c.calculate();
			}
			if (res < 0) { break ; }
		}
	}
    return (0);
}
