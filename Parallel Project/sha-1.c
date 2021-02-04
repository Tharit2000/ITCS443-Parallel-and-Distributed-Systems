#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sha1.c"
#include "sha1.h"

char *resultarray[4] =
{
    "A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D",
    "84 98 3E 44 1C 3B D2 6E BA AE 4A A1 F9 51 29 E5 E5 46 70 F1",
    "34 AA 97 3C D4 C4 DA A4 F6 1E EB 2B DB AD 27 31 65 34 01 6F",
    "DE A3 56 A2 CD DD 90 C7 A7 EC ED C5 EB B5 63 93 4F 46 04 52"
};

int main()
{
    SHA1Context sha;
    int err;
    uint8_t Message_Digest[20];

    FILE *file;
    file = fopen("data.txt","w+");
    if (file == NULL) {
        printf("Error!");
        exit(1);
    }

    for(int i=0; i<=99999999; i++)
    {
        char str[9];
        sprintf(str,"%08d",i);

        fprintf(file, "%s,", str);
        printf( "\nPassword: '%s'\n", str);

        err = SHA1Reset(&sha);
        if (err)
        {
            fprintf(stderr, "SHA1Reset Error %d.\n", err );
        }

        err = SHA1Input(&sha, (const unsigned char *) str, strlen(str));
        if (err)
        {
            fprintf(stderr, "SHA1Input Error %d.\n", err );
        }

        err = SHA1Result(&sha, Message_Digest);
        if (err)
        {
            fprintf(stderr,"SHA1Result Error %d, could not compute message digest.\n",err );
        }
        else
        {
            char res[20];
            for(int i = 0; i < 20 ; ++i)
            {
                fprintf(file, "%x", Message_Digest[i]);
                printf("%x", Message_Digest[i]);
            }
            fprintf(file, "\n");
            printf("\n");
        }
    }
    fclose(file);

    return 0;
}