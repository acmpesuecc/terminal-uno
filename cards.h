#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_id = 0;
char data[500];

// Player structure
typedef struct player_node   
{
    int id;
    char name[20];
    int remaining_cards;
    int has_won;               // Indicates if the player has won (1 = won, 0 = still in game)
    int position;              // Ranking position after a player wins
    struct player_node* next;
    struct player_node* prev;
} PLAYER;

// Function to create a new player node
PLAYER* create_player(int id, char name[]) {
    PLAYER* new_player = (PLAYER*) malloc(sizeof(PLAYER));
    new_player->id = id;
    strcpy(new_player->name, name);
    new_player->remaining_cards = 7;
    new_player->has_won = 0;         // Player starts the game as active
    new_player->position = 0;        // Ranking is assigned upon winning
    new_player->next = NULL;
    new_player->prev = NULL;
    return new_player;
}

// Function to insert a new player in the doubly circular linked list
PLAYER* insert_player(PLAYER* head, char name[]) {
    PLAYER* new_player = create_player(init_id++, name);
    if (head == NULL) {
        head = new_player;
        head->next = head;
        head->prev = head;
    } else {
        PLAYER* last = head->prev;
        last->next = new_player;
        new_player->next = head;
        new_player->prev = last;
        head->prev = new_player;
    }
    return head;
}

// Function to display the list of players
void display_players(PLAYER* head) {
    if (head == NULL) {
        printf("No players in the game.\n");
        return;
    }
    PLAYER* temp = head;
    do {
        if (!temp->has_won) { // Only display active players
            printf("Player ID: %d, Name: %s, Remaining Cards: %d\n", temp->id, temp->name, temp->remaining_cards);
        }
        temp = temp->next;
    } while (temp != head);
}

// Function to search for a player by ID and display their name
void search_player_name(PLAYER* head, int id) {
    PLAYER* current = head;
    do {
        if (current->id == id) {
            printf("Player Name: %s\n", current->name);
            return;
        }
        current = current->next;
    } while (current != head);
}


PLAYER* remove_player(PLAYER* head, PLAYER* player) {
    if (head == NULL || player == NULL) return head;

    
    if (player->next == player && player->prev == player) {
        free(player);
        return NULL;
    }

    
    if (head == player) {
        head = player->next;
    }

    
    player->prev->next = player->next;
    player->next->prev = player->prev;

    free(player);
    return head;
}


PLAYER* mark_player_as_winner(PLAYER* head, PLAYER* winner, int position) {
    winner->has_won = 1;      
    winner->position = position; 

    printf("Player %s has won! Ranking position: %d\n", winner->name, position);
    
    
    head = remove_player(head, winner);

    return head;
}


void game_loop(PLAYER* head) {
    int num_players = 0;
    int rank = 1; 
    PLAYER* temp = head;

    
    do {
        num_players++;
        temp = temp->next;
    } while (temp != head);

    
    while (num_players > 1) {
        PLAYER* current_player = head;
        do {
            
            if (!current_player->has_won && current_player->remaining_cards <= 0) {
                
                head = mark_player_as_winner(head, current_player, rank++);
                num_players--; 
            }
            current_player = current_player->next;
        } while (current_player != head);
    }


    printf("Final Player Remaining: %s wins!\n", head->name);
    head->position = rank;

    // Display the final rankings
    printf("\n--- Final Rankings ---\n");
    temp = head;
    do {
        printf("Rank %d: %s\n", temp->position, temp->name);
        temp = temp->next;
    } while (temp != head);
}

int main() {
    PLAYER* head = NULL;
    char name[20];
    
    // Add players to the game
    for (int i = 0; i < 4; i++) {
        printf("Enter name of player %d: ", i + 1);
        scanf("%s", name);
        head = insert_player(head, name);
    }

    // Start the game loop
    game_loop(head);

    return 0;
}
