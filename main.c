#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NrCartiPachet 52
#define MAX 100
#define VAL_MAX 15
#define NR_PLAYERS_SCARABEI 4

typedef struct List {
  int valoare;
  char carte[3];
  struct List *next;
} List;

typedef struct DCList { // Lista dublu inlantuita si circulara
  struct DCList *next;
  struct DCList *prev;
  List *pachet;
  int nr_jucator;

} DCList;

List *CreateList() {
  List *list = (List *)malloc(sizeof(List));
  list->valoare = 0;
  strcpy(list->carte, "\0");
  list->next = NULL;
  return list;
}

List *InitializarePachet() {
  int i, j, valoare = VAL_MAX;
  char string1[] = "AKQJT98765432", string2[] = "shcd";
  List *pachet = CreateList();
  pachet->carte[0] = string1[0];
  pachet->carte[1] = string2[0];
  pachet->carte[2] = '\0';
  pachet->valoare = valoare;
  List *p1 = pachet, *p2 = pachet;

  for (i = 0; i < (int)strlen(string1); i++) {
    for (j = 0; j < (int)strlen(string2); j++) {
      if ((i == 0) && (j == 0))
        j++;
      pachet = CreateList();
      pachet->carte[0] = string1[i];
      pachet->carte[1] = string2[j];
      pachet->carte[2] = '\0';
      if (i > 3)
        pachet->valoare = valoare - 1;
      else
        pachet->valoare = valoare;
      p2->next = pachet;
      p2 = p2->next;
    }
    valoare--;
  }
  return p1;
}

List *Inversare(List *list) {
  List *pachet = list, *current, *prev, *urm;
  current = list;
  prev = NULL;
  urm = NULL;
  while (current != NULL) {
    urm = current->next;
    current->next = prev;
    prev = current;
    current = urm;
  }
  pachet = prev;
  return pachet;
}

List *N_Piles(int n, int v[MAX], List *list) {
  List *ptr[MAX], *pachet = list;
  int i, j, nr;

  nr = NrCartiPachet / n; // nr carti subpachet
  for (i = 1; i <= n; i++) {
    ptr[i] = pachet;
    for (j = 0; j < nr; j++) {
      pachet = pachet->next;
    }
  }
  list = ptr[v[1]];
  for (i = 1; i <= (n - 1); i++) {
    for (j = 0; j < (nr - 1); j++) {
      ptr[v[i]] = ptr[v[i]]->next;
    }
    ptr[v[i]]->next = ptr[v[i + 1]];
  }
  for (j = 0; j < (nr - 1); j++) {
    ptr[v[n]] = ptr[v[n]]->next;
  }
  ptr[v[n]]->next = NULL;
  return list;
}

List *Intercalare(List *list) {
  List *l, *p, *k, *m = list;
  int i;
  l = list;
  p = list;
  for (i = 0; i < (NrCartiPachet / 2); i++) {
    p = p->next;
  }
  for (i = 0; i < (NrCartiPachet / 2); i++) {
    k = p->next;
    if (i != (NrCartiPachet / 2 - 1))
      p->next = l->next;
    else
      p->next = NULL;
    l->next = p;
    l = p->next;
    p = k;
  }
  return m;
}

List *Adaugare_inceput(List *list, char s[3], int valoare) {
  List *newcell = CreateList();

  strcpy(newcell->carte, s);
  newcell->valoare = valoare;
  newcell->next = list;
  list = newcell;
  return list;
}

List *Adaugare_final(List *list, char s[3], int valoare) {
  List *newcell = CreateList(), *p = list;
  strcpy(newcell->carte, s);
  newcell->valoare = valoare;
  if (p == NULL) {
    list = newcell;
    newcell->next = NULL;
    return list;
  } else {
    while (p->next != NULL)
      p = p->next;
    p->next = newcell;
    newcell->next = NULL;
    return list;
  }
}

List *Amestecare_Mongean(List *list) {
  List *p = list;
  List *pachet = (List *)malloc(sizeof(List));
  int i;

  pachet->carte[0] = p->carte[0];
  pachet->carte[1] = p->carte[1];
  pachet->carte[2] = '\0';
  pachet->valoare = p->valoare;
  pachet->next = NULL;
  p = p->next;
  for (i = 1; i < 52; i++) {
    if (i % 2 != 0) {
      pachet = Adaugare_inceput(pachet, p->carte, p->valoare);
      p = p->next;
    } else {
      if (i % 2 == 0) {
        pachet = Adaugare_final(pachet, p->carte, p->valoare);
        p = p->next;
      }
    }
  }

  return pachet;
}

