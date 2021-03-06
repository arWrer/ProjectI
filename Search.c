#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <Windows.h>

#define MAX_COUNT     200
#define MAX_NAME_LEN  16    // 檜葷曖 譆渠 望檜

typedef struct ReadData  //擒ヶ曖 薑爾蒂 盪濰й 掘褻羹 摹樹
{
    int rnum;
    char PRDLST_NM[MAX_COUNT];            // 薯ヶ貲
    char PRIMARY_FNCLTY[MAX_COUNT];   // 輿蹂晦棟
    char DISPOS[MAX_COUNT];                    // 諼⑽
    char NTK_MTHD[MAX_COUNT];              // 撥鏃寞徹
} R_DATA;

typedef struct IngReadData // 艙曄模 薑爾 盪濰 掘褻羹 摹樹
{
    int Ingnum;
    char PRDCT_N[30];     // 艙曄模貲
    char PRI[100];               // �艘�
} IR_DATA;
typedef struct SearchData  //鼻撮匐儀曖 擒ヶ薑爾 盪濰
{
    int snum;
    char SPRDLST_NM[MAX_COUNT];            // 薯ヶ貲
    char SPRIMARY_FNCLTY[MAX_COUNT];   // 輿蹂晦棟
    char SDISPOS[MAX_COUNT];                    // 諼⑽
    char SNTK_MTHD[MAX_COUNT];              // 撥鏃寞徹
} S_DATA;

typedef struct tm tm;

static R_DATA data[1024];    // з儅 薑爾蒂 盪濰й 寡翮擊 摹樹
static S_DATA sdata[1024];   //鼻撮薑爾蒂 盪濰й 掘褻羹寡翮 摹樹
static IR_DATA irdata[300];   // 艙曄模 盪濰 
static int data_count = 0;      // csvだ橾曖 о跡 偃熱


void Title();                   // 詭檣�飛�
void Top();                 

void gotoxy(int x, int y);

char* GetNextString(char* ap_src_str, char a_delimiter, char* ap_buffer);                               // csvだ橾擊 嫡嬴 巍葆蒂 晦薄戲煎 僥濠翮擊 澀塭 掘褻羹縑 盪濰ж朝 л熱

void Search();
int ReadData(const char* ap_file_name, R_DATA* ap_data, int* ap_data_count);                     // 薯ヶ 薑爾蒂 csvだ橾縑憮 檗橫螃朝 л熱
void ProductSearch(R_DATA* ap_data, S_DATA* s_data, int a_count);                                      // 薯ヶ匐儀 蟾晦�飛�, 酈錶萄 殮溘
void ChoicePSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, char* search);               // 酈錶萄諦 婦溼氈朝 薯ヶ擊 釭翮
void ResultPSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, int pick, char* search);  // 釭翮脹 薯ヶ擊 鼻撮匐儀
void FileWrite(S_DATA* s_data, char* search);                                                                        // 鼻撮匐儀и 薑爾蒂 �鷜瘍銝料釋狤� 盪濰

int IngReadData(const char* ap_file_name, IR_DATA* ap_data, int* ap_data_count);               // 撩碟 薑爾蒂 csvだ橾縑憮 檗橫螃朝 л熱
void bubble(int arr[], int count);                                                                                                // 幗綰薑溺 葬蝶お縑 盪濰脹 陪熱蒂 螃葷離牖戲煎 薑溺п 撩碟匐儀縑 餌辨
void ResultNSearch(IR_DATA* ap_data, unsigned int a_count);                                                 // 10偃曖 楠渾и 撩碟薑爾 轎溘

void History(R_DATA* ap_data, int a_count);                                                                             // 匐儀и 晦煙擊 翱, 錯晦遽戲煎 瓊嬴獄棻
void HistoryRead(R_DATA* ap_data, int a_count, char* search);                                               // 晦煙 轎溘
int getTotalLine();                                                                                                                     // �鷜瘍銝� だ橾曖 塭檣熱 奩�納埮�

void ShowData(R_DATA* ap_data, unsigned int a_count);                                                         // 薯ヶ薑爾蒂 轎溘ж朝 л熱   -纔蝶お辨
void IngShowData(IR_DATA* ap_data, unsigned int a_count);                                                   // 撩碟薑爾蒂 轎溘ж朝 л熱   -纔蝶お辨


