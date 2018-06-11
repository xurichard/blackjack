/*Blackjack
	Display welcome message
	Bet, two cards, Hit, Stand, Split
	Player wins when they beat the computer or reach 21
	Player loses when the computer has a higher score than the player or the player plays past 21
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int balance=2500;

int bet();
void shuffle(int *deck, int size_of_deck);
void quit();
void game_play();
void initialize_deck(int deck[], int size_of_deck);
void play_round(int wager, int deck[], int *deck_index);
int card_value(int value);
char *csuit(int value);
char *cface(int value);
int hit(int deck[], int *deck_index);
void ace(int value1, int value2, int *ace1);
int checkTotal(int cards[], int num_cards);

int main(void){ //begin game: display welcome message
	printf("Welcome to Blackjack!\n");
	printf("Press enter to begin.\n");
	getchar();
	
	while(1){
		game_play();
		printf("Thanks for playing, would you like to restart? (Yes or No): ");

		char answer[6];
		scanf("%s", answer);
		if(strcmp(answer, "No") == 0){
			quit(); //checked and done
		} else{
			balance = 2500;
		}
	}
}

void game_play(){ //play game
	int deck[52];
	initialize_deck(deck, 52); //checked and done
	shuffle(&deck[0], 52);
	int deck_index = 0;

	while(balance>0){
		printf("Your current balance is %d\n", balance);
		int mybet = bet(); //checked and done
		play_round(mybet, deck, &deck_index);
	}
}

void play_round(int wager, int deck[], int *deck_index){ //deal cards
	int player_cards[12]; // can't have more than 11 cards without getting 21, end of array when element is NULL
	int player_num_cards = 0;
	int dealer_cards[12];
	int dealer_num_cards = 0;

	// deal 2 cards to player
	for(int i = 1; i < 3; i++){
		int myHit = hit(deck, deck_index);
		player_cards[player_num_cards] = myHit;
		player_num_cards++;

	    char *cn = csuit(myHit);
	    char *cf = cface(myHit);
	    printf("Your card %d: %s of %s. \n", i, cf, cn);
	}
	printf("\n");

	// deal 2 cards to dealer
	for(int i = 1; i < 3; i++){
		int myHit = hit(deck, deck_index);
		dealer_cards[dealer_num_cards] = myHit;
		dealer_num_cards++;
	}
	printf("Dealer's face-up card: %s of %s.\n", cface(dealer_cards[1]), csuit(dealer_cards[1])); // first card is faced down

	printf("\n");

	while(1){
		int myTotal = checkTotal(player_cards, player_num_cards);
		int dealerTotal = checkTotal(dealer_cards, dealer_num_cards);
		printf("Your current total: %d | Dealer's faceup total: %d\n", myTotal, checkTotal(dealer_cards+1, dealer_num_cards-1));
		// checks if player won or lost immediately
		if(myTotal == 21){
			if(dealerTotal != 21){
				// dealer hits until soft 17
				printf("You won! \n");
				balance += wager;
			}else{
				printf("It was a draw. \n");
			}
			break;
		} else if(myTotal > 21){
			printf("You lost. \n");
			balance -= wager;
			break;
		} else{
			// player action
			printf("Would you like to 'hit' or 'stand'? \n");
			char input[6];
			scanf("%5s", input);
			printf("\n");
			if(strcmp(input, "hit")==0){
				int myHit = hit(deck, deck_index);
				player_cards[player_num_cards] = myHit;
				player_num_cards++;

				char *cn = csuit(myHit);
			    char *cf = cface(myHit);
			    printf("Your card: %s of %s.\n", cf, cn);
			} else if(strcmp(input,"stand")==0){
				// dealer hits until soft 17
				while(dealerTotal < 17){
					if(dealerTotal > myTotal){
						break;
					}
					dealerTotal = checkTotal(dealer_cards, dealer_num_cards);
					int myHit = hit(deck, deck_index);
					dealer_cards[dealer_num_cards] = myHit;
					dealer_num_cards++;
				}

				if(dealerTotal > 21 || dealerTotal < myTotal){
					printf("You won! \n");
					balance += wager;
				}else if(dealerTotal == myTotal){
					printf("It was a draw. \n");
				}else{
					printf("You lost. \n");
					balance -= wager;
				}
				break;
			}else{
				printf("Invalid entry. Please try again. Please type 'hit' to be dealt a new card or 'stand' to stand.\n");
			}
		}
	}

	printf("The dealer had: ");
	for(int i = 0; i < dealer_num_cards; i++){
		int card = dealer_cards[i];
		printf("%s of %s", cface(dealer_cards[i]), csuit(dealer_cards[i]));
		if(i < dealer_num_cards - 1){
			printf(", ");
		}
	}
	printf("\n");

	printf("You had: ");
	for(int i = 0; i < player_num_cards; i++){
		int card = player_cards[i];
		printf("%s of %s", cface(player_cards[i]), csuit(player_cards[i]));
		if(i < player_num_cards - 1){
			printf(", ");
		}
	}
	printf("\n");

	printf("\n\n");
}

// Find total based on cards in hand, finds the largest sum less than 21 based on aces
void helper(int remaining_cards[], int possible_total, int possible[], int num_cards){
	if(num_cards <= 0){
		if(possible_total <= 32)
			possible[possible_total] = 1;
	} else{
		if(strcmp(cface(remaining_cards[0]), "Ace") == 0){
			helper(&remaining_cards[1], possible_total + 11, possible, num_cards-1);
			helper(&remaining_cards[1], possible_total + 1, possible, num_cards-1);
		} else{
			helper(&remaining_cards[1], possible_total + card_value(remaining_cards[0]), possible, num_cards-1);
		}
	}
}


int checkTotal(int cards[], int num_cards){
	int possible[32] = {0}; // 21 + possible 10

	// generate all possible combinations with ace being 1 or 11
	helper(cards, 0, possible, num_cards);

	// look for largest total less than or equal to 21
	for(int i = 21; i >= 0; i--){
		if(possible[i]){
			return i;
		}
	}

	for(int i = 32; i >= 21; i--){
		if(possible[i]){
			return i;
		}
	}
	return 32;
}


int hit(int deck[], int *deck_index){ //play card
    int deal_1 = deck[*deck_index];
	*deck_index = *deck_index + 1;
	if(*deck_index > 52){
		shuffle(&deck[0], 52);
		*deck_index = 0;
	}
	return deck[*deck_index];
}

char *csuit(int value){ //card suits
    switch(value/13){
        case 0 :
            return (char *)"Spades";
        case 1 :
            return (char *)"Clubs";
        case 2 :
            return (char *)"Diamonds";
        case 3 :
            return (char *)"Hearts";
        default:
        	return (char *)NULL;
    }
}

char *cface(int value){ //card value
    char *output;
    switch(value%13){
        case 0 :
            output = (char *) "Ace";
            break;
        case 1 :
            output = (char *)"2";
            break;
        case 2 :
            output = (char *)"3";
            break;
        case 3 :
            output = (char *)"4";
            break;
        case 4 :
            output = (char *)"5";
            break;
        case 5 :
            output = (char *)"6";
            break;
        case 6 :
            output = (char *)"7";
            break;
        case 7 :
            output = (char *)"8";
            break;
        case 8 :
            output = (char *)"9";
            break;
        case 9 :
            output = (char *)"10";
            break;
        case 10 :
            output = (char *)"Jack";
            break;
        case 11 :
            output = (char *)"Queen";
            break;
        case 12 :
            output = (char *)"King";
    }
    return output;
}

int card_value(int value){ //determine card value
    int a;
    if(((value%13)+1)<10)
        a = value%13+1;
    else 
        a = 10;
    return a;
}

int bet(){ //wager on Blackjack
	int wager = 0;
	printf("Please enter your bet: \n");
	scanf("%d", &wager);
	while(wager>balance || wager<=0){
		printf("You cannot bet that amount. Please enter a new bet: \n");
		if(scanf(" %d", &wager) <= 0){
			getchar();
		}
	}
	return wager;
}

void quit(){ //quit program
    printf("Are you sure you want to quit? Type 'Y' or 'N'.\n");
    char answer[6];
	scanf("%s", answer);
    if(strcmp(answer,"Y")==0)
        exit(0);
    else 
        printf("Quit cancelled.\n");
}

void initialize_deck(int deck[], int size_of_deck){ //initial deck of cards
    for(int i = 0;i<size_of_deck;i++){
        deck[i] = i;
    }
}

// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
void shuffle(int *deck, int size_of_deck){ //shuffling cards
	srand(time(NULL));
	for (int i = size_of_deck-1; i > 0; i--){
		int j = rand() % (i+1);
		int temp = deck[j];
    	deck[j] = deck[i];
    	deck[i] = temp;
	}
}