void afisare(List *list) {
  List *p = list;

  while (p != NULL) {
    printf("%s", p->carte);

    if (p->next != NULL) {
      printf(" ");
    }

    p = p->next;
  }
}

List *Stergere_inceput(List *list) {
  List *p = list;
  if (list == NULL)
    return NULL;
  else {
    list = list->next;
    free(p);
  }
  return list;
}

int Nr_carti_jucator(List *list) {
  List *p = list;
  int nr = 0;
  while (p != NULL) {
    nr++;
    p = p->next;
  }
  return nr;
}

int minim(int a, int b) {
  if (a < b)
    return a;
  else
    return b;
}

List *isCarte(List *list, char c) {
  // if(isCarte == NULL) ---> nu s a gasit carte cu simbolul "c" transmis ca
  // parametru
  List *p = list;
  while (p != NULL) {
    if (p->carte[1] == c)
      return p;
    else
      p = p->next;
  }
  return NULL;
}

List *StergerePrimaAparitie(List *list, char c) {
  List *curr = list, *prev;
  while (curr->carte[1] != c) {
    prev = curr;
    curr = curr->next;
  }
  if (curr == list) {
    list = list->next;
    free(curr);
  } else {
    prev->next = curr->next;
    free(curr);
  }
  return list;
}

DCList *
Adaugare_jucator(DCList *list, int nr,
                 List *pachet) { // se adauga jucatorul la finalul listei
  DCList *p, *player = (DCList *)malloc(sizeof(DCList));
  player->next = NULL;
  player->prev = NULL;
  player->nr_jucator = nr;
  player->pachet = pachet;
  p = list;
  if (p == NULL) {
    list = player;
    player->next = NULL;
    player->prev = NULL;
  } else {
    while (p->next != list)
      p = p->next;

    p->next = player;
    player->next = list;
    player->prev = p;
    list->prev = player;
  }
  return list;
}

List *freeList(List *list) {
  List *p1 = list, *p2;
  while (p1 != NULL) {
    p2 = p1;
    p1 = p1->next;
    free(p2);
  }
  return list;
}

void afisare_jucatori(DCList *list) {
  DCList *l = list;
  printf("\n");
  printf("Player 1: ");
  afisare(l->pachet);
  printf("\n");
  l = l->next;
  printf("Player 2: ");
  afisare(l->pachet);
  printf("\n");
  l = l->next;
  printf("Player 3: ");
  afisare(l->pachet);
  printf("\n");
  l = l->next;
  printf("Player 4: ");
  afisare(l->pachet);
  printf("\n");
  l = l->next;
}

List *Carte_curenta(List *list, List *list2) {
  strcpy(list->carte, list2->carte);
  list->valoare = list2->valoare;
  list->next = NULL;
  return list;
}

DCList *FreeJucatori(DCList *list) {
  DCList *p = list, *l;
  int i;
  for (i = 0; i < 3; i++) {
    l = p;
    p = p->next;
    free(l);
  }
  free(p);
  return NULL;
}

