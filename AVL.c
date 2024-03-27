#include <stdio.h>
#include <stdlib.h>
#include "AVL.h"

// retorna o maior entre os dois numeros a e b
int numMax(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

// Retorna a altura de um nó ou -1 se o nó for NULL
int altura(PONT raiz)
{
    if (!raiz)
        return (-1);
    return (raiz->h);
}

// Exibe a árvore AVL
void exibirArvore(PONT raiz)
{
    if (raiz != NULL) // Se a árvore não estiver vazia
    {
        printf("%i", raiz->chave);
        printf("(");
        exibirArvore(raiz->esq);
        exibirArvore(raiz->dir);
        printf(")");
    }
}

// Retorna o número de nós da árvore
int numeroNos(PONT raiz)
{
    if (!raiz)
        return 0;
    return (numeroNos(raiz->esq) + 1 + numeroNos(raiz->dir));
}

// Retorna o fator de balanceamento de um nó
int getBalance(PONT N)
{
    if (N == NULL) // Se o nó for NULL
    {
        return 0;
    }
    return altura(N->esq) - altura(N->dir); // Retorna a diferença entre as alturas da subárvore esquerda e direita
}

// Inicializa a árvore
PONT inicializa()
{
    return (NULL);
}

// Cria um novo nó
PONT criaNo(TIPOCHAVE ch)
{
    PONT novoNo =
        (PONT)malloc(sizeof(NO)); // Aloca memória para o novo nó
    novoNo->esq = NULL;
    novoNo->dir = NULL; // Inicializa os ponteiros esquerdo e direito como NULL
    novoNo->chave = ch; // Atribui a chave ao novo nó
    novoNo->h = 0;      // Inicializa a altura do novo nó como 0
    return (novoNo);
}

// Funções de rotação
PONT direita(PONT r)
{
    PONT aux = r->esq;                                 // Cria um nó auxiliar para a subárvore esquerda
    r->esq = aux->dir;                                 // O ponteiro esquerdo do nó raiz aponta para a subárvore direita do nó auxiliar
    aux->dir = r;                                      // O ponteiro direito do nó auxiliar aponta para o nó raiz
    r->h = numMax(altura(r->dir), altura(r->esq)) + 1; // Atualiza a altura do nó raiz
    aux->h = numMax(altura(aux->esq), r->h) + 1;       // Atualiza a altura do nó auxiliar
    return aux;
}

// Função para rotacionar um nó para a esquerda
PONT esquerda(PONT r)
{
    PONT aux;          // Cria um nó auxiliar
    aux = r->dir;      // O nó auxiliar recebe a subárvore direita do nó raiz
    r->dir = aux->esq; // O ponteiro direito do nó raiz aponta para a subárvore esquerda do nó auxiliar
    aux->esq = r;      // O ponteiro esquerdo do nó auxiliar aponta para o nó raiz
    r->h = numMax(altura(r->dir),
                  altura(r->esq)) +
           1;
    aux->h = numMax(altura(aux->dir),
                    r->h) +
             1; // Atualiza a altura do nó raiz e do nó auxiliar
    return (aux);
}

// Funções de rotação dupla
PONT esquerdaDireita(PONT r)
{
    r->esq = esquerda(r->esq); // Rotaciona a subárvore esquerda para a esquerda
    return (direita(r));       // Rotaciona o nó raiz para a direita
}

// Função para rotacionar um nó para a direita e depois para a esquerda
PONT direitaEsquerda(PONT r)
{
    r->dir = direita(r->dir); // Rotaciona a subárvore direita para a direita
    return (esquerda(r));     // Rotaciona o nó raiz para a esquerda
}

// Função para rotacionar um nó para a esquerda e depois para a direita
PONT insere(PONT raiz, TIPOCHAVE ch)
{
    if (!raiz) // Se a árvore estiver vazia, cria um novo nó
        return criaNo(ch);

    if (ch < raiz->chave) // Se a chave for menor que a chave do nó raiz
    {
        raiz->esq = insere(raiz->esq, ch);              // Insere na subárvore esquerda
        if (altura(raiz->esq) - altura(raiz->dir) == 2) // Se a diferença entre as alturas da subárvore esquerda e direita for 2
        {
            if (ch < raiz->esq->chave)
                raiz = direita(raiz); // Rotaciona o nó raiz para a direita
            else
                raiz = esquerdaDireita(raiz); // Rotaciona o nó raiz para a esquerda e depois para a direita
        }
    }
    else if (ch > raiz->chave) // Se a chave for maior que a chave do nó raiz
    {
        raiz->dir = insere(raiz->dir, ch);              // Insere na subárvore direita
        if (altura(raiz->dir) - altura(raiz->esq) == 2) // Se a diferença entre as alturas da subárvore direita e esquerda for 2
        {
            if (ch > raiz->dir->chave) //
                raiz = esquerda(raiz);
            else
                raiz = direitaEsquerda(raiz);
        }
    }
    raiz->h = numMax(altura(raiz->esq), altura(raiz->dir)) + 1;
    return raiz;
}

// Função para encontrar o nó com o valor mínimo em uma árvore.
PONT minValueNode(PONT node)
{
    PONT current = node;

    // Loop para encontrar a folha mais à esquerda
    while (current->esq != NULL)
    {
        current = current->esq;
    }

    return current;
}

// Função para remover um nó da árvore
PONT removeNo(PONT raiz, TIPOCHAVE ch)
{
    if (raiz == NULL)
    {
        return raiz; // Elemento não encontrado, retorna a árvore sem alterações
    }

    // Procura o elemento a ser removido
    if (ch < raiz->chave)
    {
        raiz->esq = removeNo(raiz->esq, ch);
    }
    else if (ch > raiz->chave)
    {
        raiz->dir = removeNo(raiz->dir, ch);
    }
    else
    {
        // Nó com apenas um filho ou sem filhos
        if (raiz->esq == NULL || raiz->dir == NULL)
        {
            PONT temp = raiz->esq ? raiz->esq : raiz->dir;

            // Sem filhos
            if (temp == NULL)
            {
                temp = raiz;
                raiz = NULL;
            }
            else
            {                  // Um filho
                *raiz = *temp; // Copia o conteúdo do filho não vazio
            }
            free(temp);
        }
        else
        {
            // Nó com dois filhos: obtém o sucessor (menor nó da subárvore direita)
            PONT temp = minValueNode(raiz->dir);

            raiz->chave = temp->chave;                    // Copia a chave do sucessor para este nó
            raiz->dir = removeNo(raiz->dir, temp->chave); // Remove o sucessor
        }
    }

    if (raiz == NULL)
        return raiz; // Se a árvore tinha apenas um nó

    // Atualiza a altura do nó atual
    raiz->h = 1 + numMax(altura(raiz->esq), altura(raiz->dir));

    // Obtém o fator de balanceamento deste nó ancestral
    int balance = getBalance(raiz);

    // Se o nó se tornar desbalanceado, então trata os 4 casos
    // Caso Esquerda Esquerda
    if (balance > 1 && getBalance(raiz->esq) >= 0)
    {
        return direita(raiz);
    }

    // Caso Esquerda Direita
    if (balance > 1 && getBalance(raiz->esq) < 0)
    {
        raiz->esq = esquerda(raiz->esq);
        return direita(raiz);
    }

    // Caso Direita Direita
    if (balance < -1 && getBalance(raiz->dir) <= 0)
    {
        return esquerda(raiz);
    }

    // Caso Direita Esquerda
    if (balance < -1 && getBalance(raiz->dir) > 0)
    {
        raiz->dir = direita(raiz->dir);
        return esquerda(raiz);
    }

    return raiz; // Retorna o ponteiro para o nó (inalterado ou atualizado)
}

PONT contem(TIPOCHAVE ch, PONT raiz)
{
    if (raiz == NULL)
        return (NULL);
    if (raiz->chave == ch)
        return (raiz);
    if (raiz->chave > ch)
        return (contem(ch, raiz->esq));
    return (contem(ch, raiz->dir));
}

// função para buscar um nó na árvore
PONT buscaNo(PONT raiz, TIPOCHAVE ch, PONT *pai)
{
    PONT atual = raiz;
    *pai = NULL;
    while (atual)
    {
        if (atual->chave == ch)
            return (atual);
        *pai = atual;
        if (ch < atual->chave)
            atual = atual->esq;
        else
            atual = atual->dir;
    }
    return (NULL);
}