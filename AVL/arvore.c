#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "arvore.h"

void inicializa_arvore(Arvore *arvore) {
    arvore->raiz = NULL;
}

int arvore_vazia(Arvore *arvore) {
    return arvore->raiz == NULL;
}

void remove_no(Arvore *arvore, No *no) {
    if (no->esquerda) {
        remove_no(arvore, no->esquerda); 
    }
    if (no->direita) {
        remove_no(arvore, no->direita);
    }
    if (!no->pai) {
        arvore->raiz = NULL;
    } else {
        if (no->pai->esquerda == no) {
            no->pai->esquerda = NULL;
        } else {
            no->pai->direita = NULL;
        }
    }
    free(no);
}

void percorrer_InOrder(No* no, void (*callback) (int)) {
    if (no) {
        percorrer_InOrder(no->esquerda, callback);
        callback(no->valor);
        percorrer_InOrder(no->direita, callback);
    }
}

void percorrer_PreOrder(No* no, void (*callback) (int)) {
    if (no) {
        callback(no->valor);
        percorrer_PreOrder(no->esquerda, callback);
        percorrer_PreOrder(no->direita, callback);
    }
}

void percorrer_PosOrder(No* no, void (*callback) (int)) {
    if (no) {
        percorrer_PosOrder(no->esquerda, callback);
        percorrer_PosOrder(no->direita, callback);
        callback(no->valor);
    }
}

int max(int x, int y) { 
    return (x > y) ? x : y;
} 

int altura(No* no) {
    if (!no || (!no->esquerda && !no->direita))
        return 0;
    return 1 + max(altura(no->esquerda), altura(no->direita));
}

int fb(No* no) {
    int altura_esquerda = 0, altura_direita = 0;
    if (no->esquerda) {
        altura_esquerda = altura(no->esquerda) + 1;
    }
    if (no->direita) {
        altura_direita = altura(no->direita) + 1;
    }
    return altura_esquerda - altura_direita;
}

No* rse(No* satelite) {
    No *pai = satelite->pai, *pivo = satelite->direita, *rebarba = NULL;
    if (!pivo->esquerda) {
        pivo->esquerda = satelite;
        satelite->pai = pivo;
    } else {
        pivo->esquerda->esquerda = satelite;
        satelite->pai = pivo->esquerda;
        rebarba = pivo->esquerda;
    }    
    pivo->pai = pai;
    satelite->direita = NULL;
    if (pai && pai->esquerda == satelite)
        pai->esquerda = pivo;
    else if (pai)
        pai->direita = pivo;
    balanceamento(rebarba);
    return pivo;
}

No* rsd(No* satelite) {
    No *pai = satelite->pai, *pivo = satelite->esquerda, *rebarba = NULL;
    if (!pivo->direita) {
        pivo->direita = satelite;
        satelite->pai = pivo;
    } else {
        pivo->direita->direita = satelite;
        satelite->pai = pivo->direita;
        rebarba = pivo->direita;
    }
    pivo->pai = pai;
    satelite->esquerda = NULL;
    if (pai && pai->esquerda == satelite)
        pai->esquerda = pivo;
    else if (pai)
        pai->direita = pivo;
    balanceamento(rebarba);
    return pivo;
}

No* rde(No* satelite) {
    satelite->direita = rsd(satelite->direita);
    return rse(satelite);
}

No* rdd(No* satelite) {
    satelite->esquerda = rse(satelite->esquerda);
    return rsd(satelite);
}

No* balanceamento(No* no) {
    while (no) {
        int fator = fb(no);
        if (fator > 1) {
            int fatorFilho = fb(no->esquerda);
            if (fatorFilho > 0) {
                return rsd(no);
            } else {
                return rdd(no);
            }
        } else if (fator < -1) {
            int fatorFilho = fb(no->direita);
            if (fatorFilho < 0) {
                return rse(no);
            } else {
                return rde(no);
            }
        }
        no = no->pai;
    }
    return no;
}

int adiciona_no(Arvore *arvore, int valor) {
    No* no = malloc(sizeof(No));
    if (!no)
        return 0;
    no->valor = valor;
    no->esquerda = NULL;
    no->direita = NULL;
    if (arvore_vazia(arvore)) {
        no->pai = NULL;
        arvore->raiz = no;
        return 1;
    } else {
        No *i = arvore->raiz, *pivo = NULL;
        do {
            if (i->valor == valor) {
                free(no);
                return ERRO_CHAVE_REPETIDA;
            } else if (valor < i->valor) {
                if (!i->esquerda) {
                    no->pai = i;
                    i->esquerda = no;
                    pivo = balanceamento(i->pai);
                    if (pivo && !pivo->pai)
                        arvore->raiz = pivo;
                    return 1;
                } else {
                    i = i->esquerda;
                }
            } else {
                if (!i->direita) {
                    no->pai = i;
                    i->direita = no;
                    pivo = balanceamento(i->pai);
                    if (pivo && !pivo->pai)
                        arvore->raiz = pivo;
                    return 1;
                } else {
                    i = i->direita;
                }
            }
        } while (1);
    }        
}

void limpa_subarvore(Arvore *arvore, No *no) {
    if (no) {
        limpa_subarvore(arvore, no->esquerda);
        limpa_subarvore(arvore, no->direita);
        remove_no(arvore, no);
    }
}