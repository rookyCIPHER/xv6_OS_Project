#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_USERS_ALLOWED 5
#define MAX_USERNAME_LENGTH 100
#define MAX_PSW_LENGTH 200


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





int main(){
    int file=open("current_user.txt", O_RDONLY);
    printf(1,"called system call\n");
    char buffer[MAX_USERNAME_LENGTH];
    read(file,buffer,MAX_USERNAME_LENGTH);
    close(file);

    char* allowed_user="root";

    //checking if its the priviliged user
    if(string_compare(buffer,allowed_user)==1){
        printf(1,"Enter details of new user\n");

        //taking username as input
        printf(1,"Username: ");
        char user_entered[MAX_USERNAME_LENGTH];
        int c=read(0,user_entered,MAX_USERNAME_LENGTH);
        char user[c];
        for(int i=0;i<c-1;i++){
            user[i]=user_entered[i];
        }
        user[c-1]='\0';



        //taking password as input
        printf(1,"Password: ");
        char psw_entered[MAX_PSW_LENGTH];
        c=read(0,psw_entered,MAX_PSW_LENGTH);
        char pass[c];
        for(int i=0;i<c-1;i++){
            pass[i]=psw_entered[i];
        }
        pass[c-1]='\0';
        
        //making a copy of old data in the user_data file
        file=open("user_data.txt", O_RDONLY);
        char content[(MAX_USERS_ALLOWED+1)*(MAX_PSW_LENGTH+MAX_USERNAME_LENGTH+5)];
        int cc=read(file,content,(MAX_USERS_ALLOWED+1)*(MAX_PSW_LENGTH+MAX_USERNAME_LENGTH+5));
        close(file);

        int i;
        int iter=0;
        for(i=cc;i<cc+strlen(user);i++){
            
            if(i<(MAX_USERS_ALLOWED+1)*(MAX_PSW_LENGTH+MAX_USERNAME_LENGTH+5)){
                content[i]=user[iter];
                iter++;

            }else {
                printf(1,"i overflowed-1\n");
            }
        }

        content[i]=',';
        i++;

        cc=i;
        iter=0;
        for(i=cc;i<cc+strlen(pass);i++){
            if(i<(MAX_USERS_ALLOWED+1)*(MAX_PSW_LENGTH+MAX_USERNAME_LENGTH+5)){
                content[i]=pass[iter];
                iter++;
            } else {
                printf(1,"i overflowed-2\n");
            }

        }

        content[i]='\n';
        i++;
    
        // close(file);

        //writing to the user_data file
        file=open("user_data.txt", O_WRONLY);
        int length = i;
        int bytesWritten = write(file,content,length);
        if(bytesWritten!=length){
            printf(2,"Error in writing to file-3!\n");
            close(file);
            exit();
            return -1;
        }
        close(file);

        return 0;

        
    } else {
        printf(2,"Permission Denied. Root permissions required.\n");
        exit();
        return -1;
    }

    return 0;


}