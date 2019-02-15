import numpy as np
import matplotlib.pyplot as plt
import warnings
import sys

def logistic(val):
	try:
		return 1/(1 + np.exp(-val))
	except Warning:
		#print('np.exp() overflowed at logistic', -val)
		return 0.0
def errFunc(x, y, w):
	tmp = np.dot(w, x)
	if y==1:
		try:
			return np.log(1 + np.exp(-tmp))
		except Warning:
			#print('np.exp() overflowed at errFunc', -tmp)
			return -tmp
	else:
		try:
			return np.log(1 + np.exp(tmp))
		except Warning:
			#print('np.exp() overflowed at errFunc', tmp)
			return tmp
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

warnings.filterwarnings('error')
N, D, X, Y, w, fp = 0, 38, list(), list(), list(), open('Kifu', 'r')
for line in iter(fp):
	s, N = line.split(' '), N+1
	tmp = [int(x) for x in s]
	tmp = tmp[:3] + tmp[5:11] + tmp[13:19] + tmp[21:24] + tmp[40:43] + tmp[45:51] + tmp[53:59] + tmp[61:]
	X.append(tmp[:-1])
	Y.append(tmp[-1])
fp.close()
fp = open('Eval', 'rb')
X, Y, w = np.array(X), np.array(Y), np.fromfile(fp, dtype=np.float64)
fp.close()
#print(X)
#print(Y)
print(len(X), len(X[0]), len(Y), len(w))
#print(N, D)
#print(w)
learningTime = 300
ita, lr = 10, 0
EINS, x_axis = list(), np.arange(learningTime)
for i in range(learningTime):
	tmp, gra = Ein(X, Y, w), graEin(X, Y, w)
	if i % 30 == 0:
		print(i, 'times', tmp)
	EINS.append(tmp)
	lr += tmp**2
	w -= ita/np.sqrt(lr)*gra
print(w)
#print(x_axis)
#print(EINS)
fig = plt.figure()
plt.clf()
plt.plot(x_axis, EINS, '-o')
fig.savefig('learning_status.png')
fp = open('newEval', 'wb')
for val in w:
	#print(val)
	fp.write(val)
fp.close()
