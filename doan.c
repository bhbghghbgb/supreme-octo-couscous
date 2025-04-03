#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_IN_SINHVIEN "thongtinSV.txt"
#define FILE_IN_DIEMSO "diemso.txt"
#define FILE_IN_DANGNHAP "dangnhap.txt"
#define FILE_OUT_SINHVIEN "thongtinSV_out_c.txt"
#define FILE_OUT_DIEMSO "diemso_out_c.txt"
#define FILE_OUT_DANGNHAP "dangnhap_out_c.txt"
#define FILE_OUT_SINHVIEN_ALL "thongtinSV_export_c.txt"
#define MAX_STUDENTS 1024
#define MAX_INPUT_SIZE 1024
#define MAX_PROMPT_SIZE 1024
#define MAX_MA_SV_LENGTH 50
#define MAX_HO_TEN_LENGTH 100
#define MAX_GIOI_TINH_LENGTH 10
#define MAX_MAT_KHAU_LENGTH 50
#define MAX_LINE_LENGTH 256
#define MAX_LOGIN_ATTEMPTS 5
#define MIN_TRUNG_BINH_HK_WARNING 4.0
#define TRUNG_BINH_HK_SCALE 10
#define KTLT_WEIGHT 4
#define MMT_WEIGHT 3
#define CTDL_WEIGHT 3
#define ADMIN_MA_SV "11377"

typedef struct {
  char maSv[MAX_MA_SV_LENGTH];
  float ktlt;  // Ky Thuat Lap Trinh
  float mmt;   // Mang May Tinh
  float ctdl;  // Cau Truc Du Lieu
} DiemSo;

typedef struct {
  char maSv[MAX_MA_SV_LENGTH];
  char gioiTinh[MAX_GIOI_TINH_LENGTH];
  int namSinh;
  char hoTenSv[MAX_HO_TEN_LENGTH];
  float trungBinhHk;
} SinhVien;

typedef struct {
  char maSv[MAX_MA_SV_LENGTH];
  char matKhau[MAX_MAT_KHAU_LENGTH];
} DangNhap;

void tinhTrungBinhHK(SinhVien *sv, DiemSo *diem) {
  sv->trungBinhHk = (diem->ktlt * KTLT_WEIGHT + diem->mmt * MMT_WEIGHT +
                     diem->ctdl * CTDL_WEIGHT) /
                    TRUNG_BINH_HK_SCALE;
}

// Ham nay doc chuoi dau vao va sao chep vao bo dem dich
// Khong co gioi han kich thuoc bo dem (output), nen co the gay tran bo dem
// Su dung can than va ko input nhieu hon output buffer size khi test
void readLineWithTrimming(const char *prompt, char *output, size_t size,
                          bool allowEmpty) {
  char input[MAX_INPUT_SIZE];
  do {
    printf("%s", prompt);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline character
    // Remove leading spaces
    char *start = input;
    while (*start == ' ') start++;
    // Remove trailing spaces
    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';
    if (strlen(start) >= size) {
      printf("Input is too long. Please enter up to %zu characters.\n",
             size - 1);
      continue;
    }
    strncpy(output, start, size - 1);
    output[size - 1] = '\0';  // Ensure null-termination
  } while (strlen(output) == 0 && !allowEmpty);
}

bool readFloat(const char *prompt, float *output, bool allowEmpty) {
  char input[MAX_INPUT_SIZE];
  while (true) {
    readLineWithTrimming(prompt, input, sizeof(input), allowEmpty);
    if (strlen(input) == 0 && allowEmpty) {
      return false;
    }
    char *endptr;
    errno = 0;
    *output = strtof(input, &endptr);
    if (errno == 0 && *endptr == '\0') {
      return true;
    }
    printf("Gia tri khong hop le. Vui long nhap lai (dang so thuc).\n");
  }
}

