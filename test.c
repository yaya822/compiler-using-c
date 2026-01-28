#include<stdio.h>

    typedef struct tab{
        char c;
        struct tab* next;
    }tab;
    char ch;
    char mot[100];
    fptr = fopen("execute.txt", "r");
    fptr2 = fopen("output.txt", "w");
    fptr_dictionnaire=fopen("dictionnaire.txt","r");
    fptr_error= fopen("error.txt", "w");

    // analyse lexiquale : decouper le texte en tokens 

    if (fptr != NULL){
        ch=fgetc(fptr);
        tab* t=(tab*)malloc(sizeof(tab));
        tab *courant=t;
        while (ch!=EOF){
             printf("%c",ch);
            if(ch!=' ' && ch!='\n'){
                courant->c=ch;
                courant=courant->next;                
            }

            else{
                if(courant!=NULL){
                    courant->next->c='\0';
                for(tab* head=t;head!=NULL;head=head->next){
                    fprintf( fptr2, "%c" ,head->c);
                
                }
                fprintf(fptr2,"\n");
                 i=0;
            }
            }
            ch=fgetc(fptr);
        }

     if(courant!=NULL){
                    courant->next->c='\0';
                for(tab* head=t;head!=NULL;head=head->next){
                    fprintf( fptr2, "%c" ,head->c);
                
                }
                fprintf(fptr2,"\n");
                 i=0;
            }
}    
    else{
        printf("not able to open the file ");

        }


    fclose(fptr2);