#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

#define MAX_STUDENTS 1024

typedef struct
{
    char maSv[50];
    float ktlt; // Ky Thuat Lap Trinh
    float mmt;  // Mang May Tinh
    float ctdl; // Cau Truc Du Lieu
} DiemSo;

typedef struct
{
    char maSv[50];
    char gioiTinh[10];
    int namSinh;
    char hoTenSv[100];
    float trungBinhHk;
} SinhVien;

typedef struct
{
    char maSv[50];
    char matKhau[50];
} DangNhap;

void tinhTrungBinhHK(SinhVien *sv, DiemSo *diem)
{
    sv->trungBinhHk = (diem->ktlt * 4 + diem->mmt * 3 + diem->ctdl * 3) / 10;
}

void readLineWithTrimming(const char *prompt, char *output, bool allowEmpty)
{
    char input[1024];
    do
    {
        printf("%s", prompt);
        fgets(input, 1024, stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        // Remove leading spaces
        char *start = input;
        while (*start == ' ')
            start++;
        // Remove trailing spaces
        char *end = start + strlen(start) - 1;
        while (end > start && *end == ' ')
            end--;
        *(end + 1) = '\0';
        strcpy(output, start);
    } while (strlen(output) == 0 && !allowEmpty);
}

bool readFloat(char *prompt, float *output, bool allowEmpty)
{
    char input[1024];
    while (true)
    {
        readLineWithTrimming(prompt, input, allowEmpty);
        if (strlen(input) == 0 && allowEmpty)
        {
            return false;
        }
        char *endptr;
        errno = 0;
        *output = strtof(input, &endptr);
        if (errno == 0 && *endptr == '\0')
        {
            return true;
        }
        printf("Gia tri khong hop le. Vui long nhap lai (dang so thuc).\n");
    }
}

bool readInt(char *prompt, int *output, bool allowEmpty)
{
    char input[1024];
    while (true)
    {
        readLineWithTrimming(prompt, input, allowEmpty);
        if (strlen(input) == 0 && allowEmpty)
        {
            return false;
        }
        char *endptr;
        errno = 0;
        *output = strtol(input, &endptr, 10);
        if (errno == 0 && *endptr == '\0')
        {
            return true;
        }
        printf("Gia tri khong hop le. Vui long nhap lai (dang so nguyen).\n");
    }
}

void printSinhVien(SinhVien *sv)
{
    printf("MaSV: %s, GioiTinh: %s, NamSinh: %d, HoTen: %s, TrungBinhHK: %.2f\n",
           sv->maSv, sv->gioiTinh, sv->namSinh, sv->hoTenSv, sv->trungBinhHk);
}

void printDiemSo(DiemSo *ds)
{
    printf("MaSV: %s, KTLT: %.2f, MMT: %.2f, CTDL: %.2f\n",
           ds->maSv, ds->ktlt, ds->mmt, ds->ctdl);
}

void printDangNhap(DangNhap *dn)
{
    printf("MaSV: %s, MatKhau: %s\n", dn->maSv, dn->matKhau);
}

// Globals to hold the data
SinhVien sinhVienArray[MAX_STUDENTS];
DiemSo diemSoArray[MAX_STUDENTS];
DangNhap dangNhapArray[MAX_STUDENTS];
int sinhVienCount = 0;
int diemSoCount = 0;
int dangNhapCount = 0;

bool sinhVienLaGiangVien(const char *maSv)
{
    return strcmp(maSv, "11377") == 0;
}

void readSinhVien(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        perror("Cannot open SinhVien file");
        exit(1);
    }
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        SinhVien sv;
        sscanf(line, "%s %s %d %[^\n]", sv.maSv, sv.gioiTinh, &sv.namSinh, sv.hoTenSv);
        sv.trungBinhHk = 0.f;
        sinhVienArray[sinhVienCount++] = sv;
    }
    fclose(file);
}

void readDiemSo(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        perror("Cannot open DiemSo file");
        exit(1);
    }
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        DiemSo ds;
        sscanf(line, "%s %f %f %f", ds.maSv, &ds.ktlt, &ds.mmt, &ds.ctdl);
        diemSoArray[diemSoCount++] = ds;
    }
    fclose(file);
}