int getTotalLine() {
    FILE* hp;
    hp = fopen("SearchHistory.txt", "r");
    char tmp;
    int cnt = 0;
    if (hp == NULL) {
        exit(0);
    }
    while (fscanf(hp, "%c", &tmp) != EOF) {                         // だ橾曖 部擊 虜陳陽 謙猿
                                                                        
        if (tmp == '\n')                                                            // 偃ч僥濠蒂 虜釭賊 cnt 隸陛
            cnt++;
    }
    fclose(hp);
    return --cnt;
}

void TimeNow() {
    time_t ltime;
    tm* today;
    int year, month, day, hour, min, sec;
    time(&ltime);

    today = (localtime(&ltime));

    year = today->tm_year + 1900;
    month = today->tm_mon + 1;
    day = today->tm_mday;
    hour = today->tm_hour;
    min = today->tm_min;
    sec = today->tm_sec;

    printf(" %-d-%02d-%02d %02d:%02d:%02d\n", year, month, day, hour, min, sec);
}
void Title()
{
    char select;
    system("cls");
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    gotoxy(0, 2);
    Top();

    gotoxy(0, 9);
    printf(" s : 匐儀\n\n");
    printf(" h : �鷜瘍銝娉n\n");
    printf(" q : 謙猿\n\n\n\n");
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf(" 詭景蒂 摹鷗п輿撮蹂 : ");
    scanf(" %c", &select);
    
    if (select == 's')
        Search();
    else if (select == 'h')
         if (ReadData("SupplementList.csv", data, &data_count)) {
            History(data, data_count);
         }
    else if (select == 'q')
        exit(1);
}

void Top() {
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
    printf("弛                                                 弛\n");
    printf("弛                勒鬼晦棟衝ヶ 匐儀                弛\n");
    printf("弛                                                 弛\n");
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");


}
void gotoxy(int x, int y) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(consoleHandle, pos);
}
void ClearLineFromReadBuffer(void)
{
    while (getchar() != '\n');
}
char* GetNextString(char* ap_src_str, char a_delimiter, char* ap_buffer)
{
    // NULL 僥濠 傳朝 a_delimiter 僥濠陛 釭螢陽梱雖 奩犒ж賊憮 
    // ap_src_str檜 陛葬酈朝 僥濠蒂 ap_buffer陛 陛葬酈朝 詭賅葬縑 犒餌и棻.
    while (*ap_src_str && *ap_src_str != a_delimiter) *ap_buffer++ = *ap_src_str++;

    // 葆雖虞 嬪纂縑 氈朝 僥濠陛 還夥翎 僥濠檜賊 還夥翎 僥濠 渠褐 NULL 僥濠蒂 蹺陛и棻.
    // 還夥翎 僥濠陛 嬴棲塭賊 僥濠翮曖 部縑 NULL 僥濠蒂 蹺陛и棻.
    if (*(ap_buffer - 1) == '\n') *(ap_buffer - 1) = 0;
    else *ap_buffer = 0;

    // ap_src_str ん檣攪陛 陛葬酈朝 嬪纂朝 僥濠翮 碟葬縑 餌辨脹 a_delimiter 僥濠曖
    // 嬪纂檜剪釭 NULL 僥濠曖 嬪纂殮棲棻. 虜擒 a_delimiter 僥濠 嬪纂蒂 陛葬鑑棻賊
    // 棻擠 僥濠朝 ',' 棻擠 嬪纂縑憮 霞ч腆 匙檜晦 陽僥縑 勒傘嗉棻.
    if (*ap_src_str == a_delimiter) ap_src_str++;

    // 鬚儀擊 諫猿и 嬪纂曖 輿模蒂 奩�納挬�.
    return ap_src_str;
}

