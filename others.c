#include "functions.h"

/* Telas */
// Tela da parte inicial do jogo
void inicio(Estado *estado_atual){
    int flag = 0;

    clrscr();

    desenhaInterface();
    desenhaInicio();

    do{
        fflush(stdin);
        gets(estado_atual->jogador.nome);

        if(strlen(estado_atual->jogador.nome) > NOME_MAX || strlen(estado_atual->jogador.nome) < 1){
            desenhaInterface();

            desenhaRetangulo();

            if(strlen(estado_atual->jogador.nome) > NOME_MAX){
                gotoxy(18, 14);
                printf("Uau, mas que nome bonito, pena que %c", 130);
                gotoxy(18,15);
                printf("grande demais...");
                gotoxy(18, 17);
                printf("Que tal um apelido?");

                flag = 1;
            } else {
                if(flag == 0){
                    gotoxy(18,14);
                    printf("N%co precisa se preocupar, n%co vamos", 198, 198);
                    gotoxy(18, 15);
                    printf("rir do seu nome");
                    gotoxy(18,17);
                    printf("Que tal tentarmos de novo?");

                    flag = 2;
                } else {
                    gotoxy(18,14);
                    printf("Bom, agora %c um pouco pequeno", 130);
                    gotoxy(18, 15);
                    printf("demais...");
                    gotoxy(18,17);
                    printf("Vamos tentar mais uma vez, ok?");
                }
            }

            getch();

            fflush(stdin);
            desenhaInicio();
        }
    } while(strlen(estado_atual->jogador.nome) > NOME_MAX || strlen(estado_atual->jogador.nome) < 1);

    if(flag == 2){
        gotoxy(18,14);
        printf("Rah, que nome fe... Quer dizer,");
        gotoxy(18, 15);
        printf("que nome lindo, nossa...");
        gotoxy(15,18);
        printf("   Erhnn... Que tal irmos pro jogo?");
        gotoxy(18,18);

        getch();
    }
}


/* Obtencao de coordenadas */
void localizaPosicoes(char mapa[LINHAS_MAPA][COLUNAS_MAPA], Estado *estado_atual) {
    int i, j, k = 0, l = 0;

    for (j = 0; j < COLUNAS_MAPA; j++) {
        for (i = 0; i < LINHAS_MAPA; i++) {
            switch(mapa[i][j]){
                case 'M':
                    estado_atual->jogador.pos_inicial.x = j;
                    estado_atual->jogador.pos_inicial.y = i;

                    estado_atual->jogador.pos_atual.x = j;
                    estado_atual->jogador.pos_atual.y = i;

                    break;
                case 'Q':
                    estado_atual->jogador.qtd_queijos++;

                    break;
                case 'G':
                    estado_atual->gato[k].pos_inicial.x = j;
                    estado_atual->gato[k].pos_inicial.y = i;

                    estado_atual->gato[k].pos_atual.x = j;
                    estado_atual->gato[k].pos_atual.y = i;

                    k++;

                    break;
                case 'T':
                    estado_atual->porta[l].pos_porta.x = j;
                    estado_atual->porta[l].pos_porta.y = i;

                    l++;

                    break;
            }
        }
    }
}


/* Outras funcoes */
// Gera um numero aleatorio de 0 a 3
int geraNumero(){
    srand(time(NULL));
    int num;

    num = (rand() % 4);

    return num;
}

// Remove o cursor da tela
void removeCursor(){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Testando
void leRanking(FILE *arq, SAVE ranking[MAX_SAVES]){
    int i = 0;

    while (!feof(arq) && i < MAX_SAVES){
        fread(&ranking[i], sizeof(SAVE), 1, arq);

        i++;
    }
}

void salvaRanking(FILE *arq, SAVE ranking[MAX_SAVES]){
    int i = 0;

    while(i < MAX_SAVES){
        fwrite(&ranking[i], sizeof(SAVE), 1, arq);

        i++;
    }
}

int atualizaVetor(SAVE ranking[MAX_SAVES], Jogador last_win){ // na real tem q ser um jogador (estado)
    int flag = 0;

    if(last_win.pontuacao >= ranking[MAX_SAVES - 1].score){
        strcpy(ranking[MAX_SAVES - 1].nome, last_win.nome);
        ranking[MAX_SAVES - 1].score = last_win.pontuacao;

        flag = 1;
    }

    return flag;
}

void ordenaVetor(SAVE vetor[MAX_SAVES]){
    SAVE auxiliar;
    int i = 0;

    do{
        if(vetor[MAX_SAVES - 1].score >= vetor[i].score){
            strcpy(auxiliar.nome, vetor[i].nome);
            auxiliar.score = vetor[i].score;

            strcpy(vetor[i].nome, vetor[MAX_SAVES - 1].nome);
            vetor[i].score = vetor[MAX_SAVES - 1].score;

            strcpy(vetor[MAX_SAVES - 1].nome, auxiliar.nome);
            vetor[MAX_SAVES - 1].score = auxiliar.score;
        }

        i++;
    } while(i < MAX_SAVES - 1);
}

void atualizaRanking(Jogador last_win){
    FILE *arq;
    SAVE ranking[MAX_SAVES] = {{"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}};
    int flag;

    if(!(arq = fopen("ranking.bin", "rb"))){
        printf("Erro na leitura do arquivo");
    } else {
        leRanking(arq, ranking);

        fclose(arq);

        flag = atualizaVetor(ranking, last_win);

        if(flag){
            ordenaVetor(ranking);

            if(!(arq = fopen("ranking.bin", "wb"))){
                printf("Erro na escrita do arquivo");
            } else {
                salvaRanking(arq, ranking);

                fclose(arq);
            }
        }
    }

    clrscr();
    desenhaInterface();

    desenha_ranking(ranking);

    getch();
}

void desenha_ranking(SAVE ranking[MAX_SAVES]){
    int i = 0, j = 0;

    textbackground(YELLOW);

    for(j = 0; j <= LINHAS_RANKING; j++){
        if(j == 0 || j == 4 || j == LINHAS_RANKING){
            for(i = 0; i <= COLUNAS_RANKING; i++){
                gotoxy(i + 18, j + 9);
                printf(" ");
            }
        } else {
            gotoxy(18, j + 9);
            printf(" ");

            gotoxy(COLUNAS_RANKING + 18, j + 9);
            printf(" ");
        }
    }

    textbackground(BLACK);

    for(i = 0; i < MAX_SAVES; i++){
        gotoxy(20, i + 15);
        printf("%20s     %6d\n", ranking[i].nome, ranking[i].score);
    }

    textcolor(RED);
    gotoxy(32, 11);
    printf("RANKING");

    textcolor(WHITE);
}