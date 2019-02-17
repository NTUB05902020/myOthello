import numpy as np
import matplotlib.pyplot as plt

y_axis, fp = list(), open('EINS', 'r')
for line in iter(fp): y_axis.append(float(line))
fp.close()
x_axis = np.arange(len(y_axis))

fig = plt.figure()
plt.clf()
plt.plot(x_axis, y_axis, '-o')
fig.savefig('learning_status.png')