void readDangNhap(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        perror("Cannot open DangNhap file");
        exit(1);
    }
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        DangNhap dn;
        sscanf(line, "%s %s", dn.maSv, dn.matKhau);
        dangNhapArray[dangNhapCount++] = dn;
    }
    fclose(file);
}

void printAllSinhVien()
{
    for (int i = 0; i < sinhVienCount; i++)
    {
        printSinhVien(&sinhVienArray[i]);
    }
}

void printAllDiemSo()
{
    for (int i = 0; i < diemSoCount; i++)
    {
        printDiemSo(&diemSoArray[i]);
    }
}

void printAllDangNhap()
{
    for (int i = 0; i < dangNhapCount; i++)
    {
        printDangNhap(&dangNhapArray[i]);
    }
}

SinhVien *getSinhVien(const char *maSv)
{
    for (int i = 0; i < sinhVienCount; i++)
    {
        if (strcmp(sinhVienArray[i].maSv, maSv) == 0)
        {
            return &sinhVienArray[i];
        }
    }
    return NULL;
}

DiemSo *getDiemSo(const char *maSv)
{
    for (int i = 0; i < diemSoCount; i++)
    {
        if (strcmp(diemSoArray[i].maSv, maSv) == 0)
        {
            return &diemSoArray[i];
        }
    }
    return NULL;
}

DangNhap *getDangNhap(const char *maSv)
{
    for (int i = 0; i < dangNhapCount; i++)
    {
        if (strcmp(dangNhapArray[i].maSv, maSv) == 0)
        {
            return &dangNhapArray[i];
        }
    }
    return NULL;
}

bool dangNhap(char *sinhVienDaDangNhap)
{
    printf("Moi dang nhap\n");
    for (int attempts = 0; attempts < 5; ++attempts)
    {
        char maSvInput[50], matKhauInput[50];

        // Ask user for credentials
        readLineWithTrimming("Enter MaSV: ", maSvInput, false);

        DangNhap *dangNhapEntry = getDangNhap(maSvInput);
        if (!dangNhapEntry)
        {
            printf("MaSV khong ton tai. Vui long thu lai.\n");
            continue;
        }

        readLineWithTrimming("Enter MatKhau: ", matKhauInput, false);

        if (strcmp(dangNhapEntry->matKhau, matKhauInput) == 0)
        {
            strcpy(sinhVienDaDangNhap, maSvInput);
            printf("Dang nhap thanh cong!\n");
            return true;
        }
        else
        {
            printf("Sai thong tin dang nhap. Vui long thu lai.\n");
        }
    }
    printf("Ban da thu qua 5 lan. Thoat chuong trinh.\n");
    return false;
}

void dangNhapAsSv(const char *sinhVienDaDangNhap)
{
    printf("Dang nhap voi tu cach Sinh vien\n"
           "Thong tin SV:\n");
    printSinhVien(getSinhVien(sinhVienDaDangNhap));
    printf("Chi tiet diem so SV:\n");
    printDiemSo(getDiemSo(sinhVienDaDangNhap));
    printf("Sinh vien se dang xuat.\n");
    return;
}

void giaoDien()
{
    char sinhVienDaDangNhap[50];
    printf("Chao mung den voi do an Thuc hanh mon Ky thuat lap trinh: Quan ly Sinh vien\n");
    do
    {
        printf("Ban da dang xuat\n");
        if (!dangNhap(sinhVienDaDangNhap))
        {
            return;
        }

        if (!sinhVienLaGiangVien(sinhVienDaDangNhap))
        {
            dangNhapAsSv(sinhVienDaDangNhap);
            continue;
        }

        printf("Dang nhap voi tu cach Giang vien\n");
        // Add more functionality for GiangVien here...
    } while (true);
}

int main()
{
    readSinhVien("thongtinSV.txt");
    readDiemSo("diemso.txt");
    readDangNhap("dangnhap.txt");

    // Populate TrungBinhHK for SinhVienArray
    for (int i = 0; i < sinhVienCount; i++)
    {
        DiemSo *diem = getDiemSo(sinhVienArray[i].maSv);
        if (diem != NULL)
        {
            tinhTrungBinhHK(&sinhVienArray[i], diem);
        }
    }

    printAllSinhVien();
    printAllDiemSo();
    printAllDangNhap();

    giaoDien();

    printf("Thoat chuong trinh.\n");
    return 0;
}