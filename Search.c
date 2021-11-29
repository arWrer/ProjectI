#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <Windows.h>

#define MAX_COUNT     200
#define MAX_NAME_LEN  16    // �̸��� �ִ� ����

typedef struct ReadData  //��ǰ�� ������ ������ ����ü ����
{
    int rnum;
    char PRDLST_NM[MAX_COUNT];            // ��ǰ��
    char PRIMARY_FNCLTY[MAX_COUNT];   // �ֿ���
    char DISPOS[MAX_COUNT];                    // ����
    char NTK_MTHD[MAX_COUNT];              // ������
} R_DATA;

typedef struct IngReadData // ����� ���� ���� ����ü ����
{
    int Ingnum;
    char PRDCT_N[30];     // ����Ҹ�
    char PRI[100];               // ȿ��
} IR_DATA;
typedef struct SearchData  //�󼼰˻��� ��ǰ���� ����
{
    int snum;
    char SPRDLST_NM[MAX_COUNT];            // ��ǰ��
    char SPRIMARY_FNCLTY[MAX_COUNT];   // �ֿ���
    char SDISPOS[MAX_COUNT];                    // ����
    char SNTK_MTHD[MAX_COUNT];              // ������
} S_DATA;

typedef struct tm tm;

static R_DATA data[1024];    // �л� ������ ������ �迭�� ����
static S_DATA sdata[1024];   //�������� ������ ����ü�迭 ����
static IR_DATA irdata[300];   // ����� ���� 
static int data_count = 0;      // csv������ �׸� ����


void Title();                   // ����ȭ��
void Top();                 

void gotoxy(int x, int y);

char* GetNextString(char* ap_src_str, char a_delimiter, char* ap_buffer);                               // csv������ �޾� �޸��� �������� ���ڿ��� �߶� ����ü�� �����ϴ� �Լ�

void Search();
int ReadData(const char* ap_file_name, R_DATA* ap_data, int* ap_data_count);                     // ��ǰ ������ csv���Ͽ��� �о���� �Լ�
void ProductSearch(R_DATA* ap_data, S_DATA* s_data, int a_count);                                      // ��ǰ�˻� �ʱ�ȭ��, Ű���� �Է�
void ChoicePSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, char* search);               // Ű����� �����ִ� ��ǰ�� ����
void ResultPSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, int pick, char* search);  // ������ ��ǰ�� �󼼰˻�
void FileWrite(S_DATA* s_data, char* search);                                                                        // �󼼰˻��� ������ �����丮���Ͽ� ����

int IngReadData(const char* ap_file_name, IR_DATA* ap_data, int* ap_data_count);               // ���� ������ csv���Ͽ��� �о���� �Լ�
void bubble(int arr[], int count);                                                                                                // �������� ����Ʈ�� ����� ������ ������������ ������ ���а˻��� ���
void ResultNSearch(IR_DATA* ap_data, unsigned int a_count);                                                 // 10���� ������ �������� ���

void History(R_DATA* ap_data, int a_count);                                                                             // �˻��� ����� ��, ���������� ã�ƺ���
void HistoryRead(R_DATA* ap_data, int a_count, char* search);                                               // ��� ���
int getTotalLine();                                                                                                                     // �����丮 ������ ���μ� ��ȯ�Լ�

void ShowData(R_DATA* ap_data, unsigned int a_count);                                                         // ��ǰ������ ����ϴ� �Լ�   -�׽�Ʈ��
void IngShowData(IR_DATA* ap_data, unsigned int a_count);                                                   // ���������� ����ϴ� �Լ�   -�׽�Ʈ��


