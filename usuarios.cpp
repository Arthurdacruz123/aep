#include <stdio.h>
#include <string.h>

#define MAX_USUARIOS 10
#define TAMANHO_SENHA 50
#define ARQUIVO "usuarios.txt"


typedef struct {
    char nome[50];
    char senha[TAMANHO_SENHA];
} Usuario;

void criptografar(char *senha) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] ^= 0xAA;  // Operação XOR
    }
}

void salvarUsuarios(Usuario usuarios[], int numUsuarios) {
    FILE *arquivo = fopen(ARQUIVO, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    for (int i = 0; i < numUsuarios; i++) {
        criptografar(usuarios[i].senha); // Criptografar antes de salvar
        fprintf(arquivo, "%s;%s\n", usuarios[i].nome, usuarios[i].senha);
        criptografar(usuarios[i].senha); // Descriptografar após salvar
    }
    fclose(arquivo);
}

int carregarUsuarios(Usuario usuarios[]) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    int numUsuarios = 0;
    if (arquivo == NULL) {
        return 0; 
    }
    while (fscanf(arquivo, "%49[^;];%49[^\n]\n", usuarios[numUsuarios].nome, usuarios[numUsuarios].senha) != EOF) {
        criptografar(usuarios[numUsuarios].senha); 
        numUsuarios++;
    }
    fclose(arquivo);
    return numUsuarios;
}

void adicionarUsuario(Usuario usuarios[], int *numUsuarios) {
    if (*numUsuarios >= MAX_USUARIOS) {
        printf("Limite de usuários alcançado!\n");
        return;
    }
    printf("Digite o nome do usuário: ");
    scanf(" %[^\n]", usuarios[*numUsuarios].nome);
    printf("Digite a senha do usuário: ");
    scanf(" %[^\n]", usuarios[*numUsuarios].senha);

    criptografar(usuarios[*numUsuarios].senha); 
    (*numUsuarios)++;
    printf("Usuário adicionado!\n");
}

void alterarUsuario(Usuario usuarios[], int numUsuarios) {
    char nome[50];
    printf("Digite o nome do usuário a ser alterado: ");
    scanf(" %[^\n]", nome);

    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {
            printf("Digite a nova senha para o usuário %s: ", nome);
            scanf(" %[^\n]", usuarios[i].senha);
            criptografar(usuarios[i].senha); 
            printf("Senha alterada!\n");
            return;
        }
    }
    printf("Usuário não encontrado!\n");
}

void excluirUsuario(Usuario usuarios[], int *numUsuarios) {
    char nome[50];
    printf("Digite o nome do usuário a ser excluído: ");
    scanf(" %[^\n]", nome);

    for (int i = 0; i < *numUsuarios; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {
            for (int j = i; j < *numUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];  
            }
            (*numUsuarios)--;
            printf("Usuário excluído!\n");
            return;
        }
    }
    printf("Usuário não encontrado!\n");
}

void listarUsuarios(Usuario usuarios[], int numUsuarios) {
    if (numUsuarios == 0) {
        printf("Nenhum usuário cadastrado!\n");
        return;
    }
    for (int i = 0; i < numUsuarios; i++) {
        printf("Nome: %s\n", usuarios[i].nome);
        
        char senhaDescriptografada[TAMANHO_SENHA];
        strcpy(senhaDescriptografada, usuarios[i].senha);
        criptografar(senhaDescriptografada); 
        printf("Senha: %s\n", senhaDescriptografada);
    }
}

int main() {
    Usuario usuarios[MAX_USUARIOS];
    int numUsuarios = carregarUsuarios(usuarios);
    int opcao;

    do {
        printf("\n*** MENU ***\n");
        printf("1. Adicionar Usuário\n");
        printf("2. Alterar Usuário\n");
        printf("3. Excluir Usuário\n");
        printf("4. Listar Usuários\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarUsuario(usuarios, &numUsuarios);
                salvarUsuarios(usuarios, numUsuarios);
                break;
            case 2:
                alterarUsuario(usuarios, numUsuarios);
                salvarUsuarios(usuarios, numUsuarios);
                break;
            case 3:
                excluirUsuario(usuarios, &numUsuarios);
                salvarUsuarios(usuarios, numUsuarios);
                break;
            case 4:
                listarUsuarios(usuarios, numUsuarios);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}
