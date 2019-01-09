import matplotlib.pyplot as plt
import numpy as np

H = [0.0078125,0.015625,  0.03125,  0.0625]
error = [0.000167905, 0.000671732, 0.00268876,  0.0107857]

tmp = 0.0001
ref = []
for i, value in enumerate(H):
  ref.append(tmp*pow(4, i))

plt.figure(1)

plt.loglog(H, error,  'r', label='$L_2$ norm error')
plt.loglog(H, error,  'r*')

plt.loglog(H, ref, color='black', linestyle='--', label='second order reference line')

plt.xlabel("mesh size $h$")
plt.ylabel("$L_2$ norm error $\epsilon$")
plt.title("convergence report")
plt.legend(loc=4)


plt.show()
