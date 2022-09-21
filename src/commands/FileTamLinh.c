// // C program for reading
// // struct from a file
// #include <stdio.h>
// #include <stdlib.h>

// // struct UserInfo with 3 fields
// typedef struct
// {
//     char *current_user;
//     char *host_name;
//     char *root_dir;
//     char *current_dir;
// } UserInfo;

// // Driver program
// int main()
// {
//     // FILE *outfile;

//     // // open file for writing
//     // outfile = fopen ("usershell", "w");
//     // if (outfile == NULL)
//     // {
//     //     fprintf(stderr, "\nError opened file\n");
//     //     exit (1);
//     // }

//     //  UserInfo input1 = {"root", "rohan", "sharma","mo"};
//     //  UserInfo input2 = {"riit", "mahendra", "dhoni","no"};

//     // // write struct to file
//     // fwrite (&input1, sizeof( UserInfo), 1, outfile);
//     // fwrite (&input2, sizeof( UserInfo), 1, outfile);

//     // if(fwrite != 0)
//     //     printf("contents to file written successfully !\n");
//     // else
//     //     printf("error writing file !\n");

//     // // close file
//     // fclose (outfile);

//     // FILE *infile;
//     // UserInfo input[5];

//     // // Open UserInfo.dat for reading
//     // infile = fopen("usershell", "rb");
//     // if (infile == NULL)
//     // {
//     //     fprintf(stderr, "\nError opening file\n");
//     //     exit(1);
//     // }

//     // unsigned int n_stud = 0;
//     // while (fread(&input[n_stud], sizeof(UserInfo), 1, infile) == 1)
//     // {
//     //     n_stud++;
//     // }

//     // printf("%ld",n_stud);

//     // for (int i = 0; i < n_stud; i++)
//     // {
//     //     printf("%s\n", input[i].current_user);
//     // }
//     // // close file
//     // fclose(infile);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 50
#define N_STUD 2

typedef struct stud
{
    char name[LEN + 1];
    unsigned int id;
    float average;
} stud_t;

typedef struct UrIn
{
    char current_user[255];
    char host_name[255];
    char root_dir[255];
    char current_dir[255];
} UserInfo;

int main()
{
    FILE *fp;
    UserInfo student[N_STUD];
    int i;
    char row[LEN + 1];
    unsigned int n_stud;

    // strcpy(student[0].current_dir, "Steven");
    // strcpy(student[0].current_user, "1");
    // strcpy(student[0].host_name, "Stevedn");
    // strcpy(student[0].root_dir, "Steven");

    // strcpy(student[1].current_dir, "Steven2");
    // strcpy(student[1].current_user, "2");
    // strcpy(student[1].host_name, "Stevedn2");
    // strcpy(student[1].root_dir, "Steven2");

    // fp = fopen("out.bin", "wb"); /* Open the file for writing */
    // if (fp == NULL)
    // {
    //     printf("Error: file out.bin cannot be opened\n");
    //     exit(1);
    // }

    //   /* Write the file */
    //   /* NOTE 2: a binary file sometimes cannot be readable
    //      correctly in a PC that it is not the one which generates it,
    //      because, for instance, integer numbers can be coded with a
    //      different number of bytes.
    //   */

    //   /* Write in binary all the data contained in the structure */
    // fwrite(student, sizeof(UserInfo), N_STUD, fp);

    //   /* DIMENSION OF THE GENERATED FILE
    //      The dimension of the generated file will be:
    //      n_stud*sizeof(stud_t)
    //      in the case of the computer used to test the program:
    //      2*60 = 120 byte
    //    */

    // fclose(fp); /* Close the file */

    fp = fopen("out.bin", "r"); /* Open the file for reading */
    if (fp == NULL)
    {
        printf("Error: file out.bin cannot be opened\n");
        exit(1);
    }

    /* Read the file */
    
    while (fread(&student[n_stud], sizeof(UserInfo), 1, fp) == 1)
    {
        n_stud++;
    }

    fclose(fp); /* Close the file */

    /* Print the read records */
    for (i = 0; i < n_stud; i++)
    {
        printf("%s %s \n", student[i].current_user, student[i].current_dir);
    }

    return 0;
}