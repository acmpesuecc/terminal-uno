#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#define MAX_CARDS 108
#define MAX_PLAYERS 10

typedef struct {
    char color;
    int value;
} Card;

typedef struct {
    char name[20];
    Card hand[MAX_CARDS];
    int handSize;
} Player;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printCard(Card card) {
    char* color;
    char* value;
    switch(card.color) {
        case 'R': color = "\033[0;31mR\033[0m"; break;
        case 'B': color = "\033[0;34mB\033[0m"; break;
        case 'G': color = "\033[0;32mG\033[0m"; break;
        case 'Y': color = "\033[0;33mY\033[0m"; break;
        default: color = "W"; break;
    }

    switch(card.value) {
        case 10: value = "S"; break;
        case 11: value = "R"; break;
        case 12: value = "D2"; break;
        case 13: value = "W"; break;
        case 14: value = "+4"; break;
        default: value = malloc(2); sprintf(value, "%d", card.value);
    }

    printf("┌─────────┐\n");
    printf("│%s        │\n", color);
    printf("│    %2s   │\n", value);
    printf("│        %s│\n", color);
    printf("└─────────┘\n");

    if(card.value < 10 || card.value > 14) free(value);
}

void printHand(Player player) {
    printf("Your hand:\n");
    for (int i = 0; i < player.handSize; i++) {
        printf("   ┌───┐ ");
    }
    printf("\n");
    for (int i = 0; i < player.handSize; i++) {
        printf("  │ %c │", player.hand[i].color);
    }
    printf("\n");
    for (int i = 0; i < player.handSize; i++) {
        if (player.hand[i].value < 10) {
            printf("  │ %d │", player.hand[i].value);
        } else {
            switch(player.hand[i].value) {
                case 10: printf("  │ S │"); break;
                case 11: printf("  │ R │"); break;
                case 12: printf("  │D2 │"); break;
                case 13: printf("  │ W │"); break;
                case 14: printf("  │+4 │"); break;
            }
        }
    }
    printf("\n");
    for (int i = 0; i < player.handSize; i++) {
        printf("   └───┘ ");
    }
    printf("\n");
    for (int i = 0; i < player.handSize; i++) {
        printf("    %d    ", i+1);
    }
    printf("\n");
}

void printDiscardPile(Card topCard) {
    printf("Discard Pile:\n");
    printCard(topCard);
}

void printDrawPile() {
    printf("Draw Pile:\n");
    printf("┌─────────┐\n");
    printf("│███████ ┌─┐\n");
    printf("│███████ │ │\n");
    printf("│███████ └─┘\n");
    printf("└─────────┘\n");
