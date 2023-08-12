
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "utils.h"

#define MAX_LABEL_SIZE 50

char buffer[MAX_LABEL_SIZE+1];

size_t find_index(char* string, char c){
    size_t i = 0;
    while(string[i]){
        if(string[i]==c)
            return i;
        i++;
    }
    return i;
}

void print_marks(char* file_path){
    FILE* file = fopen(file_path, "a+");
    if(!file){
        fprintf(stderr, "Could not open file: %s\n", file_path);
        return;
    }
    // copy everything from file to tmp file, except specified label
    printf("LABEL : PATH\n");
    printf("------------\n");
    while(fgets(buffer, MAX_LABEL_SIZE, file)){ 
        size_t split_index = find_index(buffer, ':');
        if(split_index >= MAX_LABEL_SIZE){
            fprintf(stderr, "Found too long entry in %s:\n%s\n", file_path, buffer);
            fclose(file);
            return;
        }
        if(buffer[strlen(buffer)-1] == '\n')
            buffer[strlen(buffer)-1] = '\0';
        buffer[split_index] = '\0';
        printf("%s : %s\n", buffer, buffer+split_index+1);
    }
    fclose(file);
}

void print_labels(char* file_path){
    FILE* file = fopen(file_path, "a+");
    if(!file){
        fprintf(stderr, "Could not open file: %s\n", file_path);
        return;
    }
    // copy everything from file to tmp file, except specified label
    while(fgets(buffer, MAX_LABEL_SIZE, file)){ 
        size_t split_index = find_index(buffer, ':');
        if(split_index >= MAX_LABEL_SIZE){
            fprintf(stderr, "Found too long entry in %s:\n%s\n", file_path, buffer);
            fclose(file);
            return;
        }
        buffer[split_index] = '\0';
        printf("%s\n", buffer);
    }
    fclose(file);
}

int add_mark(char* label, char* path, char* file_path){
    size_t label_size = strlen(label);
    if(label_size > MAX_LABEL_SIZE){
        fprintf(stderr, "Label %s is too long!\n", label);
        return 1;
    }
    if(find_index(label, ':') != label_size){
        fprintf(stderr, "Label can not contain semicolons.\n");
        return 1;
    }
    FILE* file = fopen(file_path, "a+");
    if(!file){
        return 1;
    }
    // check if label already exits
    while(fgets(buffer, MAX_LABEL_SIZE, file)){ 
        size_t split_index = find_index(buffer, ':');
        if(split_index >= MAX_LABEL_SIZE){
            fprintf(stderr, "Found too long entry in %s:\n%s\n", file_path, buffer);
            fclose(file);
            return 1;
        }
        if(label_size == split_index && !strncmp(label, buffer, split_index)){
            fprintf(stderr, "Label %s already exits!\n", label);
            return 1;
        }
    }
    fclose(file);

    file = fopen(file_path, "a"); 
    if(!file)
        return 1;
    fprintf(file, "%s:%s\n", label, path);
    fclose(file);
    return 0;
}

int delete_mark(char* label, char* file_path, char* tmp_file_path){
    FILE* file = fopen(file_path, "a+");
    if(!file)
        return 1;
    FILE* tmp_file = fopen(tmp_file_path, "w");
    if(!tmp_file){
        fclose(file);
        return 1;
    }
    char found_label = 0;
    size_t label_size = strlen(label);
    // copy everything from file to tmp file, except specified label
    while(fgets(buffer, MAX_LABEL_SIZE, file)){ 
        size_t split_index = find_index(buffer, ':');
        if(split_index >= MAX_LABEL_SIZE){
            fprintf(stderr, "Found too long entry in %s:\n%s\n", file_path, buffer);
            fclose(file);
            fclose(tmp_file);
            return 1;
        }
        if(split_index != label_size || strncmp(label, buffer, split_index)){
            fprintf(tmp_file, "%s", buffer);
        }
        else{
            found_label = 1;
        }
    }

    fclose(file);
    fclose(tmp_file);

    if(!found_label){
        fprintf(stderr, "Could not find label %s in %s\n", label, file_path);
        return 1;
    }

    // rename temporary file to mark file
    if(rename(tmp_file_path, file_path)){
        fprintf(stderr, "Could not rename %s to %s\n", tmp_file_path, file_path);
        return 1;
    }

    return 0;
}


