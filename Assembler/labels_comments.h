#ifndef LABELS_COMMENTS_H
#define LABELS_COMMENTS_H


asm_error_t scanLabel (FILE* asm_file, size_t* count_elems, int* byte_code);
void labelsVerify (int* byte_code, size_t count_elems);
bool scanComment (help_var_t* variables, FILE* asm_file);
bool checkLabel (help_var_t* variables, FILE* asm_file, size_t* count_elems, int* byte_array, size_t count_lines, size_t* count_labels);
void checkComment (help_var_t* variables, FILE* asm_file, size_t count_lines);


#endif // LABELS_COMMENTS_H
