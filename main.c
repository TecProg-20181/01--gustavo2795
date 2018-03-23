#include <stdio.h>
#include "estrutura.h" //Include das estruturas Imagem e Pixel.
#include "funcoes.h" //Include das funções de manipulação.

int main() {
  Image imagem;

  imagem = ler_arquivo_entrada(imagem);

  int numero_opcoes;
  scanf("%d", &numero_opcoes);

  for(int contador = 0; contador < numero_opcoes; ++contador) {
    int opcao;
    scanf("%d", &opcao);

    switch(opcao) {
      case 1: { // Modifica a imagem para escala de cinza.
        imagem = escala_de_cinza(imagem);
        break;
      }
      case 2: { //Aplica o filtro sépia na imagem.
        imagem = filtro_sepia(imagem);
        break;
      }
      case 3: { //Aplica o filtro Blur na imagem.
        int tamanho = 0;
        scanf("%d", &tamanho);
        blur(imagem.altura, imagem.pixel, tamanho, imagem.largura);
        break;
      }
      case 4: { //Rotaciona para direita um determinado número de vezes a imagem.
        int quantas_vezes = 0;
        scanf("%d", &quantas_vezes);
        quantas_vezes = quantas_vezes % 4;
        for (int contador = 0; contador < quantas_vezes; ++contador) {
          imagem = rotacionar90direita(imagem);
        }
        break;
      }
      case 5: { //Espelha verticalmente a imagem.
        imagem = espelhamento_vertical(imagem);
        break;
      }
      case 6: { //Aplica o filtro negativo.
        inverter_cores(imagem.pixel, imagem.largura, imagem.altura);
        break;
      }
      case 7: { //Corta a imagem em um novo tamanho passado no arquivo de entrada.
        int nova_largura, nova_altura;
        scanf("%d %d", &nova_largura, &nova_altura);
        int largura_original, altura_original;
        scanf("%d %d", &largura_original, &altura_original);

        imagem = cortar_imagem(imagem, nova_largura, nova_altura,
                            largura_original, altura_original);
        break;
      }
    }
  }

  printa_imagem(imagem, cor_maxima);

  return 0;
}
