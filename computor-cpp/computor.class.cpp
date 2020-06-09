// ************************************************************************** //
//                                                                            //
//                                                        ::::::::            //
//   computor.class.cpp                                 :+:    :+:            //
//                                                     +:+                    //
//   By: pacovali <pacovali@student.codam.nl>         +#+                     //
//                                                   +#+                      //
//   Created: 2020/06/04 12:37:04 by pacovali      #+#    #+#                 //
//   Updated: 2020/06/04 12:37:04 by pacovali      ########   odam.nl         //
//                                                                            //
// ************************************************************************** //

#include "computor.class.hpp"

using namespace std;

Computor::Computor() {}

Computor::~Computor() {
	cout << "\nComputor is shutting down" << endl;
}

char	skip(stringstream& ss) {
	char	c = ss.peek();
	string	allowed("+-xX.=*^ 1234567890");
	
	while (!(isdigit(c) || c == '=' || c == '+' || c == '.' ||
			 c == '-' || c == 'x' || c == 'X' || c == '\xff')) {
		if (allowed.find(c) ==  string::npos && c != '\xff') { return (-2); }
		ss.ignore(1);
		if ((c == '.' || c == '^') && !isdigit(ss.peek())) { return (-2); }
		c = ss.peek();
	}
	return (c);
}

bool	Computor::add_values(float& k, float& p, char& sign) {
	if (isnan(k) || p < 0) { return (false); }
	if (isnan(p)) { p = 0; }
	if (p - (int)p != 0) {
		cout << "Error: power " << p << " is not an integer" << endl;
		return (false);
	}
	values_[(int)p].push_back(k);
	k = NAN;
	p = NAN;
	sign = 1;
	return (true);
}

int		Computor::parseArgument(stringstream& ss) {
	bool			is_begin = true;
	bool			get_power = false;
	float			number = NAN;
	float			power = NAN;
	char			right_side_sign_inversion = 1;
	char			sign = 1;
	int				token_qty[2] = {0, 0};
	char			c = 0;

	values_.clear();
	while (c != '\xff') {
		c = skip(ss);
		if (c == '.' || c == -2) {
			return (false);
		} else if (isdigit(c)) {
			if (!isnan(number) && !isnan(power)) { return (false); }
			if (!get_power && !isnan(power)) { return (false); }
			if (get_power) { ss >> power; get_power = false; } else { ss >> number; number *= sign * right_side_sign_inversion; }
			is_begin = false;
		} else if (c == 'x'|| c == 'X') {
			ss.ignore(1);
			if (ss.peek() != '^') { power = 1; } else { get_power = true; }
			if (isnan(number)) { number = 1.0 * sign * right_side_sign_inversion; }
			is_begin = false;
		} else if (c == '-'|| c == '+') {
			if (get_power) { return (false); }
			if (!is_begin && !add_values(number, power, sign)) { return (false); }
			token_qty[right_side_sign_inversion < 0]++;
			sign = c == '-' ? -1 : 1;
			ss.ignore(1);
			is_begin = false;
		} else if (c == '=') {
			if (get_power || right_side_sign_inversion < 0) { return (false); }
			if (!add_values(number, power, sign)) { return (false); }
			token_qty[right_side_sign_inversion < 0]++;
			if (token_qty[0] < 1) { return (false); }
			right_side_sign_inversion = -1;
			ss.ignore(1);
			is_begin = true;
		}
	}
	if (isnan(number) || !add_values(number, power, sign)) { return (false); }
	token_qty[right_side_sign_inversion < 0]++;
	if (token_qty[0] < 1 || token_qty[1] < 1) { return (false); }
	return (true);
}

int		Computor::parseArgument() {
	string s;
	cout << "\n---------------------------------------------\n" << endl;
	cout << "Type entire expression: ";
	getline(cin, s);
	for (auto i = 0u; i < s.size(); i++) {
		if (isdigit(s[i]) && s[i + 1] != '.' && !isdigit(s[i + 1])) { s.insert(i + 1, " "); }
		if (s[i] == '.') {
			if ( (i == 0 || (i > 0 && !isdigit(s[i - 1]) )) && isdigit(s[i + 1])) { s.insert(i, "0"); }
			else if ( i > 0 && !isdigit(s[i + 1]) && isdigit(s[i - 1])) { s.insert(i + 1, "0"); }
		}
	}
	if (s.find("exit") == 0) { return (-1); }
	stringstream ss(s);
	return (parseArgument(ss));
}

