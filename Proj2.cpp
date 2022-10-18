// NOME: GUILHERME CÂMARA MARQUES RA: 201154031
// NOME: BRUNO MOREIRA RA: 

#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_INSERT 9
#define SIZE_IDLIST 4

int clearBuffer() {    
  while ( getchar() != '\n' );
}

typedef struct veiculo {
  char cod_cli[12];
  char cod_vei[8];
  char client[50];
  char veiculo[50];
  char dias[4];
} veic_t;

typedef struct remove {
  char cod_cli[12];
  char cod_vei[8];
} reg_id_t;


int loadFiles(veic_t *regs_locs_vei, reg_id_t *regs_id_list) {
  FILE *insere, *busca;

  if ((insere = fopen("insere.bin", "rb")) == NULL) {
    printf("Nao foi possivel abrir o arquivo insere.bin");
    return 0;
  }

  if((busca = fopen("busca_p.bin", "rb")) == NULL) {
    printf("Nao foi possivel abrir o arquivo busca_p.bin");
    return 0;
  }

  for(int i=0; i<SIZE_INSERT; i++) {
    fread(&(regs_locs_vei[i]), sizeof(veic_t), 1, insere);
  }

  for(int i=0; i<SIZE_IDLIST; i++) {
    fread(&(regs_id_list[i]), sizeof(reg_id_t), 1, busca);
  }

  return 1;
}

int main() {
  veic_t regs_locs_vei[SIZE_INSERT]; //armazena os registros para serem inseridos
  reg_id_t regs_id_list[SIZE_IDLIST]; //armazena os registros para serem buscados
  int option;

  if(!(loadFiles(regs_locs_vei, regs_id_list))) { // chama loadFiles para carregar na memoria os arquivos insere.bin e remove.bin
    printf("Nao foi possivel carregar os arquivos");
    return 0;
  }

  for(int i=0; i<SIZE_INSERT; i++) {
    printf("%s|%s|%s|%s|%s \n", regs_locs_vei[i].cod_cli, regs_locs_vei[i].cod_vei, regs_locs_vei[i].client, regs_locs_vei[i].veiculo, regs_locs_vei[i].dias);
  }

  printf("\n");

  for(int i=0; i<SIZE_IDLIST; i++) {
    printf("%s|%s \n", regs_id_list[i].cod_cli, regs_id_list[i].cod_vei);
  }

  //Menu de opcoes:
  do {
    printf("=================================\n");
    printf("1- inserir registro\n");
    printf("2- buscar registro\n");
    printf("3 - sair\n");
    printf("=================================\n");
    scanf("%d", &option);
    clearBuffer();

    switch (option) {
    case 1: {
      /* code */
      break;
    }
    case 2: {
      /* code */
      break;
    }
    case 3: {
      printf("Saindo...\n");
      break;
    }
    default:
      printf("Opcao invalida, digite novamente\n\n");
      break;
    }

  } while(option != 3);


  return 1;
}