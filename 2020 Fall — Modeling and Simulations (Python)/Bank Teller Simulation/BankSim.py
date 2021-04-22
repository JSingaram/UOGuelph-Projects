import numpy as np
import random
import matplotlib.pyplot as plt
import re

import simpy


SEED_MAX = 10000 # Max. seed value
NUM_CUSTOMERS = 500  # Total number of customers
INTERVAL_CUSTOMERS = 10.0  # Generate new customers roughly every x seconds
MIN_PATIENCE = 100  # Min. customer patience
MAX_PATIENCE = 200  # Max. customer patience
NUM_RUNS = 1000 # Number of runs for the simulation
SERVICE_LAMBDA = 10.0 # Lambda for exponential distribution of service time

# Create variables to store data
arrival_time = np.zeros(NUM_CUSTOMERS, dtype=float)
wait_time = np.zeros(NUM_CUSTOMERS, dtype=float)
service_time = np.zeros(NUM_CUSTOMERS, dtype=float)
left_early = np.zeros(NUM_RUNS, dtype=int)



# Generates a random seed
def seed():
    return random.randint(0, SEED_MAX)


def source(env, number, interval, counter, run):
    """Source generates customers randomly"""
    for i in range(number):
        c = customer(env, 'Customer%02d' % i, counter, run)
        env.process(c)
        t = random.expovariate(1.0 / interval)
        yield env.timeout(t)


def customer(env, name, counter, run):
    num = re.findall("(\d+)", name)

    int_name = int(num[0])
    service_time[int_name] = -1.0
    arrival_time[int_name] = env.now

    with counter.request() as req:
        patience = random.uniform(MIN_PATIENCE, MAX_PATIENCE)
        results = yield req | env.timeout(patience)

        wait_time[int_name] = env.now - arrival_time[int_name]

        if req in results:

            checkout_time = random.expovariate(1.0 / SERVICE_LAMBDA)
            yield env.timeout(checkout_time)
            service_time[int_name] = env.now - arrival_time[int_name] - wait_time[int_name]

        else:
            left_early[run] = left_early[run] +1


# Outputs results of single run
def output1():
    plt.figure(figsize=(12, 10))
    plt.suptitle('Simulation Results')

    plt.subplot(2, 2, 1)
    counts, bins = np.histogram(arrival_time, 50)
    plt.hist(bins[:-1], bins, weights=counts)
    plt.xlabel('Arrival time')
    plt.ylabel('Abundance')
    plt.title('Arrival times in grocery store queue for one run')
    plt.grid(True)

    plt.subplot(2, 2, 2)
    counts, bins = np.histogram(wait_time, 50)
    plt.hist(bins[:-1], bins, weights=counts)
    plt.xlabel('Wait time')
    plt.ylabel('Abundance')
    plt.title('Wait times in grocery store queue for one run')
    plt.grid(True)

    plt.subplot(2, 2, 3)
    counts, bins = np.histogram(service_time, 50, (0, max(service_time)))
    plt.hist(bins[:-1], bins, weights=counts)
    plt.xlabel('Service time')
    plt.ylabel('Abundance')
    plt.title('Service times in grocery store queue for one run')
    plt.text(15, 26, "%d customers left early." %(left_early[NUM_RUNS -1]), fontsize=15)
    plt.grid(True)


# Outputs results of all runs
def output1000():
    plt.subplot(2, 2, 4)
    counts, bins = np.histogram(left_early, 30)
    plt.hist(bins[:-1], bins, weights=counts)
    plt.xlabel('Number of customers leaving early')
    plt.ylabel('Abundance')
    plt.title('Number of customers leaving early for 1000 runs')
    plt.grid(True)


# Returns maximum value in an arry
def max(x):
    y = x[0]
    for i in range(len(x)):
        if (x[i] > y):
            y = x[i]
    return y


# Setup and start the simulation
random.seed(seed())
env = simpy.Environment()
counter = simpy.Resource(env, capacity=1)
env.process(source(env, NUM_CUSTOMERS, INTERVAL_CUSTOMERS, counter, NUM_RUNS -1))
env.run()

output1()

#Run 999 more times to get left_early values
for run in range(NUM_RUNS -2):
    random.seed(seed())
    env = simpy.Environment()
    counter = simpy.Resource(env, capacity=1)
    env.process(source(env, NUM_CUSTOMERS, INTERVAL_CUSTOMERS, counter, int(run)))
    env.run()

output1000()
plt.show()
