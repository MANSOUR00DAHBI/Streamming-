#include "H.h"
#define H_H
#define FILESIZE 4096
#define MESSAGE "Hello from mmap!"


void Info_file(const char* filename) {
    if (stat(filename, &fileStat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    printf(" File          :> %s \n", filename);
    printf(" Size          :> %ld bytes \n", fileStat.st_size);
    printf(" Permissions   :> %o \n", fileStat.st_mode);
    printf(" Last accessed :> %ld \n", fileStat.st_atime);

}
int Folder(const char* dirName) {

    if (mkdir(dirName, 0777) == -1) {
        perror("Error creating directory");
        exit(EXIT_FAILURE);
    }

    printf("Directory created: %s\n", dirName);
    return 0;
}

int main(int argc , char *argv[]) {
       
    ASSERT(argc <= 3);
    if (argc <= 3) {
        fprintf(stderr, "Usage: %s <filename> <directory_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *filename = argv[1];
    const char *namedir    = argv[2];
     Info_file(filename);
           
     Folder(namedir);
  
     // Step 3: Open the file for memory mapping
     int fd = open(filename, O_RDWR | O_CREAT, 0666);
     if (fd == -1) {
         perror("Error opening file for writing");
         exit(EXIT_FAILURE);
     }

     // Step 4: Set the file size
     if (lseek(fd, FILESIZE - 1, SEEK_SET) == -1) {
         perror("Error calling lseek() to 'stretch' the file");
         close(fd);
         exit(EXIT_FAILURE);
     }

     if (write(fd, "", 1) == -1) {
         perror("Error writing last byte of the file");
         close(fd);
         exit(EXIT_FAILURE);
     }

     // Step 5: Map the file to memory
     char* map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
     if (map == MAP_FAILED) {
         perror("Error mmapping the file");
         close(fd);
         exit(EXIT_FAILURE);
     }

     // Step 6: Write to the mapped memory
     strcpy(map, MESSAGE);
     printf("Written to memory: %s\n", map);

     // Step 7: Read from the mapped memory
     printf("Read from memory: %s\n", map);

     // Step 8: Unmap the memory and close the file
     if (munmap(map, FILESIZE) == -1) {
         perror("Error unmapping the file");
     }

     close(fd);

    return EXIT_SUCCESS;
}