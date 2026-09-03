#include <stdio.h>
#include <stdlib.h>

int pgm_to_binary (char pgm[]) {

    char type[3];
    int width,height,shades_of_gray,num;

    FILE *in = fopen(pgm,"r");
    if(in == NULL) {
        printf("Erro ao abrir arquivo");
        return 0;
    }

    FILE *out = fopen("bin","wb+");
    if(out == NULL) {
        printf("Erro ao abrir arquivo");
        return 0;
    }

    fscanf(in,"%2s",type);
    type[1]='5';
    type[2]='\0';
    fwrite(type,sizeof(type),1,out);

    fscanf(in,"%d %d",&width,&height);
    fwrite(&width,sizeof(width),1,out);
    fwrite(&height,sizeof(height),1,out);

    fscanf(in,"%d",&shades_of_gray);
    fwrite(&shades_of_gray,sizeof(shades_of_gray),1,out);

    while ((fscanf(in,"%d",&num)) != EOF) {
        fwrite(&num,sizeof(num),1,out);
    }
    
    fclose(in);
    fclose(out);

    return 1;
}   

int thresholding(int L) {

    char type[3];
    int width,height,shades_of_gray,num,zero,C;
    FILE *bin = fopen("bin","rb");
    if(bin == NULL) {
        printf("Erro ao abrir arquivo");
        return 0;
    }

    FILE *out = fopen("limiarizado.pgm","w");
    if(out == NULL) {
        printf("Erro ao abrir arquivo");
        return 0;
    }

    fread(type,sizeof(type),1,bin);
    type[1]='2';
    fprintf(out,"%s\n",type);

    fread(&width,sizeof(width),1,bin);
    fread(&height,sizeof(height),1,bin);
    fprintf(out,"%d %d\n",width,height);

    fread(&shades_of_gray,sizeof(shades_of_gray),1,bin);
    fprintf(out,"%d",shades_of_gray);

    C = shades_of_gray;
    zero = 0;
    int i=0;
    while ((fread(&num,sizeof(num),1,bin)) != 0) {

        if(i%12==0)
            fputc('\n',out);

        if(num < L){
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
    printf("Digite o nome do arquivo para converter para binario: ");
    scanf("%s",pgm);

    if(!pgm_to_binary(pgm)) {
        return 0;
    }

    printf("\nArquivo convertido para binario com sucesso, agora escolha um valor de limiarizacao: ");
    scanf("%d",&L);

    if(thresholding(L))
        printf("\nSucesso na limiarizacao, confira o resultado\n");
    
    return 0;
}