bool readInt(const char *prompt, int *output, bool allowEmpty) {
  char input[MAX_INPUT_SIZE];
  while (true) {
    readLineWithTrimming(prompt, input, sizeof(input), allowEmpty);
    if (strlen(input) == 0 && allowEmpty) {
      return false;
    }
    char *endptr;
    errno = 0;
    *output = strtol(input, &endptr, 10);
    if (errno == 0 && *endptr == '\0') {
      return true;
    }
    printf("Gia tri khong hop le. Vui long nhap lai (dang so nguyen).\n");
  }
}

void printSinhVien(SinhVien *sv) {
  printf("MaSV: %s, GioiTinh: %s, NamSinh: %d, HoTen: %s, TrungBinhHK: %.2f\n",
         sv->maSv, sv->gioiTinh, sv->namSinh, sv->hoTenSv, sv->trungBinhHk);
}

void printDiemSo(DiemSo *ds) {
  printf("MaSV: %s, KTLT: %.2f, MMT: %.2f, CTDL: %.2f\n", ds->maSv, ds->ktlt,
         ds->mmt, ds->ctdl);
}

void printDangNhap(DangNhap *dn) {
  printf("MaSV: %s, MatKhau: %s\n", dn->maSv, dn->matKhau);
}

// Globals to hold the data
SinhVien sinhVienArray[MAX_STUDENTS];
DiemSo diemSoArray[MAX_STUDENTS];
DangNhap dangNhapArray[MAX_STUDENTS];
int sinhVienCount = 0;
int diemSoCount = 0;
int dangNhapCount = 0;

bool sinhVienLaGiangVien(const char *maSv) {
  return strcmp(maSv, ADMIN_MA_SV) == 0;
}

void readSinhVien(const char *filePath) {
  FILE *file = fopen(filePath, "r");
  if (!file) {
    perror("Cannot open SinhVien file");
    exit(1);
  }
  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = 0;  // Remove newline character
    SinhVien sv;
    sscanf(line, "%s %s %d %[^\n]", sv.maSv, sv.gioiTinh, &sv.namSinh,
           sv.hoTenSv);
    sv.trungBinhHk = 0.f;
    sinhVienArray[sinhVienCount++] = sv;
  }
  fclose(file);
}

void readDiemSo(const char *filePath) {
  FILE *file = fopen(filePath, "r");
  if (!file) {
    perror("Cannot open DiemSo file");
    exit(1);
  }
  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = 0;  // Remove newline character
    DiemSo ds;
    sscanf(line, "%s %f %f %f", ds.maSv, &ds.ktlt, &ds.mmt, &ds.ctdl);
    diemSoArray[diemSoCount++] = ds;
  }
  fclose(file);
}

void readDangNhap(const char *filePath) {
  FILE *file = fopen(filePath, "r");
  if (!file) {
    perror("Cannot open DangNhap file");
    exit(1);
  }
  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = 0;  // Remove newline character
    DangNhap dn;
    sscanf(line, "%s %s", dn.maSv, dn.matKhau);
    dangNhapArray[dangNhapCount++] = dn;
  }
  fclose(file);
}

void printAllSinhVien() {
  for (int i = 0; i < sinhVienCount; i++) {
    printSinhVien(&sinhVienArray[i]);
  }
}

void printAllDiemSo() {
  for (int i = 0; i < diemSoCount; i++) {
    printDiemSo(&diemSoArray[i]);
  }
}

void printAllDangNhap() {
  for (int i = 0; i < dangNhapCount; i++) {
    printDangNhap(&dangNhapArray[i]);
  }
}

