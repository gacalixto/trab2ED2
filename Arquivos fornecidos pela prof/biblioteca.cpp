#include<stdio.h>
#include<conio.h>

int main() {
    FILE *fd;
    
    //////////////////////////////
    struct livro {
        char isbn[14];
        char titulo[50];
        char autor[50];
        char ano[5];
    } vet[3] = {{"1111111111111", "Dom Quixote", "Autor-T", "1600"},
                {"2222222222222", "A Parte Que Falta", "Silverstein", "2016"},
                {"3333333333333", "O Poder do Hábito", "Autor-T", "2015"}};
       
    fd = fopen("biblioteca.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct busca_p {
        char isbn[14];
    } vet_bp[3] = {{"1111111111111"},
                {"7777777777777"},
                {"3333333333333"}};
       
    fd = fopen("busca_p.bin", "w+b");
    fwrite(vet_bp, sizeof(vet_bp), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	const int quantidade = 2;
    char nomes[quantidade][50] = {"Autor-T", "Silverstein"};
       
    fd = fopen("busca_s.bin", "w+b");
    for (int i=0; i<quantidade; i++)
       fwrite(nomes[i], sizeof(char), 50, fd);
    fclose(fd);
    
    /*char buffer[50];
    fd = fopen("busca_s.bin", "r+b");
    for (int i=0; i<quantidade; i++)
       {
         fread(buffer, sizeof(buffer), 1, fd);
         printf("\n%s",buffer);
       }
    fclose(fd);
    getch();*/
}
