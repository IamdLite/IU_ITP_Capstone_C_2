#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_SIZE_NAME 1
#define MAX_SIZE_NAME 20
#define MIN_SIZE_FACULTY 4
#define MAX_SIZE_FACULTY 30
#define MIN_SIZE_SOFTWARE 2
#define MAX_SIZE_SOFTWARE 20
#define MIN_STUDENT_ID 0
#define MAX_STUDENT_ID 1000
#define MIN_GRADE 0
#define MAX_GRADE 100
#define MIN_DURATION 40
#define MAX_DURATION 180
#define MIN_EXAM_ID 0
#define MAX_EXAM_ID 500

typedef struct {
    unsigned short student_id;
    char name[MAX_SIZE_NAME];
    char faculty[MAX_SIZE_FACULTY];
} Student;

typedef struct {
    unsigned short exam_id;
    enum {
        WRITTEN=0,
        DIGITAL
    } ExamType;
    union {
        unsigned short duration;
        char required_software[MAX_SIZE_SOFTWARE];
    } ExamInfo ;
} Exam;

typedef struct {
    unsigned short exam_id: 9;
    unsigned short student_id: 10;
    unsigned short grade: 7;
} ExamGrade;

enum {
    ADD_STUDENT=0,
    ADD_EXAM,
    ADD_GRADE,
    UPDATE_EXAM,
    UPDATE_GRADE,
    SEARCH_STUDENT,
    SEARCH_GRADE,
    DELETE_STUDENT,
    LIST_ALL_STUDENTS,
    END
} query;

typedef struct {
    unsigned short students_count;
    unsigned short exams_count;
    unsigned short grades_count;
} Record;


//NOTE: Structure of query is guaranteed to be correct
//IMPORTANT: DELETE_STUDENT should delete all records of student


/* ================== FUNCTIONS DECLARATION ===============================*/

bool is_name_valid(const char name[MAX_SIZE_NAME]);

void Add_Student(Student student[],char student_id[], char name[MAX_SIZE_NAME], char faculty[MAX_SIZE_FACULTY], FILE* output, Record* record); //add student to record
enum  {
    ADD_STUDENT_SUCCESS = 0,
    EXISTING_STUDENT_AS,
    INVALID_STUDENT_ID_AS,
    INVALID_NAME_AS,
    INVALID_FACULTY_AS
}err_add_student;

void  Add_Exam(Exam exam[], char exam_id[], char exam_type[7], char exam_info[MAX_SIZE_SOFTWARE], FILE* output, Record* record); // add exam to record
enum {
    ADD_EXAM_SUCCESS = 5,
    EXISTING_EXAM_ID_AE,
    INVALID_EXAM_ID_AE,
    INVALID_EXAM_TYPE_AE,
    INVALID_DURATION_AE,
    INVALID_SOFTWARE_AE
}err_add_exam;

void Add_Grade(ExamGrade exam_grade[],  Exam exam[], Student student[], char exam_id[], char student_id[], char grade[], FILE* output, Record* record); // record student grade
enum  {
    ADD_GRADE_SUCCESS = 11,
    NOT_EXISTING_EXAM_AG,
    INVALID_EXAM_ID_AG,
    STUDENT_ID_NOT_FOUND_AG,
    INVALID_STUDENT_ID_AG,
    INVALID_GRADE_AG
}err_add_grade;

void Update_Exam(Exam exam[], char exam_id[], char new_exam_type[7], char new_exam_info[MAX_SIZE_SOFTWARE], FILE* output, Record* record); //modify examination details
enum {
    UPDATE_EXAM_SUCCESS = 17,
    INVALID_EXAM_TYPE_UE,
    INVALID_DURATION_UE,
    INVALID_SOFTWARE_UE

}err_update_exam;

void Update_Grade(ExamGrade exam_grade[], char exam_id[],  char student_id[], char new_grade[], FILE* output, Record* record); //change student grade
enum {
    UPDATE_GRADE_SUCCESS = 21,
    INVALID_GRADE_UG
}err_update_grade;



void Search_Student(Student student[], char student_id[], FILE* output, Record* record); //search for given student_id in records
enum {
    SEARCH_STUDENT_SUCCESS = 23,
    NOT_EXISTING_STUDENT_ID_SS,
    INVALID_STUDENT_ID_sS
}err_search_Student;


