#ifndef LEARN_HPP
#define LEARN_HPP
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cstdio>
#include <cstdlib>
using std::vector;
using std::transform;
using std::back_inserter;
using std::plus;
using std::inner_product;
using std::exp;
using std::log;

const auto logistic = [&](double a) -> double{double ea = exp(a); return ea/(1+ea);};
const auto dot = [&](const vector<double> &vec1, const vector<double> &vec2) -> double{
	return inner_product(vec1.begin(), vec1.end(), vec2.begin(), 0);
};
vector<double> vecAdd(const vector<double> &vec1, const vector<double> &vec2);
vector<double> scalMul(const vector<double> &v, const int &scalar);
vector<double> scalMul(const vector<double> &v, const double &scalar);
vector<double> scalDiv(const vector<double> &v, const int &scalar);
double Ein(const vector<double> &W, const vector<vector<double>> &X, const vector<int> &Y);
vector<double> graEin(const vector<double> &W, const vector<vector<double>> &X, const vector<int> &Y);

#endif