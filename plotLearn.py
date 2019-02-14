import numpy as np
import matplotlib.pyplot as plt

def logistic(val):
	return 1/(1 + np.exp(-val))
def errFunc(x, y, w):
	if y==1: return np.log(1 + np.exp(-np.dot(w, x)))
	else: return np.log(1 + np.exp(np.dot(w, x)))
def Ein(X, Y, w):
	ret, N = 0.0, len(Y)
	for i in range(N): ret += errFunc(X[i], Y[i], w)
	return ret/N
def graEin(X, Y, w):
	ret, N = np.zeros(len(X[0])), len(Y)
	for i in range(N):
		if Y[i] == 1: ret -= logistic(-np.dot(w, X[i]))*X[i]
		else: ret += logistic(np.dot(w, X[i]))*X[i]
	return ret/N
"""
fp = open('Kifu', 'r')
line = fp.readline()
N, D = 0, len(line.split(' '))-1
fp.seek(0)"""
X, Y, w = list(), list(), list()
"""for line in iter(fp):
	s, N = line.split(' '), N+1
	tmp = [int(x) for x in s]
	X.append(tmp[:-1])
	Y.append(tmp[-1])
fp.close()"""
D = 38
fp = open('Eval', 'rb')
X, Y, w = np.array(X), np.array(Y), np.fromfile(fp, dtype=np.float64)
fp.close()
#print(X)
#print(Y)
#print(N, D)
print(w)
learningTime = 10000
learningRate = 100
EINS, x_axis = list(), np.arange(learningTime)
for i in range(learningTime):
	tmp, gra = Ein(X, Y, w), graEin(X, Y, w)
	EINS.append(tmp)
	w -= learningRate*gra
plt.plot(x_axis, EINS, '-o')
plt.savefig('learning_status.png')
