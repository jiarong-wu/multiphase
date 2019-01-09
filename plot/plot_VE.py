import matplotlib.pyplot as plt
import numpy as np

r = []
numerical_solution = []
analytical_solution = []

r_file_in = open('../solver/output/analysis/r.out', 'r')
for line in r_file_in.readlines():
  r.append(float(line))

numerical_file_in = open('../solver/output/analysis/numerical.out', 'r')
for line in numerical_file_in.readlines():
  numerical_solution.append(float(line))

analytical_file_in = open('../solver/output/analysis/analytical.out', 'r')
for line in analytical_file_in.readlines():
  analytical_solution.append(float(line))



plt.figure(1)

plt.plot(r, numerical_solution,  'b', label='numerical solution')
plt.plot(r, analytical_solution, 'r', label='analytical solution')


plt.xlabel("distance r [m]")
plt.ylabel("hydraulic head h [m]")
plt.title("h vs. r")
plt.legend(loc=4)


plt.show()
