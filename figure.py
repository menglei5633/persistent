import numpy as np
import matplotlib.pyplot as plt

x = [0.001, 0.002, 0.003, 0.004, 0.005]
mem_02t_chen = [3974, 3705, 2618, 2130, 2068]
mem_04t_chen = [4054, 3917, 2754, 2241, 2156]
mem_06t_chen = [4166, 4116, 2898, 2372, 2366]
mem_08t_chen = [4726, 4722, 3788, 2989, 2932]

mem_02t_dai = [213513, 155000   , 153450, 130588, 109663]
mem_04t_dai = [233081, 208475, 162363, 144538, 108888]


plt.subplot(231)
plt.plot(x, mem_02t_chen, label = "chen")
plt.plot(x, mem_02t_dai, label = "dai")
plt.title("s = 0.2T")
plt.legend()

plt.subplot(232)
plt.plot(x, mem_04t_chen, label = "chen")
plt.plot(x, mem_04t_dai, label = "dai")
plt.title("s = 0.4T")
plt.legend()

plt.subplot(233)
plt.plot(x, mem_06t_chen, label = "chen")
#plt.plot(x, mem_06t_dai, label = "dai")
plt.title("s = 0.6T")

plt.subplot(234)
plt.plot(x, mem_08t_chen, label = "chen")
#plt.plot(x, mem_o,2t_dai, label = "dai")
plt.title("s = 0.8T")

plt.subplot(235)
plt.plot(x, mem_02t_chen, label = "s = 0.2T")
plt.plot(x, mem_04t_chen, label = "s = 0.4T")
plt.plot(x, mem_06t_chen, label = "s = 0.6T")
plt.plot(x, mem_08t_chen, label = "s = 0.8T")
plt.title("mem")

plt.legend()
plt.show()
