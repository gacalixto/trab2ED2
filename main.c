#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <windows.h>
#define tam 41
// Autores:Gabriel Calixto e Leonardo Bezerra

typedef struct liv
{
    char  ISBN[14],title[50],author[50],year[5];
    int next;
} Livro;

typedef struct ind
{
    int RRN,NEXT;
    char ISBN[14];


}INDEX;

typedef struct aut{

    char NAME[50];
    int RRN;
    

}AUTHOR;



int pega_registro(FILE *p_out, char *p_reg);
void openFile(FILE **fil,char *filname,char *stringmod);
void insertRegister(FILE* fil,FILE *index,FILE *index2,FILE *indexA, int user, Livro book, char *indexN,char*indexN2,char *indexN3);
void dumpFile(FILE *fil);
void hashSfile(FILE *fil,int size_data,Livro book,int old);
void read_booklist(FILE *fil,FILE *index,FILE *index2, FILE *indexA,FILE *bklist, char *indexN,char *indexN2,char *indexN3);
int searchRegister(FILE*fil,char *ISBN);
char* init_string(char *str, char w, int tama);
void print_book(Livro book);
int positInfile(FILE *fil, int position, int offset);
void orderIndex(INDEX *indexstr,int quantd_reg);
void searchByindex(FILE *fil, FILE *index);
void bufferindex(FILE *index, int *quantd_reg, INDEX *indstr);
void bufferindexA(FILE *indexA,FILE *index2, int *quantd,int *quant_reg,AUTHOR *LIST, INDEX *list);

int main()
{
    char opc;

    FILE *file=NULL,*bklist=NULL, *last,*index,*index2,*indexA;
    char filename[50], booklist[]="biblioteca.bin",lastfile[]="last.bin",defaultfile[]="library.bin";
    char indexs[20],a[10],indexs2[20],indexs3[20];
    Livro book;

    do
    {

        system("COLOR 0A");
        system("cls");
        printf("LIBRARY FILE MANAGER\n\n\n");
        printf(" [1]-INSERT INTO FILE\n [2] - Search\n [3]-Search a register\n [4]-Load FILE\n [5]-Dump FILE \n [6]-Load Book List \n [r]-Open Last File\n [e]-Close FILE & exit \n");
        opc=getch();

        switch(opc)
        {
        case '1':

            if(file)
            {

                insertRegister(file,index,index2,indexA,1,book,indexs,indexs2,indexs3);
                break;
            }
            system("cls");
            printf("File is not defined!!\n\nPRESS ANY BUTTON");
            getch();
            break;

        case '2':
            system("cls");
          if(file && index){

            searchByindex(file,index);
          }else{

              printf("Please Open File...");

          }
          getch();
        break;
        /*case '3':
            system("cls");
            if(file)
            {
                rewind(file);
                removeFile(filename);
                rewind(file);

            }

            else{
                printf("File Not Open!\nPress any key to go back to the menu!\n");
                getch();
            }
            system("cls");
            break;
            */
        case '4':
            system("cls");
            printf("FILE NAME: ");
            gets(filename);
            strcpy(indexs,filename);
             
             strcat(indexs,"_IND");
             strcat(indexs,".bin");
             openFile(&index,indexs,"a+b");
            strcpy(indexs,filename);
             strcat(indexs,"_A");
             strcat(indexs,".bin");
             strcpy(indexs2,indexs);
             openFile(&indexA,indexs,"a+b");
            strcpy(indexs,filename);
             strcat(indexs,"_IND2");
             strcat(indexs,".bin");
             strcpy(indexs3,indexs);
             openFile(&index2,indexs,"a+b");

             strcat(filename,".bin");
            
            openFile(&file,filename,"a+b");
            
            rewind(file);
            rewind(index);
            last = fopen(lastfile,"w+b");
            rewind(last);
            fwrite(filename,sizeof(char)*strlen(filename),1,last);
            fclose(last);
            break;

        case '5':
            printf("FILE NAME: ");
            gets(filename);
            openFile(&file,filename,"a+b");
            if(file)
                dumpFile(file);
            else{
                printf("File Not Open!\nPress any key to go back to the menu!\n");
                getch();
            }
            getch();
            system("cls");
            fclose(file);
            break;

        case '6':

            system("cls");

            printf("FILE NAME (book list): ");
           
            if((bklist=fopen(booklist,"rb"))!=NULL && file!=NULL)
            {

                rewind(bklist);
                rewind(file);
                read_booklist(file,index,index2,indexA,bklist,indexs,indexs2,indexs3);
                fclose(bklist);






            }
            else
            {

                printf("List Not found or i/o file not found !! \n\n Press Any key...");

                getch();

            }


            break;
        case 'r':

        	last = fopen(lastfile,"rb");
        	if(last){

				rewind(last);

				fgets(filename,sizeof(lastfile)*100,last);

        		openFile(&file,filename,"a+b");
            	rewind(file);
         	   	fclose(last);
        	}else{

        		openFile(&file,defaultfile,"a+b");
            	rewind(file);
         	   	fclose(last);


			}
        break;
        }

    }
    while(opc!='e');
    fclose(file);
    fclose(index);
    fclose(indexA);
    fclose(index2);
    return 0;
}
void openFile(FILE **fil,char *filname, char *stringmod)
{   
	int quant =0;
    
    *fil = fopen(filname,"r+b");

    if(!*fil)
    {

        printf("creating %s",filname);
        Sleep(1000);



        printf(".");
        Sleep(500);

        printf(".");
        Sleep(300);
        printf(".");
        Sleep(500);

        printf(".");
        Sleep(500);

        printf(".");
        Sleep(300);

        printf(".");
        fclose(*fil);

        *fil = fopen(filname,"ab");


        /*inicia lista de posi��es disponiveis v�zia */

        fwrite(&quant,sizeof(int),1,*fil);


    }
    fclose(*fil);

    *fil = fopen(filname,stringmod);
}

