#include "../headers.h"
#include "ls.h"

// handle ls dir h

int compare(const void *a, const void *b) {
    char a_copy[MAX_BUFFER_SIZE];
    char b_copy[MAX_BUFFER_SIZE];
    strcpy(a_copy, *(char**)a);
    strcpy(b_copy, *(char**)b);
    for(int i = 0; i < strlen(a_copy); i++) {
        a_copy[i] = tolower(a_copy[i]);
    }
    for(int i = 0; i < strlen(b_copy); i++) {
        b_copy[i] = tolower(b_copy[i]);
    }
    return strcmp(a_copy, b_copy);
}

int ls(int argn, char** args)
{
    int to_return = 0;
    int lon = 0, all = 0;
    char* dirs[MAX_BUFFER_SIZE];
    char* fls[MAX_BUFFER_SIZE];
    int num_dirs = 0, num_fls = 0, invalids = 0;
    for(int i = 1; i < argn; i++) {
        if(strcmp(args[i], "-l") == 0) {
            lon = 1;
        } else if(strcmp(args[i], "-a") == 0) {
            all = 1;
        } else if(strcmp(args[i], "-la") == 0 || strcmp(args[i], "-al") == 0) {
            lon = 1;
            all = 1;
        } else if(opendir(args[i]) != NULL) {
            dirs[num_dirs] = args[i];
            num_dirs++;
        } else if(access(args[i], F_OK) == 0) {
            fls[num_fls] = args[i];
            num_fls++;
        } else if(strncmp(args[i], "~", 1) == 0) {
            // dirs[num_dirs] = owd;
            char* temp = (char*)malloc(sizeof(char) * 1024);
            strcpy(temp, owd);
            strcat(temp, args[i] + 1);
            if(opendir(temp) != NULL) {
                dirs[num_dirs] = temp;
                num_dirs++;
            } else if(access(temp, F_OK) == 0) {
                fls[num_fls] = temp;
                num_fls++;
            } else {
                invalids++;
                printf("%s: No such file or directory\n", args[i]);
                to_return = 1;
            }
        } else {
            invalids++;
            printf("%s: No such file or directory\n", args[i]);
            to_return = 1;
        }
    }
    // if(to_return) return to_return;
    qsort(fls, num_fls, sizeof(char*), compare);
    for(int i = 0; i < num_fls; i++) {
        if(lon) {
            file_details(fls[i]);
        }
        char per[11];
        get_permissions(fls[i], per);
        if(per[3] == 'x' || per[6] == 'x' || per[9] == 'x') {
            printf(GRN);
        }
        printf("%s" RESET "\n", fls[i]);
    }
    if(num_dirs == 0 && num_fls == 0 && invalids == 0) {
        return list(".", lon, all);
    } else if(num_dirs == 1 && num_fls == 0 && invalids == 0) {
        return list(dirs[0], lon, all);
    } else if(num_dirs > 0 && (num_fls > 0 || invalids > 0)) {
        printf("\n");
    }
    qsort(dirs, num_dirs, sizeof(char*), compare);
    for(int i = 0; i < num_dirs; i++) {
        if(i) printf("\n");
        printf("%s:\n", dirs[i]);
        to_return += list(dirs[i], lon, all);
    }
    return to_return;
}

int numLength(int num) {
    int len = 0;
    while(num) {
        num /= 10;
        len++;
    }
    return len;
}