void Search() {

    char select;
    system("cls");
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf("晦棟摹鷗");
    gotoxy(1, 5);
    printf("1 : 薯ヶ匐儀");
    gotoxy(1, 7);
    printf("2 : 撩碟匐儀");
    gotoxy(1, 9);
    printf("h : ��");
    gotoxy(1, 11);
    printf("q : 謙猿");
    gotoxy(0, 15);
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf(" 詭景蒂 摹鷗п輿撮蹂 : ");
    scanf(" %c", &select);
    getchar();


    switch (select) {
    case '1': {
        if (ReadData("SupplementList.csv", data, &data_count)) {
            ProductSearch(data, sdata, data_count);
        }
        break;
    }
    case '2': {
        if (IngReadData("Ingredient.csv", irdata, &data_count)) {
            ResultNSearch(irdata, data_count);
        }
        break;
    }
    case 'h': {
        Title();
        break;
    }
    case 'q': {
        return 0;
    }
    }
}
int ReadData(const char* ap_file_name, R_DATA* ap_data, int* ap_data_count)
{
    R_DATA* p_start = ap_data;

    FILE* p_file = NULL;  
    if (0 == fopen_s(&p_file, ap_file_name, "rt")) {

        char one_line_string[300], * p_pos;

        if (NULL != fgets(one_line_string, 300, p_file)) {
            while (NULL != fgets(one_line_string, 300, p_file)) {
                p_pos = GetNextString(one_line_string, ',', ap_data->PRDLST_NM);        //羅廓簞 還擊 嫡嬴 巍葆陛 釭螃晦瞪梱雖 檗堅 巍葆陛 釭螞棻賊 PRDLST_NM縑 盪濰
                p_pos = GetNextString(p_pos, ',', ap_data->PRIMARY_FNCLTY);              // 盪濰и 頂辨擊 薯諼и 釭該雖 僥濰縑憮 翕橾и 褻勒 褒чж罹 盪濰
                p_pos = GetNextString(p_pos, ',', ap_data->DISPOS); 
                p_pos = GetNextString(p_pos, ',', ap_data->NTK_MTHD);
                ap_data++; // 棻擠 盪濰 嬪纂煎 檜翕и棻.
            }
            *ap_data_count = ap_data - p_start;
        }
        fclose(p_file);  // だ橾擊 殘朝棻.
        return 1;
    }
    return 0;  // だ橾縑憮 薑爾 檗晦 褒ぬ
}

void ProductSearch(R_DATA* ap_data, S_DATA* s_data, int a_count) {

    char search[20] = { '\0' };
    
    system("cls");
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf("薯ヶ匐儀");
    gotoxy(1, 5);
    printf("錳ж朝 薯ヶ檜釭 撩碟擊 匐儀п輿撮蹂 : \n");
    printf(" ex) �姣� 濰 瑞纔檣\n");
    printf("     賊羲 嶸骯敕 綠顫團\n");
    printf("     Щ煎夥檜螃す蝶 婦瞰\n\n");
    printf("  h : �沔n\n");
    printf("  q : 謙猿\n\n\n\n");
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf(" 匐儀п輿撮蹂 : ");
    gets(search);
    if (strcmp(search, "h") == 0) {

        Title();
    }
    else if (strcmp(search, "q") == 0)
        exit(1); 
    ChoicePSearch(ap_data, s_data, a_count, search);
}
void ChoicePSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, char* search) {

    char* ptr, * ptr1;
    char name[20];
    int num = -1;
    int count = 0;
    int pick;
    system("cls");
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf("薯ヶ匐儀_摹鷗");
    gotoxy(0, 5);
    for (int i = 0; i < a_count; i++, ap_data++) {

        num++;
        ptr = strstr(ap_data->PRDLST_NM, search);                // 匐儀橫陛 んл脹 薯ヶ貲擊 盪濰
        ptr1 = strstr(ap_data->PRIMARY_FNCLTY, search);     // 匐儀橫陛 んл脹 輿蹂晦棟擊 盪濰

        if ((ptr != NULL) || (ptr1 != NULL)) {                                 // 匐儀橫陛 薯ヶ貲檜釭 輿蹂晦棟縑 んл脹棻賊  
            count += 1;

            printf("%-2d : %s\n", count, ap_data->PRDLST_NM);                    // 薯ヶ貲 轎溘

            memcpy(--s_data, ap_data, 800);
        }
    }

    for (int i = 0; i < num; i++) {
        ap_data--;
    }
    if (count == 0)
    {
        printf("п渡ж朝 薯ヶ檜 橈蝗棲棻... 顫檜ぎ煎 檜翕");
        Sleep(2000);
        Title();
    }

    printf("\n\n\n\n");
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf("\n濠撮и 薑爾蒂 錳и棻賊 薯ヶ 廓�ㄧ� 摹鷗п輿撮蹂 : ");
    scanf("%d",&pick);
    
    ResultPSearch(ap_data, s_data, count, pick, search);
}
void ResultPSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, int pick, char* search) {                        // в攪葭脹 擒ヶ曖 薑爾蒂 厥擊 掘褻羹陛 ж釭渦 в蹂 
    char* ptr;
    char select;
    int num = 0;
    s_data--;

    for (int i = pick; i <= a_count; i++,num++) {
        s_data++;
    }
   
    FileWrite(s_data, search);
 
    system("cls");
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf("匐儀唸婁\n\n");
    printf(" 檜葷 : %s\n", s_data->SPRDLST_NM);
    printf(" 撩碟 : %s\n", s_data->SPRIMARY_FNCLTY);
    printf(" 賅曄 : %s\n", s_data->SDISPOS);
    printf(" 撥鏃 : %s\n\n", s_data->SNTK_MTHD);

    printf(" h : �沔n\n");
    printf(" q : 謙猿\n\n\n\n");
    for (int i = 0; i < num; i++) {
        s_data--;
    }
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
   
    printf(" 詭景蒂 摹鷗п輿撮蹂 : ");
    scanf(" %c", &select);
    
    switch (select) {

    case 'h':
        Title();
    case 'q':
        exit(1);
    }
}

