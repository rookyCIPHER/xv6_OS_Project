#include "user.h"
#include "fcntl.h"


#define MAX_USERNAME_LENGTH 100
#define MAX_PSW_LENGTH 200
#define MAX_USERS_ALLOWED 5
#define NULL


int string_compare(char* a,char* b){
    while(*a!='\0' && *b!='\0'){
        if(*a!=*b){
            return 0;
        }
        a++,b++;
    }

    if(*a=='\0' && *b=='\0'){return 1;}
    return 0;
}

void read_login(){
    int file=open("user_data.txt", O_RDONLY);


    //When OS is started for the very first time 
    if(file==-1){

        //creating the user login data file
        file=open("user_data.txt", O_CREATE | O_WRONLY);
        char* username="root";
        char* psw="admin";
        char* buffer="root,admin\n";
        int length = strlen(buffer);
        int bytesWritten = write(file,buffer,length);

        if(bytesWritten!=length){
            printf(2,"Error in writing to file-1!\n");
        }

        close(file);


        //creating the current user file
        file=open("current_user.txt", O_CREATE | O_WRONLY);
        bytesWritten = write(file,username,strlen(username));
        if(bytesWritten!=strlen(username)){
            printf(2,"Error in writing to file-2!\n");
        }

        close(file);


        


        //authenticating user id
        int flag=0;
        while(flag!=1){
            printf(1,"Username: ");
            char user_entered[MAX_USERNAME_LENGTH];
            int c=read(0,user_entered,MAX_USERNAME_LENGTH);
            char user[c];
            for(int i=0;i<c-1;i++){
                user[i]=user_entered[i];
            }
            user[c-1]='\0';

            if(string_compare(user,username)==1){
                flag=1;
            } else{
                printf(1,"Username does not exist.\n");
            }

        }

        //authenticating password
        flag=0;
        while(flag!=1){
            printf(1,"Password: ");
            char user_entered[MAX_USERNAME_LENGTH];
            int c=read(0,user_entered,MAX_PSW_LENGTH);
            char pass[c];
            for(int i=0;i<c-1;i++){
                pass[i]=user_entered[i];
            }
            pass[c-1]='\0';

            if(string_compare(pass,psw)==1){
                flag=1;
            } else{
                printf(1,"Incorrect Password\n");
            }

        }
        return;

    } 

    //When users other than root also exist
    else {

        //reading the user login data file
        char buffer[(MAX_USERS_ALLOWED+1)*(MAX_PSW_LENGTH+MAX_USERNAME_LENGTH+5)];
        int cc=read(file,buffer,(MAX_USERS_ALLOWED+1)*(MAX_PSW_LENGTH+MAX_USERNAME_LENGTH+5));
        close(file);


        while(1){

            printf(1,"Username: ");
            char user_entered[MAX_USERNAME_LENGTH];
            int c=read(0,user_entered,MAX_USERNAME_LENGTH);
            char user[c];
            for(int i=0;i<c-1;i++){
                user[i]=user_entered[i];
            }
            user[c-1]='\0';




            //initializing users list
            int i=0;
            while(i<cc){
                int counter=0;
                while(1){
                    if(buffer[i]==','){
                        break;
                    }
                    i++;
                    counter++;
                }

                char username[counter+1];

                for(int j=0;j<counter;j++){
                    username[j]=buffer[i-counter+j];
                }
                username[counter]='\0';

                int flag=string_compare(username,user);
                while(flag==1){
                    char* pass;
                    printf(1,"Password: ");
                    char user_entered[MAX_USERNAME_LENGTH];
                    int c=read(0,user_entered,MAX_PSW_LENGTH);
                    pass=malloc(c*sizeof(char));
                    for(int t=0;t<c-1;t++){
                        pass[t]=user_entered[t];
                    }
        

                    int x=0;
                    while(1){
                        if(buffer[i]=='\n'){
                            break;
                        }
                        x++;
                        i++;
                    }
                    i++;
                    char password[x];
                    for(int j=0;j<x-1;j++){
                        password[j]=buffer[i-x+j];
                    }
                    password[x-1]='\0';

                    if(string_compare(password,pass)==1){
                        unlink("current_user.txt");
                        int file=open("current_user.txt", O_CREATE | O_WRONLY);
                        int bytesWritten = write(file,user,strlen(user));
                        if(bytesWritten!=strlen(username)){
                            printf(2,"Error in writing to file-2!\n");
                        }

                        close(file);
                        return;
                    }

                }
                while(1){
                    if(buffer[i]=='\n'){
                        break;
                    }
                    i++;
                }
                i++;

            }

        }

    }

    
}