void dumpFile(FILE *fil)
{

    char ch[tam],ch2[tam],c[tam];
    int i;
    rewind(fil);
    system("cls");
    printf("$$ DUMP OF FILE      by:Calixtoguerreiro $$\n\n");
    while(fgets(ch,tam,fil)!= NULL)
    {
        i=0;
        while(i<tam-1)
        {
            if(isprint(ch[i]))
            {
                printf("%X ",ch[i]);
                ch2[i]=ch[i];
            }
            else if(!isprint(ch[i]))
            {
                ch2[i]='.';
                printf("00 ");

            }
            i++;

        }
        ch2[i]='\0';
		strcpy(ch,init_string(ch,' ',tam));
        printf("  %s\n",ch2);


    }
    printf("\n");
    printf("\n\n\nPRESS ANY KEY....");
    getch();

}

void insertRegister(FILE* fil,FILE *index,FILE *index2,FILE *indexA, int user, Livro book, char *indexN,char*indexN2,char *indexN3)
{
    INDEX indstr[tam],indlist[tam];
    AUTHOR indA[tam];
    int quant_reg=0,quant_A=0,AUTHORcont=0;
    int regSize,list,i,rrnatual;

    bufferindex(index,&quant_reg,indstr);
    bufferindexA(indexA,index2,&quant_A,&quant_reg,indA,indlist);


    

    getch();
    rewind(fil);
     /**/
    rrnatual=quant_reg;
    //APAGANDO ARQUIVO
    fclose(index);
    index = fopen(indexN,"wb");
    fclose(index);
    index = fopen(indexN,"a+b");
    fclose(indexA);
    index = fopen(indexN2,"wb");
    fclose(indexA);
    index = fopen(indexN2,"a+b");
    fclose(index2);
    index = fopen(indexN3,"wb");
    fclose(index2);
    index = fopen(indexN3,"a+b");
    




    if(user)
    {   do{
		    system("cls");
            printf("ISBN:");
            gets(book.ISBN);
        }while(strlen(book.ISBN !=13));
        printf("\nTitle:");
        gets(book.title);
        printf("\nAuthor:");
        gets(book.author);
        printf("\nYear:");
        gets(book.year);
    }
    regSize=strlen(book.ISBN) + strlen(book.author) + strlen(book.title) +strlen(book.year); // Soma de todos os tamanhos de strings da STRUCT
    
    
    
    strcpy(indstr[rrnatual].ISBN,book.ISBN);
    indstr[rrnatual].RRN = rrnatual;
     quant_reg++;
     rrnatual = quant_reg;
    orderIndex(indstr,rrnatual);

    rewind(index);
    
    fwrite(&quant_reg,sizeof(int),1,index); 
   
    for(i=0;i<quant_reg;i++){
        fwrite(indstr[i].ISBN,sizeof(indstr[i].ISBN),1,index);
        fwrite(&indstr[i].RRN,sizeof(indstr[i].RRN),1,index);
        fputc('|',index);
        //printf("%s  %d \n",indstr[i].ISBN,indstr[i].RRN);

    }

    
    rewind(fil);
	fread(&list,sizeof(int),1,fil);//Recebe primeiro inteiro do arquivo que indica a quantidade de registros do arquivo de dados
    fseek(fil,SEEK_END,SEEK_CUR); //Rola para o fim do arquivo atual
    hashSfile(fil,regSize,book,0);
    rewind(index);
    rewind(fil);
    printf("Livro Salvo\n !!\a");
    return;
}

