# home-server

## trash.cron
A simple bash script designed for cron to take a designated list of pre-defined trash folders, take a log of files that have been recently added and remove them after a given amount of days.

## baccarat.c
A first venture into C, this project was predominantly to become accustomed to the syntax of the language in the hopes of using it to optimise similar but more complex probabilistic scenarios and achieve results in a timely manner. This is also a first venture into programming a full probability tree in a mathematically exact nature, as opposed to previous results relying on Monte Carlo methods. Yet to do is develop a recursive algorithm for the card deal that can be applied to games with deeper trees, such as Blackjack.
It would also be nicer to let the user input the deck distribution perhaps as an argument to the program as opposed to having to manipulate the code and re-compile, although beyond the scope of what the program was initially designed to achieve.

An example of the output is shown below:
```
Deck Length is 52
Number of Decks is 8
Total Cards is 416
Deck Distribution (zero value, one, two, ..., nine):
128 32 32 32 32 32 32 32 32 32

Time to Completion: 67ms

Banker Wins: 2292252566437888
Player Wins: 2230518282592256
Ties: 475627426473216
Total Permutations: 4998398275503360

Banker (Commission) House Edge: 1.058
Banker (Non-Commission) House Edge: 1.458
Player House Edge: 1.235
Tie House Edge: 14.360
Tiger Tie House Edge: 30.735
Tiger House Edge: 16.684
Small Tiger House Edge: 14.333
Big Tiger House Edge: 15.253
```
The output has been verified for accuracy by comparing against results from https://thewizardofodds.com. The bets listed are as provided by Skycity Adelaide Casino in 2020. Of note is the extremely high house edge for the newly introduced "Tiger" bets. Manipulating the deck distribution with some intial intuitions (6's helpful for winning Tigers, 8's and 9's often result in naturals and are unhelpful), it can be found that negative house edges (in favour of the player) are achievable in fairly often scenarios. This perhaps explains the low payouts (despite questions of ethicality for "legitmate" gamblers). Further reading on counting systems for the Tiger side bet (aka. "Super Six") can be read about in this excellent article by Dr. Eliot Jacobson at the following link: https://www.888casino.com/blog/card-counting-the-super-six-baccarat-side-bet
