#include <stdio.h>
#include <string.h>
#include "file_io.h"

int load_csv(const char* filename, Student** head){
    FILE* fp=fopen(filename, "r");
    if(fp==NULL) return -1;
    
    char line[256];
    if(fgets(line, sizeof(line), fp)==NULL){
        fclose(fp);
        return -2;
    }

    if(strstr(line, "id,name,score")==NULL){
        fclose(fp);
        return -3;
    }

    free_all_students(head);
    int count=0;

    while(fgets(line, sizeof(line), fp)){
        int id, score;
        char name[32];
        if(sscanf(line, "%d,%[^,],%d", &id, name, &score)==3){
            add_student(head, id, name, score);
            count++;
        }
    }

    fclose(fp);
    return count;
}

int save_csv(const char* filename, Student* head){
    FILE* fp=fopen(filename, "w");
    if(fp==NULL) return -1;

    fprintf(fp, "id,name,score\n");
    
    Student* cur=head;
    int count=0;
    while(cur!=NULL){
        fprintf(fp, "%d,%s,%d\n", cur->id, cur->name, cur->score);
        cur=cur->next;
        count++;
    }

    fclose(fp);
    return count;
}
