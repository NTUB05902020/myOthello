#include "learn.hpp"


vector<double> vecAdd(const vector<double> &vec1, const vector<double> &vec2){
	vector<double> ret;
	transform(vec1.begin(), vec1.end(), vec2.begin(), back_inserter(ret), plus<double>());
	return ret;
}

vector<double> scalMul(const vector<double> &v, const int &scalar){
	vector<double> ret;
	switch(scalar){
		case 1: ret.assign(v.begin(), v.end()); break;
		case 0: ret.assign(v.size(), 0); break;
		case -1: transform(v.begin(), v.end(), back_inserter(ret), [&](double a)->double{return -a;}); break;
		default: printf("try to multiply double with a int not 1, 0, -1\n"); exit(1);
	}
	return ret;
}

vector<double> scalMul(const vector<double> &v, const double &scalar){
	vector<double> ret;
	transform(v.begin(), v.end(), back_inserter(ret), [&](double a)->double{return a*scalar;});
	return ret;
}

vector<double> scalDiv(const vector<double> &v, const int &scalar){
	vector<double> ret;
	transform(v.begin(), v.end(), back_inserter(ret), [&](double a) -> double{return a/scalar;});
	return ret;
}

/*=================not in learn.hpp=====================*/
double func1(const vector<double> &x, const int &y, const vector<double> &W){
	return log(1 + exp(dot(W, scalMul(x, -y))));
}

/*======================================================*/




double Ein(const vector<double> &W, const vector<vector<double>> &X, const vector<int> &Y){
	double ret = 0.0;  int N = X.size();
	for(int i=0;i<N;++i) ret += func1(X[i], Y[i], W);
	return ret/N;
}


vector<double> graEin(const vector<double> &W, const vector<vector<double>> &X, const vector<int> &Y){
	int D = W.size(), N = X.size(); vector<double> ret(D, 0.0);
	for(int i=0;i<N;++i){
		vector<double> tmp = scalMul(X[i], -Y[i]);
		ret = vecAdd(ret, scalMul(tmp, logistic(dot(W, tmp))));
	}
	return scalDiv(ret, N);
}