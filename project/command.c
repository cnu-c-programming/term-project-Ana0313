#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "file_io.h"

ShellResult handle_save(char* args, Student** head, const char* csv_path){
    (void)args;
    int count=save_csv(csv_path, *head);
    if(count<0) return SHELL_ERR_FILE_WRITE;
    printf("Saved %d students to %s.\n", count, csv_path);
    return SHELL_OK;
}

ShellResult handle_reload(char* args, Student** head, const char* csv_path){
    (void)args;
    int count=load_csv(csv_path, head);
    if(count<0) return SHELL_ERR_FILE_OPEN;
    printf("Reloaded %d students from %s.\n", count, csv_path);
    return SHELL_OK;
}

ShellResult handle_add(char* args, Student** head, const char* csv_path){
    (void)csv_path;
    if(args==NULL) return SHELL_ERR_MISSING_ARGUMENT;
    
    int id, score;
    char name[32];
    if(sscanf(args, "%d %31s %d", &id, name, &score)!=3){
        return SHELL_ERR_MISSING_ARGUMENT;
    }
    
    if(id<=0) return SHELL_ERR_INVALID_ARGUMENT;
    if(score<0 || score>100) return SHELL_ERR_INVALID_SCORE;
    if(find_student(*head, id)!=NULL) return SHELL_ERR_DUPLICATE_STUDENT;

    add_student(head, id, name, score);
    printf("Student added.\n");
    return SHELL_OK;
}

ShellResult handle_delete(char* args, Student** head, const char* csv_path){
    (void)csv_path;
    if(args==NULL) return SHELL_ERR_MISSING_ARGUMENT;
    
    int id=atoi(args);
    if(id<=0) return SHELL_ERR_INVALID_ARGUMENT;

    if(delete_student(head, id)){
        printf("Student deleted.\n");
        return SHELL_OK;
    }
    return SHELL_ERR_STUDENT_NOT_FOUND;
}

ShellResult handle_update(char* args, Student** head, const char* csv_path){
    (void)csv_path;
    if(args==NULL) return SHELL_ERR_MISSING_ARGUMENT;
    
    int id, score;
    if(sscanf(args, "%d %d", &id, &score)!=2) return SHELL_ERR_MISSING_ARGUMENT;
    if(score<0 || score>100) return SHELL_ERR_INVALID_SCORE;

    if(update_student(*head, id, score)){
        printf("Student updated.\n");
        return SHELL_OK;
    }
    return SHELL_ERR_STUDENT_NOT_FOUND;
}

ShellResult handle_find(char* args, Student** head, const char* csv_path){
    (void)csv_path;
    if(args==NULL) return SHELL_ERR_MISSING_ARGUMENT;
    
    int id=atoi(args);
    Student* cur=find_student(*head, id);
    if(cur!=NULL){
        printf("ID: %d\nName: %s\nScore: %d\n", cur->id, cur->name, cur->score);
        return SHELL_OK;
    }
    return SHELL_ERR_STUDENT_NOT_FOUND;
}

ShellResult handle_list(char* args, Student** head, const char* csv_path){
    (void)args; (void)csv_path;
    if(*head==NULL){
        printf("No students found.\n");
        return SHELL_OK;
    }

    printf("%-4s %-10s %s\n", "ID", "Name", "Score");
    Student* cur=*head;
    while(cur!=NULL){
        printf("%-4d %-10s %d\n", cur->id, cur->name, cur->score);
        cur=cur->next;
    }
    return SHELL_OK;
}

ShellResult handle_stats(char* args, Student** head, const char* csv_path){
    (void)args; (void)csv_path;
    if(*head==NULL){
        printf("No student data available.\n");
        return SHELL_OK;
    }

    int count=0, sum=0, max=-1, min=101;
    Student* cur=*head;

    while(cur!=NULL){
        count++;
        sum+=cur->score;
        if(cur->score>max) max=cur->score;
        if(cur->score<min) min=cur->score;
        cur=cur->next;
    }

    printf("Count: %d\nAverage: %.1f\nMax: %d\nMin: %d\n", count, (double)sum/count, max, min);
    return SHELL_OK;
}

