#include <stdio.h>
#include <time.h>
#define numDecks 8
// PAYOUTS
#define bankerPayout 1.95
#define playerPayout 2
#define tiePayout 9
#define tigerTiePayout 36
#define tiger5Payout 13
#define tiger6Payout 21
#define smallTigerPayout 23
#define bigTigerPayout 51

// free to modify
unsigned deckBaseDist[] = { 16, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

clock_t start, now;
double timeElapsed;

unsigned long long bankerWins = 0, playerWins = 0, ties = 0;
unsigned long singlePairs = 0, doublePairs = 0, twinPairs = 0; //unused for now - better to run a seperate program
unsigned long tigerTies = 0, tigers = 0, smallTigers = 0, bigTigers = 0;

//number of zero cards, ones, twos, ..., nines
unsigned deckDist[10];
unsigned deckLength = 0;
unsigned totalCards;
unsigned deal[] = { 0, 0, 0, 0, 0, 0 };

void dealCards();
void getResult(unsigned playerScore, unsigned bankerScore, unsigned numberOfCards);
unsigned fourCardWinCheck();
unsigned fiveCardWinCheck();
unsigned sixCardDeal();

int main() {
	
	start = clock();
	
	//deckLength is the sum of deckDist
	for (int i = 0; i < 10; ++i) {
		deckLength = deckLength+deckBaseDist[i];
		deckDist[i] = numDecks*deckBaseDist[i];
	}
	totalCards = deckLength*numDecks;
	printf("Deck Length is %d\nNumber of Decks is %d\nTotal Cards is %d\n", deckLength, numDecks, totalCards);
	printf("Deck Distribution (zero value, one, two, ..., nine):\n");
	for (int i = 0; i < 10; ++i) {
		printf("%d ", deckDist[i]);
	}
	printf("\n");
	
	dealCards();
	printf("\nTime to Completion: %.0fms\n", 1000*((double)(clock() - start)/CLOCKS_PER_SEC));
	
	unsigned long long totalPermutations = bankerWins + playerWins + ties;
	printf("\nBanker Wins: %llu\nPlayer Wins: %llu\nTies: %llu\nTotal Permutations: %llu\n\n", bankerWins, playerWins, ties, totalPermutations);

	double bankerNonCommissionHE, bankerCommissionHE, playerHE, tieHE, tigerTieHE, tigerHE, smallTigerHE, bigTigerHE;
	bankerCommissionHE = 100*(1 - ((bankerPayout*(long double)bankerWins + 1*ties) / (long double)totalPermutations));
	tigers = smallTigers + bigTigers;
	bankerNonCommissionHE = 100*(1 - ((2*((long double)bankerWins - (long double)tigers) + 1*ties + 1.5*(long double)tigers)  / (long double)totalPermutations));
	playerHE = 100*(1 - ((playerPayout*(long double)playerWins + 1*ties) / (long double)totalPermutations));
	tieHE = 100*(1 - tiePayout*((long double)ties / (long double)totalPermutations));
	tigerTieHE = 100*(1 - tigerTiePayout*((long double)tigerTies / (long double)totalPermutations));
	tigerHE = 100*(1 - (tiger5Payout*((long double)smallTigers / (long double)totalPermutations) + tiger6Payout*((long double)bigTigers / (long double)totalPermutations)));
	smallTigerHE = 100*(1 - smallTigerPayout*((long double)smallTigers / (long double)totalPermutations));
	bigTigerHE = 100*(1 - bigTigerPayout*((long double)bigTigers / (long double)totalPermutations));

	printf("Banker (Commission) House Edge: %.3f\nBanker (Non-Commission) House Edge: %.3f\nPlayer House Edge: %.3f\nTie House Edge: %.3f\n", bankerCommissionHE, bankerNonCommissionHE, playerHE, tieHE);
	printf("Tiger Tie House Edge: %.3f\nTiger House Edge: %.3f\nSmall Tiger House Edge: %.3f\nBig Tiger House Edge: %.3f\n", tigerTieHE, tigerHE, smallTigerHE, bigTigerHE);

}

void dealCards() {
        for (unsigned i = 0; i < 10; ++i) {
                deal[0] = i;
                for (unsigned j = 0; j < 10; ++j) {
                        deal[1] = j;
                        for (unsigned k = 0; k < 10; ++k) {
                                deal[2] = k;
                                for (unsigned l = 0; l < 10; ++l) {
                                        deal[3] = l;
					for (unsigned m = 0; m < 10; ++m) {
						deal[4] = m;
						for (unsigned n = 0; n < 10; ++n) {
							deal[5] = n;
							if (fourCardWinCheck() == 1) {
								if (fiveCardWinCheck() == 1) {
									sixCardDeal();
								}
							}
						}
					}
				}
			}
                }
        }
}


void getResult(unsigned playerScore, unsigned bankerScore, unsigned numberOfCards) {
	
	unsigned long long permutations = 1;
	
	for (unsigned i = 0; i < 6; ++i) {
		int thisPerm = deckDist[deal[i]]; // i is i-th dealt card, deal[i] is the card value, deckDist[deal[i]] is how many of that card value is in the deck
		for (unsigned j = 0; j < i; ++j) {
			if ( deal[j] == deal[i] ) {
				//without replacement condition
				thisPerm = thisPerm-1;
			}
		}
		if ( thisPerm <= 0 ) { //ran out of that card - couldn't happen
			permutations = 0;
			break;
		}
		else {
			permutations = permutations*thisPerm;
		}
	}
	
	if ( bankerScore > playerScore ) {
		bankerWins = bankerWins + permutations;

		// count small and big tigers
		if ( bankerScore == 6 ) {
			if ( numberOfCards == 5 ) {
				smallTigers = smallTigers + permutations;
			}
			else {
				bigTigers = bigTigers + permutations;
			}
		}
	}
	else if ( bankerScore < playerScore ) {
		playerWins = playerWins + permutations;
	}
	else {
		ties = ties + permutations;
		
		// count tiger ties
		if ( bankerScore == 6 ) {
			tigerTies = tigerTies + permutations;
		}
	}
	
}


unsigned fourCardWinCheck() {
	// 1 -> more cards, 0 -> winning result
	
	unsigned playerScore = (deal[0]+deal[2])%10, bankerScore = (deal[1]+deal[3])%10;
	
	// natural or bbq
	if ( (playerScore == 8) || (playerScore == 9) || (bankerScore == 8) || (bankerScore == 9) ) {
		getResult(playerScore, bankerScore, 4);
		// debug line (print winning hand)
		//for (unsigned i = 0; i <= 5; ++i) { printf("%d ", deal[i]); } printf("\n"); getchar();
		return 0;
	}
	else if ( ((playerScore == 6) || (playerScore == 7)) && ((bankerScore == 6) || (bankerScore == 7)) ) {
		getResult(playerScore, bankerScore, 4);
		return 0;
	}
	else {
		return 1;
	}
}


unsigned fiveCardWinCheck() {
	
	unsigned playerScore = (deal[0]+deal[2])%10, bankerScore = (deal[1]+deal[3])%10;
	if (playerScore <= 5) {
		playerScore = (playerScore+deal[4])%10;
		//check if bank needs a 3rd card
		if (bankerScore <= 2) {
			return 1;
		}
		else if ( (bankerScore == 3) && (deal[4] != 8) ) {
			return 1;
		}
		else if ( (bankerScore == 4) && ( (deal[4] >= 2) && (deal[4] <= 7) ) ) {
			return 1;
		}
		else if ( (bankerScore == 5) && ( (deal[4] >= 4) && (deal[4] <= 7) ) ) {
			return 1;
		}
		else if ( (bankerScore == 6) && ( (deal[4] == 6) || (deal[4] == 7) ) ) {
			return 1;
		}
		else {
			getResult(playerScore, bankerScore, 5);
			return 0;
		}
	}
	// else playerScore is 6 or 7 (and stands) and bank gets 3rd card -> no more cards
	else {
		bankerScore = (bankerScore+deal[4])%10;
		getResult(playerScore, bankerScore, 5);
		return 0;
	}
}


unsigned sixCardDeal() {
	
	unsigned playerScore = (deal[0]+deal[2]+deal[4])%10, bankerScore = (deal[1]+deal[3]+deal[5])%10;
	getResult(playerScore, bankerScore, 6);
	
}
