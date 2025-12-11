#include <stdio.h>
#include <string.h>

#define MAX_CANDIDATES 5
#define ADMIN_PASSWORD "1234"

void showMenu();
void castVote(int votes[], int candidateCount);
void showResults(int votes[], char candidates[][50], int candidateCount);
void resetVotes(int votes[], int candidateCount);
int checkAdmin();

int main() {
    int votes[MAX_CANDIDATES + 1] = {0}; // last index for NOTA
    char candidates[MAX_CANDIDATES][50];
    int candidateCount;
    int choice;
    int i;

    printf("===== SIMPLE EVM (Electronic Voting Machine) =====\n\n");

    // ----- Setup Phase -----
    printf("Enter number of candidates (1 to %d): ", MAX_CANDIDATES);
    scanf("%d", &candidateCount);

    if (candidateCount < 1 || candidateCount > MAX_CANDIDATES) {
        printf("Invalid number of candidates. Exiting...\n");
        return 0;
    }

    printf("\nEnter candidate names:\n");
    for (i = 0; i < candidateCount; i++) {
        printf("Candidate %d name: ", i + 1);
        // clear input buffer
        getchar();
        fgets(candidates[i], sizeof(candidates[i]), stdin);

        // remove newline from fgets
        size_t len = strlen(candidates[i]);
        if (len > 0 && candidates[i][len - 1] == '\n') {
            candidates[i][len - 1] = '\0';
        }
    }

    // ----- Voting Phase -----
    while (1) {
        showMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            castVote(votes, candidateCount);
            break;
        case 2:
            if (checkAdmin()) {
                showResults(votes, candidates, candidateCount);
            } else {
                printf("Incorrect password! Access denied.\n");
            }
            break;
        case 3:
            if (checkAdmin()) {
                resetVotes(votes, candidateCount);
                printf("All votes have been reset to zero.\n");
            } else {
                printf("Incorrect password! Access denied.\n");
            }
            break;
        case 4:
            printf("Exiting EVM. Thank you!\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void showMenu() {
    printf("\n===== EVM MENU =====\n");
    printf("1. Cast Vote\n");
    printf("2. Show Results (Admin only)\n");
    printf("3. Reset All Votes (Admin only)\n");
    printf("4. Exit\n");
}

void castVote(int votes[], int candidateCount) {
    int i, choice;

    printf("\n----- CAST YOUR VOTE -----\n");
    for (i = 0; i < candidateCount; i++) {
        printf("%d. Candidate %d\n", i + 1, i + 1);
    }
    printf("%d. NOTA (None of the Above)\n", candidateCount + 1);

    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= candidateCount + 1) {
        votes[choice - 1]++; // store vote
        printf("Your vote has been recorded successfully.\n");
    } else {
        printf("Invalid choice. Vote cancelled.\n");
    }
}

void showResults(int votes[], char candidates[][50], int candidateCount) {
    int i;
    int totalVotes = 0;
    int maxVotes = -1;
    int winnerIndex = -1;
    int tie = 0;

    printf("\n----- VOTING RESULTS -----\n");
    for (i = 0; i < candidateCount; i++) {
        printf("Candidate %d (%s): %d votes\n", i + 1, candidates[i], votes[i]);
        totalVotes += votes[i];
    }

    printf("NOTA: %d votes\n", votes[candidateCount]);
    totalVotes += votes[candidateCount];

    printf("\nTotal votes cast: %d\n", totalVotes);

    // Find winner (ignoring NOTA)
    for (i = 0; i < candidateCount; i++) {
        if (votes[i] > maxVotes) {
            maxVotes = votes[i];
            winnerIndex = i;
            tie = 0;
        } else if (votes[i] == maxVotes) {
            tie = 1;
        }
    }

    if (totalVotes == 0) {
        printf("No votes have been cast yet.\n");
    } else if (tie) {
        printf("Result: It is a TIE between candidates.\n");
    } else {
        printf("Winner: Candidate %d (%s) with %d votes.\n",
               winnerIndex + 1, candidates[winnerIndex], maxVotes);
    }
}

void resetVotes(int votes[], int candidateCount) {
    int i;
    for (i = 0; i <= candidateCount; i++) {
        votes[i] = 0;
    }
}

int checkAdmin() {
    char password[20];

    printf("Enter admin password: ");
    scanf("%s", password);

    if (strcmp(password, ADMIN_PASSWORD) == 0) {
        return 1;
    }
    return 0;
}
