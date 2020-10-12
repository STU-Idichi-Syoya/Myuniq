#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include<ctype.h>
#define DEFAULT_LEN 100
#define VERSION "1.0.0"

typedef struct str
{
    char *str;
    int now_str_arr_len;
    int end_point; /* \0 の添え字番号を示す。*/
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
        tmp = realloc(s->str, s->now_str_arr_len*2);
        if (tmp != NULL)
        {
            /*pass*/
            s->str = tmp;
            s->now_str_arr_len *= 2;
            /* printf("called calloc str_arr_len-->%d\n", s->now_str_arr_len);*/
        }
        else
        {
            /*error*/
            fprintf(stderr,"Mem Over!!\n");
            exit(EXIT_FAILURE);
        }
    }

    s->str[s->end_point] = a;
    s->end_point++;
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
        printf("last-->%c kaigyou?-->%d\n", s->str[s->end_point - 1], s->str[s->end_point - 1] == '\n');
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
* 注意！ \n 一文字の場合、String s.strは[\0]となる。
* EOF のみの場合、0を返す。
* \nのみ場合、2を返す
* それ以外は1を返す。
*/
int readline(String *s, FILE *fp,int *flg)
{
    int c;
    *flg=0;
    String_recycle(s);
    c = getc(fp);
    while (c != EOF)
    {
        if (c == '\n')
        {
            /* String_add_char(s, '\n');*/
            if(*flg!=1)
                *flg=2;
            
            return 2;
        }
        *flg = 1;
        String_add_char(s, c);
        c = getc(fp);
    }
    return EOF;
}

void putString(String *s)
{
    char last;
    int i, flg = 0;
    i = 0;
    while (s->str[i] != '\0')
    {
        last = s->str[i];
        if (last == '\n')
            flg = 1;
        putchar(last);
        i++;
    }

    if (flg == 0)
    {
        putchar('\n');
    }
}
void print_Default(int count, String *s, int u_option)
{
    if (u_option == 1 && count > 1)
        return;
    
    putString(s);
}
/*u_option==1は重複する行を除く*/
void print_CountMode(int count, String *s, int u_option)
{
    if (u_option == 1 && count > 1)
        return;

    printf("%7d ", count);
    putString(s);
}

void usage(){
    char* out="Usage: uniq [option] [filePath] )\n-c 重複した⾏と重複していない⾏のそれぞれの数と⾏の内容を表⽰する。\n-i 英⽂字の⼤⽂字⼩⽂字を無視して判定する。例)aとAは同じとみなす。\n-u 重複した⾏を出⼒せず、重複していない⾏のみを出⼒する。\n-V バージョン情報を表⽰。\n";
    printf("%s",out);
}
void error_version(){
    printf("if you use -V, you cannot use it with other options.\n");
    printf("(-V または--versionを使用する場合、ほかのオプションと併用できません。)\n");
}
void show_version(){
    printf(VERSION);
    printf("\n");
}

/**/
int strcasecmp(const char *s1, const char *s2)
 {
   while (toupper(*s1) == toupper(*s2))
   {
     if (*s1 == 0)
       return 0;
     s1++;
     s2++;
   }
   return 1;
 }
 void hide_anime(){
    int s,i,j;
    for(s=0;s<3;s++){
        for(i=0;i<=3;i++){
            printf("\r〆(.. )受験勉強中 ");
            for(j=0;j<i;j++){
                printf("ｶｷ ");
            }
            for(j=i;j<=3;j++){
                printf("   ");
            }
            
            fflush(stdout);
            sleep(1);
        }
    }
    printf("\r| 合格発表 |関大落ち          ");
    printf("            ");
    printf("uniq version");
    printf(VERSION);
    printf("     https://idichi.tk");
    printf("\n");
 }
 
int main(int argc,char** argv)
{
    FILE *fp;
    String s1, s2;
    int b_flg=-1;
    int file_not_eof = 1, file_not_eof2 = 1,
        count = 1, flg;
    
    /*options*/
    int u_option=0;
    int c_option=0;
    int i_option=0;
    int v_option=0;
    int a_option=0;
    
    /*for option read*/
    int ch;
    
    char *tmp;

  
    
    
    /*functions pointer*/
    void(*OUTPUT_MODE_FUNC)(int,String*, int);
    int (*COMP_STR_FUNC)(const char*,const char *);
    
    
    /*String init*/
    String_init(&s1);
    String_init(&s2);
    
    /*option analy*/
    /*printf("%d",argc);*/
    while((ch=getopt(argc,argv,"ciuVA"))!=-1){
/*        printf("%c",ch);*/
        switch (ch)
        {
        case 'c':
            c_option=1;
            break;
        case 'i':
            i_option=1;
            break;
        case 'u':
            u_option=1;
            break;
        case 'V':
            v_option=1;
            break;
        case 'A':
            a_option=1;
            break;
        default:
            usage();
            exit(EXIT_SUCCESS);
            break;
        }
    }
    if(a_option){
        if((c_option+i_option+u_option+v_option)==0){
            hide_anime();
            exit(EXIT_SUCCESS);
        }
        else{
            usage();
            exit(EXIT_SUCCESS);
        }
    }
    /*-vチェック*/
    if(v_option&&(c_option+i_option+u_option)==0){
        show_version();
        exit(EXIT_SUCCESS);
    }else if(v_option&&(c_option+i_option+u_option)>0){
        error_version();
        exit(EXIT_FAILURE);
    }
    /*filePathの指定がない場合、標準入力*/
    if (optind==argc){
        fp=stdin;
    }
    /*filePathをオープン*/
    else{
        /*printf("%s",argv[optind]);*/
        /*-のときstdin*/
        if(strcmp(argv[optind],"-")==0){
            fp=stdin;
        }
        else{
            fp=fopen(argv[optind],"r");
            if(fp==NULL){
                fprintf(stderr,"uniq: %s: No such file or directory\n",argv[optind]);
                exit(EXIT_FAILURE);
            }
        }
    }







    /*output func*/
    OUTPUT_MODE_FUNC= !c_option ? print_Default : print_CountMode;
    /*compare func*/
    COMP_STR_FUNC= !i_option ? strcmp:strcasecmp;

    file_not_eof=readline(&s1, fp,&flg);
    if (file_not_eof!=EOF)
    {
        while (1)
        {
            count = 0;
            do
            {
                b_flg=flg;
                file_not_eof2 = readline(&s2, fp,&flg);
                count++;
            } while (file_not_eof2 != EOF && (COMP_STR_FUNC(s1.str, s2.str) == 0 && b_flg==flg));

            

            if (file_not_eof2 != EOF)
            {
                OUTPUT_MODE_FUNC(count, &s1, u_option);
                tmp = s1.str;
                s1.str = s2.str;
                s2.str = tmp;
                String_recycle(&s2);
            }
            else
            {
                /*<string><\n>EOFの場合、s2は空要素である。なので、空要素のときは表示しない*/
                
                if ((COMP_STR_FUNC(s1.str, s2.str) == 0))
                {
                    if(flg!=0)
                        count++;
                    OUTPUT_MODE_FUNC(count, &s1, u_option);
                }
                else
                {
                    OUTPUT_MODE_FUNC(count, &s1, u_option);
                    fflush(stdout);
                    if(flg!=0)
                        OUTPUT_MODE_FUNC(1, &s2, u_option);
                }
                

                break;
            }
        }
    }
    /*1行のみ*/
    else{
        if(flg!=0)
            OUTPUT_MODE_FUNC(1,&s1,u_option);
    }
    String_delite(&s1);
    String_delite(&s2);
    fclose(fp);
    exit(EXIT_SUCCESS);
}