void Razboi(List *list, FILE *out) {
  List *p1 = list, *p2 = list, *copie1, *copie2, *aux;
  List *l1 = (List *)malloc(sizeof(List)), *l2 = (List *)malloc(sizeof(List));
  l1->next = NULL;
  l2->next = NULL;

  int i, j, nr_carti_intoarse, nr_maini, v1[13] = {0}, v2[13] = {0}, OK;
  char s[] = "AKQJT98765432";
  copie1 = p1;

  for (i = 0; i < NrCartiPachet / 2; i++) {
    p2 = p2->next;
  }
  copie2 = p2;

  aux = p1;
  for (i = 0; i < (NrCartiPachet / 2) - 1; i++) {
    aux = aux->next;
  }
  aux->next = NULL;

  nr_maini = 1;

  while (((Nr_carti_jucator(p1) != 0) && (Nr_carti_jucator(p2) != 0)) &&
         (nr_maini < 101)) {

    nr_maini++;

    if (p1->valoare > p2->valoare) {
      p1 = Adaugare_final(p1, p1->carte, p1->valoare);
      p1 = Stergere_inceput(p1);
      p1 = Adaugare_final(p1, p2->carte, p2->valoare);
      p2 = Stergere_inceput(p2);
    } else if (p2->valoare > p1->valoare) {
      p2 = Adaugare_final(p2, p2->carte, p2->valoare);
      p2 = Stergere_inceput(p2);
      p2 = Adaugare_final(p2, p1->carte, p1->valoare);
      p1 = Stergere_inceput(p1);
    } else if (p1->valoare == p2->valoare) {

      nr_carti_intoarse = minim(minim(Nr_carti_jucator(p1) - 1, p1->valoare),
                                minim(Nr_carti_jucator(p2) - 1, p2->valoare));
      copie1 = p1;
      copie2 = p2;
      for (i = 0; i < (nr_carti_intoarse + 1); i++) {
        l1 = Adaugare_inceput(l1, copie1->carte, copie1->valoare);
        l2 = Adaugare_inceput(l2, copie2->carte, copie2->valoare);
        copie1 = copie1->next;
        copie2 = copie2->next;
      }

      if (l1->valoare > l2->valoare) {
        aux = l1;

        for (i = 0; i < nr_carti_intoarse + 1; i++) {
          p1 = Adaugare_final(p1, aux->carte, aux->valoare);
          aux = aux->next;
        }
        aux = l2;
        for (i = 0; i < nr_carti_intoarse + 1; i++) {
          p1 = Adaugare_final(p1, aux->carte, aux->valoare);
          aux = aux->next;
        }

        for (i = 0; i < nr_carti_intoarse + 1; i++) {
          p1 = Stergere_inceput(p1);
          p2 = Stergere_inceput(p2);
        }

        for (i = 0; i < nr_carti_intoarse + 1; i++) {
          l1 = Stergere_inceput(l1);
          l2 = Stergere_inceput(l2);
        }

      } else if (l1->valoare < l2->valoare) {
        aux = l2;
        for (i = 0; i < nr_carti_intoarse + 1; i++) {
          p2 = Adaugare_final(p2, aux->carte, aux->valoare);
          aux = aux->next;
        }
        aux = l1;
        for (i = 0; i < nr_carti_intoarse + 1; i++) {
          p2 = Adaugare_final(p2, aux->carte, aux->valoare);
          aux = aux->next;
        }
        for (i = 0; i < nr_carti_intoarse + 1; i++) {
          p1 = Stergere_inceput(p1);
          p2 = Stergere_inceput(p2);
        }
        for (i = 0; i < nr_carti_intoarse + 1; i++) {
          l1 = Stergere_inceput(l1);
          l2 = Stergere_inceput(l2);
        }
      }
    }
  }

  if (Nr_carti_jucator(p1) == 0) { // jucator 2 castiga
    fprintf(out, "2");
  } else if (Nr_carti_jucator(p2) == 0) { // jucator 1 castiga
    fprintf(out, "1");
  } else if (nr_maini > 100) {
    copie1 = p1;
    copie2 = p2;
    j = 0;
    while (copie1 != NULL) {
      for (j = 0; j < 13; j++) {
        if (copie1->carte[0] == s[j])
          v1[j]++;
      }
      copie1 = copie1->next;
    }
    while (copie2 != NULL) {
      for (j = 0; j < 13; j++) {
        if (copie2->carte[0] == s[j])
          v2[j]++;
      }
      copie2 = copie2->next;
    }

    i = 0;
    OK = 0;
    while ((i < 13) && (OK == 0)) {
      if (v1[i] == v2[i])
        i++;
      else {
        OK = 1;
        if (v1[i] > v2[i]) {
          fprintf(out, "1%c", s[i]);
        } else if (v1[i] < v2[i]) {
          fprintf(out, "2%c", s[i]);
        }
      }
    }
  }
  l1 = freeList(l1);
  l2 = freeList(l2);
}