void	print_values(const map<int, vector<float>>::iterator& b, const map<int, vector<float>>::iterator& e, char c) {
	bool	print_started = false;

	if (c == 's') {
		for (auto i = b; i != e; i++) {
			if (i->second[0] != 0) {
				if (print_started > 0) { cout << " "; }
				if (i->second[0] < 0) { cout << "-"; }
				if (i->second[0] > 0 && print_started) { cout << "+"; }
				if (print_started > 0) { cout << " "; }
				print_started = true;
				cout << abs(i->second[0]) << " * X^" << i->first;
			}
		}
	} else {
		for (auto j = e; j != b; j--) {
			auto i = prev(j);
			if (i->second[0] != 0) {
				if (print_started) { cout << " "; }
				if (i->second[0] < 0) { cout << "-"; }
				if (i->second[0] > 0 && print_started) { cout << "+"; }
				if (print_started) { cout << " "; }
				print_started = true;
				if ((!i->first) || (i->first && abs(i->second[0]) != 1)) { cout << abs(i->second[0]); }
				if (i->first) { cout << "x"; }
				if (i->first > 1) { cout << "^" << i->first; }
			}
		}
	}
	if (!print_started) { cout << "0"; }
	cout << " = 0" << endl;
}

void	solve_linear(map<int, vector<float>>& values_) {
	if (!values_.count(0)) { values_[0].push_back(0); }
	if (!values_.count(1)) { values_[1].push_back(0); }
	float	x = (values_.at(0)[0] ? -values_.at(0)[0] : 0) / values_.at(1)[0];
	values_.at(0)[0] = -values_.at(0)[0];
	x = x == 0 ? 0 : x;
	
	if (isnan(x) || abs(x) == INFINITY) {
		cout << "All the real numbers are solution." << endl;
		return ;
	}
	cout << "x = " << values_.at(0)[0] << " / " << values_.at(1)[0] << endl;
	if (values_.at(0)[0] > 0 && values_.at(1)[0] < 0) {
		cout << "x = -" << abs(values_.at(0)[0]) << "/" << abs(values_.at(1)[0]) << endl;
	}
	cout << "The solution is:" << endl << x << endl;
}

float	sq_rt(float x) {
	if (x < 0) { return (-1); }
	if (x == 0 || x == 1) { return (x); }
	
	float	res = x;
	while (res * res > x) { res /= 2; }
	while (res * res < x) { res *= 2; }
	
	float	half = res / 2;
	while (half >= (x / 1000000000 > 1 ? 1 : x / 1000000000)) {
		res += half * (res * res - x > 0 ? -1 : 1);
		half = half / 2;
	}
	return (res);
}

void	print_comlex(int index, float r, float i) {
	cout << "x" << index << " = ";
	if (r) { cout << r << (index < 2 ? " " : " +"); }
	
	cout << (index < 2 ? "-" : "");
	if (i != 1) { cout << " " << i; }
	cout << "i" << endl << endl;
}

void	solve_complex_quadratic(float a, float b, float d)
{
	d = -d;
	float r = -b / (2 * a);
	r = r == 0 ? 0 : r;
	float i = abs(sq_rt(d) / (2 * a));

	cout << "x1 = ( -b - sqrt(d) ) / 2a" << endl;
	cout << "x1 = ( " << (-b == 0 ? 0 : -b) << " - sqrt(" << -d << ") ) / ( 2 * " << a << " )" << endl;
	cout << "x1 = ( " << (-b == 0 ? 0 : -b) << " - " << sq_rt(d) << "i ) / " << 2 * a << endl;
	print_comlex(1, r, i);

	cout << "x2 = ( -b + sqrt(d) ) / 2a" << endl;
	cout << "x2 = ( " << (-b == 0 ? 0 : -b) << " + sqrt(" << d << ") ) / ( 2 * " << a << " )" << endl;
	cout << "x2 = ( " << (-b == 0 ? 0 : -b) << " + " << sq_rt(d) << "i ) / " << 2 * a << endl;
	print_comlex(2, r, i);
}

