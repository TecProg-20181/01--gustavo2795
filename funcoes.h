#ifndef __funcoes_h
#define __funcoes_h

#include <stdio.h>
#include "estrutura.h"

//Declaração de constantes
#define VALOR_MAXIMO_PIXEL 255
#define LARGURA_MAXIMA 512
#define ALTURA_MAXIMA 512

//Variável Global que armazena a cor máxima de um Pixel.
int cor_maxima;

/*As variáveis "linha" e "coluna" são auxiliares para percorrerem a matriz
de números da imagem.*/

/*Função que manipula pixel à pixel para torná-los na escala de cinza,
através da média dos valores entre eles.*/
Image escala_de_cinza(Image imagem){
  for (unsigned int linha = 0; linha < imagem.altura; ++linha){
    for (unsigned int coluna = 0; coluna < imagem.largura; ++coluna){
      int media = imagem.pixel[linha][coluna][0] +
                  imagem.pixel[linha][coluna][1] +
                  imagem.pixel[linha][coluna][2];
      media = media/3;
      imagem.pixel[linha][coluna][0] = media;
      imagem.pixel[linha][coluna][1] = media;
      imagem.pixel[linha][coluna][2] = media;
    }
  }

  return imagem;
}

/*A função BLUR também muda o valor de cada pixel da imagem para deixá-la com
um aspecto 'borrado'. O cálculo também envolve a média entre os pixels próximos.*/
void blur(unsigned int altura, unsigned short int pixel[LARGURA_MAXIMA][ALTURA_MAXIMA][3],
          int tamanho, unsigned int largura){
  for (unsigned int linha = 0; linha < altura; ++linha){
    for (unsigned int coluna = 0; coluna < largura; ++coluna){
      Pixel media = {0, 0, 0};
      int menor_altura;
      if(altura - 1 > linha + tamanho/2){
        menor_altura = linha + tamanho/2;
      }
      else{
        menor_altura = altura - 1;
      }
      int min_largura;
      if(largura - 1 > coluna + tamanho/2){
        min_largura = coluna + tamanho/2;
      }
      else{
        min_largura = largura - 1;
      }
      for(int x = (0 > linha - tamanho/2 ? 0 : linha - tamanho/2); x <= menor_altura; ++x){
        for(int y = (0 > coluna - tamanho/2 ? 0 : coluna - tamanho/2); y <= min_largura; ++y){
          media.r += pixel[x][y][0];
          media.g += pixel[x][y][1];
          media.b += pixel[x][y][2];
        }
      }

      media.r /= tamanho * tamanho;
      media.g /= tamanho * tamanho;
      media.b /= tamanho * tamanho;

      pixel[linha][coluna][0] = media.r;
      pixel[linha][coluna][1] = media.g;
      pixel[linha][coluna][2] = media.b;
    }
  }
}

/*Função que troca os pixels de posição sem alterar os valores.*/
Image rotacionar90direita(Image imagem){
  Image rotacionada;

  rotacionada.largura = imagem.altura;
  rotacionada.altura = imagem.largura;

  for (unsigned int linha = 0, y = 0; linha < rotacionada.altura; ++linha, ++y){
    for (int coluna = rotacionada.largura - 1, x = 0; coluna >= 0; --coluna, ++x){
      rotacionada.pixel[linha][coluna][0] = imagem.pixel[x][y][0];
      rotacionada.pixel[linha][coluna][1] = imagem.pixel[x][y][1];
      rotacionada.pixel[linha][coluna][2] = imagem.pixel[x][y][2];
    }
  }

  return rotacionada;
}

/*Função que inverte os valores dos pixels para obter o efeito 'negativo' na
imagem.*/
void inverter_cores(unsigned short int pixel[LARGURA_MAXIMA][ALTURA_MAXIMA][3],
                    unsigned int largura, unsigned int altura){
  for (unsigned int linha = 0; linha < altura; ++linha){
    for (unsigned int coluna = 0; coluna < largura; ++coluna){
      pixel[linha][coluna][0] = VALOR_MAXIMO_PIXEL - pixel[linha][coluna][0];
      pixel[linha][coluna][1] = VALOR_MAXIMO_PIXEL - pixel[linha][coluna][1];
      pixel[linha][coluna][2] = VALOR_MAXIMO_PIXEL - pixel[linha][coluna][2];
    }
  }
}

