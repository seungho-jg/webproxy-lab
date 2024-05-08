#include <stdio.h>

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL){
        perror("Error");
        return 1;
    }

    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL){
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }
    char buffer[1024];
    size_t bytes_read;
    while((bytes_read = fread(buffer, 1, sizeof(buffer), input_file)) > 0)
    {
        fwrite(buffer, 1, bytes_read, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    printf("done\n");

    return 0;
}