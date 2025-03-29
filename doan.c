#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

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

// Ham nay doc chuoi dau vao va sao chep vao bo dem dich
// Khong co gioi han kich thuoc bo dem (output), nen co the gay tran bo dem
// Su dung can than va ko input nhieu hon output buffer size khi test
void readLineWithTrimming(const char *prompt, char *output, int size, bool allowEmpty)
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
        if (strlen(start) >= size)
        {
            printf("Input is too long. Please enter up to %d characters.\n", size - 1);
            continue;
        }
        strncpy(output, start, size - 1);
        output[size - 1] = '\0'; // Ensure null-termination
    } while (strlen(output) == 0 && !allowEmpty);
}

bool readFloat(const char *prompt, float *output, bool allowEmpty)
{
    char input[1024];
    while (true)
    {
        readLineWithTrimming(prompt, input, sizeof(input), allowEmpty);
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

bool readInt(const char *prompt, int *output, bool allowEmpty)
{
    char input[1024];
    while (true)
    {
        readLineWithTrimming(prompt, input, sizeof(input), allowEmpty);
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
        readLineWithTrimming("Enter MaSV: ", maSvInput, sizeof(maSvInput), false);

        DangNhap *dangNhapEntry = getDangNhap(maSvInput);
        if (!dangNhapEntry)
        {
            printf("MaSV khong ton tai. Vui long thu lai.\n");
            continue;
        }

        readLineWithTrimming("Enter MatKhau: ", matKhauInput, sizeof(matKhauInput), false);

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

void printSvAsGv(const char *maSv)
{
    SinhVien *sv = getSinhVien(maSv);
    DiemSo *ds = getDiemSo(maSv);
    DangNhap *dn = getDangNhap(maSv);

    if (sv != NULL)
    {
        printSinhVien(sv);
    }
    else
    {
        printf("Khong tim thay thong tin SinhVien voi MaSV: %s\n", maSv);
    }

    if (ds != NULL)
    {
        printDiemSo(ds);
    }
    else
    {
        printf("Khong tim thay thong tin DiemSo voi MaSV: %s\n", maSv);
    }

    if (dn != NULL)
    {
        printDangNhap(dn);
    }
    else
    {
        printf("Khong tim thay thong tin DangNhap voi MaSV: %s\n", maSv);
    }
}

void themSinhVien()
{
    char maSv[50], hoTenSv[100], gioiTinh[10], matKhau[50];
    int namSinh;
    float ktlt, mmt, ctdl;

    printf("Nhap thong tin sinh vien moi:\n");
    // Input MaSV
    readLineWithTrimming("Nhap MaSV: ", maSv, sizeof(maSv), false);

    // Check if MaSV already exists
    if (getSinhVien(maSv) != NULL)
    {
        printf("MaSV da ton tai. Vui long thu lai.\n");
        return;
    }

    // Input other details
    readLineWithTrimming("Nhap Ho Ten Sinh Vien: ", hoTenSv, sizeof(hoTenSv), false);
    readLineWithTrimming("Nhap Gioi Tinh: ", gioiTinh, sizeof(gioiTinh), false);
    readInt("Nhap Nam Sinh: ", &namSinh, false);
    readFloat("Nhap diem Ky Thuat Lap Trinh (KTLT): ", &ktlt, false);
    readFloat("Nhap diem Mang May Tinh (MMT): ", &mmt, false);
    readFloat("Nhap diem Cau Truc Du Lieu (CTDL): ", &ctdl, false);
    readLineWithTrimming("Nhap Mat Khau: ", matKhau, sizeof(matKhau), false);

    // Create new SinhVien, DiemSo, and DangNhap
    SinhVien sv;
    strcpy(sv.maSv, maSv);
    strcpy(sv.gioiTinh, gioiTinh);
    sv.namSinh = namSinh;
    strcpy(sv.hoTenSv, hoTenSv);
    sv.trungBinhHk = 0.0f;

    DiemSo ds;
    strcpy(ds.maSv, maSv);
    ds.ktlt = ktlt;
    ds.mmt = mmt;
    ds.ctdl = ctdl;

    DangNhap dn;
    strcpy(dn.maSv, maSv);
    strcpy(dn.matKhau, matKhau);

    // Calculate TrungBinhHK
    tinhTrungBinhHK(&sv, &ds);

    // Add to arrays
    sinhVienArray[sinhVienCount++] = sv;
    diemSoArray[diemSoCount++] = ds;
    dangNhapArray[dangNhapCount++] = dn;

    printf("Them sinh vien moi thanh cong!\n");
    printSvAsGv(maSv);
}

void capNhatThongTinSinhVien()
{
    char maSv[50];
    printf("Nhap MaSV cua sinh vien can cap nhat: ");
    readLineWithTrimming("", maSv, sizeof(maSv), false);

    SinhVien *sv = getSinhVien(maSv);
    if (sv == NULL)
    {
        printf("MaSV khong ton tai.\n");
        return;
    }

    printf("Cap nhat thong tin sinh vien:\n");

    // Update HoTen
    char hoTenSv[100];
    printf("Nhap Ho Ten Sinh Vien (%s): ", sv->hoTenSv);
    readLineWithTrimming("", hoTenSv, sizeof(hoTenSv), true);
    if (strlen(hoTenSv) > 0)
    {
        strcpy(sv->hoTenSv, hoTenSv);
    }

    // Update GioiTinh
    char gioiTinh[10];
    printf("Nhap Gioi Tinh (%s): ", sv->gioiTinh);
    readLineWithTrimming("", gioiTinh, sizeof(gioiTinh), true);
    if (strlen(gioiTinh) > 0)
    {
        strcpy(sv->gioiTinh, gioiTinh);
    }

    // Update NamSinh
    int namSinh;
    printf("Nhap Nam Sinh (%d): ", sv->namSinh);
    if (readInt("", &namSinh, true))
    {
        sv->namSinh = namSinh;
    }

    // Update DiemSo
    DiemSo *ds = getDiemSo(maSv);
    if (ds == NULL)
    {
        printf("Diem so khong ton tai. Tao moi diem so.\n");
        ds = &diemSoArray[diemSoCount++];
        strcpy(ds->maSv, maSv);
    }

    float ktlt, mmt, ctdl;
    printf("Nhap diem KTLT (%.2f): ", ds->ktlt);
    if (readFloat("", &ktlt, true))
    {
        ds->ktlt = ktlt;
    }

    printf("Nhap diem MMT (%.2f): ", ds->mmt);
    if (readFloat("", &mmt, true))
    {
        ds->mmt = mmt;
    }

    printf("Nhap diem CTDL (%.2f): ", ds->ctdl);
    if (readFloat("", &ctdl, true))
    {
        ds->ctdl = ctdl;
    }

    // Recalculate TrungBinhHK
    tinhTrungBinhHK(sv, ds);

    DangNhap *dangNhap = getDangNhap(maSv);
    if (dangNhap == NULL)
    {
        // Create and add a new DangNhap if not found
        dangNhap = &dangNhapArray[dangNhapCount++];
        strcpy(dangNhap->maSv, maSv);
    }

    // Prompt for password input
    char enteredPassword[50];
    printf("Nhap mat khau cua sinh vien: ");
    readLineWithTrimming("", enteredPassword, sizeof(enteredPassword), true);

    // Check the password
    if (strlen(enteredPassword) == 0)
    {
        printf("Ban chua nhap mat khau. Khong thay doi.\n");
        return; // Exit without changing the password
    }

    // Confirm the password
    char confirmPassword[50];
    printf("Nhap lai mat khau de xac nhan: ");
    readLineWithTrimming("", confirmPassword, sizeof(confirmPassword), false);

    if (strcmp(enteredPassword, confirmPassword) == 0)
    {
        strcpy(dangNhap->matKhau, enteredPassword);
        printf("Mat khau da duoc cap nhat thanh cong.\n");
    }
    else
    {
        printf("Mat khau khong khop. Vui long thu lai.\n");
    }

    printf("Cap nhat thong tin sinh vien thanh cong!\n");
    printSvAsGv(maSv);
}

void xoaSinhVien()
{
    char maSv[50];
    printf("Nhap MaSV cua sinh vien can xoa: ");
    readLineWithTrimming("", maSv, sizeof(maSv), false);

    int index = -1;
    for (int i = 0; i < sinhVienCount; i++)
    {
        if (strcmp(sinhVienArray[i].maSv, maSv) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("MaSV khong ton tai.\n");
        return;
    }

    // Show information for confirmation
    printf("Thong tin sinh vien:\n");
    printSvAsGv(maSv);

    char confirmation[10];
    readLineWithTrimming("Ban co chac chan muon xoa sinh vien nay? (Y/N): ", confirmation, sizeof(confirmation), false);
    if (strlen(confirmation) == 0 || toupper(confirmation[0]) != 'Y')
    {
        printf("Xoa sinh vien da bi huy bo.\n");
        return; // Exit if the user cancels the deletion
    }

    // Shift elements to remove the student
    for (int i = index; i < sinhVienCount - 1; i++)
    {
        sinhVienArray[i] = sinhVienArray[i + 1];
    }
    sinhVienCount--;

    printf("Xoa sinh vien thanh cong!\n");
}

void timKiemSinhVienTheoMaSV()
{
    char maSv[50];
    printf("Nhap MaSV can tim: ");
    readLineWithTrimming("", maSv, sizeof(maSv), false);

    SinhVien *sv = getSinhVien(maSv);
    if (sv == NULL)
    {
        printf("Khong tim thay sinh vien voi MaSV: %s\n", maSv);
        return;
    }

    printf("Thong tin sinh vien:\n");
    printSvAsGv(maSv);
}

void hienThiDanhSachSinhVienKhongCanhBao()
{
    printf("Danh sach sinh vien khong bi canh bao hoc tap (TrungBinhHK >= 4.0):\n");
    for (int i = 0; i < sinhVienCount; i++)
    {
        if (sinhVienArray[i].trungBinhHk >= 4.0)
        {
            printSinhVien(&sinhVienArray[i]);
        }
    }
}

void xuatDanhSachSinhVienRaFile()
{
    // Generate export filenames
    const char *sinhVienExportPath = "thongtinSV_export_c.txt";
    const char *diemSoExportPath = "diemso_export_c.txt";
    const char *dangNhapExportPath = "dangnhap_export_c.txt";

    // Write SinhVien data to export file
    FILE *sinhVienFile = fopen(sinhVienExportPath, "w");
    if (!sinhVienFile)
    {
        perror("Khong the mo file thongtinSV_export.txt de ghi.");
        return;
    }
    for (int i = 0; i < sinhVienCount; i++)
    {
        fprintf(sinhVienFile, "%s %s %d %s\n",
                sinhVienArray[i].maSv,
                sinhVienArray[i].gioiTinh,
                sinhVienArray[i].namSinh,
                sinhVienArray[i].hoTenSv);
    }
    fclose(sinhVienFile);

    // Write DiemSo data to export file
    FILE *diemSoFile = fopen(diemSoExportPath, "w");
    if (!diemSoFile)
    {
        perror("Khong the mo file diemso_export.txt de ghi.");
        return;
    }
    for (int i = 0; i < diemSoCount; i++)
    {
        fprintf(diemSoFile, "%s %.2f %.2f %.2f\n",
                diemSoArray[i].maSv,
                diemSoArray[i].ktlt,
                diemSoArray[i].mmt,
                diemSoArray[i].ctdl);
    }
    fclose(diemSoFile);

    // Write DangNhap data to export file
    FILE *dangNhapFile = fopen(dangNhapExportPath, "w");
    if (!dangNhapFile)
    {
        perror("Khong the mo file dangnhap_export.txt de ghi.");
        return;
    }
    for (int i = 0; i < dangNhapCount; i++)
    {
        fprintf(dangNhapFile, "%s %s\n",
                dangNhapArray[i].maSv,
                dangNhapArray[i].matKhau);
    }
    fclose(dangNhapFile);

    printf("Xuat danh sach sinh vien, diem so, va dang nhap ra file thanh cong!\n");
}

void thongKeHocLuc()
{
    int gioi = 0, kha = 0, trungBinh = 0, yeu = 0, xuatSac = 0, kem = 0;

    for (int i = 0; i < sinhVienCount; i++)
    {
        float tb = sinhVienArray[i].trungBinhHk;
        if (tb >= 9.0)
            xuatSac++;
        else if (tb >= 8.0)
            gioi++;
        else if (tb >= 7.0)
            kha++;
        else if (tb >= 5.0)
            trungBinh++;
        else if (tb >= 4.0)
            yeu++;
        else
            kem++;
    }

    int totalStudents = sinhVienCount;
    printf("Thong ke hoc luc:\n");
    printf("Tong so sinh vien: %d\n", totalStudents);
    printf("Xuat sac: %d sinh vien (%.2f%%)\n", xuatSac, totalStudents > 0 ? (xuatSac * 100.0 / totalStudents) : 0.0);
    printf("Gioi: %d sinh vien (%.2f%%)\n", gioi, totalStudents > 0 ? (gioi * 100.0 / totalStudents) : 0.0);
    printf("Kha: %d sinh vien (%.2f%%)\n", kha, totalStudents > 0 ? (kha * 100.0 / totalStudents) : 0.0);
    printf("Trung binh: %d sinh vien (%.2f%%)\n", trungBinh, totalStudents > 0 ? (trungBinh * 100.0 / totalStudents) : 0.0);
    printf("Yeu: %d sinh vien (%.2f%%)\n", yeu, totalStudents > 0 ? (yeu * 100.0 / totalStudents) : 0.0);
    printf("Kem: %d sinh vien (%.2f%%)\n", kem, totalStudents > 0 ? (kem * 100.0 / totalStudents) : 0.0);
}

void dangNhapAsGv()
{
    while (true)
    {
        printf("--------------------- MENU ---------------------\n");
        printf("1. Them sinh vien.\n");
        printf("2. Cap nhat thong tin sinh vien.\n");
        printf("3. Xoa sinh vien.\n");
        printf("4. Tim kiem va hien thi thong tin sinh vien theo MaSV.\n");
        printf("5. Hien thi danh sach sinh vien khong bi canh bao hoc tap (TrungbinhHK >= 4).\n");
        printf("6. Xuat danh sach sinh vien ra file \"*_export.txt\".\n");
        printf("7. Thong ke hoc luc.\n");
        printf("------------------------------------------------\n");
        printf("0. Dang xuat.\n");

        int choice;
        if (!readInt("Nhap lua chon cua ban (0-7): ", &choice, false))
        {
            continue;
        }

        switch (choice)
        {
        case 0:
            printf("Giang vien se dang xuat.\n");
            return;
        case 1:
            themSinhVien();
            break;
        case 2:
            capNhatThongTinSinhVien();
            break;
        case 3:
            xoaSinhVien();
            break;
        case 4:
            timKiemSinhVienTheoMaSV();
            break;
        case 5:
            hienThiDanhSachSinhVienKhongCanhBao();
            break;
        case 6:
            xuatDanhSachSinhVienRaFile();
            break;
        case 7:
            thongKeHocLuc();
            break;
        default:
            printf("Lua chon khong hop le. Vui long thu lai.\n");
        }
    }
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
        dangNhapAsGv();
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