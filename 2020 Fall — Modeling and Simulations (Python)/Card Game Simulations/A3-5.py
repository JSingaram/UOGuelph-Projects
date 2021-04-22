#Based on  50000  runs, the expected winnings are 0.0290 per round
#The variance is  4.0862

import numpy as np
import random
import sys
import os

NUM_RUNS = 50000 # Number of runs for the simulation
NUM_DRAWS = 5 # Numbers of cards drawn each round
CONSECUTIVE_CARDS = 3 # Number of consecutive cards needed to win

def createDeck():
	ranks = ["Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"]
	suits = ["Clubs", "Hearts", "Diamonds", "Spades"]
	deck = []

	value = 1
	for rank in ranks:
	    suitvalue = 1
	    for suit in suits:
	        deck.append([rank + " of " + suit, value, suitvalue])
	        suitvalue = suitvalue+1
	    value = value + 1
	return deck

def game(earnings, deck):
	earnings -= 1
	random.shuffle(deck)
	cards = drawCards(deck)
	sortCards(cards)
	maxRun = testRun(cards)
	if(maxRun >= CONSECUTIVE_CARDS):
		earnings += 5
	return earnings

def drawCards(deck):
	cards = []
	for i in range(NUM_DRAWS):
		cards.append(deck[i])
	return cards

def sortCards(cards):
	for i in range(NUM_DRAWS):
		for j in range(0, NUM_DRAWS - i - 1):
			if (cards[j][1] > cards[j+1][1]):
				temp = cards[j]
				cards[j] = cards[j+1]
				cards[j+1] = temp

def testRun(cards):
	currentRun = 1
	maxRun = 1
	for i in range(len(cards) - 1):
		if (cards[i][1] + 1 == cards[i+1][1]):
			currentRun += 1
			if (currentRun > maxRun):
				maxRun = currentRun
		else:
			currentRun = 1
	return maxRun

def report(earnings):
	winnings = np.sum(earnings) / NUM_RUNS
	variance = np.var(earnings)
	print("Based on ", NUM_RUNS, " runs, the expected winnings are", "%.4f" % winnings, "per round")
	print("The variance is ", "%.4f" % variance)	


os.system("cls")
deck  = createDeck()
earnings = np.zeros(NUM_RUNS)
for i in range(NUM_RUNS):
	earnings[i] = game(earnings[i], deck.copy())
	if(i%(NUM_RUNS/10) == 0):
		sys.stdout.write('.')
		sys.stdout.flush()
os.system("cls")
report(earnings)