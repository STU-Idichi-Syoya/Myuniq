#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define DEFAULT_LEN 1

typedef struct
{
    char *str;
    int now_str_arr_len;
    int end_point; // \0 の添え字番号を示す。
} String;

void String_init(String *s)
{
    s->str = (char *)malloc(sizeof(char) * DEFAULT_LEN);
    s->now_str_arr_len = DEFAULT_LEN;
    s->end_point = 0;
}
int String_isEmpty(String *s){
    return s->str[0]=='\0';
}
void String_add_char(String *s, int a)
{
    char *tmp;
    if (s->end_point + 2 >= s->now_str_arr_len)
    {
        tmp = realloc(s->str, s->now_str_arr_len + 50);
        if (tmp != NULL)
        {
            //pass
            s->str = tmp;
            s->now_str_arr_len += 50;
            printf("called calloc str_arr_len-->%d\n", s->now_str_arr_len);
        }
        else
        {
            /*error*/
            printf("error str calloc!!\n");
        }
    }

    s->str[s->end_point++] = a;
    s->str[s->end_point] = '\0';
}
/*String を初期化し、次回も利用できるようにする*/
void String_recycle(String *s){
    s->end_point=0;
    s->str[0]='\0';
}
void String_delite(String *s){
    free(s->str);
}
/* stringを初期化してから、
*EOFの場合EOFを返し、それ以外の場合、1を返す。 
* 注意！ \n 一文字の場合、String s.strは[\0]となる。
*/
int readline(String *s, FILE *fp)
{
    int c, count = 0;
    String_recycle(s);
    c = getc(fp);
    while (c != EOF)
    {
        if (c == '\n')
        {
            String_add_char(s, '\n');
            return 1;
        }
        String_add_char(s, c);
        c = getc(fp);
    }
    return EOF;
}

void main()
{
    FILE *fp = fopen("a.txt", "r");
    String s1, s2;

    int file_not_eof = 1, file_not_eof2 = 1,
        count = 1;
    char *tmp;
    String_init(&s1);
    String_init(&s2);

    // do
    // {
    //     file_not_eof=readline(&s1,fp);

    //     if(strcmp(s1.str,s2.str)==0){
    //         count++;
    //     }else{
    //         printf("%2d %s",count);
    //         count=1;
    //         tmp=s1.str;
    //         s1.str=s2.str;
    //         s2.str=s1.str;

    //     }

    // } while (file_not_eof!=EOF&&file_not_eof2!=EOF);
    file_not_eof = readline(&s1, fp);
    if (file_not_eof != EOF)
    {
        while (1)
        {
            count = 0;
            do
            {
                file_not_eof2 = readline(&s2, fp);
                count++;
            } while (file_not_eof2 != EOF && strcmp(s1.str, s2.str) == 0);

            printf("count=%d %s",count,s1.str);

            if (file_not_eof2 != EOF)
            {
                    tmp=s1.str;
                    s1.str=s2.str;
                    s2.str=tmp;
            }else
            {   
                /*<string><\n>EOFの場合、s2は空要素である。なので、空要素のときは表示しない*/
                if(!String_isEmpty(&s2))
                    printf("count=%d %s",1,s2.str);
                break;
            }
            
        }
    }
    String_delite(&s1);String_delite(&s2);

}

