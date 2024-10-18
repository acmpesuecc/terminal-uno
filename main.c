#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "player.h"
#include "cards.h"

void display_how_to_play() {
    system("cls");
    printf("\n\n                                  HOW TO PLAY UNO\n");
    printf("===========================================================================\n");
    printf("1. Each player starts with 7 cards.\n");
    printf("2. The top card of the discard pile starts the play.\n");
    printf("3. On your turn:\n");
    printf("   - Play a card matching the top card in color, number, or symbol.\n");
    printf("   - Draw a card if you can't play.\n");
    printf("4. Special cards:\n");
    printf("   - Skip: Next player loses their turn\n");
    printf("   - Reverse: Reverses direction of play\n");
    printf("   - Draw Two: Next player draws 2 cards and loses their turn\n");
    printf("   - Wild: Change the color of play\n");
    printf("   - Wild Draw Four: Change color and next player draws 4 cards\n");
    printf("5. When you have one card left, you must shout 'UNO'!\n");
    printf("   - Press 'U' within 5 seconds of playing your second-to-last card.\n");
    printf("   - Other players can press 'C' to call you out if you forget.\n");
    printf("6. First player to get rid of all their cards wins!\n");
    printf("===========================================================================\n");
    printf("Press any key to return to the main menu...");
    getch();
}

void arrow1(int optPos, int curPos){
    if(optPos==curPos) printf(">");
}

void menuDesc(int i, int x){
    if(i == 1 && x == 1) printf("    (Start a new game)\n\n");
    else if(i == 2 && x == 2) printf("      (New to the Game?)\n\n");
    else if(i == 3 && x == 3) printf("          (The Devs)\n\n");
    else printf("\n\n");
}

char renderMainMenu() {
    int x = 1;
    while(1){
        system("cls");
        if(x<1) x=4;
        if(x>4) x=1;
        printf("========== UNO ==========\n");
        arrow1(1,x); printf(" \033[0;32m[c]\033[0m Create a Session"); menuDesc(1,x);
        arrow1(2,x); printf(" \033[0;34m[h]\033[0m How to play"); menuDesc(2,x);
        arrow1(3,x); printf(" \033[0;33m[r]\033[0m Credits"); menuDesc(3,x);
        arrow1(4,x); printf(" \033[0;31m[x]\033[0m Quit\n");
        printf("[Use Arrow keys to select]");
        char ch = getch();
        switch(ch){
            case 72: x--; break;
            case 75: x--; break;
            case 80: x++; break;
            case 77: x++; break;
            case 13:
                if(x==1) return 'c';
                if(x==2) return 'h';
                if(x==3) return 'r';
                if(x==4) return 'x';
                break;
        }
    }
}

void draw_card(char color, int value) {
    printf(" _____ \n");
    printf("|     |\n");
    printf("| %c %2d |\n", color, value);
    printf("|     |\n");
    printf(" ----- \n");
}

void draw_pile() {
    printf(" _____ \n");
    printf("|#####|\n");
    printf("|#####|\n");
    printf("|#####|\n");
    printf(" ----- \n");
}

void animate_card_play(char color, int value) {
    for (int i = 0; i < 5; i++) {
        system("cls");
        printf("\nPlaying card...");
        draw_card(color, value);
        Sleep(200);
        system("cls");
        printf("\nPlaying card..");
        draw_card(color, value);
        Sleep(200);
        system("cls");
        printf("\nPlaying card.");
        draw_card(color, value);
        Sleep(200);
    }
    system("cls");
    printf("\nCard played: ");
    draw_card(color, value);
    printf("\n");
}

int canPlay(char C, int N, struct CardStack stk, struct Card last) {
    int chk1 = 0, chk2 = 0;
    for (int i = 0; i < stk.size; i++) {
        if ((stk.cards[i].col == C) && (stk.cards[i].value == N)) {
            chk1 = 1;
            break;
        }
    }
    if ((last.col == C) || (last.value == N) || (last.col == 'N')) chk2 = 1;
    return (chk1 && chk2);
}

int main() {
    PLAYER* head = NULL;

    int number_of_players, action, dir, N;
    char name[20], C;
    char menuChoice;
    struct CardStack players[7];

    struct Card lastPlayed;

    while (1) {
        menuChoice = renderMainMenu();
        switch (menuChoice) {
            case 'c':
                srand(time(NULL));
                struct CardStack mainDeck;
                struct CardStack discardPile;
                discardPile.size = 0;
                initializeCardStack(&mainDeck);
                shuffleCardStack(&mainDeck);
                dir = 0;
                system("cls");
                printf("=== PLAYERS ==========================\n");
                printf("Enter number of players in session: ");
                scanf("%d", &number_of_players);

                for (int i = 0; i < number_of_players; i++) {
                    printf("[Player %d] Name: ", i + 1);
                    scanf("%s", name);
                    head = insert_player(head, name);
                }

                // Reset UNO flags at the start of the game
                PLAYER* current = head;
                do {
                    current->has_shouted_uno = 0;
                    current = current->next;
                } while (current != head);

                distributeCards(&mainDeck, players, number_of_players, 7);
                int win = -1;
                PLAYER* aux = head;
                int wild = 0;
                dealCard(&mainDeck, &discardPile);
                while (win == -1) {
                    aux->has_shouted_uno = 0;
                    printf("\n%s's turn\n", aux->name);
                    printf("Last Played Card: ");
                    draw_card(discardPile.cards[discardPile.size - 1].col, discardPile.cards[discardPile.size - 1].value);
                    printf("Your hand: ");
                    printPlayerStack(&players[aux->id]);
                    printf("Action: [1] Play card [2] Draw card: ");
                    scanf("%d", &action);

                    if (action == 1) {
                        printf("Enter Card Color (R, G, B, Y) and Value (0-9, 10=Skip, 11=Reverse, 12=Draw Two, 13=Wild, 14=Wild Draw Four): ");
                        scanf(" %c%d", &C, &N);
                        if (canPlay(C, N, players[aux->id], discardPile.cards[discardPile.size - 1])) {
                            playCard(&players[aux->id], &discardPile, C, N);
                            animate_card_play(C, N);
                        } else {
                            printf("Invalid card. Please try again.\n");
                        }
                    } else if (action == 2) {
                        dealCard(&mainDeck, &players[aux->id]);
                        printf("You drew a card: ");
                        draw_card(players[aux->id].cards[players[aux->id].size - 1].col, players[aux->id].cards[players[aux->id].size - 1].value);
                    }

                    // Check for win condition
                    if (players[aux->id].size == 0) {
                        win = aux->id;
                        printf("\n%s wins the game!\n", aux->name);
                    }

                    // Move to the next player
                    aux = (dir == 0) ? aux->next : aux->prev;
                }
                break;
            case 'h':
                display_how_to_play();
                break;
            case 'r':
                system("cls");
                printf("=== CREDITS ==========================\n");
                printf("UNO Game developed by [Your Name]\n");
                printf("Press any key to return to the main menu...");
                getch();
                break;
            case 'x':
                exit(0);
        }
    }
    return 0;
}