struct marks load_marks(char* file_path){
    // get max number of marks
    FILE* file = fopen(file_path, "a+");
    if(!file){
        fprintf(stderr, "Could not open %s\n", file_path);
        exit(1);
    }
    size_t mark_count = 0;
    while(fgets(buffer, MAX_LABEL_SIZE, file)){ 
        mark_count++;
        if(buffer[strlen(buffer)-1] != '\n'){
            fprintf(stderr, "In line %zul of %s: Line is too long.\n", mark_count, file_path);
            fclose(file);
            exit(1);
        }
    }

    if(!mark_count)
        return (struct marks){0,0,0};

    // allocate dynamic arrays
    char** labels = malloc(mark_count * sizeof(char*));
    if(!labels){
        fprintf(stderr, "Could not allocate from heap\n");
        exit(1);
    }
    char** paths = malloc(mark_count * sizeof(char*));
    if(!paths){
        fprintf(stderr, "Could not allocate from heap\n");
        exit(1);
    }

    // TODO: just change cursor position
    fclose(file);
    file = fopen(file_path, "r");
    if(!file){
        fprintf(stderr, "Could not open %s\n", file_path);
        exit(1);
    }

    // read in content
    size_t i = 0;
    while(fgets(buffer, MAX_LABEL_SIZE, file)){ 
        size_t split_index = find_index(buffer, ':');
        if(split_index >= MAX_LABEL_SIZE){
            fprintf(stderr, "Too long entry in %s:%zu :\n%s\n", file_path, i+1, buffer);
            fclose(file);
            exit(1);
        }
        if(buffer[strlen(buffer)-1] == '\n')
            buffer[strlen(buffer)-1] = '\0';
        size_t path_length = strlen(buffer) - split_index;
        buffer[split_index] = '\0';
        char* label = malloc(split_index);
        if(!label){
            fprintf(stderr, "Could not allocate from heap\n");
            exit(1);
        }
        strncpy(label, buffer, split_index);
        labels[i] = label;
        char* path = malloc(path_length);
        if(!path){
            fprintf(stderr, "Could not allocate from heap\n");
            exit(1);
        }
        strncpy(path, buffer+split_index+1, path_length);
        paths[i] = path;

        i++;
    }
    if(i<mark_count){
        labels[i] = 0;
        paths[i] = 0;
    }

    return (struct marks) {labels, paths, mark_count};
}

// NOTE: This method can be improved!
char* match_token(char* token, struct marks marks){
    size_t i = 0;
    while(i < marks.len && marks.labels[i]){
        if(!strcmp(token, marks.labels[i])){
            return marks.paths[i];
        }
        i++;
    }
    return token;
}

void translate_paths(size_t n, char** paths, char* file_path){    
    if(!n)
        return;
    struct marks marks = load_marks(file_path);
    for(size_t i=0; i<n; i++){
        size_t split_index = find_index(paths[i], '/');
        size_t token_len = strlen(paths[i]);
        paths[i][split_index] = '\0';
        char* result = match_token(paths[i], marks);
        printf("%s", result);
        // keep input slash
        if(token_len - split_index == 1){
            printf("/");
        }
        if(split_index < token_len - 1){
            printf("/%s", paths[i] + split_index + 1);
        }
        if(i < n-1){
            printf(" ");
        }
    }
    printf("\n");

    size_t i = 0;
    while(i < marks.len){
        free(marks.labels[i]);
        free(marks.paths[i]);
        i++;
    }
}

