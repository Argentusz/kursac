#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SETT_MAKS_SIZE 5  // Max number of digits of numbers in settings
#define TEXT_MAKS_SIZE 128  // Max symbols in line
#define STOP 6  // Stop symbol size
#define FILE_NAME_MAKS 32 // File's name max size
#define SETTINGS_NAME "settings.letitop"  // Settings file name
__attribute__((unused)) void entrance();  // entrance function described down below

/** Deleting line transfers at the end of the string **/
void format(char *a) {
    if (a[strlen(a)] == '\n') {
        a[strlen(a)] = '\0';
    }
}

/** Function used to put long type variables into settings file **/
void put_in_settings (long const *fontsize, long const *fieldsize) {
    char set_out[SETT_MAKS_SIZE];
    FILE * sett;
    sett = fopen(SETTINGS_NAME, "w");
    sprintf(set_out, "%ld", *fontsize); // Converting into a string
    fprintf(sett, "%s\n", set_out); // Putting into a file
    sprintf(set_out, "%ld", *fieldsize);
    fprintf(sett, "%s\n", set_out);
    if (fclose(sett) == EOF) {
        printf("Error closing file.");
        exit(1);
    }
}

/** Reading variables values from settings file **/
void get_from_settings (long *fontsize, long *fieldsize) {
    FILE * settings_file_name;  // File with settings
    settings_file_name = fopen(SETTINGS_NAME, "r");  // Reading File with written settings
    if (settings_file_name == NULL) {
        // If settings file doesn't exist
        // Put Default Settings
        *fontsize = 11;
        *fieldsize = 165;
        // Create Settings file with default settings
        put_in_settings(fontsize, fieldsize);
    } else {
        char fos[SETT_MAKS_SIZE], fis[SETT_MAKS_SIZE], *err;
        fgets(fos, SETT_MAKS_SIZE, settings_file_name); // Reading Values from file
        fgets(fis, SETT_MAKS_SIZE, settings_file_name);
        format(fos);
        format(fis);
        *fontsize = strtol(fos, &err, 0);  // Converting to long integer and give to global variable
        *fieldsize = strtol(fis, &err, 0);
    }
    if (fclose(settings_file_name) == EOF) {
        printf("Error closing file.");
        exit(1);
    }
}



/** Function used to write result into a file **/
void WritePut(char const *all_text, int const *line, long const *fontsize, long const *fieldsize, int mode) {
    char file_name[FILE_NAME_MAKS];
    char text[TEXT_MAKS_SIZE];
    int i, in_line, jn, j;
    FILE * out;  // File to put text in
    system("clear");
    if (mode == 2) {
        printf("Enter the file`s name\n");
        fgets(file_name, FILE_NAME_MAKS, stdin); // Get file name from user input
        format(file_name);
        out = fopen(file_name, "w");
    } else {
        out = stdout; // If user chose output in console
    }
    j = 0;
    for(i = 0; i < *line; i++) {
        // Get the line of the text
        for(jn = 0; all_text[j] != '\n'; jn++) {
            text[jn] = all_text[j++];
        }
        j++;
        text[jn] = '\0';
            // If it is small enough to fit in field then just print it ...
        if (jn * (*fontsize) < *fieldsize) {
            fprintf(out, "%s", text);
        }
        else {
            // ... If not we will print words one by one
            in_line = 0;
            char* piece = strtok(text, " ");
            in_line += (int) strlen(piece); // We'll count how many there are symbols
                                            // in line to check if the next word will fit in the line
            while(piece != NULL) {
                if(in_line * (*fontsize) > *fieldsize) {
                    // If next word of line doesn't fit in the field make a new line
                    fprintf(out, "\n"); in_line = (int) strlen(piece);
                }
                // Print space if it is not the first word of the line
                if(in_line > strlen(piece)) {
                    fprintf(out, " ");
                }
                fprintf(out, "%s", piece);
                piece = strtok(NULL, " "); // Get the next word
                if(piece != NULL) {
                    in_line += (int) strlen(piece) + 1;
                }
            }
        }
        fprintf(out, "\n");
    }
    // Close the file if it was user custom file
    if (mode == 2) {
        if (fclose(out) == EOF) {
            printf("Error closing file.");
            exit(1);
        }
    }
}


/** Function used to get input from console **/
void ConsoleI(long const *fontsize, long const *fieldsize){
    int line, mode, i, dy_len;
    char text[TEXT_MAKS_SIZE];
    char* dy_text;
    char stop[STOP] = "!stop\n"; // Used to check when the user is done with inputting his text
    mode = 0;
    line = 0;
    dy_len = 0;
    dy_text = (char*)malloc(1*sizeof(char)); // Allocating the dynamic array where user's text will be located
    if (dy_text == NULL) {
        printf("Memory Error!");
        exit(1);
    }
    else {
        printf("Pick text destination:\n1 - Console\n2 - File\n");
        while (mode != 1 && mode != 2) { scanf("%d", &mode); } // Determine where to put the result
        system("clear");
        printf("Enter the text (Print '!stop' in the empty line to end.): \n");
        getchar();
        fgets(text, TEXT_MAKS_SIZE, stdin); // Get the first line
        // Appending new text until '!stop' comes out
        while (strcmp(stop, text) != 0) {
            line++;
            // Adding memory for a text array just so it would fit the line
            if ((dy_text = (char *) realloc(dy_text,dy_len + strlen(text) * sizeof(char))) == NULL) {
                printf("Memory Error!");
                exit(1);
            }
            // just so it would fit the line
            for (i = 0; i < strlen(text); i++) {
                dy_text[dy_len++] = text[i];  // Appending current line to dynamic array
            }
            fgets(text, TEXT_MAKS_SIZE, stdin); // Getting the next line
        }
        WritePut(dy_text, &line, fontsize, fieldsize, mode); // Go to Output stage
    }
}

