#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define FPS 33
#define NL 38
#define NC 76

#define NAVE    '@'
#define TIRO    '|'
#define BARREIRA 219

#define MAX_TIROS       1000
#define MAX_BARREIRAS   1000

#define VELOCIDADE_TIRO 0.30
#define VELOCIDADE_NAVE 0.80

struct Coord2D
{
    float l, c;
};

char tela[NL][NC+1];

struct Coord2D nave;
struct Coord2D tiros[MAX_TIROS];
struct Coord2D barreiras[MAX_BARREIRAS];
int numTiros, numBarreiras, apertouEspaco;

void GotoXY( int x, int y)
{
   COORD coord;
   coord.X = x;
   coord.Y = y;
   SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
}

void inicializarJogo()
{
    int i;

    nave.c = NC / 2;
    nave.l = NL - 1;

    numTiros = 0;

    apertouEspaco = 0;

    srand(GetTickCount());
    numBarreiras = 50;
    for (i = 0; i < numBarreiras; i++)
    {
        barreiras[i].l = rand() % (NL - 1);
        barreiras[i].c = rand() %  NC;
    }
}

void modificaTela()
{
    int l, t;

    // Primeiro, vamos zerar a tela.
    for (l = 0; l < NL; l++)
    {
        memset(tela[l], ' ', NC);
        tela[l][NC] = '\0';
    }

    // Desenha a nave.
    tela[(int)nave.l][(int)nave.c] = NAVE;

    // Desenha os tiros.
    for (t = 0; t < numTiros; t++)
        tela[(int)tiros[t].l][(int)tiros[t].c] = TIRO;

    // Desenha as barreiras.
    for (t = 0; t < numBarreiras; t++)
        tela[(int)barreiras[t].l][(int)barreiras[t].c] = BARREIRA;
}

void desenha()
{
    int l;
    //system("cls");
    GotoXY(0, 0);
    for (l = 0; l < NL; l++)
        printf("%s\n", tela[l]);
}

void aguarda(int ms)
{
    int tempoInicial;
    tempoInicial = GetTickCount();
    while (GetTickCount() - tempoInicial < ms);
}

void testaTeclas()
{
    if (GetAsyncKeyState(VK_LEFT) != 0)
        // Mover nave pra esquerda.
        nave.c = max(0, nave.c - VELOCIDADE_NAVE);
    else 
    if (GetAsyncKeyState(VK_RIGHT) != 0)
        // Mover nave pra direita.
        nave.c = min(NC-1, nave.c + VELOCIDADE_NAVE);
    if (GetAsyncKeyState(VK_SPACE) != 0)
        if (!apertouEspaco)
        {
            // Atirar.
            numTiros++;
            tiros[numTiros - 1].c = nave.c;
            tiros[numTiros - 1].l = nave.l - 1;
            apertouEspaco = 1;
        }
     else
        apertouEspaco = 0;
}

void movimenta()
{
    // Movimentar os tiros.
    int t;
    for (t = numTiros - 1; t >= 0; t--)
    {
        tiros[t].l -= VELOCIDADE_TIRO;
        if (tiros[t].l < 0)
        {
            tiros[t] = tiros[numTiros - 1];
            numTiros--;
        }
    }
}

void testaColisao()
{
    int t, b;
    for (t = numTiros - 1; t >= 0; t--)
        for (b = numBarreiras - 1; b >= 0; b--)
            if ((int)tiros[t].l == (int)barreiras[b].l &&
                (int)tiros[t].c == (int)barreiras[b].c)
            {
                tiros[t] = tiros[numTiros - 1];
                numTiros--;
                barreiras[b] = barreiras[numBarreiras - 1];
                numBarreiras--;
                break;
            }
}

int main()
{
    int final = 0;
    int inicio, duracao;

    inicializarJogo();

    while (!final)
    {
        inicio = GetTickCount();
        modificaTela();
        testaTeclas();
        testaColisao();
        movimenta();
        desenha();
        duracao = GetTickCount() - inicio;

        aguarda(FPS - duracao);
    }
    return 0;
}
