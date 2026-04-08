/**
 * main.c
 * Integrated with writePage logic
 * Author: Tiago Barros / Refined by Gemini
 */

#include <stdio.h>
#include <stdlib.h>
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

typedef struct {
  char key[50];
  char item[50];
  int chance;
} Swap;

Page pages[SIZE] = {0};
Swap swaps[SIZE] = {0};

int clockHand = 0;
char cmd;
char key[50] = {0};
char item[50] = {0};

void limparLinha() { printf("                                             "); }

void addPage(const char *a, const char *b) {
  while (1) {
    if (pages[clockHand].key[0] == '\0' || pages[clockHand].chance == 0) {

      if (pages[clockHand].key[0] != '\0') {
        int foundSwapSlot = 0;
        for (int i = 0; i < SIZE; i++) {
          if (swaps[i].key[0] == '\0') {
            strcpy(swaps[i].key, pages[clockHand].key);
            strcpy(swaps[i].item, pages[clockHand].item);
            foundSwapSlot = 1;
            break;
          }
        }

        if (!foundSwapSlot) {
          screenClear();
          screenGotoxy(MINX + 1, MINY + 1);
          screenSetColor(WHITE, RED);
          printf("\n\rOOM - Out of Swap Memory");
          printf("\n\rPressione qualquer tecla para sair...");
          getchar();
          getchar();
          exit(EXIT_FAILURE);
        }
      }

      strcpy(pages[clockHand].key, a);
      strcpy(pages[clockHand].item, b);
      pages[clockHand].chance = 1;

      clockHand = (clockHand + 1) % SIZE;
      return;

    } else {
      pages[clockHand].chance = 0;
      clockHand = (clockHand + 1) % SIZE;
    }
  }
}

void writePage(const char *k, const char *newData) {
  for (int i = 0; i < SIZE; i++) {
    if (pages[i].key[0] != '\0' && strcmp(pages[i].key, k) == 0) {
      strcpy(pages[i].item, newData);
      pages[i].chance = 1; // Give it a second chance
      return;
    }
  }

  for (int i = 0; i < SIZE; i++) {
    if (swaps[i].key[0] != '\0' && strcmp(swaps[i].key, k) == 0) {
      swaps[i].key[0] = '\0';
      swaps[i].item[0] = '\0';
      addPage(k, newData);
      return;
    }
  }

  addPage(k, newData);
}

void removePage(const char *k) {
  for (int i = 0; i < SIZE; i++) {
    if (strcmp(pages[i].key, k) == 0) {
      pages[i].key[0] = '\0';
      pages[i].item[0] = '\0';
      pages[i].chance = 0;
      return;
    }
    if (strcmp(swaps[i].key, k) == 0) {
      swaps[i].key[0] = '\0';
      swaps[i].item[0] = '\0';
      return;
    }
  }
}

const char *readPage(const char *k) {
  for (int i = 0; i < SIZE; i++) {
    if (strcmp(pages[i].key, k) == 0) {
      pages[i].chance = 1;
      return pages[i].item;
    }
  }

  for (int i = 0; i < SIZE; i++) {
    if (strcmp(swaps[i].key, k) == 0) {
      char tempKey[50], tempItem[50];
      strcpy(tempKey, swaps[i].key);
      strcpy(tempItem, swaps[i].item);

      swaps[i].key[0] = '\0';
      swaps[i].item[0] = '\0';

      addPage(tempKey, tempItem);

      for (int j = 0; j < SIZE; j++) {
        if (strcmp(pages[j].key, tempKey) == 0) {
          return pages[j].item;
        }
      }
    }
  }
  return NULL;
}

int main() {
  screenInit(1);
  keyboardInit();
  timerInit(50);
  screenUpdate();

  while (1) {
    screenSetColor(YELLOW, DARKGRAY);

    for (int i = 0; i < SIZE; i++) {
      screenGotoxy(MINX + 1, MINY + 3 + i);
      (clockHand == i) ? printf("->") : printf("  ");
    }

    for (int i = 0; i < SIZE; i++) {
      screenGotoxy(MINX + 3, MINY + 3 + i);
      if (pages[i].key[0] != '\0') {
        printf("Page Index(%d), Chance(%d), Chave: %s", i, pages[i].chance,
               pages[i].key);
      } else {
        printf("Page Index(%d) [VAZIO]                      ", i);
      }
    }

    for (int i = 0; i < SIZE; i++) {
      screenGotoxy(MINX + 3, MINY + SIZE + 5 + i);
      if (swaps[i].key[0] != '\0') {
        printf("Swap Index(%d), Chave: %s", i, swaps[i].key);
      } else {
        printf("Swap Index(%d) [VAZIO]                      ", i);
      }
    }

    screenGotoxy(MINX + 1, MINY + 1);
    limparLinha();
    screenGotoxy(MINX + 1, MINY + 1);
    printf("Comando: (W)rite, (R)ead, (D)elete: ");

    scanf(" %c", &cmd);
    keyboardDestroy();

    if (cmd == 'W' || cmd == 'w') {
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Chave: ");
      scanf("%49s", key);
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Valor: ");
      scanf("%49s", item);

      writePage(key, item);

    } else if (cmd == 'R' || cmd == 'r') {
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Chave: ");
      scanf("%49s", key);
      const char *result = readPage(key);
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      if (result != NULL) {
        printf("Valor: %s (Enter)", result);
      } else {
        printf("Page Fault! (Enter)");
      }
      while (getchar() != '\n')
        ;
      getchar();

    } else if (cmd == 'D' || cmd == 'd') {
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Chave: ");
      scanf("%49s", key);
      removePage(key);
    }

    keyboardInit();
  }

  return 0;
}