/** Function used to get input from the file **/
void FileI(long const *fontsize, long const *fieldsize) {
    int line, mode, dy_len, i;
    FILE * fp;
    char* dy_text;
    char file_name[FILE_NAME_MAKS];
    char text[TEXT_MAKS_SIZE];
    mode = 0;
    dy_len = 0;
    dy_text = (char*)malloc(1*sizeof(char)); //  Allocating the dynamic array where user's text will be located
    if (dy_text == NULL) {
        printf("Memory Error!");
        exit(1);
    }
    else {
        printf("Pick text destination:\n1 - Console\n2 - File\n"); // Determine where to put the result
        while (mode != 1 && mode != 2) scanf("%d", &mode);
        printf("Enter the file`s name\n");
        getchar();
        fgets(file_name, FILE_NAME_MAKS, stdin);
        format(file_name);
        line = 0;
        fp = fopen(file_name, "r");
        if (fp == NULL) {
            printf("\nError Occurred While Trying To read the file\n");
            exit(1);
        } else {
            while (!feof(fp)) // Reading the file until it ends.
            {
                fgets(text, TEXT_MAKS_SIZE, fp);
                line++;
                // Adding memory for a text array just so it would fit the line
                if ((dy_text = (char *) realloc(dy_text,dy_len + strlen(text) * sizeof(char))) == NULL) {
                    printf("Memory Error!");
                    exit(1);
                }
                for (i = 0; i < strlen(text); i++) {
                    dy_text[dy_len++] = text[i]; // Appending the line into dynamic array
                }
            }
            // The WritePut() will now work if not all the lines have line transfer symbol at the end.
            // So we double-check it
            if (dy_text[dy_len] != '\n') {
                // Add it if not found
                if ((dy_text = (char *) realloc(dy_text,dy_len + sizeof(char))) == NULL) {
                    printf("Memory Error!");
                    exit(1);
                }
                dy_text[dy_len + 1] = '\n';
            }
            WritePut(dy_text, &line, fontsize, fieldsize, mode);


        }
        if (fclose(fp) == EOF) {
            printf("Error closing file.");
            exit(1);
        }
    }
}

/** Preferences Function **/
void settings(long *fontsize, long *fieldsize) {
    int set_mode;
    set_mode = 0;
    while (set_mode != 3) {
        printf("Current Font Size is %ld px\n"
               "Current Field Size is %ld px\n"
               "Type: \n"
               "1 - To change Font Size\n"
               "2 - To change Field Size\n"
               "3 - To Quit Preferences\n", *fontsize, *fieldsize);
        scanf(" %d", &set_mode);
        if (set_mode == 1) {
            printf("New Font Size: ");
            scanf("%ld", fontsize);
            put_in_settings(fontsize, fieldsize);
        } else if (set_mode == 2) {
            printf("New Field Size: ");
            scanf("%ld", fieldsize);
            put_in_settings(fontsize, fieldsize);
        } else if (set_mode != 3) {
            printf("Have No Operations for mode %d", set_mode);
        }
    }
    entrance(fontsize, fieldsize);
}

/** Main menu of the program **/
void entrance(long *fontsize, long *fieldsize) {
    char mode;
    system("clear");
    /* Setting User Preferences */
    printf("Text In Field Output\n\n"
           "Current Font Size is %ld px\n"
           "Current Field Size is %ld px\n", *fontsize, *fieldsize);
    printf("Print 'S' to start, 'P' to open preferences, Any other key to quit: ");
    scanf(" %c", &mode);
    printf("\n");
    if (mode == 'P' || mode == 'p') {
        settings(fontsize, fieldsize);
    } else if (mode == 'S' || mode == 's'){
        printf("Pick a mode of operation:\n"
               "1 - Type text in console\n"
               "2 - Pick a file\n"
               "9 - Go Back\n"
               "Any Key - To exit\n");
        scanf(" %c", &mode);
        printf("\n");
        if (mode == '1') {
            ConsoleI(fontsize, fieldsize);
        } else if (mode == '2') {
            FileI(fontsize, fieldsize);
        } else if (mode == '9') {
            entrance(fontsize, fieldsize);
        }
    }
}

/** Entry point **/
int main() {
    long fontsize, fieldsize;
    get_from_settings(&fontsize, &fieldsize);
    entrance(&fontsize, &fieldsize);
    return 0;
}
