#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <Windows.h>

#define MAX_COUNT     200
#define MAX_NAME_LEN  16    // 이름의 최대 길이

typedef struct ReadData  //약품의 정보를 저장할 구조체 선언
{
    int rnum;
    char PRDLST_NM[MAX_COUNT];            // 제품명
    char PRIMARY_FNCLTY[MAX_COUNT];   // 주요기능
    char DISPOS[MAX_COUNT];                    // 외형
    char NTK_MTHD[MAX_COUNT];              // 섭취방법
} R_DATA;

typedef struct IngReadData // 영양소 정보 저장 구조체 선언
{
    int Ingnum;
    char PRDCT_N[30];     // 영양소명
    char PRI[100];               // 효능
} IR_DATA;
typedef struct SearchData  //상세검색의 약품정보 저장
{
    int snum;
    char SPRDLST_NM[MAX_COUNT];            // 제품명
    char SPRIMARY_FNCLTY[MAX_COUNT];   // 주요기능
    char SDISPOS[MAX_COUNT];                    // 외형
    char SNTK_MTHD[MAX_COUNT];              // 섭취방법
} S_DATA;

typedef struct tm tm;

static R_DATA data[1024];    // 학생 정보를 저장할 배열을 선언
static S_DATA sdata[1024];   //상세정보를 저장할 구조체배열 선언
static IR_DATA irdata[300];   // 영양소 저장 
static int data_count = 0;      // csv파일의 항목 개수


void Title();                   // 메인화면
void Top();                 

void gotoxy(int x, int y);

char* GetNextString(char* ap_src_str, char a_delimiter, char* ap_buffer);                               // csv파일을 받아 콤마를 기점으로 문자열을 잘라 구조체에 저장하는 함수

void Search();
int ReadData(const char* ap_file_name, R_DATA* ap_data, int* ap_data_count);                     // 제품 정보를 csv파일에서 읽어오는 함수
void ProductSearch(R_DATA* ap_data, S_DATA* s_data, int a_count);                                      // 제품검색 초기화면, 키워드 입력
void ChoicePSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, char* search);               // 키워드와 관련있는 제품을 나열
void ResultPSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, int pick, char* search);  // 나열된 제품을 상세검색
void FileWrite(S_DATA* s_data, char* search);                                                                        // 상세검색한 정보를 히스토리파일에 저장

int IngReadData(const char* ap_file_name, IR_DATA* ap_data, int* ap_data_count);               // 성분 정보를 csv파일에서 읽어오는 함수
void bubble(int arr[], int count);                                                                                                // 버블정렬 리스트에 저장된 난수를 오름차순으로 정렬해 성분검색에 사용
void ResultNSearch(IR_DATA* ap_data, unsigned int a_count);                                                 // 10개의 랜덤한 성분정보 출력

void History(R_DATA* ap_data, int a_count);                                                                             // 검색한 기록을 연, 월기준으로 찾아본다
void HistoryRead(R_DATA* ap_data, int a_count, char* search);                                               // 기록 출력
int getTotalLine();                                                                                                                     // 히스토리 파일의 라인수 반환함수

void ShowData(R_DATA* ap_data, unsigned int a_count);                                                         // 제품정보를 출력하는 함수   -테스트용
void IngShowData(IR_DATA* ap_data, unsigned int a_count);                                                   // 성분정보를 출력하는 함수   -테스트용


