#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>
#include <ios>

class DiemSo
{
public:
    std::string maSv;
    float ktlt; // Ky Thuat Lap Trinh
    float mmt;  // Mang May Tinh
    float ctdl; // Cau Truc Du Lieu

    DiemSo(std::string ma, float k, float m, float c) : maSv(ma), ktlt(k), mmt(m), ctdl(c) {}

    static DiemSo fromString(const std::string &line)
    {
        std::istringstream iss(line);
        std::string ma;
        float k, m, c;
        iss >> ma >> k >> m >> c;
        return DiemSo(ma, k, m, c);
    }

    // Overload operator<< for cout
    friend std::ostream &operator<<(std::ostream &os, const DiemSo &ds)
    {
        os << "MaSV: " << ds.maSv << ", KTLT: " << ds.ktlt
           << ", MMT: " << ds.mmt << ", CTDL: " << ds.ctdl;
        return os;
    }
};

class SinhVien
{
public:
    std::string maSv;
    std::string gioiTinh;
    int namSinh;
    std::string hoTenSv;
    float trungBinhHk;

    SinhVien(std::string ma, std::string gioi, int nam, std::string hoTen)
        : maSv(ma), gioiTinh(gioi), namSinh(nam), hoTenSv(hoTen), trungBinhHk(0.0f) {}

    void tinhTrungBinhHK(const DiemSo &diem)
    {
        trungBinhHk = (diem.ktlt * 4 + diem.mmt * 3 + diem.ctdl * 3) / 10.0f;
    }

    static SinhVien fromString(const std::string &line)
    {
        std::istringstream iss(line);
        std::string ma, gioi, hoTen, temp;
        int nam;
        iss >> ma >> gioi >> nam;
        hoTen = "";
        while (iss >> temp)
        {
            hoTen += temp + " ";
        }
        hoTen = hoTen.substr(0, hoTen.size() - 1); // Remove trailing space
        return SinhVien(ma, gioi, nam, hoTen);
    }

    // Overload operator<< for cout
    friend std::ostream &operator<<(std::ostream &os, const SinhVien &sv)
    {
        os << "MaSV: " << sv.maSv << ", GioiTinh: " << sv.gioiTinh
           << ", NamSinh: " << sv.namSinh << ", HoTen: " << sv.hoTenSv
           << ", TrungBinhHK: " << sv.trungBinhHk;
        return os;
    }
};

class DangNhap
{
public:
    std::string maSv;
    std::string matKhau;

    DangNhap(std::string ma, std::string mk) : maSv(ma), matKhau(mk) {}

    static DangNhap fromString(const std::string &line)
    {
        std::istringstream iss(line);
        std::string ma, mk;
        iss >> ma >> mk;
        return DangNhap(ma, mk);
    }

    // Overload operator<< for cout
    friend std::ostream &operator<<(std::ostream &os, const DangNhap &dn)
    {
        os << "MaSV: " << dn.maSv << ", MatKhau: " << dn.matKhau;
        return os;
    }
};

std::unordered_map<std::string, SinhVien> readSinhVien(const std::string &filename)
{
    std::unordered_map<std::string, SinhVien> sinhVienMap;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        SinhVien sv = SinhVien::fromString(line);
        sinhVienMap.emplace(sv.maSv, std::move(sv));
    }
    return sinhVienMap;
}

std::unordered_map<std::string, DiemSo> readDiemSo(const std::string &filename)
{
    std::unordered_map<std::string, DiemSo> diemSoMap;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        DiemSo ds = DiemSo::fromString(line);
        diemSoMap.emplace(ds.maSv, std::move(ds));
    }
    return diemSoMap;
}

std::unordered_map<std::string, DangNhap> readDangNhap(const std::string &filename)
{
    std::unordered_map<std::string, DangNhap> dangNhapMap;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        DangNhap dn = DangNhap::fromString(line);
        dangNhapMap.emplace(dn.maSv, std::move(dn));
    }
    return dangNhapMap;
}

inline bool sinhVienLaGiangVien(std::string maSv)
{
    return maSv == "11377";
}

// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
inline std::string &trim(std::string &str)
{
    str.erase(str.find_last_not_of(" \n\r\t") + 1); // suffixing spaces
    str.erase(0, str.find_first_not_of(" \n\r\t")); // prefixing spaces
    return str;
}

