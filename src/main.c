/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
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
int startingClockHand;
int swapI;
char cmd;

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

          printf("\n\rOOM out of memory");
          printf("\n\rPressione qualquer tecla para sair...");

          getchar();
          getchar();

          keyboardDestroy();
          screenDestroy();
          timerDestroy();
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

void removePage(const char *key) {
  for (int i = 0; i < SIZE; i++) {
    if (strcmp(pages[i].key, key) == 0) {
      pages[i].key[0] = '\0';
      pages[i].item[0] = '\0';
      pages[i].chance = 0;
      return;
    }
    if (strcmp(swaps[i].key, key) == 0) {
      swaps[i].key[0] = '\0';
      swaps[i].item[0] = '\0';
      return;
    }
  }
}

const char *readPage(const char *key) {
  for (int i = 0; i < SIZE; i++) {
    if (strcmp(pages[i].key, key) == 0) {
      pages[i].chance = 1; // Mark as recently used
      return pages[i].item;
    }
  }

  for (int i = 0; i < SIZE; i++) {
    if (strcmp(swaps[i].key, key) == 0) {

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

char key[50] = {0};
char item[50] = {0};

int main() {
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
    printf("Digite o comando (W)rite, (R)ead, (D)elete: ");

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
      const char *result = readPage(key);
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      if (result != NULL) {
        printf("Valor: %s (Enter para continuar)", result);
      } else {
        printf("Page Fault! (Enter para continuar)");
      }
      while (getchar() != '\n')
        ;
      getchar();
    } else if (cmd == 'D' || cmd == 'd') {
      screenGotoxy(MINX + 1, MINY + 1);
      limparLinha();
      screenGotoxy(MINX + 1, MINY + 1);
      printf("Digite a chave: ");
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