void writeAllFileOut() {
  // Write SinhVien data to out file
  FILE *sinhVienFile = fopen(FILE_OUT_SINHVIEN, "w");
  if (!sinhVienFile) {
    fprintf(stderr, "Khong the mo file %s de ghi.\n", FILE_OUT_SINHVIEN);
    return;
  }
  for (int i = 0; i < sinhVienCount; i++) {
    fprintf(sinhVienFile, "%s %s %d %s\n", sinhVienArray[i].maSv,
            sinhVienArray[i].gioiTinh, sinhVienArray[i].namSinh,
            sinhVienArray[i].hoTenSv);
  }
  fclose(sinhVienFile);

  // Write DiemSo data to out file
  FILE *diemSoFile = fopen(FILE_OUT_DIEMSO, "w");
  if (!diemSoFile) {
    fprintf(stderr, "Khong the mo file %s de ghi.\n", FILE_OUT_DIEMSO);
    return;
  }
  for (int i = 0; i < diemSoCount; i++) {
    fprintf(diemSoFile, "%s %.2f %.2f %.2f\n", diemSoArray[i].maSv,
            diemSoArray[i].ktlt, diemSoArray[i].mmt, diemSoArray[i].ctdl);
  }
  fclose(diemSoFile);

  // Write DangNhap data to out file
  FILE *dangNhapFile = fopen(FILE_OUT_DANGNHAP, "w");
  if (!dangNhapFile) {
    fprintf(stderr, "Khong the mo file %s de ghi.\n", FILE_OUT_DANGNHAP);
    return;
  }
  for (int i = 0; i < dangNhapCount; i++) {
    fprintf(dangNhapFile, "%s %s\n", dangNhapArray[i].maSv,
            dangNhapArray[i].matKhau);
  }
  fclose(dangNhapFile);

  printf(
      "Xuat danh sach sinh vien, diem so, va dang nhap ra file thanh cong!\n");
}

SinhVien *getSinhVien(const char *maSv) {
  for (int i = 0; i < sinhVienCount; i++) {
    if (strcmp(sinhVienArray[i].maSv, maSv) == 0) {
      return &sinhVienArray[i];
    }
  }
  return NULL;
}

DiemSo *getDiemSo(const char *maSv) {
  for (int i = 0; i < diemSoCount; i++) {
    if (strcmp(diemSoArray[i].maSv, maSv) == 0) {
      return &diemSoArray[i];
    }
  }
  return NULL;
}

DangNhap *getDangNhap(const char *maSv) {
  for (int i = 0; i < dangNhapCount; i++) {
    if (strcmp(dangNhapArray[i].maSv, maSv) == 0) {
      return &dangNhapArray[i];
    }
  }
  return NULL;
}

bool dangNhap(char *sinhVienDaDangNhap) {
  printf("Moi dang nhap\n");
  for (int attempts = 0; attempts < MAX_LOGIN_ATTEMPTS; ++attempts) {
    char maSvInput[MAX_MA_SV_LENGTH], matKhauInput[MAX_MAT_KHAU_LENGTH];

    // Ask user for credentials
    readLineWithTrimming("Enter MaSV: ", maSvInput, sizeof(maSvInput), false);

    DangNhap *dangNhapEntry = getDangNhap(maSvInput);
    if (!dangNhapEntry) {
      printf("MaSV khong ton tai. Vui long thu lai.\n");
      continue;
    }

    readLineWithTrimming("Enter MatKhau: ", matKhauInput, sizeof(matKhauInput),
                         false);

    if (strcmp(dangNhapEntry->matKhau, matKhauInput) == 0) {
      strcpy(sinhVienDaDangNhap, maSvInput);
      printf("Dang nhap thanh cong!\n");
      return true;
    } else {
      printf("Sai thong tin dang nhap. Vui long thu lai.\n");
    }
  }
  printf("Ban da thu qua %d lan. Thoat chuong trinh.\n", MAX_LOGIN_ATTEMPTS);
  return false;
}

void dangNhapAsSv(const char *sinhVienDaDangNhap) {
  printf(
      "Dang nhap voi tu cach Sinh vien\n"
      "Thong tin SV:\n");
  printSinhVien(getSinhVien(sinhVienDaDangNhap));
  printf("Chi tiet diem so SV:\n");
  printDiemSo(getDiemSo(sinhVienDaDangNhap));
  printf("Sinh vien se dang xuat.\n");
  return;
}

