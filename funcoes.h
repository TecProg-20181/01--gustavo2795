#ifndef __funcoes_h
#define __funcoes_h

#include <stdio.h>
#include "estrutura.h"

//Declaração de constantes
#define VALOR_MAXIMO_PIXEL 255

//Variável Global que armazena a cor máxima de um Pixel.
int cor_maxima;

/*Função que manipula pixel à pixel para torná-los na escala de cinza,
através da média dos valores entre eles.*/
Image escala_de_cinza(Image imagem){
    for (unsigned int i = 0; i < imagem.altura; ++i){
        for (unsigned int j = 0; j < imagem.largura; ++j){
            int media = imagem.pixel[i][j][0] +
                        imagem.pixel[i][j][1] +
                        imagem.pixel[i][j][2];
            media = media/3;
            imagem.pixel[i][j][0] = media;
            imagem.pixel[i][j][1] = media;
            imagem.pixel[i][j][2] = media;
        }
    }

    return imagem;
}

void blur(unsigned int altura, unsigned short int pixel[512][512][3], int tamanho, unsigned int largura){
    for (unsigned int i = 0; i < altura; ++i){
        for (unsigned int j = 0; j < largura; ++j){
            Pixel media = {0, 0, 0};
            int menor_altura;
            if(altura - 1 > i + tamanho/2){
              menor_altura = i + tamanho/2;
            }
            else{
              menor_altura = altura - 1;
            }
            int min_largura;
            if(largura - 1 > j + tamanho/2){
              min_largura = j + tamanho/2;
            }
            else{
              min_largura = largura - 1;
            }
            for(int x = (0 > i - tamanho/2 ? 0 : i - tamanho/2); x <= menor_altura; ++x){
                for(int y = (0 > j - tamanho/2 ? 0 : j - tamanho/2); y <= min_largura; ++y){
                    media.r += pixel[x][y][0];
                    media.g += pixel[x][y][1];
                    media.b += pixel[x][y][2];
                }
            }

            media.r /= tamanho * tamanho;
            media.g /= tamanho * tamanho;
            media.b /= tamanho * tamanho;

            pixel[i][j][0] = media.r;
            pixel[i][j][1] = media.g;
            pixel[i][j][2] = media.b;
        }
    }
}

Image rotacionar90direita(Image imagem){
    Image rotacionada;

    rotacionada.largura = imagem.altura;
    rotacionada.altura = imagem.largura;

    for (unsigned int i = 0, y = 0; i < rotacionada.altura; ++i, ++y){
        for (int j = rotacionada.largura - 1, x = 0; j >= 0; --j, ++x){
            rotacionada.pixel[i][j][0] = imagem.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = imagem.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = imagem.pixel[x][y][2];
        }
    }

    return rotacionada;
}

void inverter_cores(unsigned short int pixel[512][512][3],
                    unsigned int largura, unsigned int h){
    for (unsigned int i = 0; i < h; ++i){
        for (unsigned int j = 0; j < largura; ++j){
            pixel[i][j][0] = VALOR_MAXIMO_PIXEL - pixel[i][j][0];
            pixel[i][j][1] = VALOR_MAXIMO_PIXEL - pixel[i][j][1];
            pixel[i][j][2] = VALOR_MAXIMO_PIXEL - pixel[i][j][2];
        }
    }
}

Image cortar_imagem(Image imagem, int nova_largura, int nova_altura,
                    int largura_original, int altura_original){
  Image cortada;

  cortada.largura = largura_original;
  cortada.altura = altura_original;

  for(int i = 0; i < altura_original; ++i){
    for(int j = 0; j < largura_original; ++j){
      cortada.pixel[i][j][0] = imagem.pixel[i + nova_altura][j + nova_largura][0];
      cortada.pixel[i][j][1] = imagem.pixel[i + nova_altura][j + nova_largura][1];
      cortada.pixel[i][j][2] = imagem.pixel[i + nova_altura][j + nova_largura][2];
    }
  }
  return cortada;
}

int verifica_valor_pixel(int valor){
  if (VALOR_MAXIMO_PIXEL > valor){
    return valor;
  }
  else{
    return VALOR_MAXIMO_PIXEL;
  }
}

Image filtro_sepia(Image imagem){
  for (unsigned int x = 0; x < imagem.altura; ++x){
    for (unsigned int j = 0; j < imagem.largura; ++j){
      unsigned short int pixel[3];
      pixel[0] = imagem.pixel[x][j][0];
      pixel[1] = imagem.pixel[x][j][1];
      pixel[2] = imagem.pixel[x][j][2];

      int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
      int menor_r;
      menor_r = verifica_valor_pixel(p);
      imagem.pixel[x][j][0] = menor_r;

      p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
      menor_r = verifica_valor_pixel(p);
      imagem.pixel[x][j][1] = menor_r;

      p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
      menor_r = verifica_valor_pixel(p);
      imagem.pixel[x][j][2] = menor_r;
    }
  }
  return imagem;
}

Image ler_arquivo_entrada(Image imagem){
  // ler largura, altura e cor_máxima da imagem.
  scanf("%u %u %d", &imagem.largura, &imagem.altura, &cor_maxima);

  // read all pixels of image
  for (unsigned int i = 0; i < imagem.altura; ++i){
    for (unsigned int j = 0; j < imagem.largura; ++j){
      scanf("%hu %hu %hu", &imagem.pixel[i][j][0],
                          &imagem.pixel[i][j][1],
                          &imagem.pixel[i][j][2]);

    }
  }
  return imagem;
}

void printa_imagem(Image imagem, int cor_maxima){
  printf("P3\n");
  // print larguraidth height and color of image
  printf("%u %u\n%d\n", imagem.largura, imagem.altura, cor_maxima);

  // print pixels of image
  for (unsigned int i = 0; i < imagem.altura; ++i){
      for (unsigned int j = 0; j < imagem.largura; ++j){
          printf("%hu %hu %hu ", imagem.pixel[i][j][0],
                                 imagem.pixel[i][j][1],
                                 imagem.pixel[i][j][2]);

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

  for (int i2 = 0; i2 < altura; ++i2){
    for (int j = 0; j < largura; ++j){
      int x = i2, y = j;
      if (horizontal == 1) y = imagem.largura - 1 - j;
      else x = imagem.altura - 1 - i2;

      Pixel aux1;
      aux1.r = imagem.pixel[i2][j][0];
      aux1.g = imagem.pixel[i2][j][1];
      aux1.b = imagem.pixel[i2][j][2];

      imagem.pixel[i2][j][0] = imagem.pixel[x][y][0];
      imagem.pixel[i2][j][1] = imagem.pixel[x][y][1];
      imagem.pixel[i2][j][2] = imagem.pixel[x][y][2];

      imagem.pixel[x][y][0] = aux1.r;
      imagem.pixel[x][y][1] = aux1.g;
      imagem.pixel[x][y][2] = aux1.b;
    }
  }
  return imagem;
}

#endif
