#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_CURSO 30
#define MAX_FILIA 30
#define MAX_CPF 12

// Estrutura de Aluno
typedef struct {
    int matricula;
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    char curso[MAX_CURSO];
    char mae[MAX_FILIA];
    char pai[MAX_FILIA];
    int ano_ingresso;
    float coeficiente;
} Aluno;

typedef struct {
    int id;
    int bytes_usados;
} Bloco;

//  gerar aluno
void gerarAluno(Aluno *a, int i) {
    a->matricula = 100000000 + i;
    sprintf(a->nome, "Aluno_%d", i);
    sprintf(a->cpf, "00000000%03d", i);
    sprintf(a->curso, "Curso_%d", i % 5);
    sprintf(a->mae, "Mae_%d", i);
    sprintf(a->pai, "Pai_%d", i);
    a->ano_ingresso = 2020 + (i % 5);
    a->coeficiente = 6.0 + (rand() % 40) / 10.0;
}

// Tamanho do registro (fixo ou variável)
int tamanhoRegistro(Aluno *a, int tipo) {
    if (tipo == 1) return sizeof(Aluno); // fixo
    else {
        return sizeof(int) + strlen(a->nome) + strlen(a->cpf)
             + strlen(a->curso) + strlen(a->mae) + strlen(a->pai)
             + sizeof(int) + sizeof(float);
    }
}

void armazenar(FILE *arquivo, int qtd, int tamBloco, int tipo, int espalhar) {
    Aluno aluno;
    Bloco bloco = {1, 0};
    int total_blocos = 1;
    int blocos_parciais = 0;
    int total_bytes = 0;

    printf("\n--- Armazenamento ---\n");

    for (int i = 0; i < qtd; i++) {
        gerarAluno(&aluno, i + 1);
        int tamReg = tamanhoRegistro(&aluno, tipo);

        // Caso o registro não caiba completamente
        if (bloco.bytes_usados + tamReg > tamBloco) {
            if (tipo == 2 && espalhar) {
                int parte1 = tamBloco - bloco.bytes_usados;
                int parte2 = tamReg - parte1;
                printf("Registro %d espalhado: %d bytes no bloco %d e %d bytes no bloco %d\n",
                       i + 1, parte1, bloco.id, parte2, bloco.id + 1);
                total_blocos++; // novo bloco usado
                bloco.bytes_usados = parte2; // novo bloco inicia com parte2 usada
            } else {
                if (bloco.bytes_usados < tamBloco) blocos_parciais++;
                total_blocos++;
                bloco.bytes_usados = tamReg;
            }
        } else {
            bloco.bytes_usados += tamReg;
        }

        total_bytes += tamReg;
        fwrite(&aluno, sizeof(Aluno), 1, arquivo);
    }

    if (bloco.bytes_usados < tamBloco) blocos_parciais++;
    float eficiencia = ((float)total_bytes / (total_blocos * tamBloco)) * 100.0;
    float ocupacao_media = eficiencia;

    printf("\n===== ARMAZENAMENTO =====\n");
    printf("Total de registros: %d\n", qtd);
    printf("Blocos utilizados: %d\n", total_blocos);
    printf("Blocos parcialmente usados: %d\n", blocos_parciais);
    printf("Tamanho do bloco: %d bytes\n", tamBloco);
    printf("Total de bytes: %d\n", total_bytes);
    printf("Eficiencia total de armazenamento: %.2f%%\n", eficiencia);
    printf("Ocupação media dos blocos: %.2f%%\n", ocupacao_media);

    // Mapa textual
    printf("\n===== MAPA DE OCUPACAO DOS BLOCOS =====\n");
    for (int b = 1; b <= total_blocos; b++) {
        float taxa = (b == total_blocos && blocos_parciais) ? eficiencia : 100.0;
        printf("Bloco %2d: %.2f%% cheio | ", b, taxa);
        int barras = (int)(taxa / 5);
        for (int j = 0; j < barras; j++) printf("#");
        printf("\n");
    }
}

// Leitura e exibição do arquivo binário
void lerArquivo(FILE *arquivo) {
    Aluno a;
    rewind(arquivo);
    printf("\n===== LEITURA DO ARQUIVO BINARIO =====\n");
    while (fread(&a, sizeof(Aluno), 1, arquivo) == 1) {
        printf("Matricula: %d | Nome: %-10s | CPF: %-11s | Curso: %-8s | Mae: %-10s | Pai: %-10s | Ano: %d | Coef: %.2f\n",
               a.matricula, a.nome, a.cpf, a.curso, a.mae, a.pai, a.ano_ingresso, a.coeficiente);
    }
}

int main() {
    int n, tamBloco, tipo, espalhar;
    FILE *arquivo;

    printf(" ---- TPI AEDS II - Armazenamento em Blocos ----\n");
    printf("Quantidade de registros: ");
    scanf("%d", &n);
    printf("Tamanho maximo do bloco (em bytes): ");
    scanf("%d", &tamBloco);
    printf("Tipo de registro (1 - Fixo | 2 - Variavel): ");
    scanf("%d", &tipo);

    if (tipo == 2) {
        printf("Permitir espalhamento (1 - Sim | 0 - Nao): ");
        scanf("%d", &espalhar);
    } else {
        espalhar = 0;
    }

    arquivo = fopen("alunos.dat", "wb");
    if (!arquivo) {
        printf("Erro ao criar o arquivo!\n");
        return 1;
    }

    armazenar(arquivo, n, tamBloco, tipo, espalhar);
    fclose(arquivo);

    arquivo = fopen("alunos.dat", "rb");
    if (arquivo) {
        lerArquivo(arquivo);
        fclose(arquivo);
    }

    printf("\n Armazenamento concluido!");
    return 0;
}