void printSvAsGv(const char *maSv) {
  SinhVien *sv = getSinhVien(maSv);
  DiemSo *ds = getDiemSo(maSv);
  DangNhap *dn = getDangNhap(maSv);

  if (sv != NULL) {
    printSinhVien(sv);
  } else {
    printf("Khong tim thay thong tin SinhVien voi MaSV: %s\n", maSv);
  }

  if (ds != NULL) {
    printDiemSo(ds);
  } else {
    printf("Khong tim thay thong tin DiemSo voi MaSV: %s\n", maSv);
  }

  if (dn != NULL) {
    printDangNhap(dn);
  } else {
    printf("Khong tim thay thong tin DangNhap voi MaSV: %s\n", maSv);
  }
}

void themSinhVien() {
  char maSv[MAX_MA_SV_LENGTH], hoTenSv[MAX_HO_TEN_LENGTH],
      gioiTinh[MAX_GIOI_TINH_LENGTH], matKhau[MAX_MAT_KHAU_LENGTH];
  int namSinh;
  float ktlt, mmt, ctdl;

  printf("Nhap thong tin sinh vien moi:\n");
  // Input MaSV
  readLineWithTrimming("Nhap MaSV: ", maSv, sizeof(maSv), false);

  // Check if MaSV already exists
  if (getSinhVien(maSv) != NULL) {
    printf("MaSV da ton tai. Vui long thu lai.\n");
    return;
  }

  // Input other details
  readLineWithTrimming("Nhap Ho Ten Sinh Vien: ", hoTenSv, sizeof(hoTenSv),
                       false);
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
  writeAllFileOut();
  printSvAsGv(maSv);
}

