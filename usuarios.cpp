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
        senha[i] ^= 0xAA;  // Opera��o XOR
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
        criptografar(usuarios[i].senha); // Descriptografar ap�s salvar
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
        printf("Limite de usu�rios alcan�ado!\n");
        return;
    }
    printf("Digite o nome do usu�rio: ");
    scanf(" %[^\n]", usuarios[*numUsuarios].nome);
    printf("Digite a senha do usu�rio: ");
    scanf(" %[^\n]", usuarios[*numUsuarios].senha);

    criptografar(usuarios[*numUsuarios].senha); 
    (*numUsuarios)++;
    printf("Usu�rio adicionado!\n");
}

void alterarUsuario(Usuario usuarios[], int numUsuarios) {
    char nome[50];
    printf("Digite o nome do usu�rio a ser alterado: ");
    scanf(" %[^\n]", nome);

    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {
            printf("Digite a nova senha para o usu�rio %s: ", nome);
            scanf(" %[^\n]", usuarios[i].senha);
            criptografar(usuarios[i].senha); 
            printf("Senha alterada!\n");
            return;
        }
    }
    printf("Usu�rio n�o encontrado!\n");
}

void excluirUsuario(Usuario usuarios[], int *numUsuarios) {
    char nome[50];
    printf("Digite o nome do usu�rio a ser exclu�do: ");
    scanf(" %[^\n]", nome);

    for (int i = 0; i < *numUsuarios; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {
            for (int j = i; j < *numUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];  
            }
            (*numUsuarios)--;
            printf("Usu�rio exclu�do!\n");
            return;
        }
    }
    printf("Usu�rio n�o encontrado!\n");
}

void listarUsuarios(Usuario usuarios[], int numUsuarios) {
    if (numUsuarios == 0) {
        printf("Nenhum usu�rio cadastrado!\n");
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
        printf("1. Adicionar Usu�rio\n");
        printf("2. Alterar Usu�rio\n");
        printf("3. Excluir Usu�rio\n");
        printf("4. Listar Usu�rios\n");
        printf("5. Sair\n");
        printf("Escolha uma op��o: ");
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
                printf("Op��o inv�lida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}
