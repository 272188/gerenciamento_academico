/*
1. Faça um programa em C que cadastre dados sobre alunos e seu histórico dentro de um 
determinado curso. Para isto, é preciso ter as seguintes estruturas de dados: 
(a) Lista de Alunos: Matricula do aluno, Nome do aluno, Código do Curso, árvore de Notas, árvore de 
Matrícula, a lista deve ser organizada pela ordem de alfabética por nome do aluno. 
(b) Árvore de Curso: Código do curso, Nome do curso, quantidade de Períodos e uma árvore de 
Disciplinas. A árvore deve ser organizada pelo nome do Código do Curso. 
(c) Árvore de disciplinas: código da disciplina, nome da disciplina, período, carga horária. A árvore deve 
ser organizada pelo código da disciplina. 
(d) Árvore de Notas: código da disciplina, semestre cursado, nota final obtida pelo aluno. A árvore deve ser 
organizada pelo código da disciplina. 
(e) Árvore de Matricula: código da disciplina, organizada pelo código da disciplina
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// Estrutura da árvore de Curso
struct ArvCursos {
    int codigoCurso;
    char nomeCurso[100];
    int qtdPeriodos;
    struct ArvDisciplinas *disciplinas;
    struct ArvCursos *esq, *dir;
};

// Estrutura da árvore de Disciplina
struct ArvDisciplinas {
    int codigoDisciplina;
    char nomeDisciplina[100];
    int periodo;
    int cargaHoraria;
    struct ArvDisciplinas *esq, *dir;
};

// Estrutura da árvore de Notas
struct ArvNotas {
    int codigoDisciplina;
    int semestre;
    float notaFinal;
    struct ArvNotas *esq, *dir;
};

// Estrutura da árvore de Matrícula
struct ArvMatriculas {
    int codigoDisciplina;
    struct ArvMatriculas *esq, *dir;
};

// Estrutura de lista de Alunos
struct Alunos {
    int matricula;
    char nomeAluno[100];
    int codigoCurso;
    struct ArvNotas *notas;
    struct ArvMatriculas *matriculas;
    struct Alunos *prox;  // Para formar a lista de alunos
};


                                                                   //BLOCO LISTA DE ALUNOS
// Função para criar um novo aluno
struct Alunos* criarAluno(int matricula, char nomeAluno[], int codigoCurso) {
    struct Alunos *novoAluno = (struct Alunos*) malloc(sizeof(struct Alunos));
    if (novoAluno != NULL) {
        novoAluno->matricula = matricula;
        strcpy(novoAluno->nomeAluno, nomeAluno);
        novoAluno->codigoCurso = codigoCurso;
        novoAluno->notas = NULL;
        novoAluno->matriculas = NULL;
        novoAluno->prox = NULL;
    } else {
        printf("Erro ao alocar memoria para o novo aluno.\n");
    }
    return novoAluno;
}


// Função para inserir aluno em ordem alfabética
void inserirAluno(struct Alunos **lista, struct Alunos *novoAluno) {
    struct Alunos *atual = *lista, *anterior = NULL;

    while (atual != NULL && strcmp(atual->nomeAluno, novoAluno->nomeAluno) < 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (anterior == NULL) {
        novoAluno->prox = *lista;
        *lista = novoAluno;
    } else {
        novoAluno->prox = atual;
        anterior->prox = novoAluno;
    }
}

// Função para remover um aluno da lista de alunos
int removerAluno(struct Alunos **lista, int matricula) {
    struct Alunos *atual = *lista;
    struct Alunos *anterior = NULL;

    // Percorrer a lista para encontrar o aluno
    while (atual != NULL && atual->matricula != matricula) {
        anterior = atual;
        atual = atual->prox;
    }

    // Se o aluno não foi encontrado
    if (atual == NULL) {
        printf("Aluno com matricula %d nao encontrado.\n", matricula);
        return 0;  // Falha na remoção
    }

    // Se o aluno for o primeiro da lista
    if (anterior == NULL) {
        *lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    // Libera a memória do aluno removido
    free(atual);

    printf("Aluno com matricula %d removido com sucesso.\n", matricula);
    return 1;  // Sucesso na remoção
}

// Função para buscar um aluno pela matrícula
struct Alunos* buscarAluno(struct Alunos *listaAlunos, int matricula) {
    struct Alunos *atual = listaAlunos;

    while (atual != NULL && atual->matricula != matricula) {
        atual = atual->prox;
    }

    return atual;  // Retorna o aluno ou NULL se não encontrado
}

// Função para imprimir a lista de alunos
void imprimirListaAlunos(struct Alunos *lista) {
    struct Alunos *atual = lista;
    while (atual != NULL) {
        printf("Alunos: %s, Matricula: %d, Curso: %d\n", atual->nomeAluno, atual->matricula, atual->codigoCurso);
        atual = atual->prox;
    }
} 

                                                                       //BLOCO DE CURSOS

// Função para criar um novo curso
struct ArvCursos* criarCurso(int codigoCurso, char nomeCurso[], int qtdPeriodos) {
    struct ArvCursos *novoCurso = (struct ArvCursos*) malloc(sizeof(struct ArvCursos));
    if (novoCurso != NULL) {
        novoCurso->codigoCurso = codigoCurso;
        strcpy(novoCurso->nomeCurso, nomeCurso);  // Copiar nome do curso
        novoCurso->qtdPeriodos = qtdPeriodos;
        novoCurso->disciplinas = NULL;  // Nenhuma disciplina inicialmente
        novoCurso->esq = NULL;
		novoCurso->dir = NULL;
    }
    return novoCurso;
}

// Função que verifica se o nó é uma folha
int ehfolhaCurso(struct ArvCursos *no) {
    int resultado = 0;
    if (no != NULL && no->esq == NULL && no->dir == NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que verifica se o nó tem um único filho
int unicoFilhoCurso(struct ArvCursos *no) {
    int resultado = 0;
    if (no != NULL) {
        if ((no->esq != NULL && no->dir == NULL) || (no->esq == NULL && no->dir != NULL)) {
            resultado = 1;
        }
    }
    return resultado;
}

// Função que verifica se o nó tem dois filhos
int doisFilhosCurso(struct ArvCursos *no) {
    int resultado = 0;
    if (no != NULL && no->esq != NULL && no->dir != NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que retorna a maior informação (código do curso) na subárvore direita
int maiorInfoCurso(struct ArvCursos *no) {
    struct ArvCursos *atual = no;
    int valor = -1;  // Valor padrão em caso de árvore vazia
    
    if (atual != NULL) {
        while (atual->dir != NULL) {
            atual = atual->dir;
        }
        valor = atual->codigoCurso;
    }
    return valor;
}


// Função para inserir um curso na árvore de cursos
int insereCurso(struct ArvCursos **raiz, int codigoCurso, char *nomeCurso, int qtdPeriodos) {
    int inseriu = 1;
    
    if (*raiz == NULL) {
        *raiz = criarCurso(codigoCurso, nomeCurso, qtdPeriodos);  // Criar novo curso
    } 
    else if (codigoCurso < (*raiz)->codigoCurso) {
        inseriu = insereCurso(&(*raiz)->esq, codigoCurso, nomeCurso, qtdPeriodos);
    } 
    else if (codigoCurso > (*raiz)->codigoCurso) {
        inseriu = insereCurso(&(*raiz)->dir, codigoCurso, nomeCurso, qtdPeriodos);
    } 
    else {
        inseriu = 0;  // Código do curso já existe
    }

    return inseriu;
}


// Função para remover um curso da árvore de cursos
struct ArvCursos* removeCurso(struct ArvCursos **raiz, int codigoCurso) {
    struct ArvCursos *Aux;

    if (*raiz != NULL) {
        if ((*raiz)->codigoCurso == codigoCurso) {
            // Caso 1: Nó folha
            if (ehfolhaCurso(*raiz)) {
                Aux = *raiz;
                *raiz = NULL;
                free(Aux);
            }
            // Caso 2: Nó com um único filho
            else if (unicoFilhoCurso(*raiz)) {
                Aux = *raiz;
                *raiz = (Aux->esq != NULL) ? Aux->esq : Aux->dir;
                free(Aux);
            } 
            // Caso 3: Nó com dois filhos
            else if (doisFilhosCurso(*raiz)) {
                int valorSubstituto = maiorInfoCurso((*raiz)->esq);
                (*raiz)->codigoCurso = valorSubstituto;
                removeCurso(&(*raiz)->esq, valorSubstituto);
            }
        } else if (codigoCurso < (*raiz)->codigoCurso) {
            removeCurso(&(*raiz)->esq, codigoCurso);
        } else {
            removeCurso(&(*raiz)->dir, codigoCurso);
        }
    }
    return *raiz;
}

/* Função para buscar um curso pelo código na árvore
int buscarCurso(struct ArvCursos *raiz, int codigoCurso) {
    int encontrado = 0; // Variável para indicar se o curso foi encontrado

    while (raiz != NULL) {
        // Se o código do curso for encontrado, incrementa a variável
        if (raiz->codigoCurso == codigoCurso) {
            encontrado++; // Curso encontrado
        }
        // Se o código procurado for menor, move para a subárvore esquerda
        if (codigoCurso < raiz->codigoCurso) {
            raiz = raiz->esq; // Percorre a subárvore esquerda
        } else {
            raiz = raiz->dir; // Percorre a subárvore direita
        }
    }

    // Retorna 1 se encontrado, 0 caso contrário
    return encontrado > 0 ? 1 : 0;
}
*/


