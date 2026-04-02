/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 */

#include <string.h>

#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define SIZE 4

typedef struct {
  char key[50];
  char item[50];
  int chance;
} Page;

Page Pages[SIZE] = {0};

int clockHand = 0;
char cmd;

void limparLinha() { printf("                                             "); }

void addPage(const char *a, const char *b) {
  // Infinite loop that acts as the sweeping clock hand
  while (1) {
    // Check the page currently pointed to by the hand
    // We replace it IF it's empty OR its chance has been reduced to 0
    if (Pages[clockHand].key[0] == '\0' || Pages[clockHand].chance == 0) {

      // Overwrite the slot
      strcpy(Pages[clockHand].key, a);
      strcpy(Pages[clockHand].item, b);
      Pages[clockHand].chance = 1;

      // Advance the hand for the NEXT time we need to add a page
      // The modulo operator (%) makes it wrap around from 3 back to 0
      clockHand = (clockHand + 1) % SIZE;
      break; // Stop looking!

    } else {
      // The chance is 1. Give it a "second chance" by setting it to 0.
      Pages[clockHand].chance = 0;

      // Advance the hand to check the next page in the circle
      clockHand = (clockHand + 1) % SIZE;
    }
  }
}

// Looks for a key, shifts elements back to fill the gap, and clears the last
// slot
void removePage(const char *key) {
  for (int i = 0; i < SIZE; i++) {
    if (strcmp(Pages[i].key, key) == 0) {
      // Clear the slot in place to maintain the clock's integrity
      Pages[i].key[0] = '\0';
      Pages[i].item[0] = '\0';
      Pages[i].chance = 0;
      break;
    }
  }
}

// Looks for a key, changes chance to 1, and returns a pointer to the item
// string
const char *readPage(const char *key) {
  for (int i = 0; i < SIZE; i++) {
    if (strcmp(Pages[i].key, key) == 0) {
      Pages[i].chance = 1; // It was referenced, so it gets a 1
      return Pages[i].item;
    }
  }
  return NULL;
}

char key[50] = {0};
char item[50] = {0};
int flag = 0;

int main() {
  static int ch = 0;
  static long timer = 0;

  screenInit(1);
  keyboardInit();
  timerInit(50);

  screenUpdate();

  while (67 == 67) {

    screenSetColor(YELLOW, DARKGRAY);
    for (int i = 0; i < SIZE; i++) {
      screenGotoxy(MINX + 1, MINY + 3 + i);
      (clockHand == i) ? printf("->") : printf("  ");
    }

    for (int i = 0; i < SIZE; i++) {
      screenGotoxy(MINX + 3, MINY + 3 + i);
      if (Pages[i].key[0] != '\0') {
        // Added blank spaces at the end to overwrite old text artifacts
        printf("Index(%d), Chance(%d), Chave: %s       ", i, Pages[i].chance,
               Pages[i].key);
      } else {
        // Explicitly clear empty rows
        printf("Index(%d) [VAZIO]                              ", i);
      }
    }

    screenGotoxy(MINX + 1, MINY + 1);
    limparLinha();
    screenGotoxy(MINX + 1, MINY + 1);

    printf("Digite o comando (W)rite, (R)ead, (D)elete: ");
    // 1. Fixed Newline bug by adding a space before %c

    scanf(" %c", &cmd);
    keyboardDestroy();

    if (cmd == 'W' || cmd == 'w') {
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Digite a chave: ");
      scanf("%49s", key);

      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Digite o valor: ");
      scanf("%49s", item);

      addPage(key, item);

    } else if (cmd == 'R' || cmd == 'r') {
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Digite a chave: ");
      scanf("%49s", key);

      // 2. Fixed Crash by checking if result is NULL before printing
      const char *result = readPage(key);
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      if (result != NULL) {
        printf("Valor: %s (Enter para continuar)                   ", result);
      } else {
        printf("Page Fault! (Enter para continuar)             ");
      }

      // Clear the '\n' left by the previous scanf
      while (getchar() != '\n')
        ;
      // Actually pause and wait for the user to press Enter again
      getchar();
    }
    // 3. Added the missing Delete logic
    else if (cmd == 'D' || cmd == 'd') {
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Digite a chave:                                    ");
      scanf("%49s", key);

      removePage(key);
    }

    keyboardInit();
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}
