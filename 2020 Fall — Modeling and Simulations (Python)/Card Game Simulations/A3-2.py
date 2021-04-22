#Based on  50000  runs, the expected winnings are -0.0580 per round
#The variance is  46.2126

import numpy as np
import random
import sys
import os

NUM_RUNS = 50000 # Number of runs for the simulation

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
	card = deck[0]
	random.shuffle(deck)
	card2 = deck[0]
	if(card2[0] == card[0]):
		earnings += 50
	return earnings

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
