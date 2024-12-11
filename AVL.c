/*                                                                       ÁRVORES AVL
Faça um programa em C que cadastre dados sobre alunos e seu histórico dentro de um determinado curso. Para isto, 
é preciso ter as seguintes estruturas de dados: 
(a) Lista de Alunos: Matricula do aluno, Nome do aluno, Código do Curso, árvore de Notas, árvore de Matrícula, 
    a lista deve ser organizada pela ordem de alfabética por nome do aluno. 
(b) Árvore de Curso: Código do curso, Nome do curso, quantidade de Períodos e uma árvore de Disciplinas. 
    A árvore deve ser organizada pelo nome do Código do Curso. 
(c) Árvore de disciplinas: código da disciplina, nome da disciplina, período, carga horária. 
    A árvore deve ser organizada pelo código da disciplina. 
(d) Árvore de Notas: código da disciplina, semestre cursado, nota final obtida pelo aluno. 
    A árvore deve ser organizada pelo código da disciplina. 
(e) Árvore de Matricula: código da disciplina, organizada pelo código da disciplina 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// Estrutura da árvore de Cursos
typedef struct ArvCursos {
    int codigoCurso;
    char nomeCurso[100];
    int qtdPeriodos;
    struct ArvDisciplinas *disciplinas;
    struct ArvCursos *esq, *dir;         
    int altura;
} ArvCursos;

// Estrutura da árvore de Disciplinas
typedef struct ArvDisciplinas {
    int codigoDisciplina;
    char nomeDisciplina[100];
    int periodo;
    int cargaHoraria;
    struct ArvDisciplinas *esq, *dir;
    int altura;
} ArvDisciplinas;

// Estrutura da árvore de Notas (AVL)
typedef struct ArvNotas {
    int codigoDisciplina;
    int semestre;
    float notaFinal;
    struct ArvNotas *esq, *dir;
    int altura;
} ArvNotas;

// Estrutura da árvore de Matrícula
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
    ArvNotas *notas;           // Árvore de Notas do Aluno
    ArvMatriculas *matriculas;  // Árvore de Matrículas do Aluno
    struct Alunos *prox;        // Ponteiro para o próximo aluno na lista
} Alunos;



                                                                     //BLOCO LISTA DE ALUNOS
// Função para criar um novo aluno
Alunos *criarAluno(int matricula, const char *nomeAluno, int codigoCurso) {
    Alunos *novoAluno = (Alunos *)malloc(sizeof(Alunos));
    if (novoAluno != NULL) {
        novoAluno->matricula = matricula;
        strcpy(novoAluno->nomeAluno, nomeAluno);
        novoAluno->codigoCurso = codigoCurso;
        novoAluno->notas = NULL;       // Inicializa árvores de notas e matrículas como vazias
        novoAluno->matriculas = NULL;
        novoAluno->prox = NULL;        // Próximo aluno é NULL
    } else {
        printf("Erro ao alocar memoria para novo aluno.\n");
    }
    return novoAluno;
}

// Função para inserir aluno em ordem alfabética
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

// Função para remover um aluno da lista de alunos
int removerAluno(Alunos **lista, int matricula) {
    Alunos *atual = *lista;
    Alunos *anterior = NULL;

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

// Função para buscar um aluno na lista de alunos
Alunos* buscarAluno(Alunos *lista, int matricula) {
    Alunos *atual = lista;

    while (atual != NULL) {
        if (atual->matricula == matricula) {
            return atual;  // Retorna o ponteiro para o aluno encontrado
        }
        atual = atual->prox;  // Move para o próximo aluno
    }

    return NULL;  // Retorna NULL se o aluno não for encontrado
}


// Função para imprimir a lista de alunos
void imprimirListaAlunos(Alunos *lista) {
    Alunos *atual = lista;
    while (atual != NULL) {
        printf("Alunos: %s, Matricula: %d, Curso: %d\n", atual->nomeAluno, atual->matricula, atual->codigoCurso);
        atual = atual->prox;
    }
}


                                                                    //BLOCO ÁRVORE DE CURSO


/*MENOR INFORMACAO
// Função auxiliar para encontrar o nó com o menor nome do curso em uma subárvore
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
// Função auxiliar para encontrar o nó com o maior nome do curso em uma subárvore
ArvCursos* maiorInfoCurso(ArvCursos *no) {
    while (no != NULL && no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

//CALCULAR ALTURA
//Função para calcular a altura de um nó Curso
int alturaCurso(ArvCursos *n) {
    return (n == NULL) ? -1 : n->altura;
}


//FATOR BALANCEAMENTO
// Função para calcular o fator de balanceamento de um nó Curso
int fbCurso(ArvCursos *no) {
    if (no == NULL){
		return 0;
	}else{
		return alturaCurso(no->esq) - alturaCurso(no->dir);
	}
}

//ROTAÇÃO A ESQUERDA
// Função para rotacionar à esquerda
ArvCursos *rotacaoEsqCurso(ArvCursos *no) {
    ArvCursos *novaRaiz = no->dir;
    no->dir = novaRaiz->esq;
    novaRaiz->esq = no;

    // Atualiza as alturas usando max para encontrar o maior valor entre as alturas das subárvores
    no->altura = maior(alturaCurso(no->esq), alturaCurso(no->dir)) + 1;
    novaRaiz->altura = maior(alturaCurso(novaRaiz->esq), alturaCurso(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTAÇÃO A DIREITA
// Função para rotacionar à direita
ArvCursos *rotacaoDirCurso(ArvCursos *no) {
    ArvCursos *novaRaiz = no->esq;  // O nó que se tornará a nova raiz
    no->esq = novaRaiz->dir;  // O filho à direita da nova raiz se torna o filho à esquerda do nó original
    novaRaiz->dir = no;  // O nó original se torna o filho à direita da nova raiz

    // Atualizando as alturas
    no->altura = maior(alturaCurso(no->esq), alturaCurso(no->dir)) + 1;  // Atualiza a altura do nó original
    novaRaiz->altura = maior(alturaCurso(novaRaiz->esq), alturaCurso(novaRaiz->dir)) + 1;  // Atualiza a altura da nova raiz

    return novaRaiz;  // Retorna a nova raiz
}


//ROTAÇÕES DUPLAS
//Função Rotação dupla esquerda-direita
ArvCursos *rotacaoEsqDirCurso(ArvCursos *no) {
    no->esq = rotacaoEsqCurso(no->esq);
    return rotacaoDirCurso(no);
}
//Função Rotação dupla direita-esquerda
ArvCursos *rotacaoDirEsqCurso(ArvCursos *no) {
    no->dir = rotacaoDirCurso(no->dir);
    return rotacaoEsqCurso(no);
}

//BALANCEAMENTO
// Função de balanceamento da árvore de cursos
ArvCursos *balanceamentoArvCurso(ArvCursos *raiz, int codigoCurso) {
    // Verifica o balanceamento do nó atual
    int balanceamento = fbCurso(raiz);

    // Caso 1 - Desbalanceamento à esquerda
    if (balanceamento > 1 && codigoCurso < raiz->esq->codigoCurso) {
        return rotacaoDirCurso(raiz);
    }

    // Caso 2 - Desbalanceamento à direita
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
// Função para criar um novo curso
ArvCursos *criarCurso(int codigoCurso, const char *nomeCurso, int qtdPeriodos) {
    ArvCursos *novoCurso = (ArvCursos *)malloc(sizeof(ArvCursos));
    if (novoCurso != NULL) {
        novoCurso->codigoCurso = codigoCurso;
        strcpy(novoCurso->nomeCurso, nomeCurso);
        novoCurso->qtdPeriodos = qtdPeriodos;
        novoCurso->disciplinas = NULL;  // A árvore de disciplinas começa vazia
        novoCurso->esq = NULL;
        novoCurso->dir = NULL;
        novoCurso->altura = 0;          // Altura inicial da AVL é 0
    } else {
        printf("Erro ao alocar memoria para novo curso.\n");
    }
    return novoCurso;
}

//INSERÇÃO
//Função de Inserção Balanceada
// Função de Inserção Balanceada
ArvCursos *inserirCurso(ArvCursos *raiz, int codigoCurso, const char *nomeCurso, int qtdPeriodos) {
    if (raiz == NULL) {
        // Se a raiz for NULL, cria um novo curso
        return criarCurso(codigoCurso, nomeCurso, qtdPeriodos);
    }

    // Inserção normal de um nó na árvore
    if (codigoCurso < raiz->codigoCurso) {
        raiz->esq = inserirCurso(raiz->esq, codigoCurso, nomeCurso, qtdPeriodos);
    } else if (codigoCurso > raiz->codigoCurso) {
        raiz->dir = inserirCurso(raiz->dir, codigoCurso, nomeCurso, qtdPeriodos);
    } else {
        // Cursos com códigos iguais não são permitidos, retorna a própria raiz
        return raiz;
    }

    // Atualiza a altura do nó atual após a inserção
    raiz->altura = 1 + maior(alturaCurso(raiz->esq), alturaCurso(raiz->dir));

    // Chama a função de balanceamento para corrigir a árvore, se necessário
    return balanceamentoArvCurso(raiz, codigoCurso);  // Passa o codigoCurso
}


// Função para remover um curso da árvore AVL de cursos
ArvCursos* removerCurso(ArvCursos *raiz, int codigoCurso) {
    if (raiz == NULL) {
        printf("Curso não encontrado!\n");
        return NULL;
    }

    if (codigoCurso < raiz->codigoCurso) {
        raiz->esq = removerCurso(raiz->esq, codigoCurso);
    } else if (codigoCurso > raiz->codigoCurso) {
        raiz->dir = removerCurso(raiz->dir, codigoCurso);
    } else {
        if (raiz->esq == NULL && raiz->dir == NULL) { // Nó folha
            free(raiz);
            printf("Curso folha removido: %d!\n", codigoCurso);
            return NULL;
        } else if (raiz->esq != NULL && raiz->dir != NULL) { // Nó com dois filhos
            ArvCursos *aux = maiorInfoCurso(raiz->esq);
            raiz->codigoCurso = aux->codigoCurso;
            strcpy(raiz->nomeCurso, aux->nomeCurso);
            raiz->qtdPeriodos = aux->qtdPeriodos;
            raiz->disciplinas = aux->disciplinas;
            raiz->esq = removerCurso(raiz->esq, aux->codigoCurso);
        } else { // Nó com um filho
            ArvCursos *aux = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            free(raiz);
            printf("Curso com 1 filho removido: %d!\n", codigoCurso);
            return aux;
        }
    }

    raiz->altura = maior(alturaCurso(raiz->esq), alturaCurso(raiz->dir)) + 1;
    return balanceamentoArvCurso(raiz, codigoCurso);
}

// Função auxiliar recursiva para buscar o curso e retornar o ponteiro para a estrutura do curso
ArvCursos* buscarCursoPorReferencia(ArvCursos *raiz, int codigoCurso) {
    ArvCursos *resultado = NULL;

    if (raiz != NULL) {
        if (raiz->codigoCurso == codigoCurso) {
            resultado = raiz; // Curso encontrado
        } else if (codigoCurso < raiz->codigoCurso) {
            resultado = buscarCursoPorReferencia(raiz->esq, codigoCurso); // Percorre a subárvore esquerda
        } else {
            resultado = buscarCursoPorReferencia(raiz->dir, codigoCurso); // Percorre a subárvore direita
        }
    }

    return resultado; // Retorna o ponteiro do curso encontrado ou NULL se não encontrado
}

// Função para imprimir a árvore de cursos
void imprimirCursoNivel(ArvCursos *Raiz, int nivel) {
	int i;
    if (Raiz != NULL) {
        // Imprime a subárvore direita
        imprimirCursoNivel(Raiz->dir, nivel + 1);
        
        // Indenta para mostrar a hierarquia
        for (i = 0; i < nivel; i++) {
            printf("\t");
        }
        
        // Imprime o código do curso, o nome do curso, a quantidade de períodos e a altura do nó
        printf("Código: %d - Nome: %s - Períodos: %d (Altura: %d)\n", 
               Raiz->codigoCurso, 
               Raiz->nomeCurso, 
               Raiz->qtdPeriodos, 
               Raiz->altura);
        
        // Imprime a subárvore esquerda
        imprimirCursoNivel(Raiz->esq, nivel + 1);
    }
}


                                                                      //BLOCO ÁRVORE DE DISCIPLINAS



//MAIOR INFORMACAO
// Função auxiliar para encontrar o nó com o maior nó em uma subárvore de disciplina
ArvDisciplinas* maiorInfoDisciplina(ArvDisciplinas *no) {
    while (no != NULL && no->dir != NULL) {
        no = no->dir;
    }
    return no;
}


//CALCULAR ALTURA
//Função para calcular a altura de um nó Disciplina																  																  //Função para calcular a altura de um nó Disciplina
int alturaDisciplina(ArvDisciplinas *n) {
    if (n == NULL) {
        return -1;
    }
    return n->altura; // Corrigido para retornar a altura
}

//FATOR BALANCEAMENTO
// Função para calcular o fator de balanceamento de um nó Disciplina
int fbDisciplina(ArvDisciplinas *no) {
    if (no == NULL){
		return 0;
	}else{
		return alturaDisciplina(no->esq) - alturaDisciplina(no->dir);
	}
}

//ROTAÇÃO A ESQUERDA
// Função para rotacionar à esquerda
ArvDisciplinas *rotacaoEsqDisciplina(ArvDisciplinas *no) {
    ArvDisciplinas *novaRaiz = no->dir;
    no->dir = novaRaiz->esq;
    novaRaiz->esq = no;

    no->altura = maior(alturaDisciplina(no->esq), alturaDisciplina(no->dir)) + 1;
    novaRaiz->altura = maior(alturaDisciplina(novaRaiz->esq), alturaDisciplina(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTAÇÃO A DIREITA
// Função para rotacionar à direita
ArvDisciplinas *rotacaoDirDisciplina(ArvDisciplinas *no) {
    ArvDisciplinas *novaRaiz = no->esq;
    no->esq = novaRaiz->dir;
    novaRaiz->dir = no;

    no->altura = maior(alturaDisciplina(no->esq), alturaDisciplina(no->dir)) + 1;
    novaRaiz->altura = maior(alturaDisciplina(novaRaiz->esq), alturaDisciplina(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTAÇÕES DUPLAS
//Função Rotação dupla esquerda-direita
ArvDisciplinas *rotacaoEsqDirDisciplina(ArvDisciplinas *no) {
    no->esq = rotacaoEsqDisciplina(no->esq);
    return rotacaoDirDisciplina(no);
}
//Função Rotação dupla direita-esquerda
ArvDisciplinas *rotacaoDirEsqDisciplina(ArvDisciplinas *no) {
    no->dir = rotacaoDirDisciplina(no->dir);
    return rotacaoEsqDisciplina(no);
}


//BALANCEAMENTO
// Função de balanceamento da árvore de cursos
ArvDisciplinas *balanceamentoArvDisciplina(ArvDisciplinas *raiz, int codigoDisciplina) {
    // Verifica o balanceamento do nó atual
    int balanceamento = fbDisciplina(raiz);

    // Caso 1 - Desbalanceamento à esquerda
    if (balanceamento > 1 && codigoDisciplina < raiz->esq->codigoDisciplina) {
        return rotacaoDirDisciplina(raiz);
    }

    // Caso 2 - Desbalanceamento à direita
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


// Função para criar uma nova disciplina																  																  																  // Função para criar uma nova disciplina
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

//INSERÇÃO
//Função de Inserção Balanceada
ArvDisciplinas *inserirDisciplina(ArvDisciplinas *raiz, int codigoDisciplina, char nomeDisciplina[], int periodo, int cargaHoraria) {
    if (raiz == NULL) {
        return criarDisciplina(codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
    }

    // Inserção normal de um nó na árvore
    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = inserirDisciplina(raiz->esq, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = inserirDisciplina(raiz->dir, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
    } else {
        return raiz; // Disciplinas com códigos iguais não são permitidos
    }

    // Atualiza a altura do nó atual após a inserção
    raiz->altura = 1 + maior(alturaDisciplina(raiz->esq), alturaDisciplina(raiz->dir));

    // Chama a função de balanceamento para corrigir a árvore, se necessário
    return balanceamentoArvDisciplina(raiz, codigoDisciplina);
}

// Função para remover uma disciplina da árvore de disciplinas
ArvDisciplinas *removerDisciplina(ArvDisciplinas *raiz, int codigoDisciplina) {
    if (raiz == NULL) {
        printf("Disciplina com codigo %d nao encontrada.\n", codigoDisciplina);
        return raiz; // Disciplina não encontrada
    }

    // Navega na árvore para encontrar a disciplina
    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = removerDisciplina(raiz->esq, codigoDisciplina);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = removerDisciplina(raiz->dir, codigoDisciplina);
    } else {
        // Disciplina encontrada
        if (raiz->esq == NULL || raiz->dir == NULL) {
            // Caso 1: O nó tem 0 ou 1 filho
            ArvDisciplinas *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                // Caso 2: O nó é uma folha
                free(raiz);
                return NULL;
            } else {
                // Caso 1: O nó tem um filho
                ArvDisciplinas *aux = temp;
                free(raiz);
                return aux;
            }
        } else {
            // Caso 3: O nó tem dois filhos
            ArvDisciplinas *temp = maiorInfoDisciplina(raiz->esq); // Encontrar o maior na subárvore esquerda
            raiz->codigoDisciplina = temp->codigoDisciplina; // Substitui o código da disciplina
            strcpy(raiz->nomeDisciplina, temp->nomeDisciplina);
            raiz->periodo = temp->periodo;
            raiz->cargaHoraria = temp->cargaHoraria;

            // Remove o nó duplicado
            raiz->esq = removerDisciplina(raiz->esq, temp->codigoDisciplina);
        }
    }

    // Atualiza a altura do nó atual após a remoção
    raiz->altura = 1 + maior(alturaDisciplina(raiz->esq), alturaDisciplina(raiz->dir));

    // Chama a função de balanceamento para corrigir a árvore, se necessário
    return balanceamentoArvDisciplina(raiz, raiz->codigoDisciplina);
}

// Função para buscar uma disciplina na árvore de disciplinas
/*ArvDisciplinas *buscarDisciplina(ArvDisciplinas *raiz, int codigoDisciplina) {
    if (raiz == NULL) {
        return NULL;  // Disciplina não encontrada
    }

    if (codigoDisciplina == raiz->codigoDisciplina) {
        return raiz;  // Disciplina encontrada
    } else if (codigoDisciplina < raiz->codigoDisciplina) {
        return buscarDisciplina(raiz->esq, codigoDisciplina);  // Busca à esquerda
    } else {
        return buscarDisciplina(raiz->dir, codigoDisciplina);  // Busca à direita
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
    if (raiz == NULL) return NULL; // Se a árvore estiver vazia

    if (raiz->codigoDisciplina == codigoDisciplina) {
        return raiz; // Disciplina encontrada
    } else if (codigoDisciplina < raiz->codigoDisciplina) {
        return buscarDisciplinaPorCodigo(raiz->esq, codigoDisciplina); // Percorre a subárvore esquerda
    } else {
        return buscarDisciplinaPorCodigo(raiz->dir, codigoDisciplina); // Percorre a subárvore direita
    }
}




// Função para imprimir os detalhes de uma disciplina
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
        printf("Código: %d, Nome: %s, Carga Horária: %d\n", raiz->codigoDisciplina, raiz->nomeDisciplina, raiz->cargaHoraria);
        imprimirDisciplinas(raiz->dir);
    }
}

void imprimirArvDisciplinas(ArvDisciplinas *raiz) {
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        imprimirArvDisciplinas(raiz->esq);

        // Exibe a disciplina atual
        printf("Codigo da Disciplina: %d\n", raiz->codigoDisciplina);
        printf("Nome da Disciplina: %s\n", raiz->nomeDisciplina);
        printf("Periodo: %d\n", raiz->periodo);
        printf("Carga Horaria: %d\n\n", raiz->cargaHoraria);

        // Percorre a subárvore direita
        imprimirArvDisciplinas(raiz->dir);
    }
}



                                                                      // BLOCO ÁRVORE DE MATRICULAS


//MAIOR INFORMACAO
// Função auxiliar para encontrar o nó com o maior nó em uma subárvore de Matricula
ArvMatriculas* maiorInfoMatricula(ArvMatriculas *no) {
    while (no != NULL && no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

//Função para calcular a altura de um nó Matricula
int alturaMatricula(ArvMatriculas *n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->altura;  // Corrigido para retornar o valor da altura
    }
}



//FATOR BALANCEAMENTO
// Função para calcular o fator de balanceamento de um nó Matricula
int fbMatricula(ArvMatriculas *no) {
    if (no == NULL){
		return 0;
	}else{
		return alturaMatricula(no->esq) - alturaMatricula(no->dir);
	}
}


//ROTAÇÃO A ESQUERDA
// Função para rotacionar à esquerda
ArvMatriculas *rotacaoEsqMatricula(ArvMatriculas *no) {
    ArvMatriculas *novaRaiz = no->dir;
    no->dir = novaRaiz->esq;
    novaRaiz->esq = no;

    no->altura = maior(alturaMatricula(no->esq), alturaMatricula(no->dir)) + 1;
    novaRaiz->altura = maior(alturaMatricula(novaRaiz->esq), alturaMatricula(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTAÇÃO A DIREITA
// Função para rotacionar à direita
ArvMatriculas *rotacaoDirMatricula(ArvMatriculas *no) {
    ArvMatriculas *novaRaiz = no->esq;
    no->esq = novaRaiz->dir;
    novaRaiz->dir = no;

    no->altura = maior(alturaMatricula(no->esq), alturaMatricula(no->dir)) + 1;
    novaRaiz->altura = maior(alturaMatricula(novaRaiz->esq), alturaMatricula(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTAÇÕES DUPLAS
//Função Rotação dupla esquerda-direita
ArvMatriculas *rotacaoEsqDirMatricula(ArvMatriculas *no) {
    no->esq = rotacaoEsqMatricula(no->esq);
    return rotacaoDirMatricula(no);
}
//Função Rotação dupla direita-esquerda
ArvMatriculas *rotacaoDirEsqMatricula(ArvMatriculas *no) {
    no->dir = rotacaoDirMatricula(no->dir);
    return rotacaoEsqMatricula(no);
}


//BALANCEAMENTO
// Função de balanceamento da árvore de cursos
ArvMatriculas *balanceamentoArvMatricula(ArvMatriculas *raiz, int codigoDisciplina) {
    // Verifica o balanceamento do nó atual
    int balanceamento = fbMatricula(raiz);

    // Caso 1 - Desbalanceamento à esquerda
    if (balanceamento > 1 && codigoDisciplina < raiz->esq->codigoDisciplina) {
        return rotacaoDirMatricula(raiz);
    }

    // Caso 2 - Desbalanceamento à direita
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


// Função para criar uma nova matrícula
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

//INSERÇÃO
//Função de Inserção Balanceada
ArvMatriculas *inserirMatricula(ArvMatriculas *raiz, int matricula, int codigoDisciplina) {
    if (raiz == NULL) {
        return criarMatricula(matricula, codigoDisciplina);
    }

    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = inserirMatricula(raiz->esq, matricula, codigoDisciplina);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = inserirMatricula(raiz->dir, matricula, codigoDisciplina);
    } else {
        return raiz;  // Códigos iguais não são permitidos
    }

    raiz->altura = 1 + maior(alturaMatricula(raiz->esq), alturaMatricula(raiz->dir));
    return balanceamentoArvMatricula(raiz, codigoDisciplina); // Corrigido para passar 'codigoDisciplina'
}

// Função para remover uma matrícula
ArvMatriculas* removerMatricula(ArvMatriculas *raiz, int matricula, int codigoDisciplina) {
    if (raiz == NULL) {
        return raiz; // Se a árvore está vazia
    }
    if (codigoDisciplina < raiz->codigoDisciplina) {
        raiz->esq = removerMatricula(raiz->esq, matricula, codigoDisciplina);
    } else if (codigoDisciplina > raiz->codigoDisciplina) {
        raiz->dir = removerMatricula(raiz->dir, matricula, codigoDisciplina);
    } else {
    	if (raiz->matricula == matricula) {
        // Caso 1: Nó com apenas um filho ou sem filho
	        if (raiz->esq == NULL) {
	            ArvMatriculas *temp = raiz->dir;
	            free(raiz);
	            return temp; // Retorna o filho que não é NULL
	        } else if (raiz->dir == NULL) {
	            ArvMatriculas *temp = raiz->esq;
	            free(raiz);
	            return temp; // Retorna o filho que não é NULL
	        }
	        // Caso 2: Nó com dois filhos
	        ArvMatriculas *temp = maiorInfoMatricula(raiz->esq);
	        raiz->codigoDisciplina = temp->codigoDisciplina;
	        raiz->matricula = temp->matricula;
	        raiz->esq = removerMatricula(raiz->esq, temp->matricula, temp->codigoDisciplina);
    	}else{
			// Se a matrícula não corresponde, não removemos
            printf("Matrícula %d não encontrada para a disciplina %d.\n", matricula, codigoDisciplina);
		}
	}
    raiz->altura = 1 + maior(alturaMatricula(raiz->esq), alturaMatricula(raiz->dir));
    return balanceamentoArvMatricula(raiz, raiz->codigoDisciplina); // Corrigido para usar o código do nó atual
}


/*
// Função auxiliar recursiva para buscar a matrícula e retornar o ponteiro para a estrutura da matrícula
ArvMatriculas* buscarMatriculaPorReferencia(ArvMatriculas *raiz, int codigoDisciplina) {
    ArvMatriculas *resultado = NULL;

    if (raiz != NULL) {
        if (raiz->codigoDisciplina == codigoDisciplina) {
            resultado = raiz; // Matrícula encontrada
        } else if (codigoDisciplina < raiz->codigoDisciplina) {
            resultado = buscarMatriculaPorReferencia(raiz->esq, codigoDisciplina); // Percorre a subárvore esquerda
        } else {
            resultado = buscarMatriculaPorReferencia(raiz->dir, codigoDisciplina); // Percorre a subárvore direita
        }
    }

    return resultado; // Retorna o ponteiro da matrícula encontrada ou NULL se não encontrado
}
*/
ArvMatriculas *buscarMatriculaPorReferencia(ArvMatriculas *raiz, int codigoDisciplina) {
    if (raiz == NULL) {
        return NULL; // A matrícula não foi encontrada
    }

    if (raiz->codigoDisciplina == codigoDisciplina) {
        return raiz; // Matrícula encontrada
    } else if (codigoDisciplina < raiz->codigoDisciplina) {
        return buscarMatriculaPorReferencia(raiz->esq, codigoDisciplina); // Busca na subárvore esquerda
    } else {
        return buscarMatriculaPorReferencia(raiz->dir, codigoDisciplina); // Busca na subárvore direita
    }
}



