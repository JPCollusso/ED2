//Integrantes do grupo: João Pedro Meneghetti Collusso e Vitor Barreto Carvalho
#include <stdio.h>
#include <stdlib.h>

//Converte arquivo pgm do tipo P2 para um arquivo binário customizado temporário. Retorna 0 se houver algum erro durante a conversão e retorna 1 caso contrário
int pgm_to_binary (char pgm[]) { 

    char type[3]; //variável para armazenar a string do tipo original
    int width,height,shades_of_gray,pixels;

    //Abre arquivo informado pelo usuário no modo leitura
    FILE *in = fopen(pgm,"r");
    if(in == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 0;
    }

    //Abre um arquivo binário para leitura e gravação
    FILE *out = fopen("bin","wb+");
    if(out == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 0;
    }

    //Lê tipo P2 do arquivo pgm informado pelo usuário e escreve a string "P5" no arquivo bin
    fscanf(in,"%2s",type);
    type[1]='5';
    type[2]='\0';
    fwrite(type,sizeof(type),1,out);

    //Lê as demais informações de texto do cabeçalho do arquivo pgm e escreve como inteiros (4 bytes) no arquivo bin
    fscanf(in,"%d %d",&width,&height);
    fwrite(&width,sizeof(width),1,out);
    fwrite(&height,sizeof(height),1,out);

    fscanf(in,"%d",&shades_of_gray);
    fwrite(&shades_of_gray,sizeof(shades_of_gray),1,out);

    //Lê os valores de cinza de cada pixel do arquivo pgm e escreve como inteiros (4 bytes) no formato binário no arquivo bin
    while ((fscanf(in,"%d",&pixels)) != EOF) {
        fwrite(&pixels,sizeof(pixels),1,out);
    }
    
    fclose(in);
    fclose(out);

    return 1;
}   

//Aplica a limiarização a partir de um valor L informado pelo usuário. Retorna 0 se houver algum erro durante a limiarização e retorna 1 caso contrário
int thresholding(int L) {

    char type[3];
    int width,height,shades_of_gray,pixels,zero,C;

    //Abre arquivo bin, gerado na função pgm_to_binary, no modo leitura
    FILE *bin = fopen("bin","rb");
    if(bin == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 0;
    }

    //Abre arquivo limiarizado.pgm para escrita
    FILE *out = fopen("limiarizado.pgm","w");
    if(out == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 0;
    }

    //Lê a string do tipo armazenada no arquivo binário e escreve P2 no arquivo limiarizado.pgm
    fread(type,sizeof(type),1,bin);
    type[1]='2';
    fprintf(out,"%s\n",type);

    //Lê as demais informações do cabeçalho (que estão em formato de inteiros) do arquivo binário e escreve como texto em limiarizado.pgm
    fread(&width,sizeof(width),1,bin);
    fread(&height,sizeof(height),1,bin);
    fprintf(out,"%d %d\n",width,height);

    fread(&shades_of_gray,sizeof(shades_of_gray),1,bin);
    fprintf(out,"%d",shades_of_gray);

    C = shades_of_gray;
    zero = 0;
    int i=0;

    //Lê cada pixel (inteiro) do arquivo bin e escreve no arquivo limiarizado.pgm aplicando o processo de limiarização
    while ((fread(&pixels,sizeof(pixels),1,bin)) != 0) {

        if(i%12==0) //Detalhe de implementação para ter 12 pixels por linha no arquivo limiarizado.pgm
            fputc('\n',out);

        if(pixels < L){
            fprintf(out,"%d ",zero);
        }

        else {
            fprintf(out,"%d ",C);
        }

        i++;
    }

    return 1;
}

int main() {
    char pgm[100];
    int L;

    //Lê o nome do arquivo a ser transformado para binário
    printf("Digite o nome do arquivo para converter para binario: ");
    scanf("%s",pgm);

    //Encerra o programa caso ocorra um erro na função pgm_to_binary
    if(!pgm_to_binary(pgm)) {
        return 0;
    }

    //Lê valor L de limiarização
    printf("\nArquivo convertido para binario com sucesso, agora escolha um valor de limiarizacao: ");
    scanf("%d",&L);

    //Encerra o programa caso ocorra um erro na função de limiarização
    if(!thresholding(L)) {
        return 0;
    }

    printf("\nSucesso na limiarizacao, confira o resultado\n");
    
    return 0;
}