void capNhatThongTinSinhVien() {
  char maSv[MAX_MA_SV_LENGTH];
  readLineWithTrimming("Nhap MaSV cua sinh vien can cap nhat: ", maSv,
                       sizeof(maSv), false);

  SinhVien *sv = getSinhVien(maSv);
  if (sv == NULL) {
    printf("MaSV khong ton tai.\n");
    return;
  }

  printf("Cap nhat thong tin sinh vien:\n");

  // buffer de tao prompt
  char prompt[MAX_PROMPT_SIZE];

  // Update HoTen
  char hoTenSv[MAX_HO_TEN_LENGTH];
  snprintf(prompt, sizeof(prompt), "Nhap Ho Ten Sinh Vien (%s): ", sv->hoTenSv);
  readLineWithTrimming(prompt, hoTenSv, sizeof(hoTenSv), true);
  if (strlen(hoTenSv) > 0) {
    strcpy(sv->hoTenSv, hoTenSv);
  }

  // Update GioiTinh
  char gioiTinh[MAX_GIOI_TINH_LENGTH];
  snprintf(prompt, sizeof(prompt), "Nhap Gioi Tinh (%s): ", sv->gioiTinh);
  readLineWithTrimming(prompt, gioiTinh, sizeof(gioiTinh), true);
  if (strlen(gioiTinh) > 0) {
    strcpy(sv->gioiTinh, gioiTinh);
  }

  // Update NamSinh
  int namSinh;
  snprintf(prompt, sizeof(prompt), "Nhap Nam Sinh (%d): ", sv->namSinh);
  if (readInt(prompt, &namSinh, true)) {
    sv->namSinh = namSinh;
  }

  // Update DiemSo
  DiemSo *ds = getDiemSo(maSv);
  if (ds == NULL) {
    printf("Diem so khong ton tai. Tao moi diem so.\n");
    ds = &diemSoArray[diemSoCount++];
    strcpy(ds->maSv, maSv);
  }

  float ktlt, mmt, ctdl;
  snprintf(prompt, sizeof(prompt), "Nhap diem KTLT (%.2f): ", ds->ktlt);
  if (readFloat(prompt, &ktlt, true)) {
    ds->ktlt = ktlt;
  }

  snprintf(prompt, sizeof(prompt), "Nhap diem MMT (%.2f): ", ds->mmt);
  if (readFloat(prompt, &mmt, true)) {
    ds->mmt = mmt;
  }

  snprintf(prompt, sizeof(prompt), "Nhap diem CTDL (%.2f): ", ds->ctdl);
  if (readFloat(prompt, &ctdl, true)) {
    ds->ctdl = ctdl;
  }

  // Recalculate TrungBinhHK
  tinhTrungBinhHK(sv, ds);

  DangNhap *dangNhap = getDangNhap(maSv);
  if (dangNhap == NULL) {
    // Create and add a new DangNhap if not found
    dangNhap = &dangNhapArray[dangNhapCount++];
    strcpy(dangNhap->maSv, maSv);
  }

  // Prompt for password input
  char enteredPassword[MAX_MAT_KHAU_LENGTH];
  readLineWithTrimming("Nhap mat khau moi cua sinh vien (<Chua thay doi>): ",
                       enteredPassword, sizeof(enteredPassword), true);

  // Check the password
  if (strlen(enteredPassword) != 0) {
    // Confirm the password
    char confirmPassword[MAX_MAT_KHAU_LENGTH];
    readLineWithTrimming("Nhap lai mat khau de xac nhan (<Chua thay doi>): ",
                         confirmPassword, sizeof(confirmPassword), false);
    if (strcmp(enteredPassword, confirmPassword) == 0) {
      strcpy(dangNhap->matKhau, enteredPassword);
      printf("Mat khau da duoc cap nhat thanh cong.\n");
    } else {
      printf("Mat khau khong khop. Vui long thu lai.\n");
    }
  } else {
    printf("Ban chua nhap mat khau. Khong thay doi.\n");
  }

  printf("Cap nhat thong tin sinh vien thanh cong!\n");
  writeAllFileOut();
  printSvAsGv(maSv);
}

void xoaSinhVien() {
  char maSv[MAX_MA_SV_LENGTH];
  readLineWithTrimming("Nhap MaSV cua sinh vien can xoa: ", maSv, sizeof(maSv),
                       false);

  int index = -1;
  for (int i = 0; i < sinhVienCount; i++) {
    if (strcmp(sinhVienArray[i].maSv, maSv) == 0) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    printf("MaSV khong ton tai.\n");
    return;
  }

  // Show information for confirmation
  printf("Thong tin sinh vien:\n");
  printSvAsGv(maSv);

  char confirmation[2];
  readLineWithTrimming("Ban co chac chan muon xoa sinh vien nay? (Y/N): ",
                       confirmation, sizeof(confirmation), false);
  if (strlen(confirmation) == 0 || toupper(confirmation[0]) != 'Y') {
    printf("Xoa sinh vien da bi huy bo.\n");
    return;  // Exit if the user cancels the deletion
  }

  // Shift elements to remove the student
  for (int i = index; i < sinhVienCount - 1; i++) {
    sinhVienArray[i] = sinhVienArray[i + 1];
  }
  sinhVienCount--;

  // Remove from DiemSo array
  for (int i = 0; i < diemSoCount; i++) {
    if (strcmp(diemSoArray[i].maSv, maSv) == 0) {
      for (int j = i; j < diemSoCount - 1; j++) {
        diemSoArray[j] = diemSoArray[j + 1];
      }
      diemSoCount--;
      break;
    }
  }

  // Remove from DangNhap array
  for (int i = 0; i < dangNhapCount; i++) {
    if (strcmp(dangNhapArray[i].maSv, maSv) == 0) {
      for (int j = i; j < dangNhapCount - 1; j++) {
        dangNhapArray[j] = dangNhapArray[j + 1];
      }
      dangNhapCount--;
      break;
    }
  }

  printf("Xoa sinh vien thanh cong!\n");
  writeAllFileOut();
}

