#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "file_io.h"
#include "command.h"

void start_shell(const char *csv_path, Student **head){
#ifdef ADMIN_MODE
    printf("[Admin Program]\n");
#else
    printf("[Client Program]\n");
#endif

    int student_count=load_csv(csv_path, head);
    if(student_count==-3){
        printf("Error: invalid CSV header.\n");
        exit(1);
    }else if(student_count<0){
        printf("Loaded 0 students from %s.\n", csv_path);
    }else{
        printf("Loaded %d students from %s.\n", student_count, csv_path);
    }
}

void run_shell(const char *csv_path){
    Student *head=NULL;
    start_shell(csv_path, &head);

    char input[256];
    while(1){
#ifdef ADMIN_MODE
        printf("admin> ");
#else
        printf("client> ");
#endif
        if(fgets(input, sizeof(input), stdin)==NULL) break;
        
        input[strcspn(input, "\r\n")]='\0';
        if(strlen(input)==0 || input[0]=='#') continue;

        ShellResult error_type=run_command(input, &head, csv_path);

        if(error_type==SHELL_EXIT){
            printf("Goodbye.\n");
            break;
        }else if(error_type!=SHELL_OK){
            print_error(error_type);
        }
    }
    free_all_students(&head);
}

void run_command_file(const char *cmd_file, const char *csv_path){
    Student *head=NULL;
    start_shell(csv_path, &head);

    FILE *fp=fopen(cmd_file, "r");
    if(fp==NULL){
        printf("Error: could not open command file %s.\n", cmd_file);
        free_all_students(&head);
        return;
    }

    char line[256];
    int line_num=1;
    int exit_called=0;

    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line, "\r\n")]='\0';
        if(strlen(line)==0 || line[0]=='#'){
            line_num++;
            continue;
        }
        printf("\n[command file:%d] %s\n", line_num, line);
        
        ShellResult error_type=run_command(line, &head, csv_path);
        
        if(error_type==SHELL_EXIT){
            exit_called=1;
            break;
        }else if(error_type!=SHELL_OK){
            print_error(error_type);
            printf("Skipped line %d.\n", line_num);
        }
        line_num++;
    }
    fclose(fp);

    if(exit_called)  printf("Goodbye.\n");
    
    free_all_students(&head);
}

int main(int argc, char *argv[]){
    const char *csv_path=NULL;
    const char *cmd_file=NULL;

    if(argc<2){
#ifdef ADMIN_MODE
        printf("Usage: ./admin_shell  [-f command_file]\n");
#else
        printf("Usage: ./client_shell  [-f command_file]\n");
#endif
        return 1;
    }

    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "-f")==0 && i+1<argc){
            cmd_file=argv[++i];
        }else{
            csv_path=argv[i];
        }
    }

    if(csv_path==NULL){
#ifdef ADMIN_MODE
        printf("Usage: ./admin_shell  [-f command_file]\n");
#else
        printf("Usage: ./client_shell  [-f command_file]\n");
#endif
        return 1;
    }

#ifdef ADMIN_MODE
    if(cmd_file){
        run_command_file(cmd_file, csv_path);
    }else{
        run_shell(csv_path);
    }
#elif defined(CLIENT_MODE)
    if(cmd_file){
        run_command_file(cmd_file, csv_path);
    }else{
        run_shell(csv_path);
    }
#else
    return 1;
#endif
    return 0;
}
