#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

static void usagee(void);
static void scanfilesa(int argc, char *argv[]);
static void expands(FILE *from);

int TAB = 8;
int rval = EXIT_SUCCESS;
int iflag = 0, tflag = 0;

int main(int argc, char *argv[])
{
    char ch;

    while ((ch = getopt(argc, argv, "i:t:")) != -1)
    {
        switch (ch)
        {
        case 'i':
            iflag = 1;
            break;
        case 't':
            TAB = atoi(optarg);
            tflag = 1;
            break;
        default:
            usagee();
            break;
        }
    }

    argv += optind;
    argc -= optind;

    scanfilesa(argc, argv);
    exit(rval);
}

static void usagee(void)
{
    (void)fprintf(stderr, "expand --help for more information.\n");
    exit(EXIT_FAILURE);
}

static void scanfilesa(int argc, char *argv[])
{
    int i;
    int iflag, tflag, rval;

    if (argc == 0)
    {
        if (iflag)
        {
            expands(stdin);
        }
        else if (tflag)
        {
            expands(stdin);
        }
        else
        {
            expands(stdin);
        }
        return;
    }

    for (i = 0; i < argc; i++)
    {
        char *path = argv[i];
        if (path == NULL)
        {
            printf("No such file or directory\n");
        }
        else if (strcmp(path, "-") == 0)
        {
            expands(stdin);
        }
        else
        {
            FILE *fp = fopen(path, "r");
            if (fp == NULL)
            {
                (void)fprintf(stderr, "%sがありません\n", path);
                rval = EXIT_FAILURE;
            }
            else if (iflag)
            {
                expands(fp);
                (void)fclose(fp);
            }
            else if (tflag)
            {
                expands(fp);
                (void)fclose(fp);
            }
            else
            {
                expands(fp);
                (void)fclose(fp);
            }
        }
    }
}

static void expands(FILE *from)
{
    int rest = TAB;
    int ch;
    /*ファイルの終端(EOF)になるまで続ける*/
    while ((ch = fgetc(from)) != EOF)
    {

        if (ch == '\t')
        { /*タブが使われたとき、rest(tab)が0になるまで空白入れる*/
            for(int i=0;i<rest;i++)
                printf(" ");
        }
        else if (ch == '\n')
        { /*改行が入力されたとき改行する*/
            printf("\n");
        }
        else
        { /*それ以外は普通に入力*/
            printf("%c", ch);
        }
    }
    printf("\n");
}