void timKiemSinhVienTheoMaSV() {
  char maSv[MAX_MA_SV_LENGTH];
  readLineWithTrimming("Nhap MaSV can tim: ", maSv, sizeof(maSv), false);

  SinhVien *sv = getSinhVien(maSv);
  if (sv == NULL) {
    printf("Khong tim thay sinh vien voi MaSV: %s\n", maSv);
    return;
  }

  printf("Thong tin sinh vien:\n");
  printSvAsGv(maSv);
}

void hienThiDanhSachSinhVienKhongCanhBao() {
  printf(
      "Danh sach sinh vien khong bi canh bao hoc tap (TrungBinhHK >= %.1f):\n",
      MIN_TRUNG_BINH_HK_WARNING);
  for (int i = 0; i < sinhVienCount; i++) {
    if (sinhVienArray[i].trungBinhHk >= MIN_TRUNG_BINH_HK_WARNING) {
      printSinhVien(&sinhVienArray[i]);
    }
  }
}

void xuatRa1File() {
  FILE *fileOut = fopen(FILE_OUT_SINHVIEN_ALL, "w");
  if (!fileOut) {
    fprintf(stderr, "Khong the mo file %s de ghi.\n", FILE_OUT_SINHVIEN_ALL);
    return;
  }

  for (int i = 0; i < sinhVienCount; i++) {
    SinhVien *sv = &sinhVienArray[i];
    DiemSo *ds = getDiemSo(sv->maSv);
    DangNhap *dn = getDangNhap(sv->maSv);

    fprintf(fileOut, "%s %s %d %.2f", sv->maSv, sv->gioiTinh, sv->namSinh,
            sv->trungBinhHk);

    if (ds != NULL) {
      fprintf(fileOut, " %.2f %.2f %.2f", ds->ktlt, ds->mmt, ds->ctdl);
    } else {
      fprintf(fileOut, " N/A N/A N/A");
    }

    if (dn != NULL) {
      fprintf(fileOut, " %s", dn->matKhau);
    } else {
      fprintf(fileOut, " N/A");
    }

    fprintf(fileOut, " %s\n", sv->hoTenSv);
  }

  fclose(fileOut);
  printf("Xuat danh sach sinh vien tong hop ra file thanh cong!\n");
}

typedef struct {
  float minScore;
  const char *name;
  int count;
} HocLuc;

void thongKeHocLuc() {
  HocLuc hocLucCategories[] = {
      {9.0, "Xuat sac", 0},   {8.0, "Gioi", 0}, {7.0, "Kha", 0},
      {5.0, "Trung binh", 0}, {4.0, "Yeu", 0},  {-INFINITY, "Kem", 0},
  };

  int categoryCount = sizeof(hocLucCategories) / sizeof(hocLucCategories[0]);

  for (int i = 0; i < sinhVienCount; i++) {
    float tb = sinhVienArray[i].trungBinhHk;
    for (int j = 0; j < categoryCount; j++) {
      if (tb >= hocLucCategories[j].minScore) {
        hocLucCategories[j].count++;
        break;
      }
    }
  }

  printf("Thong ke hoc luc:\n");
  printf("Tong so sinh vien: %d\n", sinhVienCount);
  for (int i = 0; i < categoryCount; i++) {
    printf("%s: %d sinh vien (%.2f%%)\n", hocLucCategories[i].name,
           hocLucCategories[i].count,
           sinhVienCount > 0
               ? (hocLucCategories[i].count * 100.0 / sinhVienCount)
               : 0.0);
  }
}

typedef struct {
  char gioiTinh[MAX_GIOI_TINH_LENGTH];
  int count;
} GioiTinhCount;