void Rolling_Stone(List *list, FILE *out) {
  List *aux, *table = (List *)malloc(sizeof(List));
  int i, j, OK = 0, carte_pusa, nr_table;
  List *player[MAX];

  List *ciclu_carti = (List *)malloc(sizeof(List));
  ciclu_carti->next = NULL;
  ciclu_carti = Adaugare_final(ciclu_carti, "s", 0);
  ciclu_carti = Adaugare_final(ciclu_carti, "h", 0);
  ciclu_carti = Adaugare_final(ciclu_carti, "c", 0);
  ciclu_carti = Adaugare_final(ciclu_carti, "d", 0);
  aux = ciclu_carti;

  for (i = 0; i < 4; i++) {
    aux = aux->next;
  }
  aux->next = ciclu_carti->next;
  ciclu_carti = Stergere_inceput(ciclu_carti);

  table->next = NULL;

  player[1] = list;
  player[2] = list;
  player[3] = list;
  player[4] = list;
  for (i = 0; i < 13; i++) {
    player[2] = player[2]->next;
  }
  for (i = 0; i < 26; i++) {
    player[3] = player[3]->next;
  }
  for (i = 0; i < 39; i++) {
    player[4] = player[4]->next;
  }
  aux = player[1];
  for (i = 0; i < 12; i++) {
    aux = aux->next;
  }
  aux->next = NULL;

  aux = player[2];
  for (i = 0; i < 12; i++) {
    aux = aux->next;
  }
  aux->next = NULL;

  aux = player[3];
  for (i = 0; i < 12; i++) {
    aux = aux->next;
  }
  aux->next = NULL;

  i = 1;
  carte_pusa = 0;

  while (carte_pusa == 0) {
    if (isCarte(player[i], ciclu_carti->carte[0]) != NULL) {

      table = Adaugare_final(
          table, (isCarte(player[i], ciclu_carti->carte[0])->carte),
          (isCarte(player[i], ciclu_carti->carte[0])->valoare));
      player[i] = StergerePrimaAparitie(player[i], ciclu_carti->carte[0]);
      carte_pusa = 1;
    } else {
      ciclu_carti = ciclu_carti->next;
    }
  }

  table = Stergere_inceput(table);

  i++;
  OK = 0;

  while (OK == 0) { // cand OK se face 1, un jucator a castigat

    if (isCarte(player[i], ciclu_carti->carte[0]) == NULL) {

      aux = table;
      nr_table = Nr_carti_jucator(table);
      for (j = 0; j < nr_table; j++) {
        player[i] = Adaugare_final(player[i], aux->carte, aux->valoare);
        aux = aux->next;
      }
      for (j = 0; j < nr_table; j++) {
        table = Stergere_inceput(table);
      }

      ciclu_carti = ciclu_carti->next;
      carte_pusa = 0;
      while (carte_pusa == 0) {
        if (isCarte(player[i], ciclu_carti->carte[0]) != NULL) {
          table = Adaugare_final(
              table, (isCarte(player[i], ciclu_carti->carte[0]))->carte,
              (isCarte(player[i], ciclu_carti->carte[0]))->valoare);
          player[i] = StergerePrimaAparitie(player[i], ciclu_carti->carte[0]);
          carte_pusa = 1;
        } else {
          ciclu_carti = ciclu_carti->next;
        }
      }
    } else {
      table = Adaugare_final(
          table, (isCarte(player[i], ciclu_carti->carte[0])->carte),
          (isCarte(player[i], ciclu_carti->carte[0])->valoare));
      player[i] = StergerePrimaAparitie(player[i], ciclu_carti->carte[0]);
    }
    if ((Nr_carti_jucator(player[i])) == 0) {
      OK = 1;
      fprintf(out, "%d", i);
    }

    if (i == 4) {
      i = 1;
    } else
      i++;
  }
}