void orderIndex(INDEX *indexstr,int quantd_reg){

    /** Essa função serve para gravar um novo ISBN e RRN no arquivo de índice de forma ordenada!*/ 
    int i,j;
    INDEX tmp;
     for(i=0;i<quantd_reg-1;i++){
          for(j=i+1;j<quantd_reg;j++){
              if(strcmp(indexstr[i].ISBN,indexstr[j].ISBN)>0)
              {
                  tmp = indexstr[i];
                  indexstr[i]=indexstr[j];
                  indexstr[j] = tmp;
              }

          }      
        }
    for(i=0;i<quantd_reg;i++) printf("%s",indexstr[i].ISBN);
    getch();
}

void hashSfile(FILE *fil,int size_data,Livro book,int old)
{

    /******** Essa fun��o tem como �nico e exclusivo Objetivo gravar um dado de um livro
    no arquivo,

    int size_data = Tamanho total da soma de strings da struct
    Livro book = struct livro
    int old = indica se � uma nova grava��o ou uma REgrava��o

    obs: se for regrava��o os primeiros bytes que indicam o tamanho da posi��o n�o mudam

    Exemplo de grava��o:

    -1 (size_data + 4)(ISBN)#(Titulo)#(Autor)#(Ano)|

    | a pipe indica final do registro

    *****************************************************************************/


    if(!old)
    {
        //fprintf(fil,"%d",size_data+4);isso aqui n�o estava funcionando, o fseek nao conseguia pular isso por algum motivo
        size_data=size_data+4;
        fwrite(&size_data,sizeof(int),1,fil);
    }
    else
    {
        fseek(fil,sizeof(int),SEEK_CUR);
    }

    fputs(book.ISBN,fil);
    fputc('#',fil);
    fputs(book.title,fil);
    fputc('#',fil);
    fputs(book.author,fil);
    fputc('#',fil);
    fputs(book.year,fil);
    fputc('|',fil);






}
int searchRegister(FILE *fil,char *ISBN)
{
    /*Fun��o para percorrer o arquivo e encontrar o ISBN desejado
        que retorna um n�mero equivalente � posi��o do registro
    */
    int pos,count=0,ct,list,achou=0,tamreg,tamcampo,ptnext;
    char registro[60],campo[26],cha;
    rewind(fil);
    fseek(fil,sizeof(char)*4,SEEK_CUR);
    tamreg = pega_registro(fil,registro);
    while(achou!=1&&tamreg>0)
    {
        pos=0;
        tamcampo = get_field(registro,&pos,campo);
        if(strcmp(ISBN,campo)==0)
            achou=1;

        else
        {
            tamreg = pega_registro(fil,registro);
            count++;
        }


    }
    if(achou==0)
    {
        count = -2;
    }
    return count;
}

int pega_registro(FILE *p_out, char *p_reg)
{
    char cha;
     int bytes=0;


     if (!fread(&bytes, sizeof(int), 1, p_out))
       return 0;
     else if(bytes<=60){
            fread(p_reg, bytes, 1, p_out);
            return bytes;
          }
}

int get_field(char *p_registro, int *p_pos, char *p_campo)
{
    char ch;
    int i=0;
    p_campo[i] = '\0';

    ch = p_registro[*p_pos];
    while ((ch != '#') && (ch!=EOF) && ch!='|')
    {
        p_campo[i] = ch;
        i++;
        ch = p_registro[++(*p_pos)];
    }
    ++(*p_pos);
    p_campo[i] = '\0';

    return strlen(p_campo);
}

void read_booklist(FILE *fil,FILE *index,FILE *index2, FILE *indexA,FILE *bklist, char *indexN,char *indexN2,char *indexN3)
{

    Livro book;
	system("cls");
	printf("Add BOOK \n\n");
    while(	fread(book.ISBN,sizeof(book.ISBN),1,bklist) )
    {

        fread(book.title,sizeof(book.title),1,bklist);
        fread(book.author,sizeof(book.author),1,bklist);
        fread(book.year,sizeof(book.year),1,bklist);
        insertRegister(fil,index,index2,indexA,0,book,indexN,indexN2,indexN3);

		print_book(book);

    }

	printf("Press Any Key...");
	getch();

}