// Função recursiva para buscar um curso pela código na árvore de cursos
struct ArvCursos* buscarCurso(struct ArvCursos *raiz, int codigoCurso) {
    if (raiz == NULL || raiz->codigoCurso == codigoCurso) {
        return raiz;  // Retorna o curso ou NULL se não encontrado
    }

    if (codigoCurso < raiz->codigoCurso) {
        return buscarCurso(raiz->esq, codigoCurso);  // Busca na subárvore esquerda
    } else {
        return buscarCurso(raiz->dir, codigoCurso);  // Busca na subárvore direita
    }
}


// Função auxiliar para buscar o curso e retornar o ponteiro para a estrutura do curso
struct ArvCursos* buscarCursoPorReferencia(struct ArvCursos *raiz, int codigoCurso) {
    while (raiz != NULL) {
        if (raiz->codigoCurso == codigoCurso) {
            return raiz; // Retorna o ponteiro do curso encontrado
        }
        if (codigoCurso < raiz->codigoCurso) {
            raiz = raiz->esq; // Percorre a subárvore esquerda
        } else {
            raiz = raiz->dir; // Percorre a subárvore direita
        }
    }
    return NULL; // Curso não encontrado
}


                                                                    //BLOCO DE DISCIPLINAS
// Função para criar uma nova disciplina
struct ArvDisciplinas* criarDisciplina(int codigoDisciplina, char nomeDisciplina[], int periodo, int cargaHoraria) {
    struct ArvDisciplinas *novaDisciplina = (struct ArvDisciplinas*) malloc(sizeof(struct ArvDisciplinas));
    if (novaDisciplina != NULL) {
        novaDisciplina->codigoDisciplina = codigoDisciplina;
        strcpy(novaDisciplina->nomeDisciplina, nomeDisciplina);
        novaDisciplina->periodo = periodo;
        novaDisciplina->cargaHoraria = cargaHoraria;
        novaDisciplina->esq = NULL;
		novaDisciplina->dir = NULL;
    }
    return novaDisciplina;
}