int getTotalLine() {
    FILE* hp;
    hp = fopen("SearchHistory.txt", "r");
    char tmp;
    int cnt = 0;
    if (hp == NULL) {
        exit(0);
    }
    while (fscanf(hp, "%c", &tmp) != EOF) {                         // 파일의 끝을 만날때 종료
                                                                        
        if (tmp == '\n')                                                            // 개행문자를 만나면 cnt 증가
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
    printf("───────────────────────────────────────────────────\n");
    gotoxy(0, 2);
    Top();

    gotoxy(0, 9);
    printf(" s : 검색\n\n");
    printf(" h : 히스토리\n\n");
    printf(" q : 종료\n\n\n\n");
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
    printf(" 메뉴를 선택해주세요 : ");
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
    printf("┌─────────────────────────────────────────────────┐\n");
    printf("│                                                 │\n");
    printf("│                건강기능식품 검색                │\n");
    printf("│                                                 │\n");
    printf("└─────────────────────────────────────────────────┘\n");


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
    // NULL 문자 또는 a_delimiter 문자가 나올때까지 반복하면서 
    // ap_src_str이 가리키는 문자를 ap_buffer가 가리키는 메모리에 복사한다.
    while (*ap_src_str && *ap_src_str != a_delimiter) *ap_buffer++ = *ap_src_str++;

    // 마지막 위치에 있는 문자가 줄바꿈 문자이면 줄바꿈 문자 대신 NULL 문자를 추가한다.
    // 줄바꿈 문자가 아니라면 문자열의 끝에 NULL 문자를 추가한다.
    if (*(ap_buffer - 1) == '\n') *(ap_buffer - 1) = 0;
    else *ap_buffer = 0;

    // ap_src_str 포인터가 가리키는 위치는 문자열 분리에 사용된 a_delimiter 문자의
    // 위치이거나 NULL 문자의 위치입니다. 만약 a_delimiter 문자 위치를 가리킨다면
    // 다음 문자는 ',' 다음 위치에서 진행될 것이기 때문에 건너띈다.
    if (*ap_src_str == a_delimiter) ap_src_str++;

    // 탐색을 완료한 위치의 주소를 반환한다.
    return ap_src_str;
}

void Search() {

    char select;
    system("cls");
    printf("───────────────────────────────────────────────────\n");
    printf("기능선택");
    gotoxy(1, 5);
    printf("1 : 제품검색");
    gotoxy(1, 7);
    printf("2 : 성분검색");
    gotoxy(1, 9);
    printf("h : 홈");
    gotoxy(1, 11);
    printf("q : 종료");
    gotoxy(0, 15);
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
    printf(" 메뉴를 선택해주세요 : ");
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
                p_pos = GetNextString(one_line_string, ',', ap_data->PRDLST_NM);        //첫번째 줄을 받아 콤마가 나오기전까지 읽고 콤마가 나온다면 PRDLST_NM에 저장
                p_pos = GetNextString(p_pos, ',', ap_data->PRIMARY_FNCLTY);              // 저장한 내용을 제외한 나머지 문장에서 동일한 조건 실행하여 저장
                p_pos = GetNextString(p_pos, ',', ap_data->DISPOS); 
                p_pos = GetNextString(p_pos, ',', ap_data->NTK_MTHD);
                ap_data++; // 다음 저장 위치로 이동한다.
            }
            *ap_data_count = ap_data - p_start;
        }
        fclose(p_file);  // 파일을 닫는다.
        return 1;
    }
    return 0;  // 파일에서 정보 읽기 실패
}

void ProductSearch(R_DATA* ap_data, S_DATA* s_data, int a_count) {

    char search[20] = { '\0' };
    
    system("cls");
    printf("───────────────────────────────────────────────────\n");
    printf("제품검색");
    gotoxy(1, 5);
    printf("원하는 제품이나 성분을 검색해주세요 : \n");
    printf(" ex) 홍삼 장 루테인\n");
    printf("     면역 유산균 비타민\n");
    printf("     프로바이오틱스 관절\n\n");
    printf("  h : 홈\n\n");
    printf("  q : 종료\n\n\n\n");
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
    printf(" 검색해주세요 : ");
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
    printf("───────────────────────────────────────────────────\n");
    printf("제품검색_선택");
    gotoxy(0, 5);
    for (int i = 0; i < a_count; i++, ap_data++) {

        num++;
        ptr = strstr(ap_data->PRDLST_NM, search);                // 검색어가 포함된 제품명을 저장
        ptr1 = strstr(ap_data->PRIMARY_FNCLTY, search);     // 검색어가 포함된 주요기능을 저장

        if ((ptr != NULL) || (ptr1 != NULL)) {                                 // 검색어가 제품명이나 주요기능에 포함된다면  
            count += 1;

            printf("%-2d : %s\n", count, ap_data->PRDLST_NM);                    // 제품명 출력

            memcpy(--s_data, ap_data, 800);
        }
    }

    for (int i = 0; i < num; i++) {
        ap_data--;
    }
    if (count == 0)
    {
        printf("해당하는 제품이 없습니다... 타이틀로 이동");
        Sleep(2000);
        Title();
    }

    printf("\n\n\n\n");
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
    printf("\n자세한 정보를 원한다면 제품 번호를 선택해주세요 : ");
    scanf("%d",&pick);
    
    ResultPSearch(ap_data, s_data, count, pick, search);
}
void ResultPSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, int pick, char* search) {                        // 필터링된 약품의 정보를 넣을 구조체가 하나더 필요 
    char* ptr;
    char select;
    int num = 0;
    s_data--;

    for (int i = pick; i <= a_count; i++,num++) {
        s_data++;
    }
   
    FileWrite(s_data, search);
 
    system("cls");
    printf("───────────────────────────────────────────────────\n");
    printf("검색결과\n\n");
    printf(" 이름 : %s\n", s_data->SPRDLST_NM);
    printf(" 성분 : %s\n", s_data->SPRIMARY_FNCLTY);
    printf(" 모양 : %s\n", s_data->SDISPOS);
    printf(" 섭취 : %s\n\n", s_data->SNTK_MTHD);

    printf(" h : 홈\n\n");
    printf(" q : 종료\n\n\n\n");
    for (int i = 0; i < num; i++) {
        s_data--;
    }
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
   
    printf(" 메뉴를 선택해주세요 : ");
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
        printf("파일이 안열려");
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
                ap_data++; // 다음 저장 위치로 이동한다.
                num++;
            }

            *ap_data_count = ap_data - p_start;
        }
        fclose(p_file);  // 파일을 닫는다.
        return 1;
    }
    return 0;  // 파일에서 정보 읽기 실패
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
    printf("───────────────────────────────────────────────────\n");
    printf(" 성분정보");
    gotoxy(0, 5);
    for (int j = 0; j < a_count; j++, ap_data++) {
        for (int i = 0; i < 10; i++) {
            if (num[i] == ap_data->Ingnum) {

                printf(" %-30s %-50s\n", ap_data->PRDCT_N, ap_data->PRI);
            }
        }
    }

    printf("\n h : 홈\n\n");
    printf(" q : 종료\n\n\n\n");
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
    printf(" 메뉴를 선택해주세요 : ");
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
   
    printf("───────────────────────────────────────────────────\n");
    printf("히스토리_날짜지정");
    gotoxy(1, 5);
    printf("날짜를 지정해주세요 \n\n");
    printf(" ex) 2021-11-13\n");
    printf("     2021-11\n");
    printf("     -11\n\n\n");

    printf(" h : 홈\n\n");
    printf(" q : 종료\n\n");

    gotoxy(0, 18);
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
    printf(" 검색해주세요 : ");
    getchar();
    gets(pick);
  
    if (strcmp(pick,"h") == 0) {                                // 입력문자가 h면 타이틀 이동
        Title();
    }
    else if (strcmp(pick, "q") == 0)
        exit(1);                                                         // 입력문자가 q면 종료
    else 
        HistoryRead(ap_data, a_count, pick);          // 히스토리파일 읽음
}