void	solve_quadratic(map<int, vector<float>>& values_) {
	float c = values_.count(0) ? values_.at(0)[0] : 0;
	float b = values_.count(1) ? values_.at(1)[0] : 0;
	float a = values_.count(2) ? values_.at(2)[0] : 0;

	if ((b * b - 4 * a * c) / 100000 > 1) { cout << fixed; } else { cout << defaultfloat; }
	cout << "\nDiscriminant d = b^2 - 4ac" << endl;
	cout << "d = " << b << " * " << b << " - 4 * " << a << " * " << c << endl;
	cout << "d = " << b * b << " - " << (4 * a * c == 0 ? 0 : 4 * a * c) << endl;
	float d = b * b - 4 * a * c;
	d = d == 0 ? 0 : d;
	cout << "d = " << d << endl << endl;
	if (d > 0) {
		cout << "Discriminant is strictly positive, the two solutions are:" << endl;
		cout << "x1 = ( -b - sqrt(d) ) / 2a" << endl;
		cout << "x1 = ( " << (-b == 0 ? 0 : -b) << " - sqrt(" << d << ") ) / ( 2 * " << a << " )" << endl;
		cout << "x1 = ( " << (-b == 0 ? 0 : -b) << " - " << sq_rt(d) << " ) / " << 2 * a << endl;
		cout << "x1 = " << -b - sq_rt(d) << " / " << 2 * a << endl;
		if (a < 0 && -b - sq_rt(d) > 0) {
			cout << "x1 = - " << abs(-b - sq_rt(d)) << " / " << abs(2 * a) << endl;
		}
		cout << "x1 = " << ((-b - sq_rt(d)) / (2 * a) == 0 ? 0 : (-b - sq_rt(d)) / (2 * a)) << endl << endl;

		cout << "x2 = ( -b + sqrt(d) ) / 2a" << endl;
		cout << "x2 = ( " << (-b == 0 ? 0 : -b) << " + sqrt(" << d << ") ) / ( 2 * " << a << " )" << endl;
		cout << "x2 = ( " << (-b == 0 ? 0 : -b) << " + " << sq_rt(d) << " ) / " << 2 * a << endl;
		cout << "x2 = " << -b + sq_rt(d) << " / " << 2 * a << endl;
		if (a < 0 && -b + sq_rt(d) > 0) {
			cout << "x2 = - " << abs(-b + sq_rt(d)) << " / " << abs(2 * a) << endl;
		}
		cout << "x2 = " << ((-b + sq_rt(d)) / (2 * a) == 0 ? 0 : (-b + sq_rt(d)) / (2 * a)) << endl;
	} else if (d == 0) {
		cout << "Discriminant is equal to zero, the solution is:" << endl;
		cout << "x = -b / 2a" << endl;
		cout << "x = " << (-b == 0 ? 0 : -b) << " / (2 * " << a << ")" << endl;
		cout << "x = " << (-b == 0 ? 0 : -b)  << " / " << 2 * a << endl;
		if (a < 0 && -b > 0) {
			cout << "x2 = - " << abs(-b) << " / " << abs(2 * a) << endl;
		}
		cout << "x = " << (-b / ( 2 * a ) == 0 ? 0 : -b / ( 2 * a )) << endl;
	} else {
		cout << "Discriminant is strictly negative, there are no solutions on real numbers space." << endl;
		cout << "Solving on complex numbers space:" << endl;
		solve_complex_quadratic(a, b, d);
	}
}

void	Computor::calculate() {
	cout << "Evaluating expression...\n" << endl;
	int		max_pow = 0;
	for (auto i = values_.begin(); i != values_.end(); i++) {
		float sum = accumulate(i->second.begin(), i->second.end(), 0.0);
		i->second.clear();
		i->second.push_back(sum);
		if (sum != 0) { max_pow = max(max_pow, (int)i->first); }
	}
	cout << "Reduced form: ";
	print_values(values_.begin(), values_.end(), 's');
	cout << "Natural form: ";
	print_values(values_.begin(), values_.end(), 'n');
	cout << "Polynomial degree: " << max_pow << endl;
	if (max_pow == 0 && values_.count(0)
		&& values_.at(0)[0] != 0) {
		cout << "Equation does no make sence. Can't solve." << endl;
		return ;
	}
	if (max_pow < 2) { solve_linear(values_); }
	if (max_pow == 2) { solve_quadratic(values_); }
	if (max_pow > 2) {
		cout << "The polynomial degree is strictly greater than 2, I can't solve." << endl;
	}
}