/*Função que recebe como parâmetro nova altura e largura e executa o corte.*/
Image cortar_imagem(Image imagem, int nova_largura, int nova_altura,
                    int largura_original, int altura_original){
  Image cortada;

  cortada.largura = largura_original;
  cortada.altura = altura_original;

  for(int linha = 0; linha < altura_original; ++linha){
    for(int coluna = 0; coluna < largura_original; ++coluna){
      cortada.pixel[linha][coluna][0] = imagem.pixel[linha + nova_altura]
                                                    [coluna + nova_largura][0];
      cortada.pixel[linha][coluna][1] = imagem.pixel[linha + nova_altura]
                                                    [coluna + nova_largura][1];
      cortada.pixel[linha][coluna][2] = imagem.pixel[linha + nova_altura]
                                                    [coluna + nova_largura][2];
    }
  }
  return cortada;
}

/*Função que compara um valor passado com o valor máximo(255).*/
int verifica_valor_pixel(int valor){
  if (VALOR_MAXIMO_PIXEL > valor){
    return valor;
  }
  else{
    return VALOR_MAXIMO_PIXEL;
  }
}

/*Função que altera os valores de cada pixel para obter um efeito 'envelhecido'
na imagem.*/
Image filtro_sepia(Image imagem){
  for (unsigned int linha = 0; linha < imagem.altura; ++linha){
    for (unsigned int coluna = 0; coluna < imagem.largura; ++coluna){
      unsigned short int pixel[3];
      pixel[0] = imagem.pixel[linha][coluna][0];
      pixel[1] = imagem.pixel[linha][coluna][1];
      pixel[2] = imagem.pixel[linha][coluna][2];

      int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
      int menor_r;
      menor_r = verifica_valor_pixel(p);
      imagem.pixel[linha][coluna][0] = menor_r;

      p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
      menor_r = verifica_valor_pixel(p);
      imagem.pixel[linha][coluna][1] = menor_r;

      p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
      menor_r = verifica_valor_pixel(p);
      imagem.pixel[linha][coluna][2] = menor_r;
    }
  }
  return imagem;
}

/*Função que lê os valores contidos nos arquivos de entrada.*/
Image ler_arquivo_entrada(Image imagem){
  // Lendo o Tipo da Imagem. O tipo P3 é relacionado ao tipo ppm.
  char p3[4];
  scanf("%s", p3);
  // ler largura, altura e cor_máxima da imagem.
  scanf("%u %u %d", &imagem.largura, &imagem.altura, &cor_maxima);

  // read all pixels of image
  for (unsigned int linha = 0; linha < imagem.altura; ++linha){
    for (unsigned int coluna = 0; coluna < imagem.largura; ++coluna){
      scanf("%hu %hu %hu", &imagem.pixel[linha][coluna][0],
                          &imagem.pixel[linha][coluna][1],
                          &imagem.pixel[linha][coluna][2]);

    }
  }
  return imagem;
}

void printa_imagem(Image imagem, int cor_maxima){
  printf("P3\n");
  // print larguraidth height and color of image
  printf("%u %u\n%d\n", imagem.largura, imagem.altura, cor_maxima);

  // print pixels of image
  for (unsigned int linha = 0; linha < imagem.altura; ++linha){
      for (unsigned int coluna = 0; coluna < imagem.largura; ++coluna){
          printf("%hu %hu %hu ", imagem.pixel[linha][coluna][0],
                                 imagem.pixel[linha][coluna][1],
                                 imagem.pixel[linha][coluna][2]);

      }
      printf("\n");
  }
}

Image espelhamento_vertical(Image imagem){
  int horizontal = 0;
  scanf("%d", &horizontal);
  int largura = imagem.largura;
  int altura = imagem.altura;
  if (horizontal == 1) largura /= 2;
  else altura = altura/2;

  for (int linha = 0; linha < altura; ++linha){
    for (int coluna = 0; coluna < largura; ++coluna){
      int nova_linha = linha;
      int nova_coluna = coluna;
      if (horizontal == 1) nova_coluna = imagem.largura - 1 - coluna;
      else nova_linha = imagem.altura - 1 - linha;

      Pixel aux1;
      aux1.r = imagem.pixel[linha][coluna][0];
      aux1.g = imagem.pixel[linha][coluna][1];
      aux1.b = imagem.pixel[linha][coluna][2];

      imagem.pixel[linha][coluna][0] = imagem.pixel[nova_linha][nova_coluna][0];
      imagem.pixel[linha][coluna][1] = imagem.pixel[nova_linha][nova_coluna][1];
      imagem.pixel[linha][coluna][2] = imagem.pixel[nova_linha][nova_coluna][2];

      imagem.pixel[nova_linha][nova_coluna][0] = aux1.r;
      imagem.pixel[nova_linha][nova_coluna][1] = aux1.g;
      imagem.pixel[nova_linha][nova_coluna][2] = aux1.b;
    }
  }
  return imagem;
}

#endif
