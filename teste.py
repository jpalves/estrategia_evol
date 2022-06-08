import numpy as np
import fpga3
import atest
import matplotlib.pyplot as plt


xx = fpga3.algoritmo(10)
x  = np.linspace(0, 6.28, 100)

y=[]
for i in x:
   y.append(atest.doStuff(i))
plt.plot(x, y)
plt.plot(xx,atest.doStuff(xx),'o')
plt.show()