// Função auxiliar recursiva para buscar a matrícula e retornar o ponteiro para a estrutura da matrícula
Alunos* buscarAlunoPorMatricula(Alunos *lista, int matricula) {
    Alunos *atual = lista;
    while (atual != NULL) {
        if (atual->matricula == matricula) {
            return atual; // Aluno encontrado
        }
        atual = atual->prox; // Avança para o próximo aluno
    }
    return NULL; // Aluno não encontrado
}



// Função para imprimir as matrículas em ordem
void imprimirMatricula(ArvMatriculas *raiz) {
    if (raiz != NULL) {
        imprimirMatricula(raiz->esq); // Visita a subárvore esquerda
        printf("Codigo da Matricula: %d\n", raiz->codigoDisciplina); // Imprime o código da matrícula
        imprimirMatricula(raiz->dir); // Visita a subárvore direita
    }
}

void imprimirMatriculas(ArvMatriculas *raiz) {
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        imprimirMatriculas(raiz->esq);

        // Imprime o código da disciplina e a matrícula
        printf("Codigo da disciplina: %d, Matricula: %d\n", raiz->codigoDisciplina, raiz->matricula);

        // Percorre a subárvore direita
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


// Função para imprimir a árvore de matrículas
void imprimirMatriculaNivel(ArvMatriculas *raiz, int nivel) {
    if (raiz != NULL) {
        // Imprime a subárvore direita
        imprimirMatriculaNivel(raiz->dir, nivel + 1);
        
        // Indenta para mostrar a hierarquia
        int i;
        for (i = 0; i < nivel; i++) {
            printf("\t");
        }
        
        // Imprime o código da disciplina e a altura do nó
        printf("Código da Disciplina: %d (Altura: %d)\n", 
               raiz->codigoDisciplina, 
               raiz->altura);
        
        // Imprime a subárvore esquerda
        imprimirMatriculaNivel(raiz->esq, nivel + 1);
    }
}


// Função para imprimir os detalhes de uma matrícula
void imprimirDadosMatricula(ArvMatriculas *raizMatriculas, ArvDisciplinas *raizDisciplinas, int codigoMatricula) {
    ArvMatriculas *matricula = buscarMatriculaPorReferencia(raizMatriculas, codigoMatricula);
    
    if (matricula != NULL) {
        // Busca a disciplina associada à matrícula usando o código da disciplina armazenado na matrícula
        ArvDisciplinas *disciplina = buscarDisciplina(raizDisciplinas, matricula->codigoDisciplina);

        printf("Matricula encontrada:\n");
        printf("Codigo da Matrícula: %d\n", matricula->codigoDisciplina);
        
        if (disciplina != NULL) {
            printf("Nome da Disciplina: %s\n", disciplina->nomeDisciplina);
            printf("Periodo: %d\n", disciplina->periodo);
            printf("Carga Horária: %d\n", disciplina->cargaHoraria);
        } else {
            printf("Disciplina com codigo %d nao encontrada.\n", matricula->codigoDisciplina);
        }
    } else {
        printf("Matricula com codigo %d nao encontrada.\n", codigoMatricula);
    }
}
/*
// Função auxiliar para imprimir os dados das disciplinas com base nas matrículas
void imprimirDadosMatriculas(ArvMatriculas *raizMatriculas, ArvDisciplinas *raizDisciplinas) {
    if (raizMatriculas != NULL) {
        // Imprimir dados da matrícula à esquerda
        imprimirDadosMatriculas(raizMatriculas->esq, raizDisciplinas);

        // Imprimir dados da matrícula atual
        imprimirDadosMatricula(raizMatriculas, raizDisciplinas, raizMatriculas->codigoDisciplina);

        // Imprimir dados da matrícula à direita
        imprimirDadosMatriculas(raizMatriculas->dir, raizDisciplinas);
    }
}
*/





                                                                     // BLOCO ÁRVORE DE NOTAS

//MAIOR INFORMACAO
// Função auxiliar para encontrar o nó com o maior nó em uma subárvore de Matricula
ArvNotas* maiorInfoNota(ArvNotas *no) {
    while (no != NULL && no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

// Função auxiliar para obter a altura de um nó em uma árvore AVL
int alturaNota(ArvNotas *n) {
    if (n == NULL){
		return -1;
	} else{
		return n->altura;
	}
}

//FATOR BALANCEAMENTO
// Função para calcular o fator de balanceamento de um nó Matricula
int fbNota(ArvNotas *no) {
    if (no == NULL){
		return 0;
	}else{
		return alturaNota(no->esq) - alturaNota(no->dir);
	}
}

//ROTAÇÃO A ESQUERDA
// Função para rotacionar à esquerda
ArvNotas *rotacaoEsqNota(ArvNotas *no) {
    ArvNotas *novaRaiz = no->dir;
    no->dir = novaRaiz->esq;
    novaRaiz->esq = no;

    no->altura = maior(alturaNota(no->esq), alturaNota(no->dir)) + 1;
    novaRaiz->altura = maior(alturaNota(novaRaiz->esq), alturaNota(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTAÇÃO A DIREITA
// Função para rotacionar à direita
ArvNotas *rotacaoDirNota(ArvNotas *no) {
    ArvNotas *novaRaiz = no->esq;
    no->esq = novaRaiz->dir;
    novaRaiz->dir = no;

    no->altura = maior(alturaNota(no->esq), alturaNota(no->dir)) + 1;
    novaRaiz->altura = maior(alturaNota(novaRaiz->esq), alturaNota(novaRaiz->dir)) + 1;

    return novaRaiz;
}

//ROTAÇÕES DUPLAS
//Função Rotação dupla esquerda-direita
ArvNotas *rotacaoEsqDirNota(ArvNotas *no) {
    no->esq = rotacaoEsqNota(no->esq);
    return rotacaoDirNota(no);
}
//Função Rotação dupla direita-esquerda
ArvNotas *rotacaoDirEsqNota(ArvNotas *no) {
    no->dir = rotacaoDirNota(no->dir);
    return rotacaoEsqNota(no);
}


//BALANCEAMENTO
// Função de balanceamento da árvore de cursos
ArvNotas *balanceamentoArvNota(ArvNotas *raiz, int codigoDisciplina) {
    // Verifica o balanceamento do nó atual
    int balanceamento = fbNota(raiz);

    // Caso 1 - Desbalanceamento à esquerda
    if (balanceamento > 1 && codigoDisciplina < raiz->esq->codigoDisciplina) {
        return rotacaoDirNota(raiz);
    }

    // Caso 2 - Desbalanceamento à direita
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


// Função para criar uma nova nota
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

//INSERÇÃO
//Função de Inserção Balanceada
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

//REMOÇÃO
//Função para remover nota
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
//função para buscar uma nota
ArvNotas *buscarNota(ArvNotas *raiz, int codigoDisciplina, int semestre) {
    if (raiz == NULL) {
        return NULL; // Nota não encontrada
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
//Função para imprimir arvore de notas
void imprimirNota(ArvNotas *raiz) {
    if (raiz != NULL) {
        imprimirNota(raiz->esq); // Imprime a subárvore esquerda
        printf("Disciplina: %d, Semestre: %d, Nota: %.2f\n", raiz->codigoDisciplina, raiz->semestre, raiz->notaFinal);
        imprimirNota(raiz->dir); // Imprime a subárvore direita
    }
}


                                                             //FUNÇÕES ESPECIFICAS PARA O MENU DA MAIN
/*
i) Cadastrar alunos a qualquer momento na lista, de forma que só possa cadastrar um código de curso que já tenha sido cadastrado 
na árvore de cursos. 
*/
// Função para cadastrar aluno a qualquer momento
void cadastrarAluno(ArvCursos *raiz, Alunos **listaAlunos, int matricula, char nomeAluno[], int codigoCurso) {
    // Verifica se o curso existe na árvore
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
ii) Cadastrar cursos a qualquer momento na árvore de curso, de forma que o usuário não precise cadastrar as disciplinas 
para permitir o cadastro do curso.
*/
// Função para cadastrar um curso na árvore de cursos
void cadastrarCurso(ArvCursos **raiz) {
    int codigoCurso, qtdPeriodos;
    char nomeCurso[100];

    // Leitura dos dados do curso
    printf("Digite o codigo do curso: ");
    scanf("%d", &codigoCurso);
    printf("Digite o nome do curso: ");
    scanf(" %[^\n]s", nomeCurso);  // Leitura de string com espaços
    printf("Digite a quantidade de periodos: ");
    scanf("%d", &qtdPeriodos);

    // Verifica se o curso já existe
    ArvCursos *cursoExistente = buscarCursoPorReferencia(*raiz, codigoCurso);
    if (cursoExistente != NULL) {
        printf("Curso com o codigo %d já existe.\n", codigoCurso);
        return;
    }

    // Insere o curso na árvore
    *raiz = inserirCurso(*raiz, codigoCurso, nomeCurso, qtdPeriodos);
    printf("Curso cadastrado com sucesso!\n");
}

/* 
iii) Cadastrar disciplinas a qualquer momento em uma árvore de disciplinas de um determinado curso, ou seja, uma disciplina só pode ser cadastrada 
se o curso já estiver sido cadastrado, além disso, o período da disciplina deve ser válido, ou seja, estar entre 1 e a quantidade máxima de 
períodos do curso. A carga horária da disciplina deve ser múltiplo de 15, variando entre 30 e 90.
*/
int cadastrarDisciplina(ArvCursos *raiz, int codigoCurso, int codigoDisciplina, char nomeDisciplina[], int periodo, int cargaHoraria) {
    int resultado = 0;  // Inicialmente, assume que a operação falhou
    ArvCursos *curso = buscarCursoPorReferencia(raiz, codigoCurso);  // Localiza o curso correspondente

    if (curso) {  // Verifica se o curso foi encontrado
        if (buscarDisciplina(curso->disciplinas, codigoDisciplina) != NULL) {
            printf("Disciplina ja cadastrada no curso %s.\n", curso->nomeCurso);
        } else if (periodo < 1 || periodo > curso->qtdPeriodos) {  // Validação do período
            printf("Período invalido. O curso possui %d periodos.\n", curso->qtdPeriodos);
        } else if (cargaHoraria < 30 || cargaHoraria > 90 || cargaHoraria % 15 != 0) {  // Validação da carga horária
            printf("Carga horaria invalida. Deve ser multiplo de 15 e entre 30 e 90.\n");
        } else {
            // Inserir a disciplina na árvore de disciplinas do curso
            curso->disciplinas = inserirDisciplina(curso->disciplinas, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
            printf("Disciplina cadastrada com sucesso no curso %s.\n", curso->nomeCurso);
            resultado = 1;  // Disciplina cadastrada com sucesso
        }
    } else {
        printf("Curso nao encontrado.\n");
    }

    return resultado;  // Retorno único indicando o sucesso ou falha
}

/*
iv) Cadastrar uma matrícula, onde a mesma é uma árvore organizada e contendo somente um código de uma disciplina do curso do aluno.
*/

// Função para cadastrar uma matrícula de um aluno em uma disciplina 
int cadastrarMatricula(Alunos *lista, int matricula, int codigoDisciplina) {
    int resultado = 0;  // 0 para falha, 1 para sucesso
    Alunos *aluno = lista;

    // Busca o aluno pela matrícula na lista de alunos
    while (aluno != NULL && aluno->matricula != matricula) {
        aluno = aluno->prox;  // Continua na lista até encontrar o aluno ou o final
    }

    if (aluno != NULL) {
        // Aluno encontrado, verifica se a matrícula na disciplina já existe
        if (buscarMatriculaPorReferencia(aluno->matriculas, codigoDisciplina) == NULL) {
            // Disciplina não está cadastrada, realiza a inserção
            aluno->matriculas = inserirMatricula(aluno->matriculas, matricula, codigoDisciplina);
            printf("Matricula cadastrada com sucesso para a disciplina %d.\n", codigoDisciplina);
            resultado = 1;  // Cadastro realizado com sucesso
        } else {
            printf("Matricula ja existe para a disciplina %d.\n", codigoDisciplina);
        }
    } else {
        // Aluno não encontrado
        printf("Aluno nao encontrado.\n");
    }

    return resultado;  // Retorna 1 para sucesso, 0 para falha
}

/*
v) Cadastrar Notas, permitir o cadastro de notas somente de disciplinas que estejam na árvore de matricula, e quando a nota for cadastrada 
a disciplina deve ser removida da árvore de matricula para árvore de notas.
*/ 

int cadastrarNota(ArvMatriculas **matriculas, ArvNotas **notas, int matricula, int codigoDisciplina, int semestre, float notaFinal) {
    int resultado = 0;  // Variável de controle para o resultado

    // Busca a disciplina na árvore de matrículas
    ArvMatriculas *disciplinaMatriculada = buscarMatriculaPorReferencia(*matriculas, codigoDisciplina);
    
    if (disciplinaMatriculada != NULL) {
        // A disciplina está matriculada, então cadastramos a nota
        if (buscarNota(*notas, codigoDisciplina, semestre) == NULL) {
            // Insere a nota se ainda não estiver cadastrada
            *notas = inserirNota(*notas, codigoDisciplina, semestre, notaFinal);

            // Remove a disciplina da árvore de matrículas após cadastrar a nota
            *matriculas = removerMatricula(*matriculas, matricula, codigoDisciplina);
            printf("Nota cadastrada com sucesso para a disciplina %d.\n", codigoDisciplina);
            resultado = 1;  // Sucesso
        } else {
            printf("Erro ao cadastrar a nota. Disciplina %d ja possui nota cadastrada para o semestre %d.\n", codigoDisciplina, semestre);
        }
    } else {
        // A disciplina não está na árvore de matrículas
        printf("Disciplina %d nao possui matricula.\n", codigoDisciplina);
    }

    return resultado;  // Retorna 1 se bem-sucedido, 0 caso contrário
}

//vi) Mostrar todos os alunos de um determinado curso.
void mostrarAlunosCurso(Alunos *listaAlunos, int codigoCurso) {
    Alunos *atual = listaAlunos;
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

//vii)Mostrar todos os cursos do Campus.
void mostrarCursos(ArvCursos *raiz) {
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
// Função para mostrar todas as disciplinas de um curso
void mostrarDisciplinasCurso(ArvCursos *raiz, int codigoCurso) {
    ArvCursos *curso = buscarCursoPorReferencia(raiz, codigoCurso);  // Função que retorna o curso pelo código
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
 
//ix) Mostrar todas as disciplinas de um determinado período de um curso.
// Função recursiva para imprimir disciplinas de um determinado período
void mostrarDisciplinasPorPeriodo(ArvDisciplinas *raiz, int periodo) {
    if (raiz != NULL) {
        // Percorrer a subárvore esquerda
        mostrarDisciplinasPorPeriodo(raiz->esq, periodo);

        // Verificar se a disciplina está no período desejado
        if (raiz->periodo == periodo) {
            printf("Codigo: %d, Nome: %s, Carga Horaria: %d\n",
                   raiz->codigoDisciplina,
                   raiz->nomeDisciplina,
                   raiz->cargaHoraria);
        }

        // Percorrer a subárvore direita
        mostrarDisciplinasPorPeriodo(raiz->dir, periodo);
    }
}

// Função para mostrar todas as disciplinas de um determinado período de um curso
void disciplinasPeriodo(ArvCursos *raiz, int codigoCurso, int periodo) {
    // Variável para armazenar o curso encontrado
    ArvCursos *curso = buscarCursoPorReferencia(raiz, codigoCurso);  // Função para buscar o curso

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

//x) Mostrar todas as disciplinas que um determinado aluno está matriculado.
// Função para mostrar todas as disciplinas nas quais um aluno está matriculado
void mostrarDisciplinasAluno(struct Alunos *lista, struct ArvCursos *raiz, int matricula) {
    struct Alunos *aluno = lista;

    // Procura o aluno na lista
    while (aluno != NULL) {
        if (aluno->matricula == matricula) {
            printf("O aluno %s esta matriculado nas seguintes disciplinas:\n", aluno->nomeAluno);

            // Verifica se o aluno tem alguma matrícula
            if (aluno->matriculas == NULL) {
                printf("Nenhuma matricula encontrada. (A arvore de matriculas esta vazia)\n");
            } else {
                printf("Matriculas encontradas!\n Imprimindo disciplinas:\n");  // Para depuração
                imprimirMatriculas(aluno->matriculas);  // Chama a função para imprimir as matrículas
            }
            return;  // Sai após encontrar o aluno e exibir as disciplinas
        }
        aluno = aluno->prox;  // Avança para o próximo aluno
    }

    // Se o aluno não for encontrado
    printf("Aluno nao encontrado.\n");
}

//xi) Mostrar todas as notas de disciplinas de um determinado período de um determinado aluno. 
// Função auxiliar para exibir notas de um período específico em uma árvore de notas
void exibirNotasPeriodo(ArvNotas *notas, int periodo) {
    if (notas != NULL) {
        // Percorre a subárvore esquerda
        exibirNotasPeriodo(notas->esq, periodo);
        
        // Exibe a nota se o semestre (período) for o especificado
        if (notas->semestre == periodo) {
            printf("Codigo da Disciplina: %d\n", notas->codigoDisciplina);
            printf("Nota Final: %.2f\n", notas->notaFinal);
            printf("Periodo: %d\n\n", notas->semestre);
        }

        // Percorre a subárvore direita
        exibirNotasPeriodo(notas->dir, periodo);
    }
}
// Função principal para mostrar as notas de um aluno em um período específico
void mostrarNotasAlunoPeriodo(Alunos *listaAlunos, int matricula, int periodo) {
    Alunos *aluno = buscarAluno(listaAlunos, matricula);  // Busca o aluno pela matrícula

    if (aluno != NULL) {
        printf("Notas do Aluno %d - %s no Periodo %d:\n", matricula, aluno->nomeAluno, periodo);
        // Chama a função auxiliar para exibir as notas no período especificado
        exibirNotasPeriodo(aluno->notas, periodo);
    } else {
        printf("Aluno com matricula %d nao encontrado.\n", matricula);
    }
}
//xii) Mostrar a nota de uma disciplina de um determinado aluno, mostrando o período e a carga horária da disciplina.
// Função principal para mostrar a nota de uma disciplina de um aluno, com período e carga horária
void mostrarNotaDisciplinaAluno(Alunos *listaAlunos, int matricula, int codigoDisciplina, int semestre, ArvDisciplinas *disciplinas) {
    // Buscar o aluno na lista de alunos usando a função já implementada
    Alunos *aluno = buscarAluno(listaAlunos, matricula);

    if (aluno != NULL) {
        // Buscar a nota da disciplina na árvore de notas do aluno
        ArvNotas *nota = buscarNota(aluno->notas, codigoDisciplina, semestre);

        if (nota != NULL) {
            // Buscar a disciplina para obter o período e a carga horária
            ArvDisciplinas *disciplina = buscarDisciplina(disciplinas, codigoDisciplina);

            if (disciplina != NULL) {
                // Exibir as informações da nota, período e carga horária
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

//xiii)Remover uma disciplina de um determinado curso desde que não tenha nenhum aluno matriculado na mesma.
int removerDisciplinaCurso(ArvCursos **raiz, int codigoCurso, int codigoDisciplina, Alunos *listaAlunos) {
    int resultado = 0;  // Variável para armazenar o resultado final

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
            // Verifica se há algum aluno matriculado na disciplina
            Alunos *atual = listaAlunos;
            int alunosMatriculados = 0;

            // Percorre a lista de alunos e suas matrículas
            while (atual != NULL && alunosMatriculados == 0) {
                ArvMatriculas *matricula = atual->matriculas;

                // Verifica as disciplinas nas matrículas do aluno
                while (matricula != NULL) {
                    if (matricula->codigoDisciplina == codigoDisciplina) {
                        alunosMatriculados = 1;  // Aluno encontrado matriculado na disciplina
                        break;
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
                curso->disciplinas = removerDisciplina(curso->disciplinas, codigoDisciplina);
                printf("Disciplina com codigo %d removida com sucesso do curso %d.\n", codigoDisciplina, codigoCurso);
                resultado = 1;  // Sucesso na remoção
            }
        }
    }

    return resultado;  // Retorna 1 se a remoção for bem-sucedida, ou 0 se houver algum problema
}

//xiv)Permita remover uma disciplina da árvore de matrícula de um determinado aluno.
int removerDisciplinaMatricula(Alunos *listaAlunos, int matricula, int codigoDisciplina) {
    Alunos *aluno = buscarAluno(listaAlunos, matricula);
    int sucesso = 0;

    if (aluno != NULL) {
        // Remover a disciplina da árvore de matrículas
        aluno->matriculas = removerMatricula(aluno->matriculas, matricula, codigoDisciplina);

        // Verificar se foi removida
        if (buscarMatriculaPorReferencia(aluno->matriculas, codigoDisciplina) == NULL) {
            printf("Disciplina com código %d removida com sucesso da matrícula do aluno %d.\n", codigoDisciplina, matricula);
            sucesso = 1;
        } else {
            printf("Falha ao remover a disciplina com código %d do aluno %d.\n", codigoDisciplina, matricula);
        }
    } else {
        printf("Aluno com matrícula %d não encontrado.\n", matricula);
    }

    return sucesso;
}

/*
xv) Mostrar o histórico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva nota organizadas pelo período 
que a disciplina está cadastrada no curso. 
*/
// Função auxiliar para percorrer a árvore de matrículas em ordem, sem utilizar return
void percorrerArvMatriculas(ArvMatriculas *raiz, Alunos *aluno, ArvCursos *curso) {
    if (raiz != NULL) {
        // Visita a subárvore esquerda
        percorrerArvMatriculas(raiz->esq, aluno, curso);

        // Busca a disciplina correspondente à matrícula na árvore de disciplinas do curso
        ArvDisciplinas *disciplina = buscarDisciplina(curso->disciplinas, raiz->codigoDisciplina);
        if (disciplina != NULL) {
            // Busca a nota do aluno para a disciplina
            ArvNotas *nota = buscarNota(aluno->notas, disciplina->codigoDisciplina, 0); // 0 como semestre se não for relevante
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

        // Visita a subárvore direita
        percorrerArvMatriculas(raiz->dir, aluno, curso);
    }
}


// Função principal para mostrar o histórico de um aluno
void mostrarHistorico(Alunos *listaAlunos, ArvCursos *raiz, int matricula) {
    Alunos *aluno = buscarAluno(listaAlunos, matricula);

    if (aluno != NULL) {
        // Busca o curso do aluno
        ArvCursos *curso = buscarCursoPorReferencia(raiz, aluno->codigoCurso);
        if (curso != NULL) {
            printf("Historico do aluno: %s\n", aluno->nomeAluno);
            printf("Curso: %s\n", curso->nomeCurso);
            printf("Disciplinas cursadas:\n");

            // Percorre a árvore de matrículas para exibir as disciplinas e notas
            percorrerArvMatriculas(aluno->matriculas, aluno, curso);
        } else {
            printf("Curso nao encontrado para o aluno.\n");
        }
    } else {
        printf("Aluno com matrícula %d nao encontrado.\n", matricula);
    }
}

// xvi) Funções de liberar árvores e sair do programa

//Função p/ liberar árvore de deisciplina
void liberarDisciplinas(ArvDisciplinas *raiz) {
    if (raiz != NULL) {
        liberarDisciplinas(raiz->esq);  // Libera a subárvore esquerda
        liberarDisciplinas(raiz->dir);  // Libera a subárvore direita
        free(raiz);  // Libera o nó atual
    }
}
//Função p/ liberar arvore de cursos
void liberarCursos(ArvCursos *raiz) {
    if (raiz != NULL) {
        liberarDisciplinas(raiz->disciplinas);  // Libera todas as disciplinas do curso
        liberarCursos(raiz->esq);  // Libera a subárvore esquerda
        liberarCursos(raiz->dir);  // Libera a subárvore direita
        free(raiz);  // Libera o nó atual
    }
}

//Função p/ liberar matriculas
void liberarMatriculas(ArvMatriculas *raiz) {
    if (raiz != NULL) {
        liberarMatriculas(raiz->esq);  // Libera a subárvore esquerda
        liberarMatriculas(raiz->dir);  // Libera a subárvore direita
        free(raiz);  // Libera o nó atual
    }
}

//Função p/ liberar árvore de notas
void liberarNotas(ArvNotas *raiz) {
    if (raiz != NULL) {
        liberarNotas(raiz->esq);  // Libera a subárvore esquerda
        liberarNotas(raiz->dir);  // Libera a subárvore direita
        free(raiz);  // Libera o nó atual
    }
}

//Função p/ liberar lista de alunos
void liberarAlunos(Alunos *lista) {
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
void liberarArvores(ArvCursos *raizCursos, Alunos *listaAlunos) {
    liberarCursos(raizCursos);  // Libera todos os cursos e suas disciplinas
    liberarAlunos(listaAlunos);  // Libera todos os alunos, suas matrículas e notas
}


//Função principal
                                                                       

int main() {
    ArvCursos *raiz = NULL;  // Árvore de cursos
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
                cadastrarDisciplina(raiz, codigoCurso, codigoDisciplina, nomeDisciplina, periodo, cargaHoraria);
                break;
            
            case 3:  // Cadastrar aluno
                printf("Digite a matricula: ");
                scanf("%d", &matricula);
                printf("Digite o nome do aluno: ");
                scanf(" %[^\n]s", nomeAluno);  // Leitura de string com espaços
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
                // Chama a função para mostrar os alunos do curso fornecido
                mostrarAlunosCurso(lista, codigoCurso);
                break;
            
            case 7:   // Mostrar todos os cursos
			    printf("Listando todos os cursos do campus:\n\n");
			    // Chama a função para mostrar os cursos da árvore
			    mostrarCursos(raiz);
			    break;
			
			case 8:  // Mostrar todas as disciplinas de um curso
			    printf("Digite o codigo do curso: ");
			    scanf("%d", &codigoCurso);
			    mostrarDisciplinasCurso(raiz, codigoCurso);
			    break;
			
			case 9: // mostrar as disciplinas do período
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


			    
			
			case 11: // mostrar as notas do aluno no período
			    printf("Digite a matricula do aluno: ");
			    scanf("%d", &matricula);
			    printf("Digite o periodo desejado: ");
			    scanf("%d", &periodo);
			    mostrarNotasAlunoPeriodo(lista, matricula, periodo);
			    break;
			
			case 12: // Mostrar a nota da disciplina do aluno, período e carga horária
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
			
			            // Chama a função corrigida com o argumento correto
			            mostrarNotaDisciplinaAluno(lista, matricula, codigoDisciplina, semestre, curso->disciplinas);
			
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
			    liberarArvores(raiz, lista);  // Libera toda a memória alocada
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