void thongKeGioiTinh() {
  GioiTinhCount gioiTinhCounts[MAX_STUDENTS];
  int gioiTinhTypeCount = 0;

  for (int i = 0; i < sinhVienCount; i++) {
    bool found = false;
    for (int j = 0; j < gioiTinhTypeCount; j++) {
      if (strcmp(sinhVienArray[i].gioiTinh, gioiTinhCounts[j].gioiTinh) == 0) {
        gioiTinhCounts[j].count++;
        found = true;
        break;
      }
    }
    if (!found) {
      strcpy(gioiTinhCounts[gioiTinhTypeCount].gioiTinh,
             sinhVienArray[i].gioiTinh);
      gioiTinhCounts[gioiTinhTypeCount].count = 1;
      gioiTinhTypeCount++;
    }
  }

  printf("Thong ke gioi tinh:\n");
  for (int i = 0; i < gioiTinhTypeCount; i++) {
    printf("%s: %d sinh vien (%.2f%%)\n", gioiTinhCounts[i].gioiTinh,
           gioiTinhCounts[i].count,
           sinhVienCount > 0 ? (gioiTinhCounts[i].count * 100.0 / sinhVienCount)
                             : 0.0);
  }
}

void dangNhapAsGv() {
  while (true) {
    printf("--------------------- MENU ---------------------\n");
    printf("1. Them sinh vien.\n");
    printf("2. Cap nhat thong tin sinh vien.\n");
    printf("3. Xoa sinh vien.\n");
    printf("4. Tim kiem va hien thi thong tin sinh vien theo MaSV.\n");
    printf(
        "5. Hien thi danh sach sinh vien khong bi canh bao hoc tap "
        "(TrungbinhHK >= %.1f).\n",
        MIN_TRUNG_BINH_HK_WARNING);
    printf("6. Xuat danh sach sinh vien tong hop ra file %s.\n",
           FILE_OUT_SINHVIEN_ALL);
    printf("7. Thong ke hoc luc.\n");
    printf("8. Thong ke gioi tinh.\n");
    printf("------------------------------------------------\n");
    printf("0. Dang xuat.\n");

    int choice;
    if (!readInt("Nhap lua chon cua ban (0-8): ", &choice, false)) {
      continue;
    }

    switch (choice) {
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
        xuatRa1File();
        break;
      case 7:
        thongKeHocLuc();
        break;
      case 8:
        thongKeGioiTinh();
        break;
      default:
        printf("Lua chon khong hop le. Vui long thu lai.\n");
    }
  }
}

void giaoDien() {
  char sinhVienDaDangNhap[MAX_MA_SV_LENGTH];
  printf(
      "Chao mung den voi do an Thuc hanh mon Ky thuat lap trinh: Quan ly Sinh "
      "vien\n");
  do {
    printf("Ban da dang xuat\n");
    if (!dangNhap(sinhVienDaDangNhap)) {
      return;
    }

    if (!sinhVienLaGiangVien(sinhVienDaDangNhap)) {
      dangNhapAsSv(sinhVienDaDangNhap);
      continue;
    }

    printf("Dang nhap voi tu cach Giang vien\n");
    // Add more functionality for GiangVien here...
    dangNhapAsGv();
  } while (true);
}

int main() {
  readSinhVien(FILE_IN_SINHVIEN);
  readDiemSo(FILE_IN_DIEMSO);
  readDangNhap(FILE_IN_DANGNHAP);

  // Populate TrungBinhHK for SinhVienArray
  for (int i = 0; i < sinhVienCount; i++) {
    DiemSo *diem = getDiemSo(sinhVienArray[i].maSv);
    if (diem != NULL) {
      tinhTrungBinhHK(&sinhVienArray[i], diem);
    }
  }

  //   printAllSinhVien();
  //   printAllDiemSo();
  //   printAllDangNhap();

  giaoDien();

  printf("Thoat chuong trinh.\n");
  return 0;
}