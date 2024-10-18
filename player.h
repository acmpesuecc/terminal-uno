#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_id = 0;

typedef struct player_node {
    int id;
    char name[20];
    int remaining_cards;
    struct player_node* next;
    struct player_node* prev;
} PLAYER;

PLAYER* create_player(int id, char name[]) {
    PLAYER* new_player = (PLAYER*)malloc(sizeof(PLAYER));
    new_player->id = id;
    strcpy(new_player->name, name);
    new_player->remaining_cards = 7;  // Assuming each player starts with 7 cards
    new_player->next = NULL;
    new_player->prev = NULL;
    return new_player;
}

PLAYER* insert_player(PLAYER* head, char name[]) {
    PLAYER* newplayer = create_player(init_id++, name);
    if (head == NULL) {
        head = newplayer;
        head->next = head;
        head->prev = head;
    } else {
        PLAYER* curr = head->prev;
        curr->next = newplayer;
        newplayer->next = head;
        newplayer->prev = curr;
        head->prev = newplayer;
    }
    return head;
}

void display(PLAYER* head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }

    PLAYER* temp = head;
    do {
        printf("player_id: %d    player_name: %s    remaining cards: %d\n", 
               temp->id, temp->name, temp->remaining_cards);
        temp = temp->next;
    } while (temp != head);
}

void play_game(PLAYER* head) {
    PLAYER* current = head;
    char winning_positions[100] = "";  // To store winning order

    while (current->next != current) {  // While more than one player
        // Simulate the player's turn: assume they lose a card each turn
        if (current->remaining_cards > 0) {
            current->remaining_cards--;  // Decrement the card count
        }

        // Check if the current player has no remaining cards
        if (current->remaining_cards == 0) {
            // Record winning position
            strcat(winning_positions, current->name);
            strcat(winning_positions, ", ");

            // Remove player from the list
            if (current == head) {
                head = current->next;  // Update head if necessary
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            PLAYER* to_free = current;
            current = current->next;  // Move to the next player
            free(to_free);
        } else {
            current = current->next;  // Move to next player
        }
    }

    // Display final rankings
    printf("Game Over! Winning positions: %s\n", winning_positions);
}

int main() {
    PLAYER* head = NULL;
    char name[20];
    int num_players;

    printf("Enter number of players: ");
    scanf("%d", &num_players);
    
    for (int i = 0; i < num_players; i++) {
        printf("Enter name: ");
        scanf("%s", name);
        head = insert_player(head, name);
    }

    play_game(head);
    return 0;
}
