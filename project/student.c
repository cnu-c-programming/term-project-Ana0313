#include <stdlib.h>
#include <string.h>
#include "student.h"

int add_student(Student** head, int id, const char* name, int score){
    if(find_student(*head, id)!=NULL){
        return 0;
    }

    Student* new_node=(Student*)malloc(sizeof(Student));
    if(new_node==NULL) return -1;

    new_node->id=id;
    strncpy(new_node->name, name, 31);
    new_node->name[31]='\0';
    new_node->score=score;
    new_node->next=NULL;

    if(*head==NULL){
        *head=new_node;
    }else{
        Student* cur=*head;
        while(cur->next!=NULL){
            cur=cur->next;
        }
        cur->next=new_node;
    }
    return 1;
}

int delete_student(Student** head, int id){
    Student* cur=*head;
    Student* prev=NULL;

    while(cur!=NULL){
        if(cur->id==id){
            if(prev==NULL){
                *head=cur->next;
            }else{
                prev->next=cur->next;
            }
            free(cur);
            return 1;
        }
        prev=cur;
        cur=cur->next;
    }
    return 0;
}

int update_student(Student* head, int id, int score){
    Student* target=find_student(head, id);
    if(target!=NULL){
        target->score=score;
        return 1;
    }
    return 0;
}

Student* find_student(Student* head, int id){
    Student* cur=head;
    while(cur!=NULL){
        if(cur->id==id){
            return cur;
        }
        cur=cur->next;
    }
    return NULL;
}

void free_all_students(Student** head){
    Student* cur=*head;
    Student* temp;
    while(cur!=NULL){
        temp=cur->next;
        free(cur);
        cur=temp;
    }
    *head=NULL;
}
