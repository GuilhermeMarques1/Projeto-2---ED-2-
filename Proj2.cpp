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

typedef struct index_struct {
  char key[19];
  int byteOffset;
} index_st;

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
    fread(&(regs_id_list[i]), sizeof(reg_id_t), 1 , busca);
  }

  fclose(insere);
  fclose(busca);
  return 1;
}

int createIndexArray(FILE *data, index_st *indexArray) {
  char ch, reg_size, cod_cli[11], cod_vei[7];
  int position=0, i=0, num_register=0;

  while(fread(&reg_size, sizeof(char), 1, data)) {
    fread(&(indexArray[i].key), sizeof(indexArray->key), 1, data);
    indexArray[i].key[19] = '\0';
    indexArray[i].byteOffset = position;
    num_register++;

    position += reg_size+1;
    fseek(data, position, SEEK_SET);
    i++;
  }

  return num_register;
}

int compareKeys(index_st index1, index_st index2) { //Teste se index1.key > index2.key
  for(int i=0;i<19;i++) {
    if(index1.key[i] == '|')
      continue;

    if(int(index1.key[i]) > int(index2.key[i])) {
      return 1;
    }

    if(int(index1.key[i]) < int(index2.key[i])) {
      return 0;
    }
  }

  return 0;
}

void quick_sort(index_st *indexArray, int left, int right) {
  int i, j;
  index_st x, y;
     
  i = left;
  j = right;
  x = indexArray[(left + right) / 2];
     
  while(i <= j) {
    while(compareKeys(x, indexArray[i]) && i < right) {
      i++;
    }
    while(compareKeys(indexArray[j], x) && j > left) {
      j--;
    }
    if(i <= j) {
      y = indexArray[i];
      indexArray[i] = indexArray[j];
      indexArray[j] = y;
      i++;
      j--;
    }
  }
  
  if(j > left) {
    quick_sort(indexArray, left, j);
  }
  if(i < right) {
    quick_sort(indexArray, i, right);
  }
}

void pushRegisterInArray(index_st *indexArray, veic_t insert_register, int num_register, int position) {  //Adiciona um novo registro no fim do array
  char key[19];
  
  sprintf(key, "%s|%s", insert_register.cod_cli, insert_register.cod_vei);
  key[19] = '\0';

  strcpy(indexArray[num_register].key, key);
  indexArray[num_register].byteOffset = position;
}

void insert(FILE *data, veic_t *regs_locs_vei, index_st *indexArray, int *num_register) {
  int option, reg_size, position=0;
  char insert_register[124], ch;

  printf("\n=================================\n");
  printf("Digite o numero da opcao que deseja inserir:\n\n");

  for(int i=0;i<SIZE_INSERT;i++) {
    printf("%d- ", i+1);
    if(!(strcmp(regs_locs_vei[i].cod_cli, "***"))) { //Evita listar registros ja inseridos
      printf("***\n"); 
      continue;
    }
    printf("%s|", regs_locs_vei[i].cod_cli);
    printf("%s|", regs_locs_vei[i].cod_vei);
    printf("%s|", regs_locs_vei[i].client);
    printf("%s|", regs_locs_vei[i].veiculo);
    printf("%s| \n", regs_locs_vei[i].dias);
  }
  scanf("%d", &option);
  clearBuffer();

  while((option < 1 || option > SIZE_INSERT) || !(strcmp(regs_locs_vei[option-1].cod_cli, "***"))) { //verifica se a opcao e valida
    printf("Opcao invalida, por favor digite novamente: ");
    scanf("%d", &option);
    clearBuffer();
  }
  option--;            
  
  sprintf(insert_register, "%s|%s|%s|%s|%s|", regs_locs_vei[option].cod_cli, regs_locs_vei[option].cod_vei, regs_locs_vei[option].client, regs_locs_vei[option].veiculo, regs_locs_vei[option].dias);
  reg_size = strlen(insert_register);

  rewind(data);
  while(fread(&ch, sizeof(char), 1, data)){ //posiciona no fim do arquivo
    position++;
  }
  fwrite(&reg_size, sizeof(char), 1, data);
  fwrite(insert_register, sizeof(char), reg_size, data); //insere o registro no fim do arquivo

  printf("position:%d \n", position);
  pushRegisterInArray(indexArray, regs_locs_vei[option], *num_register, position);
  (*num_register)++;

  strcpy(regs_locs_vei[option].cod_cli, "***"); //adiciona marcador no campo cod_cli para indicar que o registro ja foi inserido
  return;
}

int main() {
  veic_t regs_locs_vei[SIZE_INSERT]; //armazena os registros para serem inseridos
  reg_id_t regs_id_list[SIZE_IDLIST]; //armazena os registros para serem buscados
  FILE *data;
  index_st indexArray[25];
  int option, num_register;

  if(!(loadFiles(regs_locs_vei, regs_id_list))) { // chama loadFiles para carregar na memoria os arquivos insere.bin e remove.bin
    printf("Nao foi possivel carregar os arquivos");
    return 0;
  }

  if(!(data = fopen("data.bin", "rb"))) { //Verifica se o arquivo de dados já existe
    if((data = fopen("data.bin", "w+b")) == NULL) { //Se nao existir será criado
      printf("Nao foi possivel criar o arquivo data.bin");
      return 0;
    }
  } else { //E se ja existir, vai abrir o arquivo para escrita e leitura
    fclose(data);
    if((data = fopen("data.bin", "r+b")) == NULL) {
      printf("Nao foi possivel abrir o arquivo data.bin");
      return 0;
    }
  }

  num_register = createIndexArray(data, indexArray); //Cria o index na memoria principal, a partir do arquivo de dados

  printf("\n\n");
  quick_sort(indexArray, 0, num_register-1); //Organiza em ordem crescente

  //Menu de opcoes:
  do {
    printf("=================================\n");
    printf("1- inserir registro\n");
    printf("2- buscar registro\n");
    printf("3- sair\n");
    printf("=================================\n");
    scanf("%d", &option);
    clearBuffer();

    switch (option) {
    case 1: {
      insert(data, regs_locs_vei, indexArray, &num_register); //insere registros no fim do arquivo
      quick_sort(indexArray, 0, num_register-1);
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


  fclose(data);
  return 1;
}