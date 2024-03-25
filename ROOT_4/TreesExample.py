import time
start = time.time()
import numpy as np
import matplotlib.pyplot as plt

a=np.loadtxt("events.txt")

b=a.T

plt.hist(b[0]**2+b[1]**2,30)

print(time.time()-start)

plt.show()


start = time.time()
anp=np.load("events.out.npy")
bnp=anp.T

plt.hist(bnp[0]**2+bnp[1]**2,30)

print(time.time()-start)

plt.show()