void HistoryRead(R_DATA* ap_data, int a_count,char* search) {

    FILE* hp;
    char str[80],str1[50];
  
    
    char select;
    int num = 0;
    int line = getTotalLine();
    fopen_s(&hp, "SearchHistory.txt", "r");
    system("cls");
    printf("───────────────────────────────────────────────────\n");
    printf(" 히스토리\n"); 

    for (int i = 0; i <= line; i++) {
        char* ptr = fgets(str, 80, hp);
        char* Rtime = strstr(ptr, search);                 //search에 해당하는 문자열이 ptr에 있으면 rtime에 저장

        if (Rtime != NULL)
            printf(" %s ", str);
    }
    printf("\n");
    printf("  h : 홈\n\n");
    printf("  q : 종료\n\n\n\n");
    
    fclose(hp);
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
    printf("상세검색을 하고싶은 제품이름을 입력해주세요 : ");
    
    gets(str1);
    if (strcmp(str1, "h") == 0) {
        
        Title();
    }
    else if (strcmp(str1, "q") == 0)
        exit(1);
    
    system("cls");
    printf("───────────────────────────────────────────────────\n");
    printf("상세정보\n");
    for (int i = 0; i < a_count; i++, ap_data++) {

        if (strcmp(ap_data->PRDLST_NM,str1)==0)                // 검색어가 제품명과 일치하면 
        {                       
            printf(" 이름 : %s\n", ap_data->PRDLST_NM);
            printf(" 성분 : %s\n", ap_data->PRIMARY_FNCLTY);
            printf(" 모양 : %s\n", ap_data->DISPOS);
            printf(" 섭취 : %s\n\n", ap_data->NTK_MTHD);            
        }
        num++;
    }
    for (int i = 0; i < num; i++) {
        ap_data--;
    }
    printf(" h : 홈\n\n");
    printf(" q : 종료\n\n\n\n");
    TimeNow();
    printf("───────────────────────────────────────────────────\n");
    printf(" 메뉴를 선택해주세요 : ");
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
        // 학생별로 성적을 출력한다.
        printf("%-2d 이름 : %-30s", ap_data->Ingnum, ap_data->PRDCT_N);
        printf(" 성분 : %s\n", ap_data->PRI);

    }
}

void ShowData(R_DATA* ap_data, unsigned int a_count)
{
    for (unsigned int i = 0; i < a_count; i++, ap_data++) {
        // 학생별로 성적을 출력한다.
        printf(" 이름 : %s\n", ap_data->PRDLST_NM);
        printf(" 성분 : %s\n", ap_data->PRIMARY_FNCLTY);
        printf(" 모양 : %s\n", ap_data->DISPOS);
        printf(" 섭취 : %s\n\n", ap_data->NTK_MTHD);

    }
}

int main()
{
    SetConsoleTitle(TEXT("건강기능식품 검색 시스템"));
    Title();

    
}