int list(char* directory, int lon, int all) {
    DIR *dir = opendir(directory);
    if(dir == NULL) {
        printf("%s: No such file or directory\n", directory);
        return 1;
    }
    struct dirent *dirent;
    // save the d_name in an array
    char** files = (char**)malloc(sizeof(char*) * MAX_BUFFER_SIZE);
    int i = 0;
    int column_width[8] = {0};
    column_width[0] = 10;
    int total = 0;
    while((dirent = readdir(dir)) != NULL) {
        if(!all && dirent->d_name[0] == '.') {
            continue;
        }
        if(lon) {
            char path[MAX_BUFFER_SIZE];
            sprintf(path, "%s/%s", directory, dirent->d_name);
            struct stat s;
            stat(path, &s);
            total += (s.st_blocks * 512 + 1023) / 1024;
        }
        files[i] = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
        strcpy(files[i], dirent->d_name);
        i++;
    }
    if(i && lon) printf("total %d\n", total);
    // sort files alphabetically
    // char ***to_print = (char***)malloc(sizeof(char**) * MAX_BUFFER_SIZE);
    qsort(files, i, sizeof(char*), compare);
    for(int j = 0; j < i; j++) {
        char name[MAX_BUFFER_SIZE];
        char temp[MAX_BUFFER_SIZE];
        strcpy(temp, files[j]);
        strcpy(name, directory);
        if(directory[strlen(directory) - 1] != '/') strcat(name, "/");
        strcat(name, files[j]);
        char perm[11];
        get_permissions(name, perm);
        if(perm[0] == 'd') {
            strcpy(files[j], BLU);
        } else if(perm[3] == 'x' || perm[6] == 'x' || perm[9] == 'x') {
            strcpy(files[j], GRN);
        } else {
            strcpy(files[j], RESET);
        }
        strcat(files[j], temp);
        strcat(files[j], RESET);
        if(lon) {
            file_details(name);
        }
        printf("%s\n", files[j]);
    }
    for(int j = 0; j < i; j++) {
        free(files[j]);
    }
    free(files);
    closedir(dir);
    return 0;
}

void file_details(char* file) {
    char perm[11];
    get_permissions(file, perm);
    printf("%s", perm);

    struct stat fileStat;
    stat(file, &fileStat);
    // int currLen = digitCount(fileStat.st_nlink);
    printf(" ");
    printf("%lu ", fileStat.st_nlink);

    // Owner, left aligned
    printf("%s ", getpwuid(fileStat.st_uid)->pw_name);
    // currLen = strlen(getpwuid(fileStat.st_uid)->pw_name);
    printf(" ");

    // Group, left aligned
    printf("%s ", getgrgid(fileStat.st_gid)->gr_name);
    // currLen = strlen(getgrgid(fileStat.st_gid)->gr_name);
    printf(" ");

    // File size, right align
    // currLen = digitCount(fileStat.st_size);
    printf(" ");
    printf("%5.lu ", fileStat.st_size);

    // Time, auto-formatted
    // char t[MAX_BUFFER_SIZE];
    // strncpy(t, ctime(&fileStat.st_mtime), strlen(ctime(&fileStat.st_mtime)) - 9);
    // printf("%s ", t);
    char datetime[MAX_BUFFER_SIZE];
    strftime(datetime, 14, "%b %d %H:%M", localtime(&(fileStat.st_mtime)));
    printf("%s ", datetime);
    // printf("%s ", asctime(localtime(&(fileStat.st_mtime))) + 4);
}


void get_permissions(char* file, char* perm)
{
    struct stat s;
    stat(file, &s);
    if(S_ISDIR(s.st_mode)) {
        perm[0] = 'd';
    } else {
        perm[0] = '-';
    }
    if(s.st_mode & S_IRUSR) {
        perm[1] = 'r';
    } else {
        perm[1] = '-';
    }
    if(s.st_mode & S_IWUSR) {
        perm[2] = 'w';
    } else {
        perm[2] = '-';
    }
    if(s.st_mode & S_IXUSR) {
        perm[3] = 'x';
    } else {
        perm[3] = '-';
    }
    if(s.st_mode & S_IRGRP) {
        perm[4] = 'r';
    } else {
        perm[4] = '-';
    }
    if(s.st_mode & S_IWGRP) {
        perm[5] = 'w';
    } else {
        perm[5] = '-';
    }
    if(s.st_mode & S_IXGRP) {
        perm[6] = 'x';
    } else {
        perm[6] = '-';
    }
    if(s.st_mode & S_IROTH) {
        perm[7] = 'r';
    } else {
        perm[7] = '-';
    }
    if(s.st_mode & S_IWOTH) {
        perm[8] = 'w';
    } else {
        perm[8] = '-';
    }
    if(s.st_mode & S_IXOTH) {
        perm[9] = 'x';
    } else {
        perm[9] = '-';
    }
    perm[10] = '\0';
}