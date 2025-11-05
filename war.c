// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MISSAO 100
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;
// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
Territorio* alocarMapa(int tamanho);
void inicializarTerritorios(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missao);
void exibirMapa(const Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMenuPrincipal();
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho);
void limparBufferEntrada();
// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand(time(NULL));

    int tamanho;
    printf("Digite o número de territórios: ");
    scanf("%d", &tamanho);
    limparBufferEntrada();

    Territorio* mapa = alocarMapa(tamanho);
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    inicializarTerritorios(mapa, tamanho);

    char* missoes[] = {
        "Conquistar 3 territórios.",
        "Eliminar todos os territórios vermelhos.",
        "Dominar metade do mapa.",
        "Ter ao menos 5 territórios com 10 tropas.",
        "Defender todos os seus territórios por 3 rodadas."
    };
    int totalMissoes = 5;

    char* missaoJogador = (char*)malloc(MAX_MISSAO * sizeof(char));
    atribuirMissao(missaoJogador, missoes, totalMissoes);

    printf("\n--- SUA MISSÃO ---\n%s\n", missaoJogador);
    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    int opcao;
    int atacante, defensor;

    do {
        printf("\n===== MAPA ATUAL =====\n");
        exibirMapa(mapa, tamanho);
        printf("\nMissão: %s\n", missaoJogador);

        printf("\n===== MENU =====\n");
        printf("1. Atacar território\n");
        printf("2. Verificar missão\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                printf("Escolha o território atacante (0 a %d): ", tamanho - 1);
                scanf("%d", &atacante);
                printf("Escolha o território defensor (0 a %d): ", tamanho - 1);
                scanf("%d", &defensor);
                limparBufferEntrada();

                if (atacante >= 0 && atacante < tamanho && defensor >= 0 && defensor < tamanho) {
                    atacar(&mapa[atacante], &mapa[defensor]);
                } else {
                    printf("Índices inválidos!\n");
                }
                break;

            case 2:
                if (verificarMissao(missaoJogador, mapa, tamanho)) {
                    printf("\n🎉 Parabéns! Você cumpriu sua missão!\n");
                    opcao = 0;
                } else {
                    printf("\nMissão ainda não concluída.\n");
                }
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
                break;
        }
        printf("\nPressione ENTER para continuar...");
        getchar();

    } while (opcao != 0);
    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
liberarMemoria(mapa, missaoJogador);
    //return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa(int tamanho) {
    return (Territorio*)calloc(tamanho, sizeof(Territorio));
}
// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio* mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        sprintf(mapa[i].nome, "Território %d", i + 1);
        if (i % 2 == 0)
            strcpy(mapa[i].cor, "Azul");
        else
            strcpy(mapa[i].cor, "Vermelho");
        mapa[i].tropas = rand() % 10 + 1;
    }
}
// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("\n===== MENU =====\n");
    printf("1. Atacar território\n");
    printf("2. Verificar missão\n");
    printf("0. Sair\n");
}
// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("\n%-15s %-12s %-8s\n", "Nome", "Cor", "Tropas");
    printf("----------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%-15s %-12s %-8d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}
// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(const char* missao) {
    printf("%s\n", missao);
}
// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Você não pode atacar um território da mesma cor!\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("%s (Ataque) rolou %d\n", atacante->nome, dadoAtacante);
    printf("%s (Defesa) rolou %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
    } else {
        printf("O defensor resistiu!\n");
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}
// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}
// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho) {
    int territoriosAzuis = 0;
    int territoriosVermelhos = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, "Azul") == 0)
            territoriosAzuis++;
        else if (strcmp(mapa[i].cor, "Vermelho") == 0)
            territoriosVermelhos++;
    }

    if (strstr(missao, "3 territórios") && territoriosAzuis >= 3)
        return 1;

    if (strstr(missao, "vermelhos") && territoriosVermelhos == 0)
        return 1;

    if (strstr(missao, "metade do mapa") && territoriosAzuis >= tamanho / 2)
        return 1;

    return 0;
}
// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}