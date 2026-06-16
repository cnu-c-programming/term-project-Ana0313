#ifndef STUDENT_H
#define STUDENT_H

typedef struct Student{
    int id;
    char name[32];
    int score;
    struct Student* next;
}Student;

int add_student(Student** head, int id, const char* name, int score);
int delete_student(Student** head, int id);
int update_student(Student* head, int id, int score);
Student* find_student(Student* head, int id);
void free_all_students(Student** head);

#endif
