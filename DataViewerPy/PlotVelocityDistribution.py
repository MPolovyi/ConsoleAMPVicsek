import json
import matplotlib.pyplot as plt
import math
import numpy as np

files = ["/home/mpolovyi/Desktop/SPP_res"]

VelocityDistributionX = {}
VelocityDistributionY = {}
for file in files:
    with open(file) as data_file:
        data = json.load(data_file)

    for item in data:
        noise = item["Noise"]
        VelocityDistributionX[noise] = item["ParticleVelocitiesX"]
        VelocityDistributionY[noise] = item["ParticleVelocitiesY"]

# VelocityDistribution = np.mean(VelocityDistribution, axis=0)

fig = plt.figure()
ax = fig.add_subplot(2, 2, 1)

for v in VelocityDistributionX.keys():
    ax.plot(VelocityDistributionX[v], marker="o", label=str(v))
plt.legend()
ax = fig.add_subplot(2, 2, 2)
for v in VelocityDistributionY.keys():
    ax.plot(VelocityDistributionY[v], marker="o", label=str(v))

VelocityDistributionModule = {}
ax = fig.add_subplot(2, 2, 3)
for v in VelocityDistributionX.keys():
    VelocityDistributionModule[v] = []
    for index in xrange(len(VelocityDistributionX[v])):
        VelocityDistributionModule[v].append(math.sqrt(math.pow(VelocityDistributionX[v][index],2) + math.pow(VelocityDistributionY[v][index],2)))

    ax.plot(VelocityDistributionModule[v])


plt.legend()
plt.show()