// Função que verifica se o nó é uma folha
int ehfolhaDisciplina(struct ArvDisciplinas *no) {
    int resultado = 0;
    if (no != NULL && no->esq == NULL && no->dir == NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que verifica se o nó tem um único filho
int unicoFilhoDisciplina(struct ArvDisciplinas *no) {
    int resultado = 0;
    if (no != NULL) {
        if ((no->esq != NULL && no->dir == NULL) || (no->esq == NULL && no->dir != NULL)) {
            resultado = 1;
        }
    }
    return resultado;
}

// Função que verifica se o nó tem dois filhos
int doisFilhosDisciplina(struct ArvDisciplinas *no) {
    int resultado = 0;
    if (no != NULL && no->esq != NULL && no->dir != NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que retorna a maior informação (código da disciplina) na subárvore direita
int maiorInfoDisciplina(struct ArvDisciplinas *no) {
    struct ArvDisciplinas *atual = no;
    int valor = -1;  // Valor padrão em caso de árvore vazia
    
    if (atual != NULL) {
        while (atual->dir != NULL) {
            atual = atual->dir;
        }
        valor = atual->codigoDisciplina;
    }
    return valor;
}


// Função para inserir uma disciplina na árvore de disciplinas
int insereDisciplina(struct ArvDisciplinas **raiz, int codigoDisciplina, char *nomeDisciplina, int periodo, int cargaHoraria) {
    int inseriu = 1;

    if (*raiz == NULL) {
        *raiz = criarDisciplina(codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);  // Função para criar um nó de disciplina
    } 
    else if (codigoDisciplina < (*raiz)->codigoDisciplina) {
        inseriu = insereDisciplina(&(*raiz)->esq, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
    } 
    else if (codigoDisciplina > (*raiz)->codigoDisciplina) {
        inseriu = insereDisciplina(&(*raiz)->dir, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
    } 
    else {
        inseriu = 0;  // Código da disciplina já existe
    }

    return inseriu;
}

// Função para remover uma disciplina da árvore de disciplinas
struct ArvDisciplinas* removeDisciplina(struct ArvDisciplinas **raiz, int codigoDisciplina) {
    struct ArvDisciplinas *Aux;

    if (*raiz != NULL) {
        if ((*raiz)->codigoDisciplina == codigoDisciplina) {
            // Caso 1: Nó folha
            if (ehfolhaDisciplina(*raiz)) {
                Aux = *raiz;
                *raiz = NULL;
                free(Aux);
            }
            // Caso 2: Nó com um único filho
            else if (unicoFilhoDisciplina(*raiz)) {
                Aux = *raiz;
                *raiz = (Aux->esq != NULL) ? Aux->esq : Aux->dir;
                free(Aux);
            } 
            // Caso 3: Nó com dois filhos
            else if (doisFilhosDisciplina(*raiz)) {
                int valorSubstituto = maiorInfoDisciplina((*raiz)->esq);
                (*raiz)->codigoDisciplina = valorSubstituto;
                removeDisciplina(&(*raiz)->esq, valorSubstituto);
            }
        } else if (codigoDisciplina < (*raiz)->codigoDisciplina) {
            removeDisciplina(&(*raiz)->esq, codigoDisciplina);
        } else {
            removeDisciplina(&(*raiz)->dir, codigoDisciplina);
        }
    }
    return *raiz;
}

// Função para buscar uma disciplina pelo código
struct ArvDisciplinas* buscarDisciplina(struct ArvDisciplinas *raiz, int codigoDisciplina) {
    struct ArvDisciplinas *resultado = NULL;  // Inicializa a variável de resultado como NULL

    if (raiz != NULL) {
        if (raiz->codigoDisciplina == codigoDisciplina) {
            resultado = raiz;  // Se encontrado, atribui o nó à variável resultado
        } else if (codigoDisciplina < raiz->codigoDisciplina) {
            resultado = buscarDisciplina(raiz->esq, codigoDisciplina);  // Busca na subárvore esquerda
        } else {
            resultado = buscarDisciplina(raiz->dir, codigoDisciplina);  // Busca na subárvore direita
        }
    }
    
    return resultado;  // Retorna o resultado da busca
}



                                                                     // BLOCO DE MATRICULAS
// Função para criar uma nova matrícula
struct ArvMatriculas* criarMatricula(int codigoDisciplina) {
    struct ArvMatriculas *novaMatricula = (struct ArvMatriculas*) malloc(sizeof(struct ArvMatriculas));
    if (novaMatricula != NULL) {
        novaMatricula->codigoDisciplina = codigoDisciplina;
        novaMatricula->esq = NULL;
		novaMatricula->dir = NULL;
    }
    return novaMatricula;
}

// Função que verifica se o nó é uma folha
int ehfolhaMatricula(struct ArvMatriculas *no) {
    int resultado = 0;
    if (no != NULL && no->esq == NULL && no->dir == NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que verifica se o nó tem um único filho
int unicoFilhoMatricula(struct ArvMatriculas *no) {
    int resultado = 0;
    if (no != NULL) {
        if ((no->esq != NULL && no->dir == NULL) || (no->esq == NULL && no->dir != NULL)) {
            resultado = 1;
        }
    }
    return resultado;
}

// Função que verifica se o nó tem dois filhos
int doisFilhosMatricula(struct ArvMatriculas *no) {
    int resultado = 0;
    if (no != NULL && no->esq != NULL && no->dir != NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que retorna a maior informação (código da disciplina) na subárvore direita
int maiorInfoMatricula(struct ArvMatriculas *no) {
    struct ArvMatriculas *atual = no;
    int valor = -1;  // Valor padrão em caso de árvore vazia
    
    if (atual != NULL) {
        while (atual->dir != NULL) {
            atual = atual->dir;
        }
        valor = atual->codigoDisciplina;
    }
    return valor;
}

// Função para inserir uma matrícula na árvore de matrículas
int insereMatricula(struct ArvMatriculas **raiz, int codigoDisciplina, int matricula) {
    int inseriu = 1;
    
    if (*raiz == NULL) {
        *raiz = criarMatricula(codigoDisciplina);  // Função para criar um nó de matrícula
    } 
    else if (codigoDisciplina < (*raiz)->codigoDisciplina) {
        inseriu = insereMatricula(&(*raiz)->esq, codigoDisciplina, matricula);
    } 
    else if (codigoDisciplina > (*raiz)->codigoDisciplina) {
        inseriu = insereMatricula(&(*raiz)->dir, codigoDisciplina, matricula);
    } 
    else {
        inseriu = 0;  // Matrícula já existe para essa disciplina
    }

    return inseriu;
}

// Função para remover uma matrícula da árvore de matrículas
struct ArvMatriculas* removeMatricula(struct ArvMatriculas **raiz, int codigoDisciplina) {
    struct ArvMatriculas *Aux;

    if (*raiz != NULL) {
        if ((*raiz)->codigoDisciplina == codigoDisciplina) {
            // Caso 1: Nó folha
            if (ehfolhaMatricula(*raiz)) {
                Aux = *raiz;
                *raiz = NULL;
                free(Aux);
            }
            // Caso 2: Nó com um único filho
            else if (unicoFilhoMatricula(*raiz)) {
                Aux = *raiz;
                *raiz = (Aux->esq != NULL) ? Aux->esq : Aux->dir;
                free(Aux);
            } 
            // Caso 3: Nó com dois filhos
            else if (doisFilhosMatricula(*raiz)) {
                int valorSubstituto = maiorInfoMatricula((*raiz)->esq);
                (*raiz)->codigoDisciplina = valorSubstituto;
                removeMatricula(&(*raiz)->esq, valorSubstituto);
            }
        } else if (codigoDisciplina < (*raiz)->codigoDisciplina) {
            removeMatricula(&(*raiz)->esq, codigoDisciplina);
        } else {
            removeMatricula(&(*raiz)->dir, codigoDisciplina);
        }
    }
    return *raiz;
}


// Função para buscar uma disciplina na árvore de matrículas
struct ArvMatriculas* buscarMatricula(struct ArvMatriculas *raiz, int codigoDisciplina) {
    struct ArvMatriculas *resultado = NULL;
    
    if (raiz != NULL) {
        if (raiz->codigoDisciplina == codigoDisciplina) {
            resultado = raiz;  // Se encontrado, retorna o nó
        } else if (codigoDisciplina < raiz->codigoDisciplina) {
            resultado = buscarMatricula(raiz->esq, codigoDisciplina);  // Busca na subárvore esquerda
        } else {
            resultado = buscarMatricula(raiz->dir, codigoDisciplina);  // Busca na subárvore direita
        }
    }
    
    return resultado;
}

void imprimirArvoreMatriculas(struct ArvMatriculas *raiz) {
    if (raiz != NULL) {
        imprimirArvoreMatriculas(raiz->esq);
        printf("Disciplina matriculada: %d\n", raiz->codigoDisciplina);
        imprimirArvoreMatriculas(raiz->dir);
    }
}

// Chamar essa função de verificação antes de imprimir
void mostrarDisciplinas(struct ArvMatriculas *raiz) {
    if (raiz == NULL) {
        printf("Nenhuma matricula encontrada.\n");
    } else {
        imprimirArvoreMatriculas(raiz);
    }
}



                                                                      // BLOCO DE NOTAS
// Função para criar uma nova nota
struct ArvNotas* criarNota(int codigoDisciplina, int semestre, float notaFinal) {
    struct ArvNotas *novaNota = (struct ArvNotas*) malloc(sizeof(struct ArvNotas));
    if (novaNota != NULL) {
        novaNota->codigoDisciplina = codigoDisciplina;
        novaNota->semestre = semestre;
        novaNota->notaFinal = notaFinal;
        novaNota->esq = NULL;
		novaNota->dir = NULL;
    }
    return novaNota;
}

// Função que verifica se o nó é uma folha
int ehfolhaNota(struct ArvNotas *no) {
    int resultado = 0;
    if (no != NULL && no->esq == NULL && no->dir == NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que verifica se o nó tem um único filho
int unicoFilhoNota(struct ArvNotas *no) {
    int resultado = 0;
    if (no != NULL) {
        if ((no->esq != NULL && no->dir == NULL) || (no->esq == NULL && no->dir != NULL)) {
            resultado = 1;
        }
    }
    return resultado;
}

// Função que verifica se o nó tem dois filhos
int doisFilhosNota(struct ArvNotas *no) {
    int resultado = 0;
    if (no != NULL && no->esq != NULL && no->dir != NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que retorna a maior informação (código da disciplina) na subárvore direita
int maiorInfoNota(struct ArvNotas *no) {
    struct ArvNotas *atual = no;
    int valor = -1;  // Valor padrão em caso de árvore vazia
    
    if (atual != NULL) {
        while (atual->dir != NULL) {
            atual = atual->dir;
        }
        valor = atual->codigoDisciplina;
    }
    return valor;
}

// Função para inserir uma nota na árvore de notas
int insereNota(struct ArvNotas **raiz, int codigoDisciplina, int semestre, float notaFinal) {
    int inseriu = 1;
    
    if (*raiz == NULL) {
        *raiz = criarNota(codigoDisciplina, semestre, notaFinal);  // Função para criar um nó de nota
    } 
    else if (codigoDisciplina < (*raiz)->codigoDisciplina) {
        inseriu = insereNota(&(*raiz)->esq, codigoDisciplina, semestre, notaFinal);
    } 
    else if (codigoDisciplina > (*raiz)->codigoDisciplina) {
        inseriu = insereNota(&(*raiz)->dir, codigoDisciplina, semestre, notaFinal);
    } 
    else {
        inseriu = 0;  // Nota já existe para essa disciplina
    }

    return inseriu;
}

// Função para remover uma nota da árvore de notas
struct ArvNotas* removeNota(struct ArvNotas **raiz, int codigoDisciplina) {
    struct ArvNotas *Aux;

    if (*raiz != NULL) {
        if ((*raiz)->codigoDisciplina == codigoDisciplina) {
            // Caso 1: Nó folha
            if (ehfolhaNota(*raiz)) {
                Aux = *raiz;
                *raiz = NULL;
                free(Aux);
            }
            // Caso 2: Nó com um único filho
            else if (unicoFilhoNota(*raiz)) {
                Aux = *raiz;
                *raiz = (Aux->esq != NULL) ? Aux->esq : Aux->dir;
                free(Aux);
            } 
            // Caso 3: Nó com dois filhos
            else if (doisFilhosNota(*raiz)) {
                int valorSubstituto = maiorInfoNota((*raiz)->esq);
                (*raiz)->codigoDisciplina = valorSubstituto;
                removeNota(&(*raiz)->esq, valorSubstituto);
            }
        } else if (codigoDisciplina < (*raiz)->codigoDisciplina) {
            removeNota(&(*raiz)->esq, codigoDisciplina);
        } else {
            removeNota(&(*raiz)->dir, codigoDisciplina);
        }
    }
    return *raiz;
}

// Função para buscar uma nota pela árvore de notas
struct ArvNotas* buscarNota(struct ArvNotas *raiz, int codigoDisciplina) {
    struct ArvNotas *resultado = NULL;  // Inicializa o resultado como NULL

    if (raiz != NULL) {
        if (raiz->codigoDisciplina == codigoDisciplina) {
            resultado = raiz;  // Se a nota foi encontrada, atribui ao resultado
        } else if (codigoDisciplina < raiz->codigoDisciplina) {
            resultado = buscarNota(raiz->esq, codigoDisciplina);  // Busca na subárvore esquerda
        } else {
            resultado = buscarNota(raiz->dir, codigoDisciplina);  // Busca na subárvore direita
        }
    }

    return resultado;  // Retorna o resultado, seja NULL ou a nota encontrada
}
 
 
                                                    //FUNÇÕES ESPECIFICAS PARA SEREM EXECUTADAS NA MAIN
                                                    
/*
i) Cadastrar alunos a qualquer momento na lista, de forma que só possa cadastrar um código de curso que já tenha sido cadastrado 
na árvore de cursos. 
*/
// Função para cadastrar aluno a qualquer momento
void cadastrarAluno(struct ArvCursos *raiz, struct Alunos **listaAlunos, int matricula, char nomeAluno[], int codigoCurso) {
    // Verifica se o curso existe na árvore
    if (buscarCurso(raiz, codigoCurso)) {
        // Se o curso foi encontrado, cria o novo aluno
        struct Alunos *novoAluno = criarAluno(matricula, nomeAluno, codigoCurso);
        if (novoAluno != NULL) {
            // Insere o aluno na lista
            inserirAluno(listaAlunos, novoAluno);
            printf("Aluno cadastrado com sucesso!\n");
        } else {
            printf("Erro ao criar o aluno.\n");
        }
    } else {
        printf("Curso com codigo %d nao encontrado. Cadastro nao realizado.\n", codigoCurso);
    }
}

/*
ii) Cadastrar cursos a qualquer momento na árvore de curso, de forma que o usuário não precise cadastrar 
as disciplinas para permitir o cadastro do curso. 
*/
// Função para cadastrar um curso na árvore de cursos
void cadastrarCurso(struct ArvCursos **raiz) {
    int codigoCurso, qtdPeriodos;
    char nomeCurso[100];

    // Leitura dos dados do curso
    printf("Digite o codigo do curso: ");
    scanf("%d", &codigoCurso);
    printf("Digite o nome do curso: ");
    scanf(" %[^\n]s", nomeCurso);  // Leitura de string com espaços
    printf("Digite a quantidade de periodos: ");
    scanf("%d", &qtdPeriodos);

    // Insere o curso na árvore
    if (insereCurso(raiz, codigoCurso, nomeCurso, qtdPeriodos)) {
        printf("Curso cadastrado com sucesso!\n");
    } else {
        printf("Curso com o codigo %d a existe.\n", codigoCurso);
    }
}

/* OPÇÃO DE FUNÇÃO 2
iii) Cadastrar disciplinas a qualquer momento em uma árvore de disciplinas de um determinado curso, ou seja, 
uma disciplina só pode ser cadastrada se o curso já estiver sido cadastrado, além disso, o período da disciplina deve ser válido, ou seja, 
estar entre 1 e a quantidade máxima de períodos do curso. A carga horária da disciplina deve ser múltiplo de 15, variando entre 30 e 90.
*/ 
int cadastrarDisciplina(struct ArvCursos *raizCursos, int codigoCurso, int codigoDisciplina, char nomeDisciplina[], int periodo, int cargaHoraria) {
    int resultado = 0;  // Variável para indicar o sucesso ou falha na operação
    struct ArvCursos *curso = NULL;  // Inicializa a variável para o curso

    // Verificar se o curso existe
    if (buscarCurso(raizCursos, codigoCurso)) {
        curso = buscarCurso(raizCursos, codigoCurso);  // Localiza o curso correspondente

        // Verificar se a disciplina já existe
        struct ArvDisciplinas *disciplinaExistente = buscarDisciplina(curso->disciplinas, codigoDisciplina);
        if (disciplinaExistente != NULL) {
            printf("Disciplina ja cadastrada no curso %s.\n", curso->nomeCurso);
            resultado = 0;  // Retorna 0 se a disciplina já existir
        } else {
            // Verificar se o período é válido
            if (periodo < 1 || periodo > curso->qtdPeriodos) {
                printf("Periodo invalido. O curso possui %d periodos.\n", curso->qtdPeriodos);
            }
            // Verificar se a carga horária é válida (múltiplo de 15 e entre 30 e 90)
            else if (cargaHoraria < 30 || cargaHoraria > 90 || cargaHoraria % 15 != 0) {
                printf("Carga horaria invalida. Deve ser multiplo de 15 e entre 30 e 90.\n");
            } 
            // Caso as verificações de período e carga horária sejam válidas
            else {
                // Inserir a disciplina na árvore de disciplinas do curso
                int inseriu = insereDisciplina(&curso->disciplinas, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
                
                if (inseriu) {
                    printf("Disciplina cadastrada com sucesso no curso %s.\n", curso->nomeCurso);
                    resultado = 1;  // Disciplina cadastrada com sucesso
                } else {
                    printf("Codigo da disciplina ja existe.\n");
                }
            }
        }
    } else {
        printf("Curso nao encontrado.\n");
    }

    return resultado;  // Retorno único indicando o sucesso ou falha
}

/*
iv) Cadastrar uma matrícula, onde a mesma é uma árvore organizada e contendo somente um código de 
uma disciplina do curso do aluno.
*/
// Função para cadastrar uma matrícula de um aluno em uma disciplina
int cadastrarMatricula(struct Alunos *alunos, int codigoMatricula, int codigoDisciplina) {
    int resultado = 0; // 0 para falha, 1 para sucesso
    struct Alunos *aluno = alunos;

    // Busca o aluno pela matrícula na lista de alunos
    while (aluno != NULL) {
        if (aluno->matricula == codigoMatricula) {
            // Aluno encontrado, tenta cadastrar a matrícula
            if (insereMatricula(&aluno->matriculas, codigoDisciplina, aluno->matricula)) {
                printf("Matricula cadastrada com sucesso para a disciplina %d.\n", codigoDisciplina);
                resultado = 1; // Cadastro realizado com sucesso
            } else {
                printf("Matricula ja existe para a disciplina %d.\n", codigoDisciplina);
            }
            return resultado; // Retorna após o cadastro
        }
        aluno = aluno->prox; // Continua na lista
    }

    // Aluno não encontrado
    printf("Aluno nao encontrado.\n");
    return resultado; // Retorna 0 para indicar falha
}

/*
v) Cadastrar Notas, permitir o cadastro de notas somente de disciplinas que estejam na árvore de matricula, 
e quando a nota for cadastrada a disciplina deve ser removida da árvore de matricula para árvore de notas. 
*/
int cadastrarNota(struct ArvMatriculas **matriculas, struct ArvNotas **notas, int codigoDisciplina, int periodo, float notaFinal) {
    int resultado = 0;  // Variável de controle para o resultado

    // Busca a disciplina na árvore de matrículas
    struct ArvMatriculas *disciplinaMatriculada = buscarMatricula(*matriculas, codigoDisciplina);
    
    if (disciplinaMatriculada != NULL) {
        // A disciplina está matriculada, então cadastramos a nota
        if (insereNota(notas, codigoDisciplina, periodo, notaFinal)) {
            // Se a inserção da nota foi bem-sucedida, removemos a disciplina da árvore de matrículas
            *matriculas = removeMatricula(matriculas, codigoDisciplina);
            printf("Nota cadastrada com sucesso para a disciplina %d.\n", codigoDisciplina);
            resultado = 1;  // Sucesso
        } else {
            printf("Erro ao cadastrar a nota. Disciplina %d ja possui nota cadastrada.\n", codigoDisciplina);
        }
    } else {
        // A disciplina não está na árvore de matrículas
        printf("Disciplina %d nao possui matricula.\n", codigoDisciplina);
    }

    return resultado;  // Retorna 1 se bem-sucedido, 0 caso contrário
}

//vi) Mostrar todos os alunos de um determinado curso.
// Função para mostrar todos os alunos de um determinado curso
void mostrarAlunosDoCurso(struct Alunos *listaAlunos, int codigoCurso) {
    struct Alunos *atual = listaAlunos;
    int encontrouAluno = 0;  // Flag para verificar se algum aluno foi encontrado

    printf("Alunos matriculados no curso %d:\n", codigoCurso);

    // Percorre a lista de alunos
    while (atual != NULL) {
        // Verifica se o aluno está matriculado no curso fornecido
        if (atual->codigoCurso == codigoCurso) {
            printf("Nome: %s, Matricula: %d\n", atual->nomeAluno, atual->matricula);
            encontrouAluno = 1;
        }
        // Avança para o próximo aluno da lista
        atual = atual->prox;
    }

    // Se nenhum aluno foi encontrado, exibe uma mensagem
    if (!encontrouAluno) {
        printf("Nenhum aluno encontrado para o curso %d.\n", codigoCurso);
    }
}


// vii)Mostrar todos os cursos do Campus. 
// Função recursiva para percorrer e mostrar todos os cursos 
//percorre a árvore de cursos em ordem crescente (in-order traversal).
void mostrarCursos(struct ArvCursos *raiz) {
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        mostrarCursos(raiz->esq);

        // Exibe o curso atual
        printf("Codigo do Curso: %d\n", raiz->codigoCurso);
        printf("Nome do Curso: %s\n", raiz->nomeCurso);
        printf("Quantidade de Periodos: %d\n\n", raiz->qtdPeriodos);

        // Percorre a subárvore direita
        mostrarCursos(raiz->dir);
    }
}

//viii)Mostrar todas as disciplinas de um determinado curso. 
// Função auxiliar para percorrer e exibir as disciplinas em ordem
void exibirDisciplinas(struct ArvDisciplinas *raizDisciplinas) {
    if (raizDisciplinas != NULL) {
        exibirDisciplinas(raizDisciplinas->esq);  // Percorre o lado esquerdo (ordem crescente)
        printf("Codigo da Disciplina: %d\n", raizDisciplinas->codigoDisciplina);
        printf("Nome da Disciplina: %s\n", raizDisciplinas->nomeDisciplina);
        printf("Periodo: %d\n", raizDisciplinas->periodo);
        printf("Carga Horaria: %d\n\n", raizDisciplinas->cargaHoraria);
        exibirDisciplinas(raizDisciplinas->dir);  // Percorre o lado direito
    }
}

// Função para mostrar todas as disciplinas de um curso
void mostrarDisciplinasCurso(struct ArvCursos *raizCursos, int codigoCurso) {
    struct ArvCursos *curso = buscarCurso(raizCursos, codigoCurso);  // Função que retorna o curso pelo código
    if (curso != NULL) {
        if (curso->disciplinas != NULL) {
            printf("Disciplinas do curso %s:\n\n", curso->nomeCurso);
            exibirDisciplinas(curso->disciplinas);  // Exibe todas as disciplinas do curso
        } else {
            printf("O curso %s nao possui disciplinas cadastradas.\n", curso->nomeCurso);
        }
    } else {
        printf("Curso nao encontrado.\n");
    }
}

//ix) Mostrar todas as disciplinas de um determinado período de um curso. 
// Função recursiva para imprimir disciplinas de um determinado período
void mostrarDisciplinasPorPeriodo(struct ArvDisciplinas *raizDisciplinas, int periodo) {
    if (raizDisciplinas != NULL) {
        // Percorrer a subárvore esquerda
        mostrarDisciplinasPorPeriodo(raizDisciplinas->esq, periodo);

        // Verificar se a disciplina está no período desejado
        if (raizDisciplinas->periodo == periodo) {
            printf("Codigo: %d, Nome: %s, Carga Horaria: %d\n",
                   raizDisciplinas->codigoDisciplina,
                   raizDisciplinas->nomeDisciplina,
                   raizDisciplinas->cargaHoraria);
        }

        // Percorrer a subárvore direita
        mostrarDisciplinasPorPeriodo(raizDisciplinas->dir, periodo);
    }
}
void disciplinasDePeriodo(struct ArvCursos *raizCursos, int codigoCurso, int periodo) {
    // Variável para armazenar o curso encontrado
    struct ArvCursos *curso = buscarCurso(raizCursos, codigoCurso);  // Função para buscar o curso

    // Verificar se o curso foi encontrado
    if (curso == NULL) {
        printf("Curso nao encontrado.\n");
    } else {
        // Verificar se o período é válido
        if (periodo < 1 || periodo > curso->qtdPeriodos) {
            printf("Periodo invalido. O curso possui %d periodos.\n", curso->qtdPeriodos);
        } else {
            // Mostrar disciplinas do período
            printf("Disciplinas do periodo %d do curso %s:\n", periodo, curso->nomeCurso);
            mostrarDisciplinasPorPeriodo(curso->disciplinas, periodo);
        }
    }
}

//x) Mostrar todas as disciplinas que um determinado aluno está matriculado
// Função auxiliar para verificar se o aluno tem matrículas
void mostrarDisciplinasAluno(struct Alunos *listaAlunos, struct ArvCursos *raizCursos, int matricula) {
    struct Alunos *aluno = listaAlunos;

    // Procura o aluno na lista
    while (aluno != NULL) {
        if (aluno->matricula == matricula) {
            printf("O aluno %s esta matriculado nas seguintes disciplinas:\n", aluno->nomeAluno);

            // Verifica se o aluno tem alguma matrícula
            if (aluno->matriculas == NULL) {
                printf("Nenhuma matricula encontrada. (A arvore de matriculas esta vazia)\n");
            } else {
                printf("Matriculas encontradas!\n Imprimindo disciplinas:\n");  // Para depuração
                imprimirArvoreMatriculas(aluno->matriculas);  // Chama a função para imprimir as matrículas
            }
            return;  // Sai após encontrar o aluno e exibir as disciplinas
        }
        aluno = aluno->prox;  // Avança para o próximo aluno
    }

    // Se o aluno não for encontrado
    printf("Aluno não encontrado.\n");
}

//xi) Mostrar todas as notas de disciplinas de um determinado período de um determinado aluno. 
// Função auxiliar para percorrer a árvore de notas e exibir notas do período especificado
void exibirNotasPeriodo(struct ArvNotas *notas, int periodo) {
    if (notas != NULL) {
        // Percorre o lado esquerdo da árvore
        exibirNotasPeriodo(notas->esq, periodo);
        
        // Verifica se o semestre (período) da nota é o especificado
        if (notas->semestre == periodo) {
            printf("Codigo da Disciplina: %d\n", notas->codigoDisciplina);
            printf("Nota Final: %.2f\n", notas->notaFinal);
            printf("Periodo: %d\n\n", notas->semestre);
        }

        // Percorre o lado direito da árvore
        exibirNotasPeriodo(notas->dir, periodo);
    }
}
// Função principal para mostrar notas de um aluno em um determinado período
void mostrarNotasAlunoPeriodo(struct Alunos *listaAlunos, int matricula, int periodo) {
    struct Alunos *aluno = buscarAluno(listaAlunos, matricula);  // Função para buscar o aluno pela matrícula

    if (aluno != NULL) {
        printf("Notas do Aluno %d - %s no Periodo %d:\n", matricula, aluno->nomeAluno, periodo);
        exibirNotasPeriodo(aluno->notas, periodo);  // Chama a função auxiliar para exibir as notas
    } else {
        printf("Aluno com matricula %d nao encontrado.\n", matricula);
    }
}


//xii) Mostrar a nota de uma disciplina de um determinado aluno, mostrando o período e a carga horária da disciplina. 
// Função principal para mostrar a nota de uma disciplina de um aluno, com período e carga horária
void mostrarNotaDisciplinaAluno(struct Alunos *listaAlunos, int matricula, int codigoDisciplina, struct ArvDisciplinas *disciplinas) {
    // Buscar o aluno na lista de alunos usando a função já implementada
    struct Alunos *aluno = buscarAluno(listaAlunos, matricula);

    if (aluno != NULL) {
        // Buscar a nota da disciplina na árvore de notas do aluno
        struct ArvNotas *nota = buscarNota(aluno->notas, codigoDisciplina);

        if (nota != NULL) {
            // Buscar a disciplina para obter o período e a carga horária
            struct ArvDisciplinas *disciplina = buscarDisciplina(disciplinas, codigoDisciplina);

            if (disciplina != NULL) {
                // Exibir as informações da nota, período e carga horária
                printf("Aluno: %s (Matricula: %d)\n", aluno->nomeAluno, aluno->matricula);
                printf("Disciplina: %s (Codigo: %d)\n", disciplina->nomeDisciplina, disciplina->codigoDisciplina);
                printf("Nota Final: %.2f\n", nota->notaFinal);
                printf("Periodo: %d\n", disciplina->periodo);
                printf("Carga Horaria: %d\n", disciplina->cargaHoraria);
            } else {
                printf("Disciplina com codigo %d nao encontrado.\n", codigoDisciplina);
            }
        } else {
            printf("Nota nao encontrada para a disciplina %d.\n", codigoDisciplina);
        }
    } else {
        printf("Aluno com matricula %d nao encontrado.\n", matricula);
    }
}


//xiii)Remover uma disciplina de um determinado curso desde que não tenha nenhum aluno matriculado na mesma.
int removerDisciplinaDeCurso(struct ArvCursos **raizCurso, int codigoCurso, int codigoDisciplina, struct Alunos *listaAlunos) {
    int resultado = 0;  // Variável para armazenar o resultado final

    // Verifica se o curso existe
    struct ArvCursos *curso = buscarCurso(*raizCurso, codigoCurso);
    if (curso == NULL) {
        printf("Curso com codigo %d nao encontrado.\n", codigoCurso);
    } else {
        // Verifica se a disciplina existe no curso
        struct ArvDisciplinas *disciplina = buscarDisciplina(curso->disciplinas, codigoDisciplina);
        if (disciplina == NULL) {
            printf("Disciplina com codigo %d nao encontrada no curso %d.\n", codigoDisciplina, codigoCurso);
        } else {
            // Verifica se há algum aluno matriculado na disciplina
            struct Alunos *atual = listaAlunos;
            int alunosMatriculados = 0;

            // Percorre a lista de alunos e suas matrículas
            while (atual != NULL && alunosMatriculados == 0) {
                struct ArvMatriculas *matricula = atual->matriculas;

                // Verifica as disciplinas nas matrículas do aluno
                while (matricula != NULL) {
                    if (matricula->codigoDisciplina == codigoDisciplina) {
                        alunosMatriculados = 1;  // Aluno encontrado matriculado na disciplina
                    }
                    matricula = matricula->dir;
                }
                atual = atual->prox;
            }

            // Se houver alunos matriculados, não pode remover
            if (alunosMatriculados) {
                printf("Nao e possivel remover a disciplina, pois ha alunos matriculados.\n");
            } else {
                // Nenhum aluno matriculado, pode remover a disciplina
                curso->disciplinas = removeDisciplina(&(curso->disciplinas), codigoDisciplina);
                printf("Disciplina com codigo %d removida com sucesso do curso %d.\n", codigoDisciplina, codigoCurso);
                resultado = 1;  // Sucesso na remoção
            }
        }
    }

    return resultado;  // Retorna 1 se a remoção for bem-sucedida, ou 0 se houver algum problema
}


//xiv)Permita remover uma disciplina da árvore de matrícula de um determinado aluno. 
int removerDisciplinaMatricula(struct Alunos *listaAlunos, int matriculaAluno, int codigoDisciplina) {
    struct Alunos *aluno = listaAlunos;
    int sucesso = 0;

    // Encontrar o aluno pela matrícula
    while (aluno != NULL && aluno->matricula != matriculaAluno) {
        aluno = aluno->prox;
    }

    // Verificar se o aluno foi encontrado
    if (aluno != NULL) {
        // Aluno encontrado, remover a disciplina de sua árvore de matrículas
        aluno->matriculas = removeMatricula(&(aluno->matriculas), codigoDisciplina);

        // Verificar se a disciplina foi removida
        if (buscarMatricula(aluno->matriculas, codigoDisciplina) == NULL) {
            printf("Disciplina com codigo %d removida com sucesso da matricula do aluno %d.\n", codigoDisciplina, matriculaAluno);
            sucesso = 1;  // Sucesso na remoção
        } else {
            printf("Falha ao remover a disciplina com codigo %d do aluno %d.\n", codigoDisciplina, matriculaAluno);
        }
    } else {
        printf("Aluno com matricula %d nao encontrado.\n", matriculaAluno);
    }

    return sucesso;
}

/*xv) Mostrar o histórico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva nota organizadas pelo 
período que a disciplina está cadastrada no curso.*/
// Função para exibir o histórico do aluno
void mostrarHistorico(struct Alunos *listaAlunos, struct ArvCursos *raizCursos, int matricula) {
    struct Alunos *aluno = buscarAluno(listaAlunos, matricula);
    struct ArvCursos *curso = NULL;  // Ponteiro para o curso do aluno
    struct ArvMatriculas *matriculaAtual = NULL;  // Ponteiro para as matrículas do aluno
    struct ArvDisciplinas *disciplina = NULL;  // Ponteiro para a disciplina
    struct ArvNotas *nota = NULL;  // Ponteiro para a nota

    // Verifica se o aluno foi encontrado
    if (aluno != NULL) {
        // Busca o curso do aluno e verifica se foi encontrado
        curso = buscarCurso(raizCursos, aluno->codigoCurso);

        // Exibir nome do curso
        if (curso != NULL) {
            printf("Historico do aluno: %s\n", aluno->nomeAluno);
            printf("Curso: %s\n", curso->nomeCurso);

            // Exibir disciplinas e notas
            matriculaAtual = aluno->matriculas;
            while (matriculaAtual != NULL) {
                // Busca a disciplina correspondente à matrícula
                disciplina = buscarDisciplina(curso->disciplinas, matriculaAtual->codigoDisciplina);
                
                // Se a disciplina foi encontrada, busca a nota
                if (disciplina != NULL) {
                    nota = buscarNota(aluno->notas, disciplina->codigoDisciplina);
                    if (nota != NULL) {
                        printf("Periodo: %d | Disciplina: %s | Nota: %.2f | Carga Horaria: %d horas\n",
                               disciplina->periodo, disciplina->nomeDisciplina, nota->notaFinal, disciplina->cargaHoraria);
                    } else {
                        printf("Periodo: %d | Disciplina: %s | Nota nao encontrada\n",
                               disciplina->periodo, disciplina->nomeDisciplina);
                    }
                } else {
                    printf("Disciplina com codigo %d nao encontrada.\n", matriculaAtual->codigoDisciplina);
                }
                // Passa para a próxima matrícula (dependendo da estrutura, pode ser diferente)
                matriculaAtual = matriculaAtual->esq;  // Aqui seria necessário ajustar para percorrer a árvore de matrículas
            }
        } else {
            printf("Curso nao encontrado para o aluno.\n");
        }
    } else {
        printf("Aluno com matrícula %d nao encontrado.\n", matricula);
    }
}

// xvi) Funções de liberar árvores e sair do programa

//Função p/ liberar árvore de deisciplina
void liberarDisciplinas(struct ArvDisciplinas *raiz) {
    if (raiz != NULL) {
        liberarDisciplinas(raiz->esq);  // Libera a subárvore esquerda
        liberarDisciplinas(raiz->dir);  // Libera a subárvore direita
        free(raiz);  // Libera o nó atual
    }
}
//Função p/ liberar arvore de cursos
void liberarCursos(struct ArvCursos *raiz) {
    if (raiz != NULL) {
        liberarDisciplinas(raiz->disciplinas);  // Libera todas as disciplinas do curso
        liberarCursos(raiz->esq);  // Libera a subárvore esquerda
        liberarCursos(raiz->dir);  // Libera a subárvore direita
        free(raiz);  // Libera o nó atual
    }
}

//Função p/ liberar matriculas
void liberarMatriculas(struct ArvMatriculas *raiz) {
    if (raiz != NULL) {
        liberarMatriculas(raiz->esq);  // Libera a subárvore esquerda
        liberarMatriculas(raiz->dir);  // Libera a subárvore direita
        free(raiz);  // Libera o nó atual
    }
}

//Função p/ liberar árvore de notas
void liberarNotas(struct ArvNotas *raiz) {
    if (raiz != NULL) {
        liberarNotas(raiz->esq);  // Libera a subárvore esquerda
        liberarNotas(raiz->dir);  // Libera a subárvore direita
        free(raiz);  // Libera o nó atual
    }
}

//Função p/ liberar lista de alunos
void liberarAlunos(struct Alunos *lista) {
    struct Alunos *atual = lista;
    while (atual != NULL) {
        struct Alunos *prox = atual->prox;
        liberarMatriculas(atual->matriculas);  // Libera as matrículas do aluno
        liberarNotas(atual->notas);  // Libera as notas do aluno
        free(atual);  // Libera o aluno atual
        atual = prox;
    }
}

//Função p/ liberar todas as arvores
void liberarArvores(struct ArvCursos *raizCursos, struct Alunos *listaAlunos) {
    liberarCursos(raizCursos);  // Libera todos os cursos e suas disciplinas
    liberarAlunos(listaAlunos);  // Libera todos os alunos, suas matrículas e notas
}


                                                                       //Função principal
                                                                       

int main() {
    struct ArvCursos *raizCursos = NULL;  // Árvore de cursos
    struct Alunos *listaAlunos = NULL;    // Lista de alunos
    int opcao, codigoCurso, codigoDisciplina, matricula, periodo, semestre, cargaHoraria, resultado, i;
    char nomeAluno[100], nomeDisciplina[100];
    float notaFinal;
    struct Alunos *aluno;
    double soma, media, tempo_decorrido, tempDecorrido[30];
    struct timeval inicio, fim;

    while (1) {
        // Exibe o menu
        printf("Menu:\n");
        printf("1. Cadastrar curso\n"); //ii)
        printf("2. Cadastrar disciplina\n"); //iii)
        printf("3. Cadastrar aluno\n"); //i)
        printf("4. Cadastrar matricula\n"); //iv)
        printf("5. Cadastrar nota\n");
        printf("6. Mostrar alunos de um curso\n");
        printf("7. Mostrar todos os cursos\n");
        printf("8. Mostrar todas as disciplinas por curso\n");
        printf("9. Mostrar todas as disciplinas por periodo\n");
        printf("10. Mostrar todas as disciplinas de um aluno\n");
        printf("11. Mostrar todas as notas por periodo de um aluno\n");
        printf("12. Mostrar a nota, periodo e carga horaria de uma disciplina\n");
        printf("13.Remover uma disciplina de um curso\n");
        printf("14.Remover uma disciplina de um aluno\n");
        printf("15. Mostrar o historico de um aluno\n");
        printf("16. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:  // Cadastrar curso
            	for (i = 0; i < 30; i++) {
			        // Marca o tempo inicial
			        gettimeofday(&inicio, NULL);
                	cadastrarCurso(&raizCursos);
                	// Marca o tempo final
			        gettimeofday(&fim, NULL);
			        // Calcula o tempo decorrido em microssegundos
			        tempo_decorrido = (double)(fim.tv_sec - inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - inicio.tv_usec);
			        
			        // Armazena o tempo decorrido
			        tempDecorrido[i] = tempo_decorrido;
			        
			        // Soma os tempos para calcular a média posteriormente
			        soma += tempo_decorrido;
				}
				// Calcula a média dos tempos decorridos
			    media = soma / 30;
			    printf("A media de tempos eh: %lf microssegundos\n", media);
                break;

            case 2:  // Cadastrar disciplina
                printf("Digite o codigo do curso: ");
                scanf("%d", &codigoCurso);
                printf("Digite o codigo da disciplina: ");
                scanf("%d", &codigoDisciplina);
                printf("Digite o nome da disciplina: ");
                scanf(" %[^\n]s", nomeDisciplina);
                printf("Digite o periodo da disciplina: ");
                scanf("%d", &periodo);
                printf("Digite a carga horaria da disciplina: ");
                scanf("%d", &cargaHoraria);
                cadastrarDisciplina(raizCursos, codigoCurso, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
                break;
            
            case 3:  // Cadastrar aluno
                printf("Digite a matricula: ");
                scanf("%d", &matricula);
                printf("Digite o nome do aluno: ");
                scanf(" %[^\n]s", nomeAluno);  // Leitura de string com espaços
                printf("Digite o codigo do curso: ");
                scanf("%d", &codigoCurso);
                cadastrarAluno(raizCursos, &listaAlunos, matricula, nomeAluno, codigoCurso);
                break;

            case 4:  // Cadastrar matricula
                printf("Digite a matricula do aluno: ");
                scanf("%d", &matricula);
                printf("Digite o codigo da disciplina: ");
                scanf("%d", &codigoDisciplina);
                cadastrarMatricula(listaAlunos, matricula, codigoDisciplina);
                break;

            case 5:  // Cadastrar nota
                printf("Digite a matricula do aluno: ");
                scanf("%d", &matricula);
                aluno = buscarAluno(listaAlunos, matricula);
                if (aluno != NULL) {
                    printf("Digite o codigo da disciplina: ");
                    scanf("%d", &codigoDisciplina);
                    printf("Digite o semestre: ");
                    scanf("%d", &semestre);
                    printf("Digite a nota final: ");
                    scanf("%f", &notaFinal);
                    cadastrarNota(&aluno->matriculas, &aluno->notas, codigoDisciplina, semestre, notaFinal);
                } else {
                    printf("Aluno nao encontrado.\n");
                }
                break;

            case 6:  // Mostrar alunos de um curso
                printf("Digite o codigo do curso para listar os alunos matriculados: ");
                scanf("%d", &codigoCurso);
                // Chama a função para mostrar os alunos do curso fornecido
                mostrarAlunosDoCurso(listaAlunos, codigoCurso);
                break;
            
            case 7:   // Mostrar todos os cursos
			    printf("Listando todos os cursos do campus:\n\n");
			    // Chama a função para mostrar os cursos da árvore
			    mostrarCursos(raizCursos);
			    break;
			
			case 8:  // Mostrar todas as disciplinas de um curso
			    printf("Digite o codigo do curso: ");
			    scanf("%d", &codigoCurso);
			    mostrarDisciplinasCurso(raizCursos, codigoCurso);
			    break;
			
			case 9: // mostrar as disciplinas do período
			    printf("Digite o codigo do curso: ");
			    scanf("%d", &codigoCurso);
			    printf("Digite o periodo desejado: ");
			    scanf("%d", &periodo);
			    disciplinasDePeriodo(raizCursos, codigoCurso, periodo);
			    break;
			
			case 10: // mostrar as disciplinas do aluno
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    mostrarDisciplinasAluno(listaAlunos, raizCursos, matricula);
			    break;
			
			case 11: // mostrar as notas do aluno no período
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    printf("Digite o periodo desejado: ");
			    scanf("%d", &periodo);
			    mostrarNotasAlunoPeriodo(listaAlunos, matricula, periodo);
			    break;
			
			case 12: //mostrar a nota da disciplina do aluno, período e carga horária
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    printf("Digite o codigo da disciplina: ");
			    scanf("%d", &codigoDisciplina);
			    printf("Digite o codigo do curso: ");
			    scanf("%d", &codigoCurso);
			    struct ArvCursos *curso = buscarCurso(raizCursos, codigoCurso);
			    
			    if (curso != NULL) {
					for (i = 0; i < 30; i++) {
						
				        // Marca o tempo inicial
				        gettimeofday(&inicio, NULL);
				        
				        mostrarNotaDisciplinaAluno(listaAlunos, matricula, codigoDisciplina, curso->disciplinas);
				        
				        // Marca o tempo final
				        gettimeofday(&fim, NULL);
				        
				        // Calcula o tempo decorrido em microssegundos
				        tempo_decorrido = (double)(fim.tv_sec - inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - inicio.tv_usec);
				        
				        // Armazena o tempo decorrido
				        tempDecorrido[i] = tempo_decorrido;
				        
				        // Soma os tempos para calcular a média posteriormente
				        soma += tempo_decorrido;
					}
				media = soma / 30;
			    printf("A media de tempos eh: %lf microssegundos\n", media);
			    } else {
			        printf("Curso com codigo %d nao encontrado.\n", codigoCurso);
			    }
			    break;
			
			case 13: //remover disciplina de um curso
			    printf("Digite o codigo do curso: ");
			    scanf("%d", &codigoCurso);
			    printf("Digite o codigo da disciplina a ser removida: ");
			    scanf("%d", &codigoDisciplina);
			    resultado = removerDisciplinaDeCurso(&raizCursos, codigoCurso, codigoDisciplina, listaAlunos);
			    if (resultado) {
			        printf("Remocao realizada com sucesso!\n");
			    } else {
			        printf("Falha ao remover a disciplina.\n");
			    }
			    break;

			
			case 14: //remover disciplina de um aluno
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    printf("Digite o codigo da disciplina a ser removida: ");
			    scanf("%d", &codigoDisciplina);
			    resultado = removerDisciplinaMatricula(listaAlunos, matricula, codigoDisciplina);
			    if (resultado) {
			        printf("Remocao realizada com sucesso!\n");
			    } else {
			        printf("Falha ao remover a disciplina.\n");
			    }
			    break;
			
			case 15: //mostrar historico
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    mostrarHistorico(listaAlunos, raizCursos, matricula);
			    break;

            case 16:  // Sair
			    liberarArvores(raizCursos, listaAlunos);  // Libera toda a memória alocada
			    printf("Memoria liberada. Saindo do programa.\n");
			    exit(0);
			    break;


            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}


 