void Search_Grade(ExamGrade exam_grade[], Exam exam[], Student student[], FILE* output, char exam_id[], char student_id[], Record* records ); // search for student grade in record
enum {
    SEARCH_GRADE_SUCCESS =  26,
    NOT_EXISTING_EXAM_ID_SG,
    INVALID_EXAM_ID_SG,
    NOT_EXISTING_STUDENT_ID_SG,
    INVALID_STUDENT_ID_SG
}err_search_grade;

void List_All_Students(Student students[], FILE* output, Record* record); //list all recorded students

void Delete_Student(Student students[], ExamGrade exam_grade[], unsigned short student_id, FILE* output, Record* record); //handles deleting jobs

void End(FILE* output); // exits programs

/* ================== FUNCTIONS DECLARATION END ===============================*/


/* =================== FUNCTIONS DEFINITION START =============================*/

int is_valid_number(const char *str) {
    // Empty string is not a valid number
    if (str == NULL || *str == '\0') {
        return 0;
    }

    // Check each character in the string
    for (int i = 0; str[i] != '\0'; i++) {
        // If the character is not a digit, return false
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }

    // If all characters are digits, return true
    return 1;
}



bool is_name_valid(const char name[MAX_SIZE_NAME]) {
    // Check if the string is not empty
    if (name == NULL || *name == '\0') {
        return false;  // Invalid
    }

    // Loop through each character in the string
    for (int i = 0; name[i] != '\0'; i++) {
        // Ensure each character is alphabetic
        if (!isalpha(name[i])) {
            return false;  // Invalid character found
        }
    }

    return true;  // Valid name (only letters)
}

char* trim_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && (str[len-1] == '\n') || (str[len-1] == '\r')) {
        str[len-1] = '\0';
    }
    return str;
}

