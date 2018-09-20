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
    int RRN;
    char ISBN[14];


}INDEX;

int pega_registro(FILE *p_out, char *p_reg);
void openFile(FILE **fil,char *filname,char *stringmod);
void insertFile(FILE* fil,FILE *index,int user, Livro book);
void dumpFile(FILE *fil);
void hashSfile(FILE *fil,int size_data,Livro book,int old);
void read_booklist(FILE *fil,FILE *index, FILE *bklist);
void removeFile(char *filename);
int searchRegister(FILE*fil,char *ISBN);
char* init_string(char *str, char w, int tama);
void print_book(Livro book);
int positInfile(FILE *fil, int position, int offset);

int main()
{
    char opc;

    FILE *file=NULL,*bklist=NULL, *last,*index;
    char filename[50], booklist[]="biblioteca.bin",lastfile[]="last.bin",defaultfile[]="library.bin";
    char indexs[20],a[10];
    Livro book;

    do
    {

        system("COLOR 0A");
        system("cls");
        printf("LIBRARY FILE MANAGER\n\n\n");
        printf(" [1]-INSERT INTO FILE\n\n [3]-Remove a register from the file\n\n [4]-Load FILE\n\n [5]-Dump FILE \n\n [6]-Load Book List \n\n [r]-Open Last File\n\n [e]-Close FILE & exit \n");
        opc=getch();

        switch(opc)
        {
        case '1':

            if(file)
            {

                insertFile(file,index,1,book);
                break;
            }
            system("cls");
            printf("File is not defined!!\n\nPRESS ANY BUTTON");
            getch();
            break;

        case '3':
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
        case '4':
            system("cls");
            printf("FILE NAME: ");
            gets(filename);
            strcpy(indexs,filename);
             strcat(filename,".bin");
             strcat(indexs,"_IND");
             strcat(indexs,".bin");
            
            openFile(&file,filename,"a+b");
            openFile(&index,indexs,"a+b");
            rewind(file);
            rewind(index);
            last = fopen(lastfile,"w+b");
            rewind(last);
            fwrite(filename,sizeof(char)*strlen(filename),1,last);
            fclose(last);
            break;

        case '5':
            if(file)
                dumpFile(file);
            else{
                printf("File Not Open!\nPress any key to go back to the menu!\n");
                getch();
            }
            getch();
            system("cls");
            break;

        case '6':

            system("cls");

            printf("FILE NAME (book list): ");
           
            if((bklist=fopen(booklist,"rb"))!=NULL && file!=NULL)
            {

                rewind(bklist);
                rewind(file);
                read_booklist(file,index,bklist);
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

void insertFile(FILE* fil,FILE *index, int user, Livro book)
{
    INDEX indstr[tam];

    int regSize,list,quant_reg;
    rewind(fil);
    fread(&quant_reg,sizeof(int),1,index);



    if(user)
    {
		system("cls");
        printf("ISBN:");
        gets(book.ISBN);
        printf("\nTitle:");
        gets(book.title);
        printf("\nAuthor:");
        gets(book.author);
        printf("\nYear:");
        gets(book.year);
    }
    regSize=strlen(book.ISBN) + strlen(book.author) + strlen(book.title) +strlen(book.year); // Soma de todos os tamanhos de strings da STRUCT
    rewind(fil);

	fread(&list,sizeof(int),1,fil);//Recebe primeiro inteiro do arquivo que indica a primeira posi��o da lista (-1 se a lista estiver vazia)

   
        printf("Livro Salvo\n !!\a");

        fseek(fil,SEEK_END-SEEK_CUR,SEEK_CUR); //Rola para o fim do arquivo atual
        hashSfile(fil,regSize,book,0);
        fwrite(&quant_reg,sizeof(int),1,index);
        fputs(book.ISBN,index);
        rewind(index);
        rewind(fil);
        quant_reg++;
        fwrite(&quant_reg,sizeof(int),1,index);
        fwrite(&quant_reg,sizeof(int),1,index);


        return;

   



}

void orderIndex(INDEX indexstr,FILE *index){
    
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


void removeFile(char *filename)
{
    char ISBN[14],reg[60],*stringnull;
    int proc=0,bytesproc=0,firstposition;
    FILE* fil;

    fil=fopen(filename,"r+b");
    if(!fil) printf("lok�o");

    printf("ISBN:");
    gets(ISBN);

    proc = searchRegister(fil,ISBN);

    if(proc!=-2)
    {
        rewind(fil);
    	fread(&firstposition,sizeof(int),1,fil);
    	rewind(fil);
    	bytesproc= positInfile(fil,proc,0);
    	stringnull = (char*)malloc(sizeof(char)*(bytesproc-5));

    	init_string(stringnull,'.',bytesproc-5);


        fputc('*',fil);
        fwrite(&firstposition,sizeof(int),1,fil);

    	fwrite(stringnull,sizeof(stringnull),1,fil);
    	rewind(fil);
    	fwrite(&proc,sizeof(int),1,fil);
    	printf("ISBN found! \n Press any key to continue");
        getch();
        //fwrite('*',sizeof(char),1,fil);//isso n�o est� funcionando ainda
        rewind(fil);
        //fprintf(fil,"%d ",proc);

    }
    else
    {
        printf("ISBN not found!\nPress any key to continue");
        getch();
    }


    fclose(fil);


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
    while ((ch != '#') && (ch!=EOF))
    {
        p_campo[i] = ch;
        i++;
        ch = p_registro[++(*p_pos)];
    }
    ++(*p_pos);
    p_campo[i] = '\0';

    return strlen(p_campo);
}

void read_booklist(FILE *fil,FILE *index,FILE *bklist)
{

    Livro book;
	system("cls");
	printf("Add BOOK \n\n");
    while(	fread(book.ISBN,sizeof(book.ISBN),1,bklist) )
    {

        fread(book.title,sizeof(book.title),1,bklist);
        fread(book.author,sizeof(book.author),1,bklist);
        fread(book.year,sizeof(book.year),1,bklist);
        insertFile(fil,index,0,book);

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