void FileWrite(S_DATA* s_data, char* search) {
    FILE* hp;

    struct tm* t;
    time_t timer;

    timer = time(NULL);
    t = localtime(&timer);

    fopen_s(&hp, "SearchHistory.txt", "a+");
    if (hp == NULL) {
        printf("だ橾檜 寰翮溥");
        return 0;
    }
    time_t seconds = time(NULL);
    //printf(" %-d - %-d - %-d % s % s \n", 1900 + t->tm_year, t->tm_mon + 1, t->tm_mday, search, s_data->SPRDLST_NM);
    fprintf(hp, "%-d-%-d-%-d %-14s %-20s \n", 1900 + t->tm_year, t->tm_mon + 1, t->tm_mday, search, s_data->SPRDLST_NM);

    fclose(hp);

}
int IngReadData(const char* ap_file_name, IR_DATA* ap_data, int* ap_data_count)
{
    IR_DATA* p_start = ap_data;
    int num=1;
    FILE* p_file = NULL;
    if (0 == fopen_s(&p_file, ap_file_name, "rt")) {

        char one_line_string[100], * p_pos;

        if (NULL != fgets(one_line_string, 100, p_file)) {

            while (NULL != fgets(one_line_string, 100, p_file)) {
                ap_data->Ingnum = num;
                p_pos = GetNextString(one_line_string, ',', ap_data->PRDCT_N);
                p_pos = GetNextString(p_pos, ',', ap_data->PRI);
                ap_data++; // 棻擠 盪濰 嬪纂煎 檜翕и棻.
                num++;
            }

            *ap_data_count = ap_data - p_start;
        }
        fclose(p_file);  // だ橾擊 殘朝棻.
        return 1;
    }
    return 0;  // だ橾縑憮 薑爾 檗晦 褒ぬ
}

void bubble(int arr[], int count) {
    int tmp;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count-1; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}
void ResultNSearch(IR_DATA* ap_data, unsigned int a_count) {
    int tmp,num[10];
    char select;
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        num[i] = rand() % a_count + 1;
    }
    bubble(num, 10);
    system("cls");
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf(" 撩碟薑爾");
    gotoxy(0, 5);
    for (int j = 0; j < a_count; j++, ap_data++) {
        for (int i = 0; i < 10; i++) {
            if (num[i] == ap_data->Ingnum) {

                printf(" %-30s %-50s\n", ap_data->PRDCT_N, ap_data->PRI);
            }
        }
    }

    printf("\n h : �沔n\n");
    printf(" q : 謙猿\n\n\n\n");
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf(" 詭景蒂 摹鷗п輿撮蹂 : ");
    scanf(" %c", &select);

    switch (select) {
    case 'h': {
        Title();
        break;
    }
    case 'q': {
        exit(1);
    }
    }
}