void capitalize(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

//------------

void Add_Student(Student student[], char student_id[], char name[MAX_SIZE_NAME], char faculty[MAX_SIZE_FACULTY], FILE* output, Record* record) {
    
    // Convert student_id from string to integer
    unsigned short id = atoi(student_id);
    printf("%hu\n", id);  // Debugging: print converted student_id

    // Check if student with the same ID already exists
    for (int i = 0; i < record->students_count; i++) {
        if (student[i].student_id == id) {
            err_add_student = EXISTING_STUDENT_AS;
            fprintf(output, "Student: %hu already exists\n", id);  // Use 'id' as an integer
            return;
        }
    }

    // Validate student_id
    if (id <= MIN_STUDENT_ID || id >= MAX_STUDENT_ID || !is_valid_number(student_id)) {
        err_add_student = INVALID_STUDENT_ID_AS;
        fprintf(output, "Invalid student id\n");
        return;
    }

    // Validate name
    char *trimmed_name = trim_newline(name);
    if (strlen(trimmed_name) <= MIN_SIZE_NAME || strlen(trimmed_name) >= MAX_SIZE_NAME || !is_name_valid(trimmed_name)) {
        err_add_student = INVALID_NAME_AS;
        fprintf(output, "Invalid name\n");
        return;
    }

    // Validate faculty
    char *trimmed_faculty = trim_newline(faculty);
    if (strlen(trimmed_faculty) <= MIN_SIZE_FACULTY || strlen(trimmed_faculty) >= MAX_SIZE_FACULTY || !is_name_valid(trimmed_faculty)) {
        err_add_student = INVALID_FACULTY_AS;
        fprintf(output, "Invalid faculty\n");
        return;
    }

    // Create a new student
    Student new_student;
    new_student.student_id = id;
    strcpy(new_student.name, trimmed_name);
    strcpy(new_student.faculty, trimmed_faculty);

    // Add the student to the array
    student[record->students_count] = new_student;
    err_add_student = ADD_STUDENT_SUCCESS;
    fprintf(output, "Student: %hu added\n", id);  // Use 'id' as an integer
    record->students_count++;
}



//-----------

void Add_Exam(Exam exam[], char exam_id[], char exam_type[7], char exam_info[MAX_SIZE_SOFTWARE], FILE* output, Record* record){

    for (int i = 0; i < record->exams_count; i++){
        if (exam[i].exam_id == atoi(exam_id)){
            err_add_exam = EXISTING_EXAM_ID_AE;
            fprintf(output, "Exam: %hu already exists\n", atoi(exam_id));
            return;
        }
    }
   if (atoi(exam_id) <= MIN_EXAM_ID || atoi(exam_id) >= MAX_EXAM_ID || !is_valid_number(exam_id)){
        err_add_exam = INVALID_EXAM_ID_AE;
        fprintf(output, "Invalid exam id\n");
         return;
    }

    if (strcmp(exam_type, "WRITTEN") != 0 && strcmp(exam_type, "DIGITAL") != 0){
        err_add_exam = INVALID_EXAM_TYPE_AE;
        fprintf(output, "Invalid exam type\n");
         return;
    }

    if (strcmp(exam_type, "WRITTEN") == 0){
        printf("Exam type: %hu\n", is_valid_number((exam_info)));
        if (atoi(exam_info) < MIN_DURATION || atoi(exam_info) > MAX_DURATION || !is_valid_number(exam_info)){
            err_add_exam = INVALID_DURATION_AE;
            fprintf(output, "Invalid duration\n");
            return;
        }
    }

    if (strcmp(exam_type, "DIGITAL") == 0){ 
    if (strlen(trim_newline(trim_newline(exam_info))) <= MIN_SIZE_SOFTWARE || strlen(trim_newline(exam_info)) >= MAX_SIZE_SOFTWARE || !is_name_valid(exam_info)){
            err_add_exam = INVALID_SOFTWARE_AE;
            fprintf(output, "Invalid software\n");
            return;
    }
    }
    

    Exam new_exam;
    new_exam.exam_id = atoi(exam_id);
    if (strcmp(exam_type, "WRITTEN") == 0){
        new_exam.ExamType = WRITTEN;
        new_exam.ExamInfo.duration = atoi(exam_info);
    } else {
        new_exam.ExamType = DIGITAL;
        strcpy(new_exam.ExamInfo.required_software, exam_info);
    }

 
            exam[record->exams_count] = new_exam;
            err_add_exam = ADD_EXAM_SUCCESS;
            fprintf(output, "Exam: %hu added\n", atoi(exam_id));
            record->exams_count++;

    }


// ---------------------------


void Add_Grade(ExamGrade exam_grade[], Exam exam[], Student student[], char exam_id[], char student_id[], char grade[], FILE* output, Record* record){

    bool exam_found = false;   
    for (int i = 0; i < record->exams_count; i++){
        if (exam[i].exam_id == atoi(exam_id)){
            exam_found = true;
            break;
        }
    }

    if (!exam_found){
        err_add_grade = NOT_EXISTING_EXAM_AG;
        fprintf(output, "Exam not found\n");
        return;
    }

    
    if (atoi(exam_id) <= MIN_EXAM_ID || atoi(exam_id) >= MAX_EXAM_ID || !is_valid_number(exam_id)){
        err_add_grade = INVALID_EXAM_ID_AG;
        fprintf(output, "Invalid exam id\n");
        return;
    }


   bool student_found = false;
   for (int i = 0; i < record->students_count; i++){
        if (student[i].student_id == atoi(student_id)){
            student_found = true;
            break;
        }
    }
        if (!student_found){
        err_add_grade = STUDENT_ID_NOT_FOUND_AG;
        fprintf(output, "Student not found\n");
        return;
    }


    if (atoi(student_id) <= MIN_STUDENT_ID || atoi(student_id) >= MAX_STUDENT_ID || !is_valid_number(student_id)){
        err_add_student = INVALID_STUDENT_ID_AG;
        fprintf(output, "Invalid student id\n");
        return;
    }

    if (atoi(grade) < MIN_GRADE || atoi(grade) > MAX_GRADE || !is_valid_number(grade)){
        err_add_grade = INVALID_GRADE_AG;
        fprintf(output, "Invalid grade\n");
        return;
    }

    ExamGrade new_grade;
    new_grade.exam_id = atoi(exam_id);
    new_grade.student_id = atoi(student_id);
    new_grade.grade = atoi(grade);

            exam_grade[record->grades_count] = new_grade;
            fprintf(output, "Grade %u added for the student: %hu\n", atoi(grade), atoi(student_id));
            err_add_grade = ADD_GRADE_SUCCESS;
            record->grades_count++;
}

//-------------

void Update_Exam(Exam exam[], char exam_id[], char new_exam_type[7], char new_exam_info[MAX_SIZE_SOFTWARE], FILE* output, Record* record){

    if (strcmp(new_exam_type, "WRITTEN") != 0 && strcmp(new_exam_type, "DIGITAL") != 0){
        err_update_exam = INVALID_EXAM_TYPE_UE;
        fprintf(output, "Invalid exam type\n");
         return;
    }

    if (strcmp(new_exam_type, "WRITTEN") == 0){
        if (atoi(new_exam_info) < MIN_DURATION || atoi(new_exam_info) > MAX_DURATION || !is_valid_number(new_exam_info)){
            err_update_exam = INVALID_DURATION_UE;
            fprintf(output, "Invalid duration\n");
             return;
        }
    } 

    if (strcmp(new_exam_type, "DIGITAL") == 0){
        if (strlen(new_exam_info) <= MIN_SIZE_SOFTWARE || strlen(new_exam_info) >= MAX_SIZE_SOFTWARE || !is_name_valid(new_exam_info)){
            err_update_exam = INVALID_SOFTWARE_UE;
            fprintf(output, "Invalid software\n");
            return;
    }
    }

    for (int i = 0; i < record->exams_count; i++){
        if (exam[i].exam_id == atoi(exam_id)){
            if (strcmp(new_exam_type, "WRITTEN") == 0){
                exam[i].ExamType = WRITTEN;
                exam[i].ExamInfo.duration = atoi(new_exam_info);
            } else {
                exam[i].ExamType = DIGITAL;
                strcpy(exam[i].ExamInfo.required_software, new_exam_info);
            }
            err_update_exam = UPDATE_EXAM_SUCCESS;
            fprintf(output, "Exam: %hu updated\n", atoi(exam_id));
        }
}
}

//-------------

void Update_Grade(ExamGrade exam_grade[], char exam_id[],  char student_id[], char new_grade[], FILE* output, Record* record){

    if (atoi(new_grade) < MIN_GRADE || atoi(new_grade) > MAX_GRADE || !is_valid_number(new_grade)){
        err_update_grade = INVALID_GRADE_UG;
        fprintf(output, "Invalid grade\n");
         return;
    }

    for (int i = 0; i < record->grades_count; i++){
        if (exam_grade[i].exam_id == atoi(exam_id) && exam_grade[i].student_id == atoi(student_id)){
            exam_grade[i].grade = atoi(new_grade);
            err_update_grade = UPDATE_GRADE_SUCCESS;
            fprintf(output, "Grade %hu updated for the student: %hu\n", atoi(new_grade), atoi(student_id));
        }
    }
}


//----------


void Search_Student(Student student[], char student_id[], FILE* output, Record* record){
     bool student_found = false;

    for (int i = 0; i < record->students_count; i++){
        if (student[i].student_id == atoi(student_id)){
            student_found = true;
            break;
        }
    }

    if (!student_found){
        err_search_Student = NOT_EXISTING_STUDENT_ID_SS;
        fprintf(output, "Student not found\n");
        return;
    }

 
    if (atoi(student_id) <= MIN_STUDENT_ID || atoi(student_id) >= MAX_STUDENT_ID || !is_valid_number(student_id)){
        err_search_Student = INVALID_STUDENT_ID_sS;
        fprintf(output, "Invalid student id\n");
        return;
    }
    if(student_found){
        for (int i = 0; i < record->students_count; i++){
            if (student[i].student_id == atoi(student_id)){
                fprintf(output, "ID: %hu, Name: %s, Faculty: %s\n", student[i].student_id, student[i].name, trim_newline(student[i].faculty));
                return;
            }
        }
    }
}


//--------------------
void Search_Grade(ExamGrade exam_grade[], Exam exam[], Student student[], FILE* output, char exam_id[], char student_id[], Record* record) {
    bool exam_found = false;
    bool student_found = false;

    // Check if the exam exists
    for (int i = 0; i < record->exams_count; i++) {
        if (exam[i].exam_id == atoi(exam_id)) {
            exam_found = true;
            break;
        }
    }

    if (!exam_found) {
        err_search_grade = NOT_EXISTING_EXAM_ID_SG;
        fprintf(output, "Exam not found\n");
        return;
    }

    // Validate the exam ID
    if (atoi(exam_id) <= MIN_EXAM_ID || atoi(exam_id) >= MAX_EXAM_ID || !is_valid_number(exam_id)) {
        err_search_grade = INVALID_EXAM_ID_SG;
        fprintf(output, "Invalid exam id\n");
        return;
    }

    // Check if the student exists
    for (int i = 0; i < record->grades_count; i++) {
        if (exam_grade[i].student_id == atoi(student_id)) {
            student_found = true;
            break;
        }
    }

    if (!student_found) {
        err_search_grade = NOT_EXISTING_STUDENT_ID_SG;
        fprintf(output, "Student not found\n");
        return;
    }

    // Validate the student ID
    printf("Student ID: %d\n", is_valid_number(student_id));
    if (atoi(student_id) <= MIN_STUDENT_ID || atoi(student_id) >= MAX_STUDENT_ID || !is_valid_number(student_id)) {
        err_search_grade = INVALID_STUDENT_ID_SG;
        fprintf(output, "Invalid student id\n");
        return;
    }

    // Retrieve and print the exam and student information
    char student_name[MAX_SIZE_NAME];
    char exam_type[8];
    char exam_info[MAX_SIZE_SOFTWARE];

    for (int i = 0; i < record->grades_count; i++) {
        if (exam_grade[i].exam_id == atoi(exam_id) && exam_grade[i].student_id == atoi(student_id)) {
            // Find student name
            for (int j = 0; j < record->students_count; j++) {
                if (student[j].student_id == atoi(student_id)) {
                    strcpy(student_name, student[j].name);
                    break;
                }
            }

            // Find exam info
            for (int k = 0; k < record->exams_count; k++) {
                if (exam[k].exam_id == atoi(exam_id)) {
                    if (exam[k].ExamType == WRITTEN) {
                        strcpy(exam_type, "WRITTEN");
                        sprintf(exam_info, "%hu", exam[k].ExamInfo.duration);
                    } else {
                        strcpy(exam_type, "DIGITAL");
                        strcpy(exam_info, exam[k].ExamInfo.required_software);
                    }
                    break;
                }
            }

            // Output the result
            fprintf(output, "Exam: %hu, Student: %hu, Name: %s, Grade: %hu, Type: %s, Info: %s\n",
                    exam_grade[i].exam_id, exam_grade[i].student_id, student_name,
                    exam_grade[i].grade, exam_type, trim_newline(exam_info));
            return;
        }
    }
}

//--------------------

void List_All_Students(Student students[], FILE* output, Record* record){
    // printf("Student IDs in the array: %hu\n", record->students_count);
    // for (int i = 0; i < record->students_count; i++) {
    //     printf("%d\n", students[i].student_id);  // Debugging: print all IDs
    // }

    for (int i = 0; i < record->students_count; i++){
        if (students[i].student_id != 0){
            fprintf(output, "ID: %d, Name: %s, Faculty: %s\n", students[i].student_id, students[i].name, trim_newline(students[i].faculty));
        }
    }
}

//---------------------

void Delete_Student(Student students[], ExamGrade exam_grade[], unsigned short student_id, FILE* output, Record* record) {
    
    bool student_deleted = false;
    bool grade_deleted = false;
    
    // Loop through the students array
    for (int i = 0; i < record->students_count; i++) {
        if (students[i].student_id == student_id) {
            // Shift all students after the deleted one to the left
            for (int j = i; j < record->students_count - 1; j++) {
                students[j] = students[j + 1];
            }
            // Decrease student count
            record->students_count--;
            student_deleted = true;
            break;  // Stop after deleting the student
        }
    }
    
    // Delete associated exam grades
    for (int i = 0; i < record->grades_count; i++) {
        if (exam_grade[i].student_id == student_id) {
            // Shift all exams after the deleted one to the left
            for (int j = i; j < record->grades_count - 1; j++) {
                exam_grade[j] = exam_grade[j + 1];
            }
            // Decrease exam count
            record->grades_count--;
            grade_deleted = true;
            i--; // Adjust index since we've shifted elements left
        }
    }

    if (student_deleted && grade_deleted) {
        fprintf(output, "Student: %hu deleted\n", student_id);
    } 
}


//---------------------

void End(FILE* output){
    fclose(output);
    exit(0);
}

//--------------------


/* =================== FUNCTIONS DEFINITION END =============================*/



int main (){

    Student student[100];
    Exam exam[100];
    ExamGrade grade[100];
    Record record;

    // printf("Welcome to cmoodle\n");
    // printf("Hey, what if we built cmoodle ? less gooo!\n");
    // printf("Size of student: %zu\nSize of exam: %zu\nSize of grade: %zu", sizeof(student), sizeof(exam), sizeof(grade)); 
    // printf("\n--------------------------\n");


/* ======================== READING FROM FILE =====================*/
    char **lines = NULL;
    char buffer[100];
    int line_count = 0;
    unsigned short capacity = 10;
   
   
    FILE* input = fopen("input.txt", "r");
    if(!input) exit(1); 

    //Allocate initial memory for the array of lines
    lines = malloc(10 * sizeof(char*));
    if(!lines) exit(1); 

    //Read the file line by line
    while (fgets(buffer, 100, input) != NULL){

        //Reallocate memory if needed
        if(line_count >= capacity){
            capacity *=2;
            char** temp = realloc(lines, capacity * sizeof(char *));
            if(!temp) exit(1); else lines = temp;
        }

        //Allocate memory for the line
        lines[line_count] = malloc(strlen(buffer) + 1); //+1 includes the null terminator
        if(lines[line_count] == NULL) {
            free(lines);
             exit(1); 
        }

        strcpy(lines[line_count], buffer); 
        line_count++;
        }



    fclose(input); //close the file after reading
 /* =========================== READING FROM FILE END =======================*/



 /* =============================== Testing FIle Input =============== */
 FILE* output = fopen("output.txt", "w");
    if(!input) exit(1); 

//  printf("Read %d lines from the file\n", line_count);

 //process each line
 for(int i = 0; i < line_count; i++){
     char* line_copy = malloc(strlen(lines[i]) + 1); // Create a copy of the line
     if (!line_copy) exit(1); // Handle memory allocation failure
     strcpy(line_copy, lines[i]);

     char* token = strtok(line_copy, " ");
     char* tokens[10] = {NULL}; // Initialize token array to NULL
     unsigned short count = 0;

     // Tokenize the string
     while(token != NULL && count < 10){
         tokens[count] = malloc(strlen(token) + 1);
         if (tokens[count] == NULL) exit(1); // Handle memory allocation failure
         strcpy(tokens[count], token);
         count++;
         token = strtok(NULL, " ");
     }

     // Handle commands
     char* command = tokens[0];
     capitalize(command);

        if (count >= 4 && strcmp(command, "ADD_STUDENT") == 0 ){
            Add_Student(student, tokens[1], tokens[2], tokens[3], output, &record);
        } 
        else if (count >= 4 && strcmp(command, "ADD_EXAM") == 0)
        {
            Add_Exam(exam, trim_newline(tokens[1]), trim_newline(tokens[2]), trim_newline(tokens[3]), output, &record);
        } 
        else if (count >= 4 && strcmp(command, "ADD_GRADE") == 0)
        {
            Add_Grade(grade, exam, student, trim_newline(tokens[1]), trim_newline(tokens[2]), trim_newline(tokens[3]), output, &record);
        } 
        else if (count >= 4 && strcmp(command, "UPDATE_EXAM") == 0)
        {
            Update_Exam(exam, trim_newline(tokens[1]), trim_newline(tokens[2]), trim_newline(tokens[3]), output, &record);
        } 
        else if (count >= 4 && strcmp(command, "UPDATE_GRADE") == 0)
        {
            Update_Grade(grade, trim_newline(tokens[1]), trim_newline(tokens[2]), trim_newline(tokens[3]), output, &record);
        } 
        else if (count >= 2 && strcmp(command, "SEARCH_STUDENT") == 0)
        {
            Search_Student(student, trim_newline(tokens[1]), output, &record);
        } 
        else if (count >= 3 && strcmp(command, "SEARCH_GRADE") == 0)
        {
            Search_Grade(grade, exam,  student, output, trim_newline(tokens[1]), trim_newline(tokens[2]), &record);
        } 
        else if (count >= 2 && strcmp(command, "DELETE_STUDENT") == 0)
        {
            Delete_Student(student, grade, atoi(trim_newline(tokens[1])), output, &record);
        } 
        else if (strcmp(trim_newline(command), "LIST_ALL_STUDENTS") == 0)
        {
            List_All_Students(student, output, &record);
        } 
        else if (strcmp(trim_newline(command), "END") == 0)
        {
            End(output);
        }
   
     // Free tokenized strings
     for (unsigned short j = 0; j < count; j++) {
         free(tokens[j]);
     }

     // Free the copied line
     free(line_copy);
     free(lines[i]); // Now safe to free lines[i] after processing
}
free(lines);


    /* =============================== Testing FIle Input END =============== */
    return 0;

}
