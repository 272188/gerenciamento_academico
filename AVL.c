/*                                                                       �RVORES AVL
Fa�a um programa em C que cadastre dados sobre alunos e seu hist�rico dentro de um determinado curso. Para isto, 
� preciso ter as seguintes estruturas de dados: 
(a) Lista de Alunos: Matricula do aluno, Nome do aluno, C�digo do Curso, �rvore de Notas, �rvore de Matr�cula, 
    a lista deve ser organizada pela ordem de alfab�tica por nome do aluno. 
(b) �rvore de Curso: C�digo do curso, Nome do curso, quantidade de Per�odos e uma �rvore de Disciplinas. 
    A �rvore deve ser organizada pelo nome do C�digo do Curso. 
(c) �rvore de disciplinas: c�digo da disciplina, nome da disciplina, per�odo, carga hor�ria. 
    A �rvore deve ser organizada pelo c�digo da disciplina. 
(d) �rvore de Notas: c�digo da disciplina, semestre cursado, nota final obtida pelo aluno. 
    A �rvore deve ser organizada pelo c�digo da disciplina. 
(e) �rvore de Matricula: c�digo da disciplina, organizada pelo c�digo da disciplina 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// Estrutura da �rvore de Cursos
typedef struct ArvCursos {
    int codigoCurso;
    char nomeCurso[100];
    int qtdPeriodos;
    struct ArvDisciplinas *disciplinas;
    struct ArvCursos *esq, *dir;         
    int altura;
} ArvCursos;

// Estrutura da �rvore de Disciplinas
typedef struct ArvDisciplinas {
    int codigoDisciplina;
    char nomeDisciplina[100];
    int periodo;
    int cargaHoraria;
    struct ArvDisciplinas *esq, *dir;
    int altura;
} ArvDisciplinas;

// Estrutura da �rvore de Notas (AVL)
typedef struct ArvNotas {
    int codigoDisciplina;
    int semestre;
    float notaFinal;
    struct ArvNotas *esq, *dir;
    int altura;
} ArvNotas;

// Estrutura da �rvore de Matr�cula
typedef struct ArvMatriculas {
	int matricula;
    int codigoDisciplina;
    struct ArvMatriculas *esq, *dir;
    int altura;
} ArvMatriculas;

// Estrutura da lista de Alunos
typedef struct Alunos {
    int matricula;
    char nomeAluno[100];
    int codigoCurso;
    ArvNotas *notas;           // �rvore de Notas do Aluno
    ArvMatriculas *matriculas;  // �rvore de Matr�culas do Aluno
    struct Alunos *prox;        // Ponteiro para o pr�ximo aluno na lista
} Alunos;



                                                                     //BLOCO LISTA DE ALUNOS
// Fun��o para criar um novo aluno
Alunos *criarAluno(int matricula, const char *nomeAluno, int codigoCurso) {
    Alunos *novoAluno = (Alunos *)malloc(sizeof(Alunos));
    if (novoAluno != NULL) {
        novoAluno->matricula = matricula;
        strcpy(novoAluno->nomeAluno, nomeAluno);
        novoAluno->codigoCurso = codigoCurso;
        novoAluno->notas = NULL;       // Inicializa �rvores de notas e matr�culas como vazias
        novoAluno->matriculas = NULL;
        novoAluno->prox = NULL;        // Pr�ximo aluno � NULL
    } else {
        printf("Erro ao alocar memoria para novo aluno.\n");
    }
    return novoAluno;
}

// Fun��o para inserir aluno em ordem alfab�tica
void inserirAluno(Alunos **lista, Alunos *novoAluno) {
    Alunos *atual = *lista, *anterior = NULL;

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

// Fun��o para remover um aluno da lista de alunos
int removerAluno(Alunos **lista, int matricula) {
    Alunos *atual = *lista;
    Alunos *anterior = NULL;

    // Percorrer a lista para encontrar o aluno
    while (atual != NULL && atual->matricula != matricula) {
        anterior = atual;
        atual = atual->prox;
    }

    // Se o aluno n�o foi encontrado
    if (atual == NULL) {
        printf("Aluno com matricula %d nao encontrado.\n", matricula);
        return 0;  // Falha na remo��o
    }

    // Se o aluno for o primeiro da lista
    if (anterior == NULL) {
        *lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    // Libera a mem�ria do aluno removido
    free(atual);

    printf("Aluno com matricula %d removido com sucesso.\n", matricula);
    return 1;  // Sucesso na remo��o
}

// Fun��o para buscar um aluno na lista de alunos
Alunos* buscarAluno(Alunos *lista, int matricula) {
    Alunos *atual = lista;

    while (atual != NULL) {
        if (atual->matricula == matricula) {
            return atual;  // Retorna o ponteiro para o aluno encontrado
        }
        atual = atual->prox;  // Move para o pr�ximo aluno
    }

    return NULL;  // Retorna NULL se o aluno n�o for encontrado
}


// Fun��o para imprimir a lista de alunos
void imprimirListaAlunos(Alunos *lista) {
    Alunos *atual = lista;
    while (atual != NULL) {
        printf("Alunos: %s, Matricula: %d, Curso: %d\n", atual->nomeAluno, atual->matricula, atual->codigoCurso);
        atual = atual->prox;
    }
}


                                                                    //BLOCO �RVORE DE CURSO


/*MENOR INFORMACAO
// Fun��o auxiliar para encontrar o n� com o menor nome do curso em uma sub�rvore
ArvCursos* menorInfoCurso(ArvCursos* no) {
    ArvCursos* atual = no;
    while (atual && atual->esq != NULL)
        atual = atual->esq;
    return atual;
}
*/
int maior(int a, int b) {
    return (a > b) ? a : b;
}