void History(R_DATA* ap_data, int a_count) {

    char pick[20];
    system("cls");
   
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf("�鷜瘍銝宰陳瞼雖薑");
    gotoxy(1, 5);
    printf("陳瞼蒂 雖薑п輿撮蹂 \n\n");
    printf(" ex) 2021-11-13\n");
    printf("     2021-11\n");
    printf("     -11\n\n\n");

    printf(" h : �沔n\n");
    printf(" q : 謙猿\n\n");

    gotoxy(0, 18);
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf(" 匐儀п輿撮蹂 : ");
    getchar();
    gets(pick);
  
    if (strcmp(pick,"h") == 0) {                                // 殮溘僥濠陛 h賊 顫檜ぎ 檜翕
        Title();
    }
    else if (strcmp(pick, "q") == 0)
        exit(1);                                                         // 殮溘僥濠陛 q賊 謙猿
    else 
        HistoryRead(ap_data, a_count, pick);          // �鷜瘍銝料釋� 檗擠
}


void HistoryRead(R_DATA* ap_data, int a_count,char* search) {

    FILE* hp;
    char str[80],str1[50];
  
    
    char select;
    int num = 0;
    int line = getTotalLine();
    fopen_s(&hp, "SearchHistory.txt", "r");
    system("cls");
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf(" �鷜瘍銝娉n"); 

    for (int i = 0; i <= line; i++) {
        char* ptr = fgets(str, 80, hp);
        char* Rtime = strstr(ptr, search);                 //search縑 п渡ж朝 僥濠翮檜 ptr縑 氈戲賊 rtime縑 盪濰

        if (Rtime != NULL)
            printf(" %s ", str);
    }
    printf("\n");
    printf("  h : �沔n\n");
    printf("  q : 謙猿\n\n\n\n");
    
    fclose(hp);
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf("鼻撮匐儀擊 ж堅談擎 薯ヶ檜葷擊 殮溘п輿撮蹂 : ");
    
    gets(str1);
    if (strcmp(str1, "h") == 0) {
        
        Title();
    }
    else if (strcmp(str1, "q") == 0)
        exit(1);
    
    system("cls");
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf("鼻撮薑爾\n");
    for (int i = 0; i < a_count; i++, ap_data++) {

        if (strcmp(ap_data->PRDLST_NM,str1)==0)                // 匐儀橫陛 薯ヶ貲婁 橾纂ж賊 
        {                       
            printf(" 檜葷 : %s\n", ap_data->PRDLST_NM);
            printf(" 撩碟 : %s\n", ap_data->PRIMARY_FNCLTY);
            printf(" 賅曄 : %s\n", ap_data->DISPOS);
            printf(" 撥鏃 : %s\n\n", ap_data->NTK_MTHD);            
        }
        num++;
    }
    for (int i = 0; i < num; i++) {
        ap_data--;
    }
    printf(" h : �沔n\n");
    printf(" q : 謙猿\n\n\n\n");
    TimeNow();
    printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");
    printf(" 詭景蒂 摹鷗п輿撮蹂 : ");
    scanf(" %c", &select);

    switch (select) {
    case 'h': {
        Title();
        break;
    }
    case 'q': {
        exit(1);
    }
    }
    return 0;
}

void IngShowData(IR_DATA* ap_data, unsigned int a_count)
{
    for (unsigned int i = 0; i < a_count; i++, ap_data++) {
        // з儅滌煎 撩瞳擊 轎溘и棻.
        printf("%-2d 檜葷 : %-30s", ap_data->Ingnum, ap_data->PRDCT_N);
        printf(" 撩碟 : %s\n", ap_data->PRI);

    }
}

void ShowData(R_DATA* ap_data, unsigned int a_count)
{
    for (unsigned int i = 0; i < a_count; i++, ap_data++) {
        // з儅滌煎 撩瞳擊 轎溘и棻.
        printf(" 檜葷 : %s\n", ap_data->PRDLST_NM);
        printf(" 撩碟 : %s\n", ap_data->PRIMARY_FNCLTY);
        printf(" 賅曄 : %s\n", ap_data->DISPOS);
        printf(" 撥鏃 : %s\n\n", ap_data->NTK_MTHD);

    }
}

int main()
{
    SetConsoleTitle(TEXT("勒鬼晦棟衝ヶ 匐儀 衛蝶蠱"));
    Title();

    
}