void Scarabeii_Egipteni(List *list, FILE *out) {

  DCList *players, *p;
  List *pachete[MAX], *aux, *table = (List *)malloc(sizeof(List));
  int i, OK, nr_table, directie = 1, scarabeu;
  char scarabei[] = "AKQJ", carte_cu_numar[] = "T9765432";

  table->next = NULL;

  pachete[1] = list;
  pachete[2] = list;
  pachete[3] = list;
  pachete[4] = list;
  for (i = 0; i < 13; i++) {
    pachete[2] = pachete[2]->next;
  }
  for (i = 0; i < 26; i++) {
    pachete[3] = pachete[3]->next;
  }
  for (i = 0; i < 39; i++) {
    pachete[4] = pachete[4]->next;
  }
  aux = pachete[1];
  for (i = 0; i < 12; i++) {
    aux = aux->next;
  }
  aux->next = NULL;

  aux = pachete[2];
  for (i = 0; i < 12; i++) {
    aux = aux->next;
  }
  aux->next = NULL;

  aux = pachete[3];
  for (i = 0; i < 12; i++) {
    aux = aux->next;
  }
  aux->next = NULL;

  players = (DCList *)malloc(sizeof(DCList));
  players->pachet = pachete[1];
  players->nr_jucator = 1;
  players->next = players;
  players->prev = NULL;

  for (i = 2; i <= NR_PLAYERS_SCARABEI; i++) {
    players = Adaugare_jucator(players, i, pachete[i]);
  }

  table = Stergere_inceput(table);

  List *CarteCurr = (List *)malloc(sizeof(List));

  directie = 1;
  scarabeu = 0; // cand scarabeu e 0 , cartea anterioara nu e scarabeu
  CarteCurr = Carte_curenta(CarteCurr, players->pachet);

  table = Adaugare_final(table, CarteCurr->carte, CarteCurr->valoare);
  players->pachet = Stergere_inceput(players->pachet);
  if (strchr(scarabei, CarteCurr->carte[0]) != NULL) {
    scarabeu = 1;
    p = players;
    players = players->next;
  } else if (CarteCurr->carte[0] == '8') {
    players = players->prev;
    directie = 2;
  }

  OK = 0;
  while (OK == 0) {

    if (Nr_carti_jucator(players->pachet) == 0) {

      if (directie == 1)
        players = players->next;
      else if (directie == 2)
        players = players->prev;

      continue;
    }

    CarteCurr = Carte_curenta(CarteCurr, players->pachet);

    table = Adaugare_final(table, CarteCurr->carte, CarteCurr->valoare);

    players->pachet = Stergere_inceput(players->pachet);

    if (scarabeu == 1) { // daca inainte s-a jucat un scarabeu
      if (strchr(scarabei, CarteCurr->carte[0]) != NULL) {
        scarabeu = 1;
        p = players;
        if (directie == 1)
          players = players->next;
        else if (directie == 2)
          players = players->prev;
      } else if (strchr(carte_cu_numar, CarteCurr->carte[0]) != NULL) {
        nr_table = Nr_carti_jucator(table);
        table = Inversare(table);
        aux = table;
        for (i = 0; i < nr_table; i++) {
          p->pachet = Adaugare_final(p->pachet, aux->carte, aux->valoare);
          aux = aux->next;
        }
        for (i = 0; i < nr_table; i++) {
          table = Stergere_inceput(table);
        }
        if (Nr_carti_jucator(p->pachet) == NrCartiPachet) {
          OK = 1;
          fprintf(out, "%d", p->nr_jucator);
        }
        scarabeu = 0;
        players = p;

      } else if (CarteCurr->carte[0] == '8') {
        if (directie == 1)
          players = players->next;
        else if (directie == 2)
          players = players->prev;
      }
    } else { // inainte nu s-a jucat un scarabeu
      if (strchr(carte_cu_numar, CarteCurr->carte[0]) != NULL) {
        if (directie == 1)
          players = players->next;
        else if (directie == 2)
          players = players->prev;
      } else if (CarteCurr->carte[0] == '8') {
        if (directie == 1) {
          players = players->prev;
          directie = 2;

        } else if (directie == 2) {
          players = players->next;
          directie = 1;
        }

      } else if (strchr(scarabei, CarteCurr->carte[0]) != NULL) {
        p = players;
        scarabeu = 1;
        if (directie == 1)
          players = players->next;
        else if (directie == 2)
          players = players->prev;
      }
    }
  }
  free(CarteCurr);
  FreeJucatori(players);
  table = freeList(table);
}

int main() {

  List *list, *p;
  char string1[MAX], tip_amestecare[MAX], string2[MAX];
  int nr, i, j, n, v[MAX];

  list = InitializarePachet();

  FILE *in = fopen("in", "r"), *out = fopen("out", "w");

  fscanf(in, "%s", string1);
  fscanf(in, "%d", &nr);

  for (i = 0; i < nr; i++) {
    fscanf(in, "%s", tip_amestecare);
    if (strcmp(tip_amestecare, "inversare") == 0)
      list = Inversare(list);
    else if (strcmp(tip_amestecare, "intercalare") == 0)
      list = Intercalare(list);
    else if (strcmp(tip_amestecare, "mongean") == 0)
      list = Amestecare_Mongean(list);
    else if (strcmp(tip_amestecare, "npiles") == 0) {
      fscanf(in, "%d", &n);
      for (j = 1; j <= n; j++) {
        fscanf(in, "%s\n", string2);
        v[j] = atoi(string2);
      }
      list = N_Piles(n, v, list);
    }
  }

  if (strcmp(string1, "amestecare") == 0) {
    p = list;
    while (p != NULL) {
      fprintf(out, "%s", p->carte);

      if (p->next != NULL) {
        fprintf(out, " ");
      }
      p = p->next;
    }
    fprintf(out, "\n");
    list = freeList(list);
  } else if (strcmp(string1, "razboi") == 0) {
    Razboi(list, out);

  } else if (strcmp(string1, "rollingstone") == 0) {
    Rolling_Stone(list, out);
    list = freeList(list);
  } else if (strcmp(string1, "scarabei") == 0) {
    Scarabeii_Egipteni(list, out);
    list = freeList(list);
  }

  fclose(in);
  fclose(out);
    
    return 0;
  }

  
  