int getTotalLine() {
    FILE* hp;
    hp = fopen("SearchHistory.txt", "r");
    char tmp;
    int cnt = 0;
    if (hp == NULL) {
        exit(0);
    }
    while (fscanf(hp, "%c", &tmp) != EOF) {                         // ������ ���� ������ ����
                                                                        
        if (tmp == '\n')                                                            // ���๮�ڸ� ������ cnt ����
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
    printf("������������������������������������������������������������������������������������������������������\n");
    gotoxy(0, 2);
    Top();

    gotoxy(0, 9);
    printf(" s : �˻�\n\n");
    printf(" h : �����丮\n\n");
    printf(" q : ����\n\n\n\n");
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
    printf(" �޴��� �������ּ��� : ");
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
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("��                                                 ��\n");
    printf("��                �ǰ���ɽ�ǰ �˻�                ��\n");
    printf("��                                                 ��\n");
    printf("������������������������������������������������������������������������������������������������������\n");


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
    // NULL ���� �Ǵ� a_delimiter ���ڰ� ���ö����� �ݺ��ϸ鼭 
    // ap_src_str�� ����Ű�� ���ڸ� ap_buffer�� ����Ű�� �޸𸮿� �����Ѵ�.
    while (*ap_src_str && *ap_src_str != a_delimiter) *ap_buffer++ = *ap_src_str++;

    // ������ ��ġ�� �ִ� ���ڰ� �ٹٲ� �����̸� �ٹٲ� ���� ��� NULL ���ڸ� �߰��Ѵ�.
    // �ٹٲ� ���ڰ� �ƴ϶�� ���ڿ��� ���� NULL ���ڸ� �߰��Ѵ�.
    if (*(ap_buffer - 1) == '\n') *(ap_buffer - 1) = 0;
    else *ap_buffer = 0;

    // ap_src_str �����Ͱ� ����Ű�� ��ġ�� ���ڿ� �и��� ���� a_delimiter ������
    // ��ġ�̰ų� NULL ������ ��ġ�Դϴ�. ���� a_delimiter ���� ��ġ�� ����Ų�ٸ�
    // ���� ���ڴ� ',' ���� ��ġ���� ����� ���̱� ������ �ǳʶ��.
    if (*ap_src_str == a_delimiter) ap_src_str++;

    // Ž���� �Ϸ��� ��ġ�� �ּҸ� ��ȯ�Ѵ�.
    return ap_src_str;
}

void Search() {

    char select;
    system("cls");
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("��ɼ���");
    gotoxy(1, 5);
    printf("1 : ��ǰ�˻�");
    gotoxy(1, 7);
    printf("2 : ���а˻�");
    gotoxy(1, 9);
    printf("h : Ȩ");
    gotoxy(1, 11);
    printf("q : ����");
    gotoxy(0, 15);
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
    printf(" �޴��� �������ּ��� : ");
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
                p_pos = GetNextString(one_line_string, ',', ap_data->PRDLST_NM);        //ù��° ���� �޾� �޸��� ������������ �а� �޸��� ���´ٸ� PRDLST_NM�� ����
                p_pos = GetNextString(p_pos, ',', ap_data->PRIMARY_FNCLTY);              // ������ ������ ������ ������ ���忡�� ������ ���� �����Ͽ� ����
                p_pos = GetNextString(p_pos, ',', ap_data->DISPOS); 
                p_pos = GetNextString(p_pos, ',', ap_data->NTK_MTHD);
                ap_data++; // ���� ���� ��ġ�� �̵��Ѵ�.
            }
            *ap_data_count = ap_data - p_start;
        }
        fclose(p_file);  // ������ �ݴ´�.
        return 1;
    }
    return 0;  // ���Ͽ��� ���� �б� ����
}

void ProductSearch(R_DATA* ap_data, S_DATA* s_data, int a_count) {

    char search[20] = { '\0' };
    
    system("cls");
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("��ǰ�˻�");
    gotoxy(1, 5);
    printf("���ϴ� ��ǰ�̳� ������ �˻����ּ��� : \n");
    printf(" ex) ȫ�� �� ������\n");
    printf("     �鿪 ����� ��Ÿ��\n");
    printf("     ���ι��̿�ƽ�� ����\n\n");
    printf("  h : Ȩ\n\n");
    printf("  q : ����\n\n\n\n");
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
    printf(" �˻����ּ��� : ");
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
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("��ǰ�˻�_����");
    gotoxy(0, 5);
    for (int i = 0; i < a_count; i++, ap_data++) {

        num++;
        ptr = strstr(ap_data->PRDLST_NM, search);                // �˻�� ���Ե� ��ǰ���� ����
        ptr1 = strstr(ap_data->PRIMARY_FNCLTY, search);     // �˻�� ���Ե� �ֿ����� ����

        if ((ptr != NULL) || (ptr1 != NULL)) {                                 // �˻�� ��ǰ���̳� �ֿ��ɿ� ���Եȴٸ�  
            count += 1;

            printf("%-2d : %s\n", count, ap_data->PRDLST_NM);                    // ��ǰ�� ���

            memcpy(--s_data, ap_data, 800);
        }
    }

    for (int i = 0; i < num; i++) {
        ap_data--;
    }
    if (count == 0)
    {
        printf("�ش��ϴ� ��ǰ�� �����ϴ�... Ÿ��Ʋ�� �̵�");
        Sleep(2000);
        Title();
    }

    printf("\n\n\n\n");
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("\n�ڼ��� ������ ���Ѵٸ� ��ǰ ��ȣ�� �������ּ��� : ");
    scanf("%d",&pick);
    
    ResultPSearch(ap_data, s_data, count, pick, search);
}
void ResultPSearch(R_DATA* ap_data, S_DATA* s_data, int a_count, int pick, char* search) {                        // ���͸��� ��ǰ�� ������ ���� ����ü�� �ϳ��� �ʿ� 
    char* ptr;
    char select;
    int num = 0;
    s_data--;

    for (int i = pick; i <= a_count; i++,num++) {
        s_data++;
    }
   
    FileWrite(s_data, search);
 
    system("cls");
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("�˻����\n\n");
    printf(" �̸� : %s\n", s_data->SPRDLST_NM);
    printf(" ���� : %s\n", s_data->SPRIMARY_FNCLTY);
    printf(" ��� : %s\n", s_data->SDISPOS);
    printf(" ���� : %s\n\n", s_data->SNTK_MTHD);

    printf(" h : Ȩ\n\n");
    printf(" q : ����\n\n\n\n");
    for (int i = 0; i < num; i++) {
        s_data--;
    }
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
   
    printf(" �޴��� �������ּ��� : ");
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
        printf("������ �ȿ���");
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
                ap_data++; // ���� ���� ��ġ�� �̵��Ѵ�.
                num++;
            }

            *ap_data_count = ap_data - p_start;
        }
        fclose(p_file);  // ������ �ݴ´�.
        return 1;
    }
    return 0;  // ���Ͽ��� ���� �б� ����
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
    printf("������������������������������������������������������������������������������������������������������\n");
    printf(" ��������");
    gotoxy(0, 5);
    for (int j = 0; j < a_count; j++, ap_data++) {
        for (int i = 0; i < 10; i++) {
            if (num[i] == ap_data->Ingnum) {

                printf(" %-30s %-50s\n", ap_data->PRDCT_N, ap_data->PRI);
            }
        }
    }

    printf("\n h : Ȩ\n\n");
    printf(" q : ����\n\n\n\n");
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
    printf(" �޴��� �������ּ��� : ");
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
   
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("�����丮_��¥����");
    gotoxy(1, 5);
    printf("��¥�� �������ּ��� \n\n");
    printf(" ex) 2021-11-13\n");
    printf("     2021-11\n");
    printf("     -11\n\n\n");

    printf(" h : Ȩ\n\n");
    printf(" q : ����\n\n");

    gotoxy(0, 18);
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
    printf(" �˻����ּ��� : ");
    getchar();
    gets(pick);
  
    if (strcmp(pick,"h") == 0) {                                // �Է¹��ڰ� h�� Ÿ��Ʋ �̵�
        Title();
    }
    else if (strcmp(pick, "q") == 0)
        exit(1);                                                         // �Է¹��ڰ� q�� ����
    else 
        HistoryRead(ap_data, a_count, pick);          // �����丮���� ����
}


