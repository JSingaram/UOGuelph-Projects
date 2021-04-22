# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.


def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print_hi('PyCharm')

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
import time, os, random

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

random.shuffle(deck)
score = 0
card1 = deck.pop(0)

while True:
    print(card1[0])
    print(card1[1])
    print(card1[2])




    os.system("cls")  # linux "clear
    print("Your score so far is", score)
    print("\n\nThe current card is", card1[0])
    while True:
        choice = input("higher or lower?")
        if len(choice) > 0:
            if choice[0].lower() in ["h", "l"]:
                break

    card2 = deck.pop(0)
    print(card2[0])
    print(card2[1])
    print(card2[2])
    print("The next card picked is", card2[0])
    time.sleep(1)

    if choice[0].lower() == "h" and card2[1] > card1[1]:
        print("Correct!")
        score += 1
        time.sleep(1)
    if choice[0].lower() == "h" and card2[1] < card1[1]:
        print("Wrong!")
        time.sleep(1)
        break
    if choice[0].lower() == "l" and card2[1] < card1[1]:
        print("Correct!")
        score += 1
        time.sleep(1)
    if choice[0].lower() == "l" and card2[1] > card1[1]:
        print("Wrong!")
        time.sleep(1)
        break
    else:
        print("draw!")
    print("Card 0: ", card2[0])
    print("Card 1: ", card2[1])
    print("Card 2: ", card2[2])
    time.sleep(5)

    card1 = card2

os.system("cls")
print("Game over!")
print("You final score is", score)
time.sleep(4)
os.system("cls")