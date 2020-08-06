import random
import sys

lambdaParam = random.uniform(0.05, 0.5)
memory_size = random.choice([1000, 3000, 5000, 10000])
num_neighbours = random.choice([6, 12, 24, 48, 64, 80, 100])
UpdateSchema = random.choice([0, 1, 2, 3])
tau = random.choice([0.02, 0.1, 0.5, 1])
gamma = random.uniform(0.7, 1)
featureDim = random.choice([500, 1000, 2000, 3000, 4000])
# print("memorySize = " + str(memory_size))
# print("memoryNumNeighbours = " + str(num_neighbours))
# print("memoryLambda = " + str(lambdaParam))
# print("memoryUpdateSchema = " + str(UpdateSchema))
# print("tau = " + str(tau))

file = open(sys.argv[1], "w+")

file.write("memorySize = " + str(memory_size) + "\n")
file.write("memoryNumNeighbors = " + str(num_neighbours) + "\n")
file.write("memoryLambda = " + str(lambdaParam) + "\n")
file.write("memoryUpdateSchema = " + str(UpdateSchema) + "\n")
file.write("tau = " + str(tau) + "\n")
file.write("gamma = " + str(gamma) + "\n")
file.write("aggregator = 1" + "\n")
file.write("featureDim = " + str(featureDim) + "\n")

file.close()