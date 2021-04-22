import numpy as np
import random
import matplotlib.pyplot as plt
import pylab
import math
from scipy.stats import norm
from scipy.stats import poisson
from scipy.special import erf
from scipy.stats import poisson
from sklearn.neighbors import KernelDensity



def uniformDistribution(n, minValue, maxValue):
	uniformSample = np.zeros(n)
	for i in range(n):
		uniformSample[i] = random.uniform(minValue, maxValue)
	return uniformSample


def uniformToStandardUniform(x):
	min = x[0]
	max = x[0]
	for i in range(len(x)):
		if (x[i] < min): min = x[i]
	for i in range(len(x)):
		if (x[i] > max): max = x[i]
	for i in range(len(x)):
		x[i] = (x[i] - min) / (max-min)
	return x


def plotUniform():	
	plt.figure()

	plt.subplot(2, 2, 1)
	counts, bins = np.histogram(uniformDistribution(100, 0, 1), 50)
	plt.hist(bins[:-1], bins, weights=counts)

	plt.subplot(2, 2, 2)
	counts, bins = np.histogram(uniformDistribution(1000, 0, 1), 50)
	plt.hist(bins[:-1], bins, weights=counts)

	plt.subplot(2, 2, 3)
	counts, bins = np.histogram(uniformDistribution(10000, 0, 1), 50)
	plt.hist(bins[:-1], bins, weights=counts)





def plotNormalPDF():
	plt.figure()

	uniformSample = uniformDistribution(100, -3, 3)
	gaussvals = normalPDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 1)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(1000, -3, 3)
	gaussvals = normalPDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 2)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(10000, -3, 3)
	gaussvals = normalPDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 3)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(100, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 4)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(1000, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 5)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(10000, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 6)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)


def plotNormalCDF():
	plt.figure()

	uniformSample = uniformDistribution(100, -3, 3)
	gaussvals = normalCDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 1)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(1000, -3, 3)
	gaussvals = normalCDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 2)
	counts, bins = np.histogram(gaussvals, 50)
	bins = normalCDF(bins, 0, 1)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(10000, -3, 3)
	gaussvals = normalCDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 3)
	counts, bins = np.histogram(gaussvals, 50)
	bins = normalCDF(bins, 0, 1)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(100, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 4)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(1000, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 5)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)

	uniformSample = uniformDistribution(10000, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 6)
	counts, bins = np.histogram(gaussvals, 50)
	plt.hist(bins[:-1], bins, weights=counts)


def normalPDF(x, mu, sigma):
	for i in range(len(x)):
		x[i] = np.exp((-0.5) * np.power((x[i]-mu) / sigma, 2)) / (sigma * np.sqrt(2*np.pi))
		#x[i] = (1/(np.sqrt(2)*np.pi*sigma)*np.exp(-np.power(x[i] - mu, 2.) / (2 * np.power(sigma, 2.))))
	return x


def normalCDF(x, mu, sigma):
	for i in range(len(x)):
		x[i] = (1 + erf( (x[i]-mu) / (sigma*np.sqrt(2)) ))/2
		#x[i] = ((1/2)*(1+erf((x[i] - mu)/( np.sqrt(2) * sigma))))
	return x


def normalToStandardNormal(x, mu, sigma):
	for i in range(len(x)):
		x[i] = (x[i]-mu) / sigma
	return x


def boxMullerTransformation(x):
	normalSample = np.zeros(len(x))
	y = uniformToStandardUniform(x)
	z = uniformDistribution(len(x), 0, 1)
	for i in range(len(x)):
		normalSample[i] = np.sqrt(-2 * np.log(z[i])) * np.cos(2 * np.pi * y[i])
	return normalSample






def intDistribution(n, minValue, maxValue):
	uniformSample = np.zeros(n)
	for i in range(n):
		uniformSample[i] = random.randint(minValue, maxValue)
	return uniformSample


def plotPoissonPMF():
	plt.figure()

	intSample = intDistribution(100, 0, 10)
	poissonvals = poissonPMF(intSample, 2)
	plt.subplot(2, 2, 1)
	counts, bins = np.histogram(poissonvals, 10)
	plt.hist(bins[:-1], bins, weights=counts)

	intSample = intDistribution(1000, 0, 10)
	poissonvals = poissonPMF(intSample, 2)
	plt.subplot(2, 2, 2)
	counts, bins = np.histogram(poissonvals, 10)
	plt.hist(bins[:-1], bins, weights=counts)

	intSample = intDistribution(10000, 0, 10)
	poissonvals = poissonPMF(intSample, 2)
	plt.subplot(2, 2, 3)
	counts, bins = np.histogram(poissonvals, 10)
	plt.hist(bins[:-1], bins, weights=counts)


def plotPoissonCMF():
	plt.figure()

	intSample = intDistribution(100, 0, 10)
	poissonvals = poissonCMF(intSample, 1)
	plt.subplot(2, 2, 1)
	counts, bins = np.histogram(poissonvals, 10)
	plt.hist(bins[:-1], bins, weights=counts)

	intSample = intDistribution(1000, 0, 10)
	poissonvals = poissonCMF(intSample, 1)
	plt.subplot(2, 2, 2)
	counts, bins = np.histogram(poissonvals, 10)
	plt.hist(bins[:-1], bins, weights=counts)

	intSample = intDistribution(10000, 0, 10)
	poissonvals = poissonCMF(intSample, 1)
	plt.subplot(2, 2, 3)
	counts, bins = np.histogram(poissonvals, 10)
	plt.hist(bins[:-1], bins, weights=counts)


def poissonPMF(x, mu):
	pmf = np.zeros(len(x))
	for i in range(len(x)):
		num = x[i]
		pmf[i] = np.exp(-mu) * pow(mu, x[i]) / math.factorial(x[i])
	return pmf


def poissonCMF(x, mu):
	cmf = np.zeros(len(x))
	for i in range(len(x)):
		num = x[i]
		for j in range(int(num)):
			cmf[i] = cmf[j] + (np.exp(-mu) * pow(mu, j) / math.factorial(j))
	return cmf





def kernelPlotNormalPDF():
	plt.figure()

	xd = np.linspace(0, 0.5, 50)
	kde = KernelDensity(bandwidth = 0.03, kernel='gaussian')

	uniformSample = uniformDistribution(100, -3, 3)
	gaussvals = normalPDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 1)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	uniformSample = uniformDistribution(1000, -3, 3)
	gaussvals = normalPDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 2)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	uniformSample = uniformDistribution(10000, -3, 3)
	gaussvals = normalPDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 3)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	xd = np.linspace(-4, 4, 50)
	kde = KernelDensity(bandwidth = 0.15, kernel='gaussian')

	uniformSample = uniformDistribution(100, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 4)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	uniformSample = uniformDistribution(1000, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 5)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	uniformSample = uniformDistribution(10000, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 6)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))


def kernelPlotNormalCDF():
	plt.figure()

	xd = np.linspace(0, 1, 50)
	kde = KernelDensity(bandwidth = 0.05, kernel='gaussian')

	uniformSample = uniformDistribution(100, -3, 3)
	gaussvals = normalCDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 1)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	uniformSample = uniformDistribution(1000, -3, 3)
	gaussvals = normalCDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 2)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	uniformSample = uniformDistribution(10000, -3, 3)
	gaussvals = normalCDF(uniformSample, 0, 1)
	plt.subplot(3, 2, 3)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	xd = np.linspace(-4, 4, 50)
	kde = KernelDensity(bandwidth = 0.15, kernel='gaussian')

	uniformSample = uniformDistribution(100, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 4)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	uniformSample = uniformDistribution(1000, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 5)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))

	uniformSample = uniformDistribution(10000, -3, 3)
	gaussvals = boxMullerTransformation(uniformSample)
	plt.subplot(3, 2, 6)
	density = sum(norm(xi).pdf(xd) for xi in gaussvals)
	kde.fit(gaussvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(gaussvals, np.full_like(gaussvals, -0.01))


def kernelPlotPoissonPMF():
	plt.figure()

	xd = np.linspace(0, 0.4, 50)
	kde = KernelDensity(bandwidth = 0.03, kernel='gaussian')

	intSample = intDistribution(100, 0, 10)
	poissonvals = poissonPMF(intSample, 2)
	plt.subplot(2, 2, 1)
	kde.fit(poissonvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(poissonvals, np.full_like(poissonvals, -0.01))

	intSample = intDistribution(1000, 0, 10)
	poissonvals = poissonPMF(intSample, 2)
	plt.subplot(2, 2, 2)
	kde.fit(poissonvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(poissonvals, np.full_like(poissonvals, -0.01))

	intSample = intDistribution(10000, 0, 10)
	poissonvals = poissonPMF(intSample, 2)
	plt.subplot(2, 2, 3)
	kde.fit(poissonvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(poissonvals, np.full_like(poissonvals, -0.01))


def kernelPlotPoissonCMF():
	plt.figure()

	xd = np.linspace(0, 1.5, 50)
	kde = KernelDensity(bandwidth = 0.05, kernel='gaussian')

	intSample = intDistribution(100, 0, 10)
	poissonvals = poissonCMF(intSample, 1)
	plt.subplot(2, 2, 1)
	# ADD KERNEL HERE
	kde.fit(poissonvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(poissonvals, np.full_like(poissonvals, -0.01))

	intSample = intDistribution(1000, 0, 10)
	poissonvals = poissonCMF(intSample, 1)
	plt.subplot(2, 2, 2)
	# ADD KERNEL HERE
	kde.fit(poissonvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(poissonvals, np.full_like(poissonvals, -0.01))

	intSample = intDistribution(10000, 0, 10)
	poissonvals = poissonCMF(intSample, 1)
	plt.subplot(2, 2, 3)
	# ADD KERNEL HERE
	kde.fit(poissonvals[:, None])
	logprob = kde.score_samples(xd[:, None])
	plt.fill_between(xd, np.exp(logprob))
	plt.plot(poissonvals, np.full_like(poissonvals, -0.01))


# Part 1 — Create and plot uniform distribution
plotUniform()


# Part 2 — Create and plot uniform distribution
plotNormalPDF()
plotNormalCDF()

# Part 3 — Create and plot Poisson distribution
plotPoissonPMF()
plotPoissonCMF()

# Part 4 — Create and plot non-parabolic distribution
kernelPlotNormalPDF()
kernelPlotNormalCDF()
kernelPlotPoissonPMF()
kernelPlotPoissonCMF()


pylab.show()
