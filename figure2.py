import numpy as np
import matplotlib.pyplot as plt

x = [0.001, 0.002, 0.003, 0.004, 0.005]
mem_02t_chen = [3974, 3705, 2618, 2130, 2068]
mem_04t_chen = [4054, 3917, 2754, 2241, 2156]
mem_06t_chen = [4166, 4116, 2898, 2372, 2366]
mem_08t_chen = [4726, 4722, 3788, 2989, 2932]

mem_02t_dai = [213513, 155000   , 153450, 130588, 109663]
mem_04t_dai = [233081, 208475, 162363, 144538, 108888]


mem_02T_c2d2_chen = [3958,3013,2279,2074,1939]
mem_02T_c2d5_chen = [3864,2671,2124,1946,1741]
mem_02T_c2d8_chen = [3854,2627,2093,1994,1730]

mem_04T_c2d2_chen = [4062,3041,2389,2147,1997]
mem_04T_c2d5_chen = [3886,2915,2122,1912,1810]
mem_04T_c2d8_chen = [3869,2598,2162,1905,1769]

mem_06T_c2d2_chen =  [4262,3260,2560,2313,2178]
mem_06T_c2d5_chen = [3942,2698,2263,1988,1844]
mem_06T_c2d8_chen = [3934,2961,2223,1980,1834]

mem_08T_c2d2_chen = [6230,4442,3268,2799,2609]
mem_08T_c2d5_chen = [4110,4078,2791,2323,2246]
mem_08T_c2d8_chen = [4550,3849,2759,2277,2110]



plt.subplot(221)
plt.plot(x, mem_02T_c2d2_chen, label = "d=2")
plt.plot(x, mem_02T_c2d5_chen, label = "d=5")
plt.plot(x, mem_02T_c2d8_chen, label = "d=8")
plt.title("s = 0.2T")
plt.legend()

plt.subplot(222)
plt.plot(x, mem_04T_c2d2_chen, label = "d=2")
plt.plot(x, mem_04T_c2d5_chen, label = "d=5")
plt.plot(x, mem_04T_c2d8_chen, label = "d=8")
plt.title("s = 0.4T")
plt.legend()

plt.subplot(223)
plt.plot(x, mem_06T_c2d2_chen, label = "d=2")
plt.plot(x, mem_06T_c2d5_chen, label = "d=5")
plt.plot(x, mem_06T_c2d8_chen, label = "d=8")
plt.title("s = 0.6T")
plt.legend()

plt.subplot(224)
plt.plot(x, mem_08T_c2d2_chen, label = "d=2")
plt.plot(x, mem_08T_c2d5_chen, label = "d=5")
plt.plot(x, mem_08T_c2d8_chen, label = "d=8")
plt.title("s = 0.8T")
plt.legend()



plt.legend()
plt.show()