char* init_string(char *str,char w,int tama){
	int i;

	for(i=0;i<tama-1;i++){

		str[i]=w;


	}
	str[i]='\0';


	return str;
}

void print_book(Livro book){

	printf("ISBN: %s \n",book.ISBN);
	printf("Title: %s \n ",book.title);
	printf("Author:  %s \n",book.author);
	printf("Year: %s\n\n",book.year);




}

int positInfile(FILE *fil, int position,int offset){

	//Essa Fun��o Recebe uma posi��o e anda no arquivo at� encontrar a posi��o
    //OFFSET =O REGISTRO VARIÁVEL; 
    //OFFSET > 0 REGISTRO FIXO;

	int INTAUX,i=0,end;
	rewind(fil);
	fseek(fil,sizeof(int),SEEK_CUR);
    
        while(fread(&INTAUX,sizeof(int),1,fil)!=EOF && i<position){

            printf("skip INTAUX: %d\n",INTAUX);
            if(!offset){
                fseek(fil,sizeof(char)*INTAUX,SEEK_CUR);
                
            }else{
                fseek(fil,sizeof(char)*offset,SEEK_CUR);

            }
            i++;

        }
   
    
	

	return INTAUX;





}

void searchByindex(FILE *fil, FILE *index){
         char ISBN[13],reg[65],campo[50];
        INDEX indstr[tam];
        int quant_reg,i,flag=1,j,tamreg,tamcamp,pos=0;
        Livro book;
        system("cls");
        printf("Type the ISBN: ");
        gets(ISBN);

        bufferindex(index,&quant_reg,indstr);

        system("cls");

        printf("A - primary search \nB - Secondary search\n EXIT -  ANY KEY");

        switch(toupper(getch())){

            case 'A':

                i=0;
                while(i<quant_reg && flag ) {

                    flag=strcmp(ISBN,indstr[i].ISBN);
                    j=i;
                    i++;

                }
                system("cls");
                if(!flag) {

                    
                    positInfile(fil,indstr[j].RRN,0);
                    fseek(fil,-4,SEEK_CUR);

                    
                    
                    tamreg = pega_registro(fil,reg);
                    
                    system("cls");
                    pos = 0;
                    tamcamp = get_field(reg,&pos,campo);
                    printf("ISBN: %s\n",campo);
                    tamcamp = get_field(reg,&pos,campo);
                    printf("Title: %s\n",campo);
                    tamcamp = get_field(reg,&pos,campo);
                    printf("Author: %s\n",campo);
                    tamcamp = get_field(reg,&pos,campo);
                    printf("Year: %s\n",campo);




                    
                  
                    printf("\n\n\n Size: %d bytes",tamreg);
                    rewind(fil);
                    

                }else{

                    printf("Book Not Found");

                }


            break;

            case 'B':

                //SECONDARY SEARCH;

            break;





        }



    
}

void bufferindex(FILE *index, int *quantd_reg, INDEX *indstr){

    
    int i;

    
   
    rewind(index);
    fread(quantd_reg,sizeof(int),1,index);
    
    printf("loaded  %d registers \n",*quantd_reg);//lê quantidade de registros presentes no INDEX
    /* FOR usado pra carregar o vetor de struct de indices*/
    for(i=0;i<*quantd_reg;i++){
        fread(indstr[i].ISBN,sizeof(indstr[i].ISBN),1,index);
        fread(&indstr[i].RRN,sizeof(indstr[i].RRN),1,index); 
        /*printtest*/
        
        fseek(index,1,SEEK_CUR);           
    }
    
}

void bufferindexA(FILE *indexA,FILE *index2, int *quantd,int *quant_reg,AUTHOR *LIST, INDEX *list){
    int i=0,j=0,aux;
    fread(quantd,sizeof(int),1,indexA);
  //leo
    while(i<*quantd){

        fread(LIST[i].NAME,sizeof(LIST[i].NAME),1,indexA);
        fread(&LIST[i].RRN,sizeof(LIST[i].RRN),1,indexA);
            aux = LIST[i].RRN;
        while(aux!=-1 && j<=*quant_reg){

                positInfile(index2,LIST[i].RRN,17);
                fread(list[j].ISBN,sizeof(list[j].ISBN),1,index2);
                fread(&list[j].RRN,sizeof(list[j].RRN),1,index2);
                aux = list[j].RRN;
                j++;




        }
        i++;
    }




}