void HistoryRead(R_DATA* ap_data, int a_count,char* search) {

    FILE* hp;
    char str[80],str1[50];
  
    
    char select;
    int num = 0;
    int line = getTotalLine();
    fopen_s(&hp, "SearchHistory.txt", "r");
    system("cls");
    printf("������������������������������������������������������������������������������������������������������\n");
    printf(" �����丮\n"); 

    for (int i = 0; i <= line; i++) {
        char* ptr = fgets(str, 80, hp);
        char* Rtime = strstr(ptr, search);                 //search�� �ش��ϴ� ���ڿ��� ptr�� ������ rtime�� ����

        if (Rtime != NULL)
            printf(" %s ", str);
    }
    printf("\n");
    printf("  h : Ȩ\n\n");
    printf("  q : ����\n\n\n\n");
    
    fclose(hp);
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("�󼼰˻��� �ϰ���� ��ǰ�̸��� �Է����ּ��� : ");
    
    gets(str1);
    if (strcmp(str1, "h") == 0) {
        
        Title();
    }
    else if (strcmp(str1, "q") == 0)
        exit(1);
    
    system("cls");
    printf("������������������������������������������������������������������������������������������������������\n");
    printf("������\n");
    for (int i = 0; i < a_count; i++, ap_data++) {

        if (strcmp(ap_data->PRDLST_NM,str1)==0)                // �˻�� ��ǰ��� ��ġ�ϸ� 
        {                       
            printf(" �̸� : %s\n", ap_data->PRDLST_NM);
            printf(" ���� : %s\n", ap_data->PRIMARY_FNCLTY);
            printf(" ��� : %s\n", ap_data->DISPOS);
            printf(" ���� : %s\n\n", ap_data->NTK_MTHD);            
        }
        num++;
    }
    for (int i = 0; i < num; i++) {
        ap_data--;
    }
    printf(" h : Ȩ\n\n");
    printf(" q : ����\n\n\n\n");
    TimeNow();
    printf("������������������������������������������������������������������������������������������������������\n");
    printf(" �޴��� �������ּ��� : ");
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
        // �л����� ������ ����Ѵ�.
        printf("%-2d �̸� : %-30s", ap_data->Ingnum, ap_data->PRDCT_N);
        printf(" ���� : %s\n", ap_data->PRI);

    }
}

void ShowData(R_DATA* ap_data, unsigned int a_count)
{
    for (unsigned int i = 0; i < a_count; i++, ap_data++) {
        // �л����� ������ ����Ѵ�.
        printf(" �̸� : %s\n", ap_data->PRDLST_NM);
        printf(" ���� : %s\n", ap_data->PRIMARY_FNCLTY);
        printf(" ��� : %s\n", ap_data->DISPOS);
        printf(" ���� : %s\n\n", ap_data->NTK_MTHD);

    }
}

int main()
{
    SetConsoleTitle(TEXT("�ǰ���ɽ�ǰ �˻� �ý���"));
    Title();

    
}
