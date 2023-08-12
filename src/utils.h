
#include <stddef.h>
#include <wchar.h>

struct marks {
    char** labels;
    char** paths; 
    size_t len;
};

/* Load mark labels and their corresponding mark path from the 
 * specified file into two arrays.
 * @params:
 *   - file_path (char*): path to mark file
 * @returns:
 *   - marks (struct marks)*/
struct marks load_marks(char* file_path);

/* Prints every currently stored mark.
 * @params:
 *   - file_path (char*): path to mark file*/
void print_marks(char* file_path);

/* Prints every currently stored label.
 * @params:
 *   - file_path (char*): path to mark file*/
void print_labels(char* file_path);

/* Add a new mark, consisting of a path and a label, to the specified file,
 * if the given label does not exist yet.
 * @params:
 *   - label (char*): label of the path
 *   - path (char*): path to be marked
 *   - file_path (char*): path to the mark file
 * @returns:
 *   - result (int): 0 if successfull, else 1*/
int add_mark(char* label, char* path, char* file_path);

/* Delete a mark, if it exists, from the given file.
 * @params:
 *   - label (char*): label of the mark to be removed
 *   - file_path (char*): path to the mark file
 *   - tmp_file_path (char*): path to a temporary file.
 *     It must hold that tmp_file_path != file_path
 * @returns:
 *   - result (int): 0 if successfull, else 1*/
int delete_mark(char* label, char* file_path, char* tmp_file_path);

/* Translate paths according to marks specified in file_path file.
 * @params:
 *   - n (size_t): size of array
 *   - paths (char**): array of swift path strings
 *   - file_path (char*): path to mark file*/
void translate_paths(size_t n, char** paths, char* file_path);

/* Try to match a token to a mark label. If no match is found,
 * return token, else the mark path.
 * @params:
 *   - token (char*): token string
 *   - marks (struct marks): marks to be matched
 * @returns:
 *   - path (char*): mark path if match was found, else token*/
char* match_token(char* token, struct marks marks);

/* Find and return the first index where a given char occurs 
 * for the first time in a given string.
 * @params:
 *   - string (char*)
 *   - c (char*)
 * @returns:
 *   - index (size_t) index of first occurance of c, strlen(string) 
 *   if not found*/
size_t find_index(char* string, char c);


/* Takes ordered tokens and converts them to the final path
 * @params:
 *   - tokens (char**): array of token strings
 *   - len (size_t): size of tokens array
 * @returns:
 *   - path (char*): resulting final path
 * NOTE: Not used / implemented for now*/
char* cvrt_tokens_to_path(char** tokens, size_t len);

/* Concatenate token string into one single valid path string.
 * @params:
 *   - tokens (char**): array of token strings
 * @returns:
 *   - path (char*): path string
 * NOTE: Not used / implemented for now*/
char* concat_tokens_to_path(char** tokens);

/* Split a string representing a path by '/' into substrings (tokens).
 * @params:
 *   - path (char*): path string
 * @returns:
 *   - tokens (char**): array of token strings
 * NOTE: Not used / implemented for now*/
char** cvrt_path_to_tokens(char* path);