//MAIOR INFORMACAO
// Fun��o auxiliar para encontrar o n� com o maior nome do curso em uma sub�rvore
ArvCursos* maiorInfoCurso(ArvCursos *no) {
    while (no != NULL && no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

//CALCULAR ALTURA
//Fun��o para calcular a altura de um n� Curso
int alturaCurso(ArvCursos *n) {
    return (n == NULL) ? -1 : n->altura;
}


//FATOR BALANCEAMENTO
// Fun��o para calcular o fator de balanceamento de um n� Curso
int fbCurso(ArvCursos *no) {
    if (no == NULL){
		return 0;
	}else{
		return alturaCurso(no->esq) - alturaCurso(no->dir);
	}
}

//ROTA��O A ESQUERDA
// Fun��o para rotacionar � esquerda
ArvCursos *rotacaoEsqCurso(ArvCursos *no) {
    ArvCursos *novaRaiz = no->dir;
    no->dir = novaRaiz->esq;
    novaRaiz->esq = no;

    // Atualiza as alturas usando max para encontrar o maior valor entre as alturas das sub�rvores
    no->altura = maior(alturaCurso(no->esq), alturaCurso(no->dir)) + 1;
    novaRaiz->altura = maior(alturaCurso(novaRaiz->esq), alturaCurso(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTA��O A DIREITA
// Fun��o para rotacionar � direita
ArvCursos *rotacaoDirCurso(ArvCursos *no) {
    ArvCursos *novaRaiz = no->esq;  // O n� que se tornar� a nova raiz
    no->esq = novaRaiz->dir;  // O filho � direita da nova raiz se torna o filho � esquerda do n� original
    novaRaiz->dir = no;  // O n� original se torna o filho � direita da nova raiz

    // Atualizando as alturas
    no->altura = maior(alturaCurso(no->esq), alturaCurso(no->dir)) + 1;  // Atualiza a altura do n� original
    novaRaiz->altura = maior(alturaCurso(novaRaiz->esq), alturaCurso(novaRaiz->dir)) + 1;  // Atualiza a altura da nova raiz

    return novaRaiz;  // Retorna a nova raiz
}


//ROTA��ES DUPLAS
//Fun��o Rota��o dupla esquerda-direita
ArvCursos *rotacaoEsqDirCurso(ArvCursos *no) {
    no->esq = rotacaoEsqCurso(no->esq);
    return rotacaoDirCurso(no);
}
//Fun��o Rota��o dupla direita-esquerda
ArvCursos *rotacaoDirEsqCurso(ArvCursos *no) {
    no->dir = rotacaoDirCurso(no->dir);
    return rotacaoEsqCurso(no);
}

//BALANCEAMENTO
// Fun��o de balanceamento da �rvore de cursos
ArvCursos *balanceamentoArvCurso(ArvCursos *raiz, int codigoCurso) {
    // Verifica o balanceamento do n� atual
    int balanceamento = fbCurso(raiz);

    // Caso 1 - Desbalanceamento � esquerda
    if (balanceamento > 1 && codigoCurso < raiz->esq->codigoCurso) {
        return rotacaoDirCurso(raiz);
    }

    // Caso 2 - Desbalanceamento � direita
    if (balanceamento < -1 && codigoCurso > raiz->dir->codigoCurso) {
        return rotacaoEsqCurso(raiz);
    }

    // Caso 3 - Desbalanceamento esquerda-direita
    if (balanceamento > 1 && codigoCurso > raiz->esq->codigoCurso) {
        return rotacaoEsqDirCurso(raiz);
    }

    // Caso 4 - Desbalanceamento direita-esquerda
    if (balanceamento < -1 && codigoCurso < raiz->dir->codigoCurso) {
        return rotacaoDirEsqCurso(raiz);
    }

    return raiz;  // Retorna a raiz se estiver balanceada
}

//CRIAR NO
// Fun��o para criar um novo curso
ArvCursos *criarCurso(int codigoCurso, const char *nomeCurso, int qtdPeriodos) {
    ArvCursos *novoCurso = (ArvCursos *)malloc(sizeof(ArvCursos));
    if (novoCurso != NULL) {
        novoCurso->codigoCurso = codigoCurso;
        strcpy(novoCurso->nomeCurso, nomeCurso);
        novoCurso->qtdPeriodos = qtdPeriodos;
        novoCurso->disciplinas = NULL;  // A �rvore de disciplinas come�a vazia
        novoCurso->esq = NULL;
        novoCurso->dir = NULL;
        novoCurso->altura = 0;          // Altura inicial da AVL � 0
    } else {
        printf("Erro ao alocar memoria para novo curso.\n");
    }
    return novoCurso;
}

//INSER��O
//Fun��o de Inser��o Balanceada
// Fun��o de Inser��o Balanceada
ArvCursos *inserirCurso(ArvCursos *raiz, int codigoCurso, const char *nomeCurso, int qtdPeriodos) {
    if (raiz == NULL) {
        // Se a raiz for NULL, cria um novo curso
        return criarCurso(codigoCurso, nomeCurso, qtdPeriodos);
    }

    // Inser��o normal de um n� na �rvore
    if (codigoCurso < raiz->codigoCurso) {
        raiz->esq = inserirCurso(raiz->esq, codigoCurso, nomeCurso, qtdPeriodos);
    } else if (codigoCurso > raiz->codigoCurso) {
        raiz->dir = inserirCurso(raiz->dir, codigoCurso, nomeCurso, qtdPeriodos);
    } else {
        // Cursos com c�digos iguais n�o s�o permitidos, retorna a pr�pria raiz
        return raiz;
    }

    // Atualiza a altura do n� atual ap�s a inser��o
    raiz->altura = 1 + maior(alturaCurso(raiz->esq), alturaCurso(raiz->dir));

    // Chama a fun��o de balanceamento para corrigir a �rvore, se necess�rio
    return balanceamentoArvCurso(raiz, codigoCurso);  // Passa o codigoCurso
}


// Fun��o para remover um curso da �rvore AVL de cursos
ArvCursos* removerCurso(ArvCursos *raiz, int codigoCurso) {
    if (raiz == NULL) {
        printf("Curso n�o encontrado!\n");
        return NULL;
    }

    if (codigoCurso < raiz->codigoCurso) {
        raiz->esq = removerCurso(raiz->esq, codigoCurso);
    } else if (codigoCurso > raiz->codigoCurso) {
        raiz->dir = removerCurso(raiz->dir, codigoCurso);
    } else {
        if (raiz->esq == NULL && raiz->dir == NULL) { // N� folha
            free(raiz);
            printf("Curso folha removido: %d!\n", codigoCurso);
            return NULL;
        } else if (raiz->esq != NULL && raiz->dir != NULL) { // N� com dois filhos
            ArvCursos *aux = maiorInfoCurso(raiz->esq);
            raiz->codigoCurso = aux->codigoCurso;
            strcpy(raiz->nomeCurso, aux->nomeCurso);
            raiz->qtdPeriodos = aux->qtdPeriodos;
            raiz->disciplinas = aux->disciplinas;
            raiz->esq = removerCurso(raiz->esq, aux->codigoCurso);
        } else { // N� com um filho
            ArvCursos *aux = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            free(raiz);
            printf("Curso com 1 filho removido: %d!\n", codigoCurso);
            return aux;
        }
    }

    raiz->altura = maior(alturaCurso(raiz->esq), alturaCurso(raiz->dir)) + 1;
    return balanceamentoArvCurso(raiz, codigoCurso);
}

// Fun��o auxiliar recursiva para buscar o curso e retornar o ponteiro para a estrutura do curso
ArvCursos* buscarCursoPorReferencia(ArvCursos *raiz, int codigoCurso) {
    ArvCursos *resultado = NULL;

    if (raiz != NULL) {
        if (raiz->codigoCurso == codigoCurso) {
            resultado = raiz; // Curso encontrado
        } else if (codigoCurso < raiz->codigoCurso) {
            resultado = buscarCursoPorReferencia(raiz->esq, codigoCurso); // Percorre a sub�rvore esquerda
        } else {
            resultado = buscarCursoPorReferencia(raiz->dir, codigoCurso); // Percorre a sub�rvore direita
        }
    }

    return resultado; // Retorna o ponteiro do curso encontrado ou NULL se n�o encontrado
}

// Fun��o para imprimir a �rvore de cursos
void imprimirCursoNivel(ArvCursos *Raiz, int nivel) {
	int i;
    if (Raiz != NULL) {
        // Imprime a sub�rvore direita
        imprimirCursoNivel(Raiz->dir, nivel + 1);
        
        // Indenta para mostrar a hierarquia
        for (i = 0; i < nivel; i++) {
            printf("\t");
        }
        
        // Imprime o c�digo do curso, o nome do curso, a quantidade de per�odos e a altura do n�
        printf("C�digo: %d - Nome: %s - Per�odos: %d (Altura: %d)\n", 
               Raiz->codigoCurso, 
               Raiz->nomeCurso, 
               Raiz->qtdPeriodos, 
               Raiz->altura);
        
        // Imprime a sub�rvore esquerda
        imprimirCursoNivel(Raiz->esq, nivel + 1);
    }
}


                                                                      //BLOCO �RVORE DE DISCIPLINAS



//MAIOR INFORMACAO
// Fun��o auxiliar para encontrar o n� com o maior n� em uma sub�rvore de disciplina
ArvDisciplinas* maiorInfoDisciplina(ArvDisciplinas *no) {
    while (no != NULL && no->dir != NULL) {
        no = no->dir;
    }
    return no;
}


//CALCULAR ALTURA
//Fun��o para calcular a altura de um n� Disciplina																  																  //Fun��o para calcular a altura de um n� Disciplina
int alturaDisciplina(ArvDisciplinas *n) {
    if (n == NULL) {
        return -1;
    }
    return n->altura; // Corrigido para retornar a altura
}

//FATOR BALANCEAMENTO
// Fun��o para calcular o fator de balanceamento de um n� Disciplina
int fbDisciplina(ArvDisciplinas *no) {
    if (no == NULL){
		return 0;
	}else{
		return alturaDisciplina(no->esq) - alturaDisciplina(no->dir);
	}
}

//ROTA��O A ESQUERDA
// Fun��o para rotacionar � esquerda
ArvDisciplinas *rotacaoEsqDisciplina(ArvDisciplinas *no) {
    ArvDisciplinas *novaRaiz = no->dir;
    no->dir = novaRaiz->esq;
    novaRaiz->esq = no;

    no->altura = maior(alturaDisciplina(no->esq), alturaDisciplina(no->dir)) + 1;
    novaRaiz->altura = maior(alturaDisciplina(novaRaiz->esq), alturaDisciplina(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTA��O A DIREITA
// Fun��o para rotacionar � direita
ArvDisciplinas *rotacaoDirDisciplina(ArvDisciplinas *no) {
    ArvDisciplinas *novaRaiz = no->esq;
    no->esq = novaRaiz->dir;
    novaRaiz->dir = no;

    no->altura = maior(alturaDisciplina(no->esq), alturaDisciplina(no->dir)) + 1;
    novaRaiz->altura = maior(alturaDisciplina(novaRaiz->esq), alturaDisciplina(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTA��ES DUPLAS
//Fun��o Rota��o dupla esquerda-direita
ArvDisciplinas *rotacaoEsqDirDisciplina(ArvDisciplinas *no) {
    no->esq = rotacaoEsqDisciplina(no->esq);
    return rotacaoDirDisciplina(no);
}
//Fun��o Rota��o dupla direita-esquerda
ArvDisciplinas *rotacaoDirEsqDisciplina(ArvDisciplinas *no) {
    no->dir = rotacaoDirDisciplina(no->dir);
    return rotacaoEsqDisciplina(no);
}


//BALANCEAMENTO
// Fun��o de balanceamento da �rvore de cursos
ArvDisciplinas *balanceamentoArvDisciplina(ArvDisciplinas *raiz, int codigoDisciplina) {
    // Verifica o balanceamento do n� atual
    int balanceamento = fbDisciplina(raiz);

    // Caso 1 - Desbalanceamento � esquerda
    if (balanceamento > 1 && codigoDisciplina < raiz->esq->codigoDisciplina) {
        return rotacaoDirDisciplina(raiz);
    }

    // Caso 2 - Desbalanceamento � direita
    if (balanceamento < -1 && codigoDisciplina > raiz->dir->codigoDisciplina) {
        return rotacaoEsqDisciplina(raiz);
    }

    // Caso 3 - Desbalanceamento esquerda-direita
    if (balanceamento > 1 && codigoDisciplina > raiz->esq->codigoDisciplina) {
        return rotacaoEsqDirDisciplina(raiz);
    }

    // Caso 4 - Desbalanceamento direita-esquerda
    if (balanceamento < -1 && codigoDisciplina < raiz->dir->codigoDisciplina) {
        return rotacaoDirEsqDisciplina(raiz);
    }

    return raiz; // Retorna a raiz se estiver balanceada
}


// Fun��o para criar uma nova disciplina																  																  																  // Fun��o para criar uma nova disciplina
ArvDisciplinas *criarDisciplina(int codigoDisciplina, char nomeDisciplina[], int periodo, int cargaHoraria) {
    ArvDisciplinas *novaDisciplina = (ArvDisciplinas *) malloc(sizeof(ArvDisciplinas));
    if (novaDisciplina != NULL) {
        novaDisciplina->codigoDisciplina = codigoDisciplina;
        strcpy(novaDisciplina->nomeDisciplina, nomeDisciplina);
        novaDisciplina->periodo = periodo;
        novaDisciplina->cargaHoraria = cargaHoraria;
        novaDisciplina->esq = NULL;
		novaDisciplina->dir = NULL;
		novaDisciplina->altura = 0;
    }else {
        printf("Erro ao alocar memoria para nova disciplina.\n");
    }
    return novaDisciplina;
}

//INSER��O
//Fun��o de Inser��o Balanceada
ArvDisciplinas *inserirDisciplina(ArvDisciplinas *raiz, int codigoDisciplina, char nomeDisciplina[], int periodo, int cargaHoraria) {
    if (raiz == NULL) {
        return criarDisciplina(codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
    }

    // Inser��o normal de um n� na �rvore
    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = inserirDisciplina(raiz->esq, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = inserirDisciplina(raiz->dir, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
    } else {
        return raiz; // Disciplinas com c�digos iguais n�o s�o permitidos
    }

    // Atualiza a altura do n� atual ap�s a inser��o
    raiz->altura = 1 + maior(alturaDisciplina(raiz->esq), alturaDisciplina(raiz->dir));

    // Chama a fun��o de balanceamento para corrigir a �rvore, se necess�rio
    return balanceamentoArvDisciplina(raiz, codigoDisciplina);
}

// Fun��o para remover uma disciplina da �rvore de disciplinas
ArvDisciplinas *removerDisciplina(ArvDisciplinas *raiz, int codigoDisciplina) {
    if (raiz == NULL) {
        printf("Disciplina com codigo %d nao encontrada.\n", codigoDisciplina);
        return raiz; // Disciplina n�o encontrada
    }

    // Navega na �rvore para encontrar a disciplina
    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = removerDisciplina(raiz->esq, codigoDisciplina);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = removerDisciplina(raiz->dir, codigoDisciplina);
    } else {
        // Disciplina encontrada
        if (raiz->esq == NULL || raiz->dir == NULL) {
            // Caso 1: O n� tem 0 ou 1 filho
            ArvDisciplinas *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                // Caso 2: O n� � uma folha
                free(raiz);
                return NULL;
            } else {
                // Caso 1: O n� tem um filho
                ArvDisciplinas *aux = temp;
                free(raiz);
                return aux;
            }
        } else {
            // Caso 3: O n� tem dois filhos
            ArvDisciplinas *temp = maiorInfoDisciplina(raiz->esq); // Encontrar o maior na sub�rvore esquerda
            raiz->codigoDisciplina = temp->codigoDisciplina; // Substitui o c�digo da disciplina
            strcpy(raiz->nomeDisciplina, temp->nomeDisciplina);
            raiz->periodo = temp->periodo;
            raiz->cargaHoraria = temp->cargaHoraria;

            // Remove o n� duplicado
            raiz->esq = removerDisciplina(raiz->esq, temp->codigoDisciplina);
        }
    }

    // Atualiza a altura do n� atual ap�s a remo��o
    raiz->altura = 1 + maior(alturaDisciplina(raiz->esq), alturaDisciplina(raiz->dir));

    // Chama a fun��o de balanceamento para corrigir a �rvore, se necess�rio
    return balanceamentoArvDisciplina(raiz, raiz->codigoDisciplina);
}

// Fun��o para buscar uma disciplina na �rvore de disciplinas
/*ArvDisciplinas *buscarDisciplina(ArvDisciplinas *raiz, int codigoDisciplina) {
    if (raiz == NULL) {
        return NULL;  // Disciplina n�o encontrada
    }

    if (codigoDisciplina == raiz->codigoDisciplina) {
        return raiz;  // Disciplina encontrada
    } else if (codigoDisciplina < raiz->codigoDisciplina) {
        return buscarDisciplina(raiz->esq, codigoDisciplina);  // Busca � esquerda
    } else {
        return buscarDisciplina(raiz->dir, codigoDisciplina);  // Busca � direita
    }
}
*/
ArvDisciplinas* buscarDisciplina(ArvDisciplinas *raiz, int codigoDisciplina) {
    if (raiz == NULL || raiz->codigoDisciplina == codigoDisciplina) {
        return raiz;
    }
    if (codigoDisciplina < raiz->codigoDisciplina) {
        return buscarDisciplina(raiz->esq, codigoDisciplina);
    } else {
        return buscarDisciplina(raiz->dir, codigoDisciplina);
    }
}

ArvDisciplinas* buscarDisciplinaPorCodigo(ArvDisciplinas *raiz, int codigoDisciplina) {
    if (raiz == NULL) return NULL; // Se a �rvore estiver vazia

    if (raiz->codigoDisciplina == codigoDisciplina) {
        return raiz; // Disciplina encontrada
    } else if (codigoDisciplina < raiz->codigoDisciplina) {
        return buscarDisciplinaPorCodigo(raiz->esq, codigoDisciplina); // Percorre a sub�rvore esquerda
    } else {
        return buscarDisciplinaPorCodigo(raiz->dir, codigoDisciplina); // Percorre a sub�rvore direita
    }
}




// Fun��o para imprimir os detalhes de uma disciplina
void imprimirDisciplina(ArvDisciplinas *raiz, int codigoDisciplina) {
    ArvDisciplinas *disciplina = buscarDisciplina(raiz, codigoDisciplina);
    
    if (disciplina != NULL) {
        printf("Disciplina encontrada:\n");
        printf("Codigo: %d\n", disciplina->codigoDisciplina);
        printf("Nome: %s\n", disciplina->nomeDisciplina);
        printf("Periodo: %d\n", disciplina->periodo);
        printf("Carga Horaria: %d\n", disciplina->cargaHoraria);
    } else {
        printf("Disciplina com codigo %d nao encontrada.\n", codigoDisciplina);
    }
}

void imprimirDisciplinas(ArvDisciplinas *raiz) {
    if (raiz != NULL) {
        imprimirDisciplinas(raiz->esq);
        printf("C�digo: %d, Nome: %s, Carga Hor�ria: %d\n", raiz->codigoDisciplina, raiz->nomeDisciplina, raiz->cargaHoraria);
        imprimirDisciplinas(raiz->dir);
    }
}

void imprimirArvDisciplinas(ArvDisciplinas *raiz) {
    if (raiz != NULL) {
        // Percorre a sub�rvore esquerda
        imprimirArvDisciplinas(raiz->esq);

        // Exibe a disciplina atual
        printf("Codigo da Disciplina: %d\n", raiz->codigoDisciplina);
        printf("Nome da Disciplina: %s\n", raiz->nomeDisciplina);
        printf("Periodo: %d\n", raiz->periodo);
        printf("Carga Horaria: %d\n\n", raiz->cargaHoraria);

        // Percorre a sub�rvore direita
        imprimirArvDisciplinas(raiz->dir);
    }
}



                                                                      // BLOCO �RVORE DE MATRICULAS


//MAIOR INFORMACAO
// Fun��o auxiliar para encontrar o n� com o maior n� em uma sub�rvore de Matricula
ArvMatriculas* maiorInfoMatricula(ArvMatriculas *no) {
    while (no != NULL && no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

//Fun��o para calcular a altura de um n� Matricula
int alturaMatricula(ArvMatriculas *n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->altura;  // Corrigido para retornar o valor da altura
    }
}



//FATOR BALANCEAMENTO
// Fun��o para calcular o fator de balanceamento de um n� Matricula
int fbMatricula(ArvMatriculas *no) {
    if (no == NULL){
		return 0;
	}else{
		return alturaMatricula(no->esq) - alturaMatricula(no->dir);
	}
}


//ROTA��O A ESQUERDA
// Fun��o para rotacionar � esquerda
ArvMatriculas *rotacaoEsqMatricula(ArvMatriculas *no) {
    ArvMatriculas *novaRaiz = no->dir;
    no->dir = novaRaiz->esq;
    novaRaiz->esq = no;

    no->altura = maior(alturaMatricula(no->esq), alturaMatricula(no->dir)) + 1;
    novaRaiz->altura = maior(alturaMatricula(novaRaiz->esq), alturaMatricula(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTA��O A DIREITA
// Fun��o para rotacionar � direita
ArvMatriculas *rotacaoDirMatricula(ArvMatriculas *no) {
    ArvMatriculas *novaRaiz = no->esq;
    no->esq = novaRaiz->dir;
    novaRaiz->dir = no;

    no->altura = maior(alturaMatricula(no->esq), alturaMatricula(no->dir)) + 1;
    novaRaiz->altura = maior(alturaMatricula(novaRaiz->esq), alturaMatricula(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTA��ES DUPLAS
//Fun��o Rota��o dupla esquerda-direita
ArvMatriculas *rotacaoEsqDirMatricula(ArvMatriculas *no) {
    no->esq = rotacaoEsqMatricula(no->esq);
    return rotacaoDirMatricula(no);
}
//Fun��o Rota��o dupla direita-esquerda
ArvMatriculas *rotacaoDirEsqMatricula(ArvMatriculas *no) {
    no->dir = rotacaoDirMatricula(no->dir);
    return rotacaoEsqMatricula(no);
}


//BALANCEAMENTO
// Fun��o de balanceamento da �rvore de cursos
ArvMatriculas *balanceamentoArvMatricula(ArvMatriculas *raiz, int codigoDisciplina) {
    // Verifica o balanceamento do n� atual
    int balanceamento = fbMatricula(raiz);

    // Caso 1 - Desbalanceamento � esquerda
    if (balanceamento > 1 && codigoDisciplina < raiz->esq->codigoDisciplina) {
        return rotacaoDirMatricula(raiz);
    }

    // Caso 2 - Desbalanceamento � direita
    if (balanceamento < -1 && codigoDisciplina > raiz->dir->codigoDisciplina) {
        return rotacaoEsqMatricula(raiz);
    }

    // Caso 3 - Desbalanceamento esquerda-direita
    if (balanceamento > 1 && codigoDisciplina > raiz->esq->codigoDisciplina) {
        return rotacaoEsqDirMatricula(raiz);
    }

    // Caso 4 - Desbalanceamento direita-esquerda
    if (balanceamento < -1 && codigoDisciplina < raiz->dir->codigoDisciplina) {
        return rotacaoDirEsqMatricula(raiz);
    }

    return raiz;  // Retorna a raiz se estiver balanceada
}


// Fun��o para criar uma nova matr�cula
ArvMatriculas *criarMatricula(int matricula, int codigoDisciplina) {
    ArvMatriculas *novaMatricula = (ArvMatriculas *) malloc(sizeof(ArvMatriculas));
    if (novaMatricula != NULL) {
    	novaMatricula->matricula = matricula;
        novaMatricula->codigoDisciplina = codigoDisciplina;
        novaMatricula->esq = NULL;
		novaMatricula->dir = NULL;
		novaMatricula->altura = 0;
    }else {
        printf("Erro ao alocar memoria para uma nova matricula.\n");
    }
    return novaMatricula;
}

//INSER��O
//Fun��o de Inser��o Balanceada
ArvMatriculas *inserirMatricula(ArvMatriculas *raiz, int matricula, int codigoDisciplina) {
    if (raiz == NULL) {
        return criarMatricula(matricula, codigoDisciplina);
    }

    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = inserirMatricula(raiz->esq, matricula, codigoDisciplina);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = inserirMatricula(raiz->dir, matricula, codigoDisciplina);
    } else {
        return raiz;  // C�digos iguais n�o s�o permitidos
    }

    raiz->altura = 1 + maior(alturaMatricula(raiz->esq), alturaMatricula(raiz->dir));
    return balanceamentoArvMatricula(raiz, codigoDisciplina); // Corrigido para passar 'codigoDisciplina'
}

// Fun��o para remover uma matr�cula
ArvMatriculas* removerMatricula(ArvMatriculas *raiz, int matricula, int codigoDisciplina) {
    if (raiz == NULL) {
        return raiz; // Se a �rvore est� vazia
    }
    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = removerMatricula(raiz->esq, matricula, codigoDisciplina);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = removerMatricula(raiz->dir, matricula, codigoDisciplina);
    } else {
    	if (raiz->matricula == matricula) {
        // Caso 1: N� com apenas um filho ou sem filho
	        if (raiz->esq == NULL) {
	            ArvMatriculas *temp = raiz->dir;
	            free(raiz);
	            return temp; // Retorna o filho que n�o � NULL
	        } else if (raiz->dir == NULL) {
	            ArvMatriculas *temp = raiz->esq;
	            free(raiz);
	            return temp; // Retorna o filho que n�o � NULL
	        }
	        // Caso 2: N� com dois filhos
	        ArvMatriculas *temp = maiorInfoMatricula(raiz->esq);
	        raiz->codigoDisciplina = temp->codigoDisciplina;
	        raiz->matricula = temp->matricula;
	        raiz->esq = removerMatricula(raiz->esq, temp->matricula, temp->codigoDisciplina);
    	}else{
			// Se a matr�cula n�o corresponde, n�o removemos
            printf("Matr�cula %d n�o encontrada para a disciplina %d.\n", matricula, codigoDisciplina);
		}
	}
    raiz->altura = 1 + maior(alturaMatricula(raiz->esq), alturaMatricula(raiz->dir));
    return balanceamentoArvMatricula(raiz, raiz->codigoDisciplina); // Corrigido para usar o c�digo do n� atual
}


/*
// Fun��o auxiliar recursiva para buscar a matr�cula e retornar o ponteiro para a estrutura da matr�cula
ArvMatriculas* buscarMatriculaPorReferencia(ArvMatriculas *raiz, int codigoDisciplina) {
    ArvMatriculas *resultado = NULL;

    if (raiz != NULL) {
        if (raiz->codigoDisciplina == codigoDisciplina) {
            resultado = raiz; // Matr�cula encontrada
        } else if (codigoDisciplina < raiz->codigoDisciplina) {
            resultado = buscarMatriculaPorReferencia(raiz->esq, codigoDisciplina); // Percorre a sub�rvore esquerda
        } else {
            resultado = buscarMatriculaPorReferencia(raiz->dir, codigoDisciplina); // Percorre a sub�rvore direita
        }
    }

    return resultado; // Retorna o ponteiro da matr�cula encontrada ou NULL se n�o encontrado
}
*/
ArvMatriculas *buscarMatriculaPorReferencia(ArvMatriculas *raiz, int codigoDisciplina) {
    if (raiz == NULL) {
        return NULL; // A matr�cula n�o foi encontrada
    }

    if (raiz->codigoDisciplina == codigoDisciplina) {
        return raiz; // Matr�cula encontrada
    } else if (codigoDisciplina < raiz->codigoDisciplina) {
        return buscarMatriculaPorReferencia(raiz->esq, codigoDisciplina); // Busca na sub�rvore esquerda
    } else {
        return buscarMatriculaPorReferencia(raiz->dir, codigoDisciplina); // Busca na sub�rvore direita
    }
}



// Fun��o auxiliar recursiva para buscar a matr�cula e retornar o ponteiro para a estrutura da matr�cula
Alunos* buscarAlunoPorMatricula(Alunos *lista, int matricula) {
    Alunos *atual = lista;
    while (atual != NULL) {
        if (atual->matricula == matricula) {
            return atual; // Aluno encontrado
        }
        atual = atual->prox; // Avan�a para o pr�ximo aluno
    }
    return NULL; // Aluno n�o encontrado
}



// Fun��o para imprimir as matr�culas em ordem
void imprimirMatricula(ArvMatriculas *raiz) {
    if (raiz != NULL) {
        imprimirMatricula(raiz->esq); // Visita a sub�rvore esquerda
        printf("Codigo da Matricula: %d\n", raiz->codigoDisciplina); // Imprime o c�digo da matr�cula
        imprimirMatricula(raiz->dir); // Visita a sub�rvore direita
    }
}

void imprimirMatriculas(ArvMatriculas *raiz) {
    if (raiz != NULL) {
        // Percorre a sub�rvore esquerda
        imprimirMatriculas(raiz->esq);

        // Imprime o c�digo da disciplina e a matr�cula
        printf("Codigo da disciplina: %d, Matricula: %d\n", raiz->codigoDisciplina, raiz->matricula);

        // Percorre a sub�rvore direita
        imprimirMatriculas(raiz->dir);
    }
}




void imprimirArvMatriculas(ArvMatriculas *raiz) {
    if (raiz != NULL) {
        imprimirArvMatriculas(raiz->esq);
        printf("Codigo Disciplina: %d, Altura: %d\n", raiz->codigoDisciplina, raiz->altura);
        imprimirArvMatriculas(raiz->dir);
    }
}


// Fun��o para imprimir a �rvore de matr�culas
void imprimirMatriculaNivel(ArvMatriculas *raiz, int nivel) {
    if (raiz != NULL) {
        // Imprime a sub�rvore direita
        imprimirMatriculaNivel(raiz->dir, nivel + 1);
        
        // Indenta para mostrar a hierarquia
        int i;
        for (i = 0; i < nivel; i++) {
            printf("\t");
        }
        
        // Imprime o c�digo da disciplina e a altura do n�
        printf("C�digo da Disciplina: %d (Altura: %d)\n", 
               raiz->codigoDisciplina, 
               raiz->altura);
        
        // Imprime a sub�rvore esquerda
        imprimirMatriculaNivel(raiz->esq, nivel + 1);
    }
}


// Fun��o para imprimir os detalhes de uma matr�cula
void imprimirDadosMatricula(ArvMatriculas *raizMatriculas, ArvDisciplinas *raizDisciplinas, int codigoMatricula) {
    ArvMatriculas *matricula = buscarMatriculaPorReferencia(raizMatriculas, codigoMatricula);
    
    if (matricula != NULL) {
        // Busca a disciplina associada � matr�cula usando o c�digo da disciplina armazenado na matr�cula
        ArvDisciplinas *disciplina = buscarDisciplina(raizDisciplinas, matricula->codigoDisciplina);

        printf("Matricula encontrada:\n");
        printf("Codigo da Matr�cula: %d\n", matricula->codigoDisciplina);
        
        if (disciplina != NULL) {
            printf("Nome da Disciplina: %s\n", disciplina->nomeDisciplina);
            printf("Periodo: %d\n", disciplina->periodo);
            printf("Carga Hor�ria: %d\n", disciplina->cargaHoraria);
        } else {
            printf("Disciplina com codigo %d nao encontrada.\n", matricula->codigoDisciplina);
        }
    } else {
        printf("Matricula com codigo %d nao encontrada.\n", codigoMatricula);
    }
}
/*
// Fun��o auxiliar para imprimir os dados das disciplinas com base nas matr�culas
void imprimirDadosMatriculas(ArvMatriculas *raizMatriculas, ArvDisciplinas *raizDisciplinas) {
    if (raizMatriculas != NULL) {
        // Imprimir dados da matr�cula � esquerda
        imprimirDadosMatriculas(raizMatriculas->esq, raizDisciplinas);

        // Imprimir dados da matr�cula atual
        imprimirDadosMatricula(raizMatriculas, raizDisciplinas, raizMatriculas->codigoDisciplina);

        // Imprimir dados da matr�cula � direita
        imprimirDadosMatriculas(raizMatriculas->dir, raizDisciplinas);
    }
}
*/





                                                                     // BLOCO �RVORE DE NOTAS

//MAIOR INFORMACAO
// Fun��o auxiliar para encontrar o n� com o maior n� em uma sub�rvore de Matricula
ArvNotas* maiorInfoNota(ArvNotas *no) {
    while (no != NULL && no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

// Fun��o auxiliar para obter a altura de um n� em uma �rvore AVL
int alturaNota(ArvNotas *n) {
    if (n == NULL){
		return -1;
	} else{
		return n->altura;
	}
}

//FATOR BALANCEAMENTO
// Fun��o para calcular o fator de balanceamento de um n� Matricula
int fbNota(ArvNotas *no) {
    if (no == NULL){
		return 0;
	}else{
		return alturaNota(no->esq) - alturaNota(no->dir);
	}
}

//ROTA��O A ESQUERDA
// Fun��o para rotacionar � esquerda
ArvNotas *rotacaoEsqNota(ArvNotas *no) {
    ArvNotas *novaRaiz = no->dir;
    no->dir = novaRaiz->esq;
    novaRaiz->esq = no;

    no->altura = maior(alturaNota(no->esq), alturaNota(no->dir)) + 1;
    novaRaiz->altura = maior(alturaNota(novaRaiz->esq), alturaNota(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTA��O A DIREITA
// Fun��o para rotacionar � direita
ArvNotas *rotacaoDirNota(ArvNotas *no) {
    ArvNotas *novaRaiz = no->esq;
    no->esq = novaRaiz->dir;
    novaRaiz->dir = no;

    no->altura = maior(alturaNota(no->esq), alturaNota(no->dir)) + 1;
    novaRaiz->altura = maior(alturaNota(novaRaiz->esq), alturaNota(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTA��ES DUPLAS
//Fun��o Rota��o dupla esquerda-direita
ArvNotas *rotacaoEsqDirNota(ArvNotas *no) {
    no->esq = rotacaoEsqNota(no->esq);
    return rotacaoDirNota(no);
}
//Fun��o Rota��o dupla direita-esquerda
ArvNotas *rotacaoDirEsqNota(ArvNotas *no) {
    no->dir = rotacaoDirNota(no->dir);
    return rotacaoEsqNota(no);
}


//BALANCEAMENTO
// Fun��o de balanceamento da �rvore de cursos
ArvNotas *balanceamentoArvNota(ArvNotas *raiz, int codigoDisciplina) {
    // Verifica o balanceamento do n� atual
    int balanceamento = fbNota(raiz);

    // Caso 1 - Desbalanceamento � esquerda
    if (balanceamento > 1 && codigoDisciplina < raiz->esq->codigoDisciplina) {
        return rotacaoDirNota(raiz);
    }

    // Caso 2 - Desbalanceamento � direita
    if (balanceamento < -1 && codigoDisciplina > raiz->dir->codigoDisciplina) {
        return rotacaoEsqNota(raiz);
    }

    // Caso 3 - Desbalanceamento esquerda-direita
    if (balanceamento > 1 && codigoDisciplina > raiz->esq->codigoDisciplina) {
        return rotacaoEsqDirNota(raiz);
    }

    // Caso 4 - Desbalanceamento direita-esquerda
    if (balanceamento < -1 && codigoDisciplina < raiz->dir->codigoDisciplina) {
        return rotacaoDirEsqNota(raiz);
    }

    return raiz;  // Retorna a raiz se estiver balanceada
}


// Fun��o para criar uma nova nota
ArvNotas *criarNota(int codigoDisciplina, int semestre, float notaFinal) {
    ArvNotas *novaNota = (ArvNotas *) malloc(sizeof(ArvNotas));
    if (novaNota != NULL) {
        novaNota->codigoDisciplina = codigoDisciplina;
        novaNota->semestre = semestre;
        novaNota->notaFinal = notaFinal;
        novaNota->esq = NULL;
		novaNota->dir = NULL;
		novaNota->altura = 0;
    }else {
        printf("Erro ao alocar memoria para uma nova nota.\n");
    }
    return novaNota;
}

//INSER��O
//Fun��o de Inser��o Balanceada
ArvNotas *inserirNota(ArvNotas *raiz, int codigoDisciplina, int semestre, float notaFinal) {
    if (raiz == NULL) {
        return criarNota(codigoDisciplina, semestre, notaFinal);
    }

    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = inserirNota(raiz->esq, codigoDisciplina, semestre, notaFinal);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = inserirNota(raiz->dir, codigoDisciplina, semestre, notaFinal);
    } else {
        return raiz;
    }

    raiz->altura = 1 + maior(alturaNota(raiz->esq), alturaNota(raiz->dir));
    return balanceamentoArvNota(raiz, codigoDisciplina);
}

//REMO��O
//Fun��o para remover nota
ArvNotas *removerNota(ArvNotas *raiz, int codigoDisciplina, int semestre) {
    if (raiz == NULL) {
        printf("Nota nao encontrada!\n");
        return NULL;
    }

    if (codigoDisciplina < raiz->codigoDisciplina || 
        (codigoDisciplina == raiz->codigoDisciplina && semestre < raiz->semestre)) {
        raiz->esq = removerNota(raiz->esq, codigoDisciplina, semestre);
    } else if (codigoDisciplina > raiz->codigoDisciplina || 
               (codigoDisciplina == raiz->codigoDisciplina && semestre > raiz->semestre)) {
        raiz->dir = removerNota(raiz->dir, codigoDisciplina, semestre);
    } else {
        if (raiz->esq == NULL && raiz->dir == NULL) {
            free(raiz);
            printf("Nota removida: Disciplina %d, Semestre %d!\n", codigoDisciplina, semestre);
            return NULL;
        } else if (raiz->esq != NULL && raiz->dir != NULL) {
            ArvNotas *aux = maiorInfoNota(raiz->esq);
            raiz->codigoDisciplina = aux->codigoDisciplina;
            raiz->semestre = aux->semestre;
            raiz->notaFinal = aux->notaFinal;
            raiz->esq = removerNota(raiz->esq, aux->codigoDisciplina, aux->semestre);
        } else {
            ArvNotas *aux = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            free(raiz);
            printf("Nota removida: Disciplina %d, Semestre %d!\n", codigoDisciplina, semestre);
            return aux;
        }
    }

    raiz->altura = 1 + maior(alturaNota(raiz->esq), alturaNota(raiz->dir));
    return balanceamentoArvNota(raiz, codigoDisciplina);
}

//BUSCAR
//fun��o para buscar uma nota
ArvNotas *buscarNota(ArvNotas *raiz, int codigoDisciplina, int semestre) {
    if (raiz == NULL) {
        return NULL; // Nota n�o encontrada
    }

    if (codigoDisciplina < raiz->codigoDisciplina || 
        (codigoDisciplina == raiz->codigoDisciplina && semestre < raiz->semestre)) {
        return buscarNota(raiz->esq, codigoDisciplina, semestre);
    } else if (codigoDisciplina > raiz->codigoDisciplina || 
               (codigoDisciplina == raiz->codigoDisciplina && semestre > raiz->semestre)) {
        return buscarNota(raiz->dir, codigoDisciplina, semestre);
    } else {
        return raiz; // Nota encontrada
    }
}

//IMPRIMIR
//Fun��o para imprimir arvore de notas
void imprimirNota(ArvNotas *raiz) {
    if (raiz != NULL) {
        imprimirNota(raiz->esq); // Imprime a sub�rvore esquerda
        printf("Disciplina: %d, Semestre: %d, Nota: %.2f\n", raiz->codigoDisciplina, raiz->semestre, raiz->notaFinal);
        imprimirNota(raiz->dir); // Imprime a sub�rvore direita
    }
}


                                                             //FUN��ES ESPECIFICAS PARA O MENU DA MAIN
/*
i) Cadastrar alunos a qualquer momento na lista, de forma que s� possa cadastrar um c�digo de curso que j� tenha sido cadastrado 
na �rvore de cursos. 
*/
// Fun��o para cadastrar aluno a qualquer momento
void cadastrarAluno(ArvCursos *raiz, Alunos **listaAlunos, int matricula, char nomeAluno[], int codigoCurso) {
    // Verifica se o curso existe na �rvore
    if (buscarCursoPorReferencia(raiz, codigoCurso)) {
        // Se o curso foi encontrado, cria o novo aluno
        Alunos *novoAluno = criarAluno(matricula, nomeAluno, codigoCurso);
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
ii) Cadastrar cursos a qualquer momento na �rvore de curso, de forma que o usu�rio n�o precise cadastrar as disciplinas 
para permitir o cadastro do curso.
*/
// Fun��o para cadastrar um curso na �rvore de cursos
void cadastrarCurso(ArvCursos **raiz) {
    int codigoCurso, qtdPeriodos;
    char nomeCurso[100];

    // Leitura dos dados do curso
    printf("Digite o codigo do curso: ");
    scanf("%d", &codigoCurso);
    printf("Digite o nome do curso: ");
    scanf(" %[^\n]s", nomeCurso);  // Leitura de string com espa�os
    printf("Digite a quantidade de periodos: ");
    scanf("%d", &qtdPeriodos);

    // Verifica se o curso j� existe
    ArvCursos *cursoExistente = buscarCursoPorReferencia(*raiz, codigoCurso);
    if (cursoExistente != NULL) {
        printf("Curso com o codigo %d j� existe.\n", codigoCurso);
        return;
    }

    // Insere o curso na �rvore
    *raiz = inserirCurso(*raiz, codigoCurso, nomeCurso, qtdPeriodos);
    printf("Curso cadastrado com sucesso!\n");
}

/* 
iii) Cadastrar disciplinas a qualquer momento em uma �rvore de disciplinas de um determinado curso, ou seja, uma disciplina s� pode ser cadastrada 
se o curso j� estiver sido cadastrado, al�m disso, o per�odo da disciplina deve ser v�lido, ou seja, estar entre 1 e a quantidade m�xima de 
per�odos do curso. A carga hor�ria da disciplina deve ser m�ltiplo de 15, variando entre 30 e 90.
*/
int cadastrarDisciplina(ArvCursos *raiz, int codigoCurso, int codigoDisciplina, char nomeDisciplina[], int periodo, int cargaHoraria) {
    int resultado = 0;  // Inicialmente, assume que a opera��o falhou
    ArvCursos *curso = buscarCursoPorReferencia(raiz, codigoCurso);  // Localiza o curso correspondente

    if (curso) {  // Verifica se o curso foi encontrado
        if (buscarDisciplina(curso->disciplinas, codigoDisciplina) != NULL) {
            printf("Disciplina ja cadastrada no curso %s.\n", curso->nomeCurso);
        } else if (periodo < 1 || periodo > curso->qtdPeriodos) {  // Valida��o do per�odo
            printf("Per�odo invalido. O curso possui %d periodos.\n", curso->qtdPeriodos);
        } else if (cargaHoraria < 30 || cargaHoraria > 90 || cargaHoraria % 15 != 0) {  // Valida��o da carga hor�ria
            printf("Carga horaria invalida. Deve ser multiplo de 15 e entre 30 e 90.\n");
        } else {
            // Inserir a disciplina na �rvore de disciplinas do curso
            curso->disciplinas = inserirDisciplina(curso->disciplinas, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
            printf("Disciplina cadastrada com sucesso no curso %s.\n", curso->nomeCurso);
            resultado = 1;  // Disciplina cadastrada com sucesso
        }
    } else {
        printf("Curso nao encontrado.\n");
    }

    return resultado;  // Retorno �nico indicando o sucesso ou falha
}

/*
iv) Cadastrar uma matr�cula, onde a mesma � uma �rvore organizada e contendo somente um c�digo de uma disciplina do curso do aluno.
*/

// Fun��o para cadastrar uma matr�cula de um aluno em uma disciplina 
int cadastrarMatricula(Alunos *lista, int matricula, int codigoDisciplina) {
    int resultado = 0;  // 0 para falha, 1 para sucesso
    Alunos *aluno = lista;

    // Busca o aluno pela matr�cula na lista de alunos
    while (aluno != NULL && aluno->matricula != matricula) {
        aluno = aluno->prox;  // Continua na lista at� encontrar o aluno ou o final
    }

    if (aluno != NULL) {
        // Aluno encontrado, verifica se a matr�cula na disciplina j� existe
        if (buscarMatriculaPorReferencia(aluno->matriculas, codigoDisciplina) == NULL) {
            // Disciplina n�o est� cadastrada, realiza a inser��o
            aluno->matriculas = inserirMatricula(aluno->matriculas, matricula, codigoDisciplina);
            printf("Matricula cadastrada com sucesso para a disciplina %d.\n", codigoDisciplina);
            resultado = 1;  // Cadastro realizado com sucesso
        } else {
            printf("Matricula ja existe para a disciplina %d.\n", codigoDisciplina);
        }
    } else {
        // Aluno n�o encontrado
        printf("Aluno nao encontrado.\n");
    }

    return resultado;  // Retorna 1 para sucesso, 0 para falha
}

/*
v) Cadastrar Notas, permitir o cadastro de notas somente de disciplinas que estejam na �rvore de matricula, e quando a nota for cadastrada 
a disciplina deve ser removida da �rvore de matricula para �rvore de notas.
*/ 

int cadastrarNota(ArvMatriculas **matriculas, ArvNotas **notas, int matricula, int codigoDisciplina, int semestre, float notaFinal) {
    int resultado = 0;  // Vari�vel de controle para o resultado

    // Busca a disciplina na �rvore de matr�culas
    ArvMatriculas *disciplinaMatriculada = buscarMatriculaPorReferencia(*matriculas, codigoDisciplina);
    
    if (disciplinaMatriculada != NULL) {
        // A disciplina est� matriculada, ent�o cadastramos a nota
        if (buscarNota(*notas, codigoDisciplina, semestre) == NULL) {
            // Insere a nota se ainda n�o estiver cadastrada
            *notas = inserirNota(*notas, codigoDisciplina, semestre, notaFinal);

            // Remove a disciplina da �rvore de matr�culas ap�s cadastrar a nota
            *matriculas = removerMatricula(*matriculas, matricula, codigoDisciplina);
            printf("Nota cadastrada com sucesso para a disciplina %d.\n", codigoDisciplina);
            resultado = 1;  // Sucesso
        } else {
            printf("Erro ao cadastrar a nota. Disciplina %d ja possui nota cadastrada para o semestre %d.\n", codigoDisciplina, semestre);
        }
    } else {
        // A disciplina n�o est� na �rvore de matr�culas
        printf("Disciplina %d nao possui matricula.\n", codigoDisciplina);
    }

    return resultado;  // Retorna 1 se bem-sucedido, 0 caso contr�rio
}

//vi) Mostrar todos os alunos de um determinado curso.
void mostrarAlunosCurso(Alunos *listaAlunos, int codigoCurso) {
    Alunos *atual = listaAlunos;
    int encontrouAluno = 0;  // Flag para verificar se algum aluno foi encontrado

    printf("Alunos matriculados no curso %d:\n", codigoCurso);

    // Percorre a lista de alunos
    while (atual != NULL) {
        // Verifica se o aluno est� matriculado no curso fornecido
        if (atual->codigoCurso == codigoCurso) {
            printf("Nome: %s, Matricula: %d\n", atual->nomeAluno, atual->matricula);
            encontrouAluno = 1;
        }
        // Avan�a para o pr�ximo aluno da lista
        atual = atual->prox;
    }

    // Se nenhum aluno foi encontrado, exibe uma mensagem
    if (!encontrouAluno) {
        printf("Nenhum aluno encontrado para o curso %d.\n", codigoCurso);
    }
}

//vii)Mostrar todos os cursos do Campus.
void mostrarCursos(ArvCursos *raiz) {
    if (raiz != NULL) {
        // Percorre a sub�rvore esquerda
        mostrarCursos(raiz->esq);

        // Exibe o curso atual
        printf("Codigo do Curso: %d\n", raiz->codigoCurso);
        printf("Nome do Curso: %s\n", raiz->nomeCurso);
        printf("Quantidade de Periodos: %d\n\n", raiz->qtdPeriodos);

        // Percorre a sub�rvore direita
        mostrarCursos(raiz->dir);
    }
}
 
//viii)Mostrar todas as disciplinas de um determinado curso.
// Fun��o para mostrar todas as disciplinas de um curso
void mostrarDisciplinasCurso(ArvCursos *raiz, int codigoCurso) {
    ArvCursos *curso = buscarCursoPorReferencia(raiz, codigoCurso);  // Fun��o que retorna o curso pelo c�digo
    if (curso != NULL) {
        if (curso->disciplinas != NULL) {
            printf("Disciplinas do curso %s:\n\n", curso->nomeCurso);
            imprimirArvDisciplinas(curso->disciplinas);  // Exibe todas as disciplinas do curso
        } else {
            printf("O curso %s nao possui disciplinas cadastradas.\n", curso->nomeCurso);
        }
    } else {
        printf("Curso nao encontrado.\n");
    }
}
 
//ix) Mostrar todas as disciplinas de um determinado per�odo de um curso.
// Fun��o recursiva para imprimir disciplinas de um determinado per�odo
void mostrarDisciplinasPorPeriodo(ArvDisciplinas *raiz, int periodo) {
    if (raiz != NULL) {
        // Percorrer a sub�rvore esquerda
        mostrarDisciplinasPorPeriodo(raiz->esq, periodo);

        // Verificar se a disciplina est� no per�odo desejado
        if (raiz->periodo == periodo) {
            printf("Codigo: %d, Nome: %s, Carga Horaria: %d\n",
                   raiz->codigoDisciplina,
                   raiz->nomeDisciplina,
                   raiz->cargaHoraria);
        }

        // Percorrer a sub�rvore direita
        mostrarDisciplinasPorPeriodo(raiz->dir, periodo);
    }
}

// Fun��o para mostrar todas as disciplinas de um determinado per�odo de um curso
void disciplinasPeriodo(ArvCursos *raiz, int codigoCurso, int periodo) {
    // Vari�vel para armazenar o curso encontrado
    ArvCursos *curso = buscarCursoPorReferencia(raiz, codigoCurso);  // Fun��o para buscar o curso

    // Verificar se o curso foi encontrado
    if (curso == NULL) {
        printf("Curso nao encontrado.\n");
    } else {
        // Verificar se o per�odo � v�lido
        if (periodo < 1 || periodo > curso->qtdPeriodos) {
            printf("Periodo invalido. O curso possui %d periodos.\n", curso->qtdPeriodos);
        } else {
            // Mostrar disciplinas do per�odo
            printf("Disciplinas do periodo %d do curso %s:\n", periodo, curso->nomeCurso);
            mostrarDisciplinasPorPeriodo(curso->disciplinas, periodo);
        }
    }
}

//x) Mostrar todas as disciplinas que um determinado aluno est� matriculado.
// Fun��o para mostrar todas as disciplinas nas quais um aluno est� matriculado
void mostrarDisciplinasAluno(struct Alunos *lista, struct ArvCursos *raiz, int matricula) {
    struct Alunos *aluno = lista;

    // Procura o aluno na lista
    while (aluno != NULL) {
        if (aluno->matricula == matricula) {
            printf("O aluno %s esta matriculado nas seguintes disciplinas:\n", aluno->nomeAluno);

            // Verifica se o aluno tem alguma matr�cula
            if (aluno->matriculas == NULL) {
                printf("Nenhuma matricula encontrada. (A arvore de matriculas esta vazia)\n");
            } else {
                printf("Matriculas encontradas!\n Imprimindo disciplinas:\n");  // Para depura��o
                imprimirMatriculas(aluno->matriculas);  // Chama a fun��o para imprimir as matr�culas
            }
            return;  // Sai ap�s encontrar o aluno e exibir as disciplinas
        }
        aluno = aluno->prox;  // Avan�a para o pr�ximo aluno
    }

    // Se o aluno n�o for encontrado
    printf("Aluno nao encontrado.\n");
}

//xi) Mostrar todas as notas de disciplinas de um determinado per�odo de um determinado aluno. 
// Fun��o auxiliar para exibir notas de um per�odo espec�fico em uma �rvore de notas
void exibirNotasPeriodo(ArvNotas *notas, int periodo) {
    if (notas != NULL) {
        // Percorre a sub�rvore esquerda
        exibirNotasPeriodo(notas->esq, periodo);
        
        // Exibe a nota se o semestre (per�odo) for o especificado
        if (notas->semestre == periodo) {
            printf("Codigo da Disciplina: %d\n", notas->codigoDisciplina);
            printf("Nota Final: %.2f\n", notas->notaFinal);
            printf("Periodo: %d\n\n", notas->semestre);
        }

        // Percorre a sub�rvore direita
        exibirNotasPeriodo(notas->dir, periodo);
    }
}
// Fun��o principal para mostrar as notas de um aluno em um per�odo espec�fico
void mostrarNotasAlunoPeriodo(Alunos *listaAlunos, int matricula, int periodo) {
    Alunos *aluno = buscarAluno(listaAlunos, matricula);  // Busca o aluno pela matr�cula

    if (aluno != NULL) {
        printf("Notas do Aluno %d - %s no Periodo %d:\n", matricula, aluno->nomeAluno, periodo);
        // Chama a fun��o auxiliar para exibir as notas no per�odo especificado
        exibirNotasPeriodo(aluno->notas, periodo);
    } else {
        printf("Aluno com matricula %d nao encontrado.\n", matricula);
    }
}
//xii) Mostrar a nota de uma disciplina de um determinado aluno, mostrando o per�odo e a carga hor�ria da disciplina.
// Fun��o principal para mostrar a nota de uma disciplina de um aluno, com per�odo e carga hor�ria
void mostrarNotaDisciplinaAluno(Alunos *listaAlunos, int matricula, int codigoDisciplina, int semestre, ArvDisciplinas *disciplinas) {
    // Buscar o aluno na lista de alunos usando a fun��o j� implementada
    Alunos *aluno = buscarAluno(listaAlunos, matricula);

    if (aluno != NULL) {
        // Buscar a nota da disciplina na �rvore de notas do aluno
        ArvNotas *nota = buscarNota(aluno->notas, codigoDisciplina, semestre);

        if (nota != NULL) {
            // Buscar a disciplina para obter o per�odo e a carga hor�ria
            ArvDisciplinas *disciplina = buscarDisciplina(disciplinas, codigoDisciplina);

            if (disciplina != NULL) {
                // Exibir as informa��es da nota, per�odo e carga hor�ria
                printf("Aluno: %s (Matricula: %d)\n", aluno->nomeAluno, aluno->matricula);
                printf("Disciplina: %s (Codigo: %d)\n", disciplina->nomeDisciplina, disciplina->codigoDisciplina);
                printf("Nota Final: %.2f\n", nota->notaFinal);
                printf("Periodo: %d\n", disciplina->periodo);
                printf("Carga Horaria: %d\n", disciplina->cargaHoraria);
            } else {
                printf("Disciplina com codigo %d nao encontrada.\n", codigoDisciplina);
            }
        } else {
            printf("Nota nao encontrada para a disciplina %d no semestre %d.\n", codigoDisciplina, semestre);
        }
    } else {
        printf("Aluno com matricula %d nao encontrado.\n", matricula);
    }
}

//xiii)Remover uma disciplina de um determinado curso desde que n�o tenha nenhum aluno matriculado na mesma.
int removerDisciplinaCurso(ArvCursos **raiz, int codigoCurso, int codigoDisciplina, Alunos *listaAlunos) {
    int resultado = 0;  // Vari�vel para armazenar o resultado final

    // Verifica se o curso existe
    ArvCursos *curso = buscarCursoPorReferencia(*raiz, codigoCurso);
    if (curso == NULL) {
        printf("Curso com codigo %d nao encontrado.\n", codigoCurso);
    } else {
        // Verifica se a disciplina existe no curso
        ArvDisciplinas *disciplina = buscarDisciplina(curso->disciplinas, codigoDisciplina);
        if (disciplina == NULL) {
            printf("Disciplina com codigo %d nao encontrada no curso %d.\n", codigoDisciplina, codigoCurso);
        } else {
            // Verifica se h� algum aluno matriculado na disciplina
            Alunos *atual = listaAlunos;
            int alunosMatriculados = 0;

            // Percorre a lista de alunos e suas matr�culas
            while (atual != NULL && alunosMatriculados == 0) {
                ArvMatriculas *matricula = atual->matriculas;

                // Verifica as disciplinas nas matr�culas do aluno
                while (matricula != NULL) {
                    if (matricula->codigoDisciplina == codigoDisciplina) {
                        alunosMatriculados = 1;  // Aluno encontrado matriculado na disciplina
                        break;
                    }
                    matricula = matricula->dir;
                }
                atual = atual->prox;
            }

            // Se houver alunos matriculados, n�o pode remover
            if (alunosMatriculados) {
                printf("Nao e possivel remover a disciplina, pois ha alunos matriculados.\n");
            } else {
                // Nenhum aluno matriculado, pode remover a disciplina
                curso->disciplinas = removerDisciplina(curso->disciplinas, codigoDisciplina);
                printf("Disciplina com codigo %d removida com sucesso do curso %d.\n", codigoDisciplina, codigoCurso);
                resultado = 1;  // Sucesso na remo��o
            }
        }
    }

    return resultado;  // Retorna 1 se a remo��o for bem-sucedida, ou 0 se houver algum problema
}

//xiv)Permita remover uma disciplina da �rvore de matr�cula de um determinado aluno.
int removerDisciplinaMatricula(Alunos *listaAlunos, int matricula, int codigoDisciplina) {
    Alunos *aluno = buscarAluno(listaAlunos, matricula);
    int sucesso = 0;

    if (aluno != NULL) {
        // Remover a disciplina da �rvore de matr�culas
        aluno->matriculas = removerMatricula(aluno->matriculas, matricula, codigoDisciplina);

        // Verificar se foi removida
        if (buscarMatriculaPorReferencia(aluno->matriculas, codigoDisciplina) == NULL) {
            printf("Disciplina com c�digo %d removida com sucesso da matr�cula do aluno %d.\n", codigoDisciplina, matricula);
            sucesso = 1;
        } else {
            printf("Falha ao remover a disciplina com c�digo %d do aluno %d.\n", codigoDisciplina, matricula);
        }
    } else {
        printf("Aluno com matr�cula %d n�o encontrado.\n", matricula);
    }

    return sucesso;
}

/*
xv) Mostrar o hist�rico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva nota organizadas pelo per�odo 
que a disciplina est� cadastrada no curso. 
*/
// Fun��o auxiliar para percorrer a �rvore de matr�culas em ordem, sem utilizar return
void percorrerArvMatriculas(ArvMatriculas *raiz, Alunos *aluno, ArvCursos *curso) {
    if (raiz != NULL) {
        // Visita a sub�rvore esquerda
        percorrerArvMatriculas(raiz->esq, aluno, curso);

        // Busca a disciplina correspondente � matr�cula na �rvore de disciplinas do curso
        ArvDisciplinas *disciplina = buscarDisciplina(curso->disciplinas, raiz->codigoDisciplina);
        if (disciplina != NULL) {
            // Busca a nota do aluno para a disciplina
            ArvNotas *nota = buscarNota(aluno->notas, disciplina->codigoDisciplina, 0); // 0 como semestre se n�o for relevante
            if (nota != NULL) {
                printf("Periodo: %d | Disciplina: %s | Nota: %.2f | Carga Horaria: %d horas\n",
                       disciplina->periodo, disciplina->nomeDisciplina, nota->notaFinal, disciplina->cargaHoraria);
            } else {
                printf("Periodo: %d | Disciplina: %s | Nota nao encontrada\n",
                       disciplina->periodo, disciplina->nomeDisciplina);
            }
        } else {
            printf("Disciplina com codigo %d nao encontrada.\n", raiz->codigoDisciplina);
        }

        // Visita a sub�rvore direita
        percorrerArvMatriculas(raiz->dir, aluno, curso);
    }
}


// Fun��o principal para mostrar o hist�rico de um aluno
void mostrarHistorico(Alunos *listaAlunos, ArvCursos *raiz, int matricula) {
    Alunos *aluno = buscarAluno(listaAlunos, matricula);

    if (aluno != NULL) {
        // Busca o curso do aluno
        ArvCursos *curso = buscarCursoPorReferencia(raiz, aluno->codigoCurso);
        if (curso != NULL) {
            printf("Historico do aluno: %s\n", aluno->nomeAluno);
            printf("Curso: %s\n", curso->nomeCurso);
            printf("Disciplinas cursadas:\n");

            // Percorre a �rvore de matr�culas para exibir as disciplinas e notas
            percorrerArvMatriculas(aluno->matriculas, aluno, curso);
        } else {
            printf("Curso nao encontrado para o aluno.\n");
        }
    } else {
        printf("Aluno com matr�cula %d nao encontrado.\n", matricula);
    }
}

// xvi) Fun��es de liberar �rvores e sair do programa

//Fun��o p/ liberar �rvore de deisciplina
void liberarDisciplinas(ArvDisciplinas *raiz) {
    if (raiz != NULL) {
        liberarDisciplinas(raiz->esq);  // Libera a sub�rvore esquerda
        liberarDisciplinas(raiz->dir);  // Libera a sub�rvore direita
        free(raiz);  // Libera o n� atual
    }
}
//Fun��o p/ liberar arvore de cursos
void liberarCursos(ArvCursos *raiz) {
    if (raiz != NULL) {
        liberarDisciplinas(raiz->disciplinas);  // Libera todas as disciplinas do curso
        liberarCursos(raiz->esq);  // Libera a sub�rvore esquerda
        liberarCursos(raiz->dir);  // Libera a sub�rvore direita
        free(raiz);  // Libera o n� atual
    }
}

//Fun��o p/ liberar matriculas
void liberarMatriculas(ArvMatriculas *raiz) {
    if (raiz != NULL) {
        liberarMatriculas(raiz->esq);  // Libera a sub�rvore esquerda
        liberarMatriculas(raiz->dir);  // Libera a sub�rvore direita
        free(raiz);  // Libera o n� atual
    }
}

//Fun��o p/ liberar �rvore de notas
void liberarNotas(ArvNotas *raiz) {
    if (raiz != NULL) {
        liberarNotas(raiz->esq);  // Libera a sub�rvore esquerda
        liberarNotas(raiz->dir);  // Libera a sub�rvore direita
        free(raiz);  // Libera o n� atual
    }
}

//Fun��o p/ liberar lista de alunos
void liberarAlunos(Alunos *lista) {
    struct Alunos *atual = lista;
    while (atual != NULL) {
        struct Alunos *prox = atual->prox;
        liberarMatriculas(atual->matriculas);  // Libera as matr�culas do aluno
        liberarNotas(atual->notas);  // Libera as notas do aluno
        free(atual);  // Libera o aluno atual
        atual = prox;
    }
}

//Fun��o p/ liberar todas as arvores
void liberarArvores(ArvCursos *raizCursos, Alunos *listaAlunos) {
    liberarCursos(raizCursos);  // Libera todos os cursos e suas disciplinas
    liberarAlunos(listaAlunos);  // Libera todos os alunos, suas matr�culas e notas
}


//Fun��o principal
                                                                       

int main() {
    ArvCursos *raiz = NULL;  // �rvore de cursos
    Alunos *lista = NULL;    // Lista de alunos
    int opcao, codigoCurso, codigoDisciplina, matricula, periodo, semestre, cargaHoraria, resultado, i;
    char nomeAluno[100], nomeDisciplina[100];
    float notaFinal;
    Alunos *aluno;
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
                	cadastrarCurso(&raiz);
                	// Marca o tempo final
			        gettimeofday(&fim, NULL);
			        // Calcula o tempo decorrido em microssegundos
			        tempo_decorrido = (double)(fim.tv_sec - inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - inicio.tv_usec);
			        
			        // Armazena o tempo decorrido
			        tempDecorrido[i] = tempo_decorrido;
			        
			        // Soma os tempos para calcular a m�dia posteriormente
			        soma += tempo_decorrido;
				}
				// Calcula a m�dia dos tempos decorridos
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
                cadastrarDisciplina(raiz, codigoCurso, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
                break;
            
            case 3:  // Cadastrar aluno
                printf("Digite a matricula: ");
                scanf("%d", &matricula);
                printf("Digite o nome do aluno: ");
                scanf(" %[^\n]s", nomeAluno);  // Leitura de string com espa�os
                printf("Digite o codigo do curso: ");
                scanf("%d", &codigoCurso);
                cadastrarAluno(raiz, &lista, matricula, nomeAluno, codigoCurso);
                break;

            case 4:  // Cadastrar matricula
                printf("Digite a matricula do aluno: ");
                scanf("%d", &matricula);
                printf("Digite o codigo da disciplina: ");
                scanf("%d", &codigoDisciplina);
                cadastrarMatricula(lista, matricula, codigoDisciplina);
                break;

            case 5:  // Cadastrar nota
                printf("Digite a matricula do aluno: ");
                scanf("%d", &matricula);
                aluno = buscarAluno(lista, matricula);
                if (aluno != NULL) {
                    printf("Digite o codigo da disciplina: ");
                    scanf("%d", &codigoDisciplina);
                    printf("Digite o semestre: ");
                    scanf("%d", &semestre);
                    printf("Digite a nota final: ");
                    scanf("%f", &notaFinal);
                    cadastrarNota(&aluno->matriculas, &aluno->notas, matricula, codigoDisciplina, semestre, notaFinal);
                } else {
                    printf("Aluno nao encontrado.\n");
                }
                break;

            case 6:  // Mostrar alunos de um curso
                printf("Digite o codigo do curso para listar os alunos matriculados: ");
                scanf("%d", &codigoCurso);
                // Chama a fun��o para mostrar os alunos do curso fornecido
                mostrarAlunosCurso(lista, codigoCurso);
                break;
            
            case 7:   // Mostrar todos os cursos
			    printf("Listando todos os cursos do campus:\n\n");
			    // Chama a fun��o para mostrar os cursos da �rvore
			    mostrarCursos(raiz);
			    break;
			
			case 8:  // Mostrar todas as disciplinas de um curso
			    printf("Digite o codigo do curso: ");
			    scanf("%d", &codigoCurso);
			    mostrarDisciplinasCurso(raiz, codigoCurso);
			    break;
			
			case 9: // mostrar as disciplinas do per�odo
			    printf("Digite o codigo do curso: ");
			    scanf("%d", &codigoCurso);
			    printf("Digite o periodo desejado: ");
			    scanf("%d", &periodo);
			    disciplinasPeriodo(raiz, codigoCurso, periodo);
			    break;
			
			case 10: // mostrar todas as disciplinas de um aluno
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    mostrarDisciplinasAluno(lista, raiz, matricula);
			    break;


			    
			
			case 11: // mostrar as notas do aluno no per�odo
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    printf("Digite o periodo desejado: ");
			    scanf("%d", &periodo);
			    mostrarNotasAlunoPeriodo(lista, matricula, periodo);
			    break;
			
			case 12: // Mostrar a nota da disciplina do aluno, per�odo e carga hor�ria
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    printf("Digite o codigo da disciplina: ");
			    scanf("%d", &codigoDisciplina);
			    printf("Digite o semestre: ");
			    scanf("%d", &semestre);
			    printf("Digite o codigo do curso: ");
			    scanf("%d", &codigoCurso);
			
			    // Buscar o curso
			    ArvCursos *curso = buscarCursoPorReferencia(raiz, codigoCurso);
			
			    if (curso != NULL) {
			        for (i = 0; i < 30; i++) {
			            // Marca o tempo inicial
			            gettimeofday(&inicio, NULL);
			
			            // Chama a fun��o corrigida com o argumento correto
			            mostrarNotaDisciplinaAluno(lista, matricula, codigoDisciplina, semestre, curso->disciplinas);
			
			            // Marca o tempo final
			            gettimeofday(&fim, NULL);
			
			            // Calcula o tempo decorrido em microssegundos
			            tempo_decorrido = (double)(fim.tv_sec - inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - inicio.tv_usec);
			
			            // Armazena o tempo decorrido
			            tempDecorrido[i] = tempo_decorrido;
			
			            // Soma os tempos para calcular a m�dia posteriormente
			            soma += tempo_decorrido;
			        }
			        // Calcula a m�dia dos tempos decorridos
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
			    resultado = removerDisciplinaCurso(&raiz, codigoCurso, codigoDisciplina, lista);
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
			    resultado = removerDisciplinaMatricula(lista, matricula, codigoDisciplina);
			    if (resultado) {
			        printf("Remocao realizada com sucesso!\n");
			    } else {
			        printf("Falha ao remover a disciplina.\n");
			    }
			    break;
			
			case 15: //mostrar historico
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    mostrarHistorico(lista, raiz, matricula);
			    break;

            case 16:  // Sair
			    liberarArvores(raiz, lista);  // Libera toda a mem�ria alocada
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