// Function to read a single line, ignoring leading newlines or spaces
std::string readLineWithTrimming()
{
    std::string input;
    do
    {
        std::numeric_limits<std::streamsize>::max();
        std::getline(std::cin, input);
        trim(input);
    } while (input.empty()); // Skip empty lines
    return input;
}

class DataManager
{
private:
    std::unordered_map<std::string, SinhVien> sinhVienMap;
    std::unordered_map<std::string, DiemSo> diemSoMap;
    std::unordered_map<std::string, DangNhap> dangNhapMap;

    std::string sinhVienDaDangNhap; // Stores the ID of the logged-in student

public:
    // Constructor that reads the files and populates the maps
    DataManager(const std::string &sinhVienFile, const std::string &diemSoFile, const std::string &dangNhapFile)
    {
        sinhVienMap = readSinhVien(sinhVienFile);
        diemSoMap = readDiemSo(diemSoFile);
        dangNhapMap = readDangNhap(dangNhapFile);

        // Populate TrungBinhHK for SinhVien
        for (const auto &[maSv, diem] : diemSoMap)
        {
            auto it = sinhVienMap.find(maSv);
            if (it != sinhVienMap.end())
            {
                it->second.tinhTrungBinhHK(diem);
            }
        }
    }

    bool dangNhap()
    {
        std::cout << "Moi dang nhap\n";

        for (int attempts = 0; attempts < 5; ++attempts)
        {
            std::string maSvInput, matKhauInput;

            // Ask user for credentials
            // Prompt and validate MaSV first
            std::cout << "Enter MaSV: ";
            maSvInput = readLineWithTrimming();

            auto sinhVienIt = dangNhapMap.find(maSvInput); // Check if MaSV exists
            if (sinhVienIt == dangNhapMap.end())
            {
                std::cout << "MaSV khong ton tai. Vui long thu lai.\n";
                continue; // Skip directly to the next attempt without asking for the password
            }

            // If MaSV exists, ask for the password
            std::cout << "Enter MatKhau: ";
            matKhauInput = readLineWithTrimming();

            if (sinhVienIt->second.matKhau == matKhauInput)
            {
                // Successful login
                sinhVienDaDangNhap = maSvInput; // Save the logged-in student ID
                std::cout << "Dang nhap thanh cong!\n";
                return true; // Login succeeded
            }
            else
            {
                std::cout << "Sai thong tin dang nhap. Vui long thu lai.\n";
            }
        }

        // If we reach here, login has failed 5 times
        std::cout << "Ban da thu qua 5 lan. Thoat chuong trinh.\n";
        return false; // Login failed
    }

    void giaoDien()
    {
        std::cout << "Chao mung den voi do an Thuc hanh mon Ky thuat lap trinh: Quan ly Sinh vien\n";

        // Attempt to login
        if (!dangNhap())
        {
            // Login failed after 5 attempts, exit the application
            return;
        }

        // If login succeeds, continue with application flow
        printAllMaps();
    }

    // Function to print all SinhVien
    void printAllSinhVien() const
    {
        std::cout << "SinhVien List:\n";
        for (const auto &[key, sv] : sinhVienMap)
        {
            std::cout << sv << "\n";
        }
    }

    // Function to print all DiemSo
    void printAllDiemSo() const
    {
        std::cout << "DiemSo List:\n";
        for (const auto &[key, ds] : diemSoMap)
        {
            std::cout << ds << "\n";
        }
    }

    // Function to print all DangNhap
    void printAllDangNhap() const
    {
        std::cout << "DangNhap List:\n";
        for (const auto &[key, dn] : dangNhapMap)
        {
            std::cout << dn << "\n";
        }
    }

    // Function to print all maps
    void printAllMaps() const
    {
        printAllSinhVien();
        printAllDiemSo();
        printAllDangNhap();
    }
};

int main()
{
    std::string sinhVienFile = "thongtinSV.txt";
    std::string diemSoFile = "diemso.txt";
    std::string dangNhapFile = "dangnhap.txt";

    // Instantiate DataManager and print all maps
    DataManager dataManager(sinhVienFile, diemSoFile, dangNhapFile);
    // hien thi
    dataManager.giaoDien();

    std::cout << "Thoat chuong trinh.\n";

    return 0;
}