ShellResult handle_clear(char* args, Student** head, const char* csv_path){
    (void)args; (void)head; (void)csv_path;
    printf("\033[2J\033[H");
    return SHELL_OK;
}

ShellResult handle_exit(char* args, Student** head, const char* csv_path){
    (void)args; (void)head; (void)csv_path;
    return SHELL_EXIT;
}

#ifdef ADMIN_MODE
Command commands[]={
    {"save",   handle_save,   "save",                    "Save students to CSV"},
    {"reload", handle_reload, "reload",                  "Reload students from CSV"},
    {"add",    handle_add,    "add <id> <name> <score>", "Add a student"},
    {"delete", handle_delete, "delete <id>",             "Delete a student"},
    {"update", handle_update, "update <id> <score>",     "Update student score"},
    {"find",   handle_find,   "find <id>",               "Find student by ID"},
    {"list",   handle_list,   "list",                    "List all students"},
    {"stats",  handle_stats,  "stats",                   "Show statistics"},
    {"clear",  handle_clear,  "clear",                   "Clear screen"},
    {"exit",   handle_exit,   "exit",                    "Exit program"}
};
int cmd_count=sizeof(commands)/sizeof(Command);
#endif

#ifdef CLIENT_MODE
Command commands[]={
    {"reload", handle_reload, "reload",     "Reload students from CSV"},
    {"find",   handle_find,   "find <id>",  "Find student by ID"},
    {"list",   handle_list,   "list",       "List all students"},
    {"stats",  handle_stats,  "stats",      "Show statistics"},
    {"clear",  handle_clear,  "clear",      "Clear screen"},
    {"exit",   handle_exit,   "exit",       "Exit program"}
};
int cmd_count=sizeof(commands)/sizeof(Command);
#endif

ShellResult handle_help(char* args, Student** head, const char* csv_path){
    (void)args; (void)head; (void)csv_path;
    printf("Commands:\n");
    for (int i=0; i<cmd_count; i++){
        printf("%-28s %s\n", commands[i].usage, commands[i].description);
    }
    printf("%-28s %s\n", "help", "Show help");
    return SHELL_OK;
}

ShellResult run_command(char* cmd_line, Student** head, const char* csv_path){
    if(cmd_line==NULL || strlen(cmd_line)==0) return SHELL_OK;

    char* cmd=strtok(cmd_line, " \t\n");
    if(cmd==NULL) return SHELL_OK;

    char* args=strtok(NULL, "\n");

    if(strcmp(cmd, "help")==0) return handle_help(args, head, csv_path);

    for(int i=0; i<cmd_count; i++){
        if(strcmp(cmd, commands[i].name)==0){
            return commands[i].handler(args, head, csv_path);
        }
    }

#ifdef CLIENT_MODE
    if(strcmp(cmd, "add")==0 || strcmp(cmd, "delete")==0 || 
        strcmp(cmd, "update")==0 || strcmp(cmd, "save")==0){
        return SHELL_ERR_PERMISSION_DENIED;
    }
#endif

    return SHELL_ERR_UNKNOWN_COMMAND;
}

void print_error(ShellResult error_type){
    switch(error_type){
        case SHELL_ERR_UNKNOWN_COMMAND:
            printf("Error: unknown command.\n"); break;
        case SHELL_ERR_PERMISSION_DENIED:
            printf("Unknown command or permission denied.\n"); break;
        case SHELL_ERR_MISSING_ARGUMENT:
            printf("Error: missing arguments.\n"); break;
        case SHELL_ERR_INVALID_ARGUMENT:
            printf("Error: invalid argument.\n"); break;
        case SHELL_ERR_STUDENT_NOT_FOUND:
            printf("Error: student not found.\n"); break;
        case SHELL_ERR_DUPLICATE_STUDENT:
            printf("Error: duplicate ID.\n"); break;
        case SHELL_ERR_INVALID_SCORE:
            printf("Error: invalid score.\n"); break;
        case SHELL_ERR_FILE_WRITE:
            printf("Error: could not write to file.\n"); break;
        case SHELL_ERR_FILE_OPEN:
            printf("Error: could not open file.\n"); break;
        default: break;
    }
}
