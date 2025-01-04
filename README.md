# Moodle Replacement System

## Problem Statement

You are tasked with developing a **Course Management System** for a university to replace Moodle. The system will handle **students**, their **exams**, and their **grades** while ensuring the functionalities below:

- Add, update, and delete student information.
- Manage exams (both written and digital) with appropriate details.
- Record and update students' grades.
- Perform queries like searching for students, exams, or grades.
- Validate all inputs and handle errors gracefully.

---

## Input Format

The input will be provided in a file named `input.txt`. Each line will contain one query or command, separated by spaces where necessary:

1. **ADD_STUDENT** `[student_id] [name] [faculty]`
2. **ADD_EXAM** `[exam_id] [exam_type] [exam_info]`
3. **ADD_GRADE** `[exam_id] [student_id] [grade]`
4. **UPDATE_EXAM** `[exam_id] [new_exam_type] [new_exam_info]`
5. **UPDATE_GRADE** `[exam_id] [student_id] [new_grade]`
6. **SEARCH_STUDENT** `[student_id]`
7. **SEARCH_GRADE** `[exam_id] [student_id]`
8. **DELETE_STUDENT** `[student_id]`
9. **LIST_ALL_STUDENTS**
10. **END**

---

## Output Format

The output should be written to a file named `output.txt`. Each result or error message should be printed on a new line, with a newline character `\n` after every output.

---

## Constraints

- **String Lengths**:
  - `name`: \(1 < \text{name} < 20\)
  - `faculty`: \(4 < \text{faculty} < 30\)
  - `software`: \(2 < \text{software} < 20\)
- **Numeric Ranges**:
  - `student_id`: \(0 < \text{student_id} < 1000\)
  - `exam_id`: \(0 < \text{exam_id} < 500\)
  - `grade`: \(0 \leq \text{grade} \leq 100\)
  - `duration`: \(40 \leq \text{duration} \leq 180\)

---

## Data Structures

### **Student**
- `student_id`: Integer (unique ID).
- `name`: String (student's name).
- `faculty`: String (student's faculty).

### **Exam**
- `exam_id`: Integer (unique ID).
- `exam_type`: Enum (`WRITTEN` or `DIGITAL`).
- `exam_info`: Union containing:
  - `duration`: Integer (minutes, for written exams).
  - `software`: String (required software, for digital exams).

### **ExamGrade**
- `exam_id`: Integer (exam ID).
- `student_id`: Integer (student ID).
- `grade`: Integer (student's grade).

---

## Commands and Outputs

### 1. ADD_STUDENT
**Input**: Add a student to the system.  
**Success**:  
`Student: [student_id] added`  
**Errors**:
- `Student: [student_id] already exists`
- `Invalid student id`
- `Invalid name`
- `Invalid faculty`

---

### 2. ADD_EXAM
**Input**: Add an exam to the system.  
**Success**:  
`Exam: [exam_id] added`  
**Errors**:
- `Exam: [exam_id] already exists`
- `Invalid exam id`
- `Invalid exam type`
- `Invalid duration` (for WRITTEN exams)
- `Invalid software` (for DIGITAL exams)

---

### 3. ADD_GRADE
**Input**: Add a grade for a student in an exam.  
**Success**:  
`Grade [grade] added for the student: [student_id]`  
**Errors**:
- `Exam not found`
- `Invalid exam id`
- `Student not found`
- `Invalid student id`
- `Invalid grade`

---

### 4. SEARCH_STUDENT
**Input**: Search for a student by `student_id`.  
**Success**:  
`ID: [student_id], Name: [name], Faculty: [faculty]`  
**Errors**:
- `Student not found`
- `Invalid student id`

---

### 5. SEARCH_GRADE
**Input**: Search for a grade by `exam_id` and `student_id`.  
**Success**:  
`Exam: [exam_id], Student: [student_id], Name: [name], Grade: [grade], Type: [exam_type], Info: [exam_info]`  
**Errors**:
- `Exam not found`
- `Invalid exam id`
- `Student not found`
- `Invalid student id`

---

### 6. UPDATE_EXAM
**Input**: Update the details of an exam.  
**Success**:  
`Exam: [exam_id] updated`  
**Errors**:
- `Invalid exam type`
- `Invalid duration`
- `Invalid software`

---

### 7. UPDATE_GRADE
**Input**: Update a student's grade in an exam.  
**Success**:  
`Grade [grade] updated for the student: [student_id]`  
**Errors**:
- `Invalid grade`

---

### 8. DELETE_STUDENT
**Input**: Remove a student and all their associated grades.  
**Success**:  
`Student: [student_id] deleted`  

---

### 9. LIST_ALL_STUDENTS
**Output**: List all students in the order they were added.  
Each student is displayed in the format:  
`ID: [student_id], Name: [name], Faculty: [faculty]`

---

### 10. END
**Input**: Exit the program without further output.  
**Note**: The `END` command guarantees no further queries will follow.

---

## Notes
1. Validate all numeric and string inputs.
2. Output error messages for the first error encountered in each query.
3. Continue processing queries even after encountering errors, until the `END` command is received.
4. Always append a newline character `\n` after every output.

---

## [Example Input and Output](https://drive.google.com/drive/folders/1GtEoKWCO70lht3ifY7CVvDaVOMGYJvtu?usp=drive_link)


