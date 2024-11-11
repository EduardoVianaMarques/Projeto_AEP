#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#define QTD_Usuarios 50 // quantidade de usuarios e senhas possiveis
#define cript 6 // Criptografia
//-----------------------------------------------------------------------------------------------------------
void limparTela() {
    system("cls");
}
//-----------------------------------------------------------------------------------------------------------
void mostrarMenu() {
    limparTela();
    printf("\n\n\n\t\t\t\t\t-----------Cadastro de Usuários-----------\n");
    printf("\t\t\t\t\t__________________________________________\n");
    printf("\t\t\t\t\t|\t\t1. Cadastro\t\t |\n");
    printf("\t\t\t\t\t|\t\t2. Alterar\t\t |\n");
    printf("\t\t\t\t\t|\t\t3. Consultar\t\t |\n");
    printf("\t\t\t\t\t|\t\t4. Deletar\t\t |\n");
    printf("\t\t\t\t\t|\t\t5. Sair\t\t\t |\n");
    printf("\t\t\t\t\t|\t\tDigite uma opção:\t |\n");
    printf("\t\t\t\t\t------------------------------------------\n");
    printf("\t\t\t\t\t\t  Pressione ESC para sair\n");
    printf("\t\t\t\t\t------------------------------------------\n");
}
//-----------------------------------------------------------------------------------------------------------
void criptografar(char *texto, char *resultado) {
    for (int i = 0; texto[i] != '\0'; i++) {
        char c = texto[i];
        // Criptografa apenas letras
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            resultado[i] = (c - base + cript) % 26 + base;
        } else {
            resultado[i] = c; // Mantém caracteres não alfabéticos
        }
    }
    resultado[strlen(texto)] = '\0'; 
}
//-----------------------------------------------------------------------------------------------------------
void descriptografar(char *texto, char *resultado) {
    for (int i = 0; texto[i] != '\0'; i++) {
        char c = texto[i];
        // Descriptografa apenas letras
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            resultado[i] = (c - base - cript + 26) % 26 + base; // +26 para evitar números negativos
        } else {
            resultado[i] = c; // Mantém caracteres não alfabéticos
        }
    }
    resultado[strlen(texto)] = '\0'; // Adiciona o terminador de string
}
//-----------------------------------------------------------------------------------------------------------
int validarSenha(const char *senha) {
    int tamanho = strlen(senha);
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;

    if (tamanho < 6) return 0; // A senha deve ter pelo menos 6 caracteres

    for (int i = 0; i < tamanho; i++) {
        if (isupper(senha[i])) hasUpper = 1;
        else if (islower(senha[i])) hasLower = 1;
        else if (isdigit(senha[i])) hasDigit = 1;
        else if (ispunct(senha[i])) hasSpecial = 1; // Verifica se é um caractere especial
    }

    return hasUpper && hasLower && hasDigit && hasSpecial; // Retorna 1 se todos os critérios forem atendidos
}
//-----------------------------------------------------------------------------------------------------------
void atualizarArquivo(char nome[][30], char senha[][30], int i) {
    FILE *arquivo = fopen("AEP.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para atualizar!\n");
        return;
    }
    for (int j = 0; j < i; j++) {
        char nomeCriptografado[30],senhaCriptografada[30];
    	criptografar(nome[j], nomeCriptografado);
    	criptografar(senha[j], senhaCriptografada);
    	fprintf(arquivo, "%d - %s - %s\n", j + 1, nomeCriptografado, senhaCriptografada);
    }
    fclose(arquivo);
}
//-----------------------------------------------------------------------------------------------------------
void cadastrarUsuario(char nome[][30], char senha[][30], int *i) {
    char conti;
    limparTela();
    FILE *arquivo = fopen("AEP.txt", "a"); // Abre o arquivo em modo de anexação
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    
    printf("\n\n\t\t\tInforme seu Nome\n\t\t\t-> ");
    scanf(" %s", nome[*i]); 

    char nomeCriptografado[30];
    criptografar(nome[*i], nomeCriptografado);

    do {
        printf("\n\n\t\t\tInforme sua Senha \n\t\t\tmínimo 6 caracteres| 1-número| 1 maiúscula| 1 minúscula| 1 especial\n\t\t\t-> ");
        scanf(" %s", senha[*i]); 
        if (!validarSenha(senha[*i])) {
            printf("\n\t\t\tSenha inválida! Tente novamente.\n");
        }
    } while (!validarSenha(senha[*i]));

    char senhaCriptografada[30];
    criptografar(senha[*i], senhaCriptografada);

    fprintf(arquivo, "%d - %s - %s\n", *i + 1, nomeCriptografado, senhaCriptografada); // Escrevendo o id, nome e senha no arquivo
    fclose(arquivo); // Fecha o arquivo após a escrita
    
    (*i)++;
    limparTela();
    printf("\n\t\t\t\t\t________________________________\n");
    printf("\n\t\t\t\t\t Usuário cadastrado com sucesso!\n");
    printf("\t\t\t\t\t________________________________\n");
    printf("\n\t\t\t\tDeseja cadastrar mais uma pessoa (S-sim/N-não)\n\t\t\t\t-> ");
    scanf(" %c", &conti);
    if (conti == 'S' || conti == 's') {
        cadastrarUsuario(nome, senha, i);
    }
}
//-----------------------------------------------------------------------------------------------------------
void alterarUsuario(char nome[][30], char senha[][30], int i) {
    limparTela();
    FILE *arquivo = fopen("AEP.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[100];
    char nomeCriptografado[30];
    char senhaCriptografada[30];
    int mostrarCriptografados = 0; // Variável para controlar a exibição

    // Lê todos os usuários do arquivo e armazena em arrays
    for (int j = 0; j < i; j++) {
        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%*d - %s - %s", nomeCriptografado, senhaCriptografada);
        strcpy(nome[j], nomeCriptografado); // Armazena o nome criptografado
    }
    fclose(arquivo); // Fecha o arquivo após a leitura
    printf("\n\n\n\t\t\t\t\t-------------Usuários Cadastrados-------------\n");
    for (int x = 0; x < i; x++) {
        char nomeDescriptografado[30];
        descriptografar(nome[x], nomeDescriptografado);
        printf("\t\t\t\t\t\t%d - %s\n", x + 1, nomeDescriptografado); // Exibe os nomes descriptografados
        printf("\t\t\t\t\t----------------------------------------------\n");
    }
    
    printf("\t\t\t\t\tInforme o número do usuário que deseja alterar\n\t\t\t\t\t-> ");
    int opcao;
    scanf("%d", &opcao);
    if (opcao > 0 && opcao <= i) {
        printf("\n\n\t\t\tInforme seu Nome\n\t\t\t-> ");
        scanf(" %s", nome[opcao - 1]);
        
        char nomeCriptografado[30];
        criptografar(nome[opcao - 1], nomeCriptografado);

        do {
            printf("\n\n\t\t\tInforme sua Senha \n\t\t\tmínimo 6 caracteres| 1-número| 1 maiúscula| 1 minúscula| 1 especial\n\t\t\t-> ");
            scanf(" %s", senha[opcao - 1]);
            if (!validarSenha(senha[opcao - 1])) {
                printf("Senha inválida! Tente novamente.\n");
            }
        } while (!validarSenha(senha[opcao - 1]));

        char senhaCriptografada[30];
        criptografar(senha[opcao - 1], senhaCriptografada);

        atualizarArquivo(nome, senha, i); // Atualiza o arquivo após alteração
        limparTela();
        printf("Usuário alterado com sucesso!\n");
    } else {
        printf("Usuário não encontrado!\n");
    }
}
//-----------------------------------------------------------------------------------------------------------
void consultarUsuarios(char nome[][30], int i) {
    FILE *arquivo = fopen("AEP.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[100];
    char nomeCriptografado[30];
    char senhaCriptografada[30];
    int mostrarCriptografados = 0; // Variável para controlar a exibição

    // Lê todos os usuários do arquivo e armazena em arrays
    for (int j = 0; j < i; j++) {
        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%*d - %s - %s", nomeCriptografado, senhaCriptografada);
        strcpy(nome[j], nomeCriptografado); // Armazena o nome criptografado
    }
    fclose(arquivo); // Fecha o arquivo após a leitura

    while (1) {
        limparTela();
        if (mostrarCriptografados) {
            printf("\n\n\n\t\t\t\t\t-----------Usuários Criptografados--------\n");
            for (int x = 0; x < i; x++) {
                printf("\t\t\t\t\t\t%d - %s\n", x + 1, nome[x]); // Exibe os nomes criptografados
                printf("\t\t\t\t\t------------------------------------------\n");
            }
        } else {
            printf("\n\n\n\t\t\t\t\t-----------Usuários Cadastrados-----------\n");
            for (int x = 0; x < i; x++) {
                char nomeDescriptografado[30];
                descriptografar(nome[x], nomeDescriptografado);
                printf("\t\t\t\t\t\t%d - %s\n", x + 1, nomeDescriptografado); // Exibe os nomes descriptografados
                printf("\t\t\t\t\t------------------------------------------\n");
            }
        }

        printf("\n\t\t\t\t\tPressione SPACE para alternar entre as listas\n");
        printf("\t\t\t\t\t------------------------------------------\n");
        printf("\n\t\t\t\t\t\t   Pressione ESC para Sair\n");
        int tecla = _getch();
        if (tecla == 32) { // Se a tecla pressionada for SPACE
            mostrarCriptografados = !mostrarCriptografados; // Alterna a exibição
        } else if (tecla == 27) { // Se a tecla pressionada for ESC
            break; // Sai da função
        }
    }
}
//-----------------------------------------------------------------------------------------------------------
void deletarUsuario(char nome[][30], char senha[][30], int *i) {
    limparTela();
    FILE *arquivo = fopen("AEP.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[100];
    char nomeCriptografado[30];
    char senhaCriptografada[30];
    int mostrarCriptografados = 0; // Variável para controlar a exibição

    // Lê todos os usuários do arquivo e armazena em arrays
    for (int j = 0; j < *i; j++) {
        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%*d - %s - %s", nomeCriptografado, senhaCriptografada);
        strcpy(nome[j], nomeCriptografado); // Armazena o nome criptografado
    }
    fclose(arquivo); // Fecha o arquivo após a leitura
    printf("\n\n\n\t\t\t\t\t-------------Usuários Cadastrados-------------\n");
    for (int x = 0; x < *i; x++) {
        char nomeDescriptografado[30];
        descriptografar(nome[x], nomeDescriptografado);
        printf("\t\t\t\t\t\t%d - %s\n", x + 1, nomeDescriptografado); // Exibe os nomes descriptografados
        printf("\t\t\t\t\t----------------------------------------------\n");
    }
    
    printf("\t\t\t\t\tInforme o número do usuário que deseja deletar\n\t\t\t\t\t-> ");
    int opcao;
    scanf("%d", &opcao);
    if (opcao > 0 && opcao <= *i) {
        char confirmacao;
        printf("\t\t\t\t\tVocê tem certeza que deseja deletar este usuário?\n\t\t\t\t\t (S-sim/N-não)\n\t\t\t\t\t-> ");
        scanf(" %c", &confirmacao);
        if (confirmacao == 'S' || confirmacao == 's') {
            for (int x = opcao - 1; x < *i - 1; x++) {
                strcpy(nome[x], nome[x + 1]);
                strcpy(senha[x], senha[x + 1]);
            }
            (*i)--;
            atualizarArquivo(nome, senha, *i); // Atualiza o arquivo após deleção
            limparTela();
            printf("Usuário deletado com sucesso!\n");
        }
    } else {
        printf("Usuário não encontrado!\n");
    }
}
//-----------------------------------------------------------------------------------------------------------
int main() {
    setlocale(LC_ALL, "Portuguese");
    char nome[QTD_Usuarios][30], senha[QTD_Usuarios][30];
    int i = 0;
    //-----------------------------------inicializar i------------------------------------------------------
    FILE *arquivo = fopen("AEP.txt", "r");
    if (arquivo != NULL) {
        char caractere;
        while ((caractere = fgetc(arquivo)) != EOF) {
            if (caractere == '\n') {
                i++;
            }
        }
        fclose(arquivo);
    }
    //--------------------------------------------------------------------------------------------------------
    while (1) {
        mostrarMenu();
        int opcao = _getch();
        if (opcao == 27) {
            break;
        }
        switch (opcao) {
            case '1':
                cadastrarUsuario(nome, senha, &i);
                break;
            case '2':
                alterarUsuario(nome, senha, i);
                break;
            case '3':
                consultarUsuarios(nome, i);
                break;
            case '4':
                deletarUsuario(nome, senha, &i);
                break;
            case '5':
                return 0;
            default:
                printf("Opção Inválida\n");
                break;
        }
    }
    return 0;
}
