#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define DEFAULT_LEN 100

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
int String_isEmpty(String *s)
{
    return s->str[0] == '\0';
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
            // printf("called calloc str_arr_len-->%d\n", s->now_str_arr_len);
        }
        else
        {
            /*error*/
            /*fprintf("\n");*/
        }
    }

    s->str[++s->end_point] = a;
    s->str[s->end_point] = '\0';
}
/*空要素の場合、\0 を返す*/
char String_get_last(String *s)
{
    if (String_isEmpty(s))
    {
        return '\0';
    }
    else
    {
        return s->str[s->end_point - 1];
    }
}
/*String を初期化し、次回も利用できるようにする*/
void String_recycle(String *s)
{
    s->end_point = 0;
    s->str[0] = '\0';
}
void String_delite(String *s)
{
    free(s->str);
}
/* stringを初期化してから、
*EOFの場合EOFを返し、それ以外の場合、1を返す。 
* 注意！ \n 一文字の場合、String s.strは[\n\0]となる。
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

/*u_option==1は重複する行を除く*/
void print_Default(int count, String *s, int u_option)
{
    char last = String_get_last(s);

    if (u_option == 1 && count != 1)
        return;

    if (last != '\0' && last == '\n')
        printf("%s", s->str);
    else
    {
        printf("%s\n", s->str);
    }
}
/*u_option==1は重複する行を除く*/
void print_CountMode(int count, String *s, int u_option)
{
    char last = String_get_last(s);

    if (u_option == 1 && count != 1)
        return;
    if (last != '\0' && last == '\n')
        printf("      %d %s", count, s->str);
    else
        printf("      %d %s\n", count, s->str);
}

void main()
{
    char *s = "ab\0de";
    printf("%s", s);
}
