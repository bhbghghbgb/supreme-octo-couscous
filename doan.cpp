#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>
#include <ios>
#include <optional>

class DiemSo
{
public:
    std::string maSv;
    float ktlt; // Ky Thuat Lap Trinh
    float mmt;  // Mang May Tinh
    float ctdl; // Cau Truc Du Lieu

    DiemSo() : maSv(""), ktlt(0.0f), mmt(0.0f), ctdl(0.0f) {}

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

    SinhVien() : maSv(""), gioiTinh(""), namSinh(0), hoTenSv(""), trungBinhHk(0.0f) {}

    SinhVien(std::string ma, std::string gioi, int nam, std::string hoTen)
        : maSv(ma), gioiTinh(gioi), namSinh(nam), hoTenSv(hoTen), trungBinhHk(0.0f) {}

    void tinhTrungBinhHK(const DiemSo &diem)
    {
        trungBinhHk = (diem.ktlt * 4 + diem.mmt * 3 + diem.ctdl * 3) / 10;
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

    DangNhap() : maSv(""), matKhau("") {}

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

inline bool sinhVienLaGiangVien(const std::string &maSv)
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

// Function to read a single line, ignoring leading newlines or spaces, with optional retries
std::string readLineWithTrimming(const std::string &prompt, bool allowEmpty = false)
{
    std::string input;
    do
    {
        std::cout << prompt;
        std::getline(std::cin, input);
        trim(input);
    } while (input.empty() && !allowEmpty); // Retry only if input is empty and retry is enabled
    return input;
}

std::optional<float> readFloat(const std::string &prompt, bool allowEmpty = false)
{
    while (true)
    {
        std::string input = readLineWithTrimming(prompt, allowEmpty);
        if (input.empty() && allowEmpty)
        {
            return std::nullopt; // Return empty optional if input is blank and allowEmpty is true
        }
        try
        {
            return std::stof(input); // Convert and return valid float
        }
        catch (const std::exception &)
        {
            std::cout << "Gia tri khong hop le. Vui long nhap lai (dang so thuc).\n";
        }
    }
}

std::optional<int> readInt(const std::string &prompt, bool allowEmpty = false)
{
    while (true)
    {
        std::string input = readLineWithTrimming(prompt, allowEmpty);
        if (input.empty() && allowEmpty)
        {
            return std::nullopt; // Return empty optional if input is blank and allowEmpty is true
        }
        try
        {
            return std::stoi(input); // Convert and return valid integer
        }
        catch (const std::exception &)
        {
            std::cout << "Gia tri khong hop le. Vui long nhap lai (dang so nguyen).\n";
        }
    }
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
            maSvInput = readLineWithTrimming("Enter MaSV: ");

            auto sinhVienIt = getDangNhap(maSvInput);
            if (!sinhVienIt) // Check if MaSV exists
            {
                std::cout << "MaSV khong ton tai. Vui long thu lai.\n";
                continue; // Skip directly to the next attempt without asking for the password
            }

            // If MaSV exists, ask for the password
            matKhauInput = readLineWithTrimming("Enter MatKhau: ");

            if (sinhVienIt->get().matKhau == matKhauInput)
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
        do
        {
            std::cout << "Ban da dang xuat\n";
            // Attempt to login
            if (!dangNhap())
            {
                // Login failed after 5 attempts, exit the application
                return;
            }

            // If login succeeds, continue with application flow
            if (!laGiangVien())
            {
                dangNhapAsSv();
                getSinhVien().hoTenSv += "1";
                continue;
            }
            dangNhapAsGv();
        } while (!askToExit());

        // printAllMaps();
    }

    void dangNhapAsSv()
    {
        std::cout << "Dang nhap voi tu cach Sinh vien\n"
                  << "Thong tin SV:\n"
                  << getSinhVien() << "\n"
                  << "Chi tiet diem so SV:\n"
                  << getDiemSo() << "\n"
                  << "Sinh vien se dang xuat.\n";
        return;
    }

    void dangNhapAsGv()
    {
        std::cout << "Dang nhap voi tu cach Giang vien\n";
        while (true)
        {
            int choice = displayMenu();

            // Handle menu choices
            switch (choice)
            {
            case 0:
                std::cout << "Giang vien se dang xuat.\n";
                return; // Return control to giaoDien
            case 1:
                themSinhVien();
                break;
            case 2:
                // capNhatThongTinSinhVien();
                break;
            case 3:
                // xoaSinhVien();
                break;
            case 4:
                // timKiemSinhVienTheoMaSV();
                break;
            case 5:
                // hienThiDanhSachSinhVienKhongCanhBao();
                break;
            case 6:
                // xuatDanhSachSinhVienRaFile();
                break;
            default:
                std::cout << "Lua chon khong hop le. Vui long thu lai.\n";
            }
        }
    }

    int displayMenu()
    {
        std::string choiceStr;
        int choice = -1; // Initialize with an invalid value

        while (true)
        {
            std::cout << "--------------------- MENU ---------------------\n"
                      << "1. Them sinh vien.\n"
                      << "2. Cap nhat thong tin sinh vien.\n"
                      << "3. Xoa sinh vien.\n"
                      << "4. Tim kiem va hien thi thong tin sinh vien theo MaSV.\n"
                      << "5. Hien thi danh sach sinh vien khong bi canh bao hoc tap (TrungbinhHK >= 4).\n"
                      << "6. Xuat danh sach sinh vien ra file \"thongtinSV_export.txt\".\n"
                      << "------------------------------------------------\n"
                      << "0. Dang xuat.\n";

            choiceStr = readLineWithTrimming("Nhap lua chon cua ban (0-6): ");
            try
            {
                choice = std::stoi(choiceStr); // Convert the input to an integer
                if (choice >= 0 && choice <= 6)
                {
                    break; // Valid choice, exit the loop
                }
                else
                {
                    std::cout << "Lua chon khong hop le. Vui long nhap lai (0-6).\n";
                }
            }
            catch (const std::exception &)
            {
                std::cout << "Lua chon khong hop le. Vui long nhap lai (0-6).\n";
            }
        }

        return choice;
    }

    void themSinhVien()
    {
        std::cout << "Nhap thong tin sinh vien moi:\n";

        // Gather SinhVien information
        std::string maSv = readLineWithTrimming("Nhap MaSV: ");

        // Check if MaSV already exists to avoid duplicates
        if (getSinhVien(maSv))
        {
            std::cout << "MaSV da ton tai. Vui long thu lai.\n";
            return; // Early exit if duplicate is detected
        }

        std::string hoTenSv = readLineWithTrimming("Nhap Ho Ten Sinh Vien: ");
        std::string gioiTinh = readLineWithTrimming("Nhap Gioi Tinh: ");
        int namSinh = readInt("Nhap Nam Sinh: ").value();

        // Gather DiemSo information
        float ktlt = readFloat("Nhap diem Ky Thuat Lap Trinh (KTLT): ").value();
        float mmt = readFloat("Nhap diem Mang May Tinh (MMT): ").value();
        float ctdl = readFloat("Nhap diem Cau Truc Du Lieu (CTDL): ").value();

        // Gather DangNhap information
        std::string matKhau = readLineWithTrimming("Nhap Mat Khau: ");

        // Create objects and calculate TrungBinhHK
        SinhVien sinhVien(maSv, gioiTinh, namSinh, hoTenSv);
        DiemSo diemSo(maSv, ktlt, mmt, ctdl);
        sinhVien.tinhTrungBinhHK(diemSo); // Calculate trungBinhHK using diemSo

        DangNhap dangNhap(maSv, matKhau);

        std::cout << sinhVien << "\n"
                  << diemSo << "\n";

        // Add objects to their respective maps
        sinhVienMap.emplace(maSv, std::move(sinhVien));
        diemSoMap.emplace(maSv, std::move(diemSo));
        dangNhapMap.emplace(maSv, std::move(dangNhap));

        std::cout << "Them sinh vien moi thanh cong!\n";
        printSvAsGv(maSv);
    }

    void capNhatThongTinSinhVien()
    {
        std::cout << "Cap nhat thong tin sinh vien:\n";

        // Prompt user for MaSV to identify the student
        std::string maSv = readLineWithTrimming("Nhap MaSV cua sinh vien can cap nhat: ");
        auto sv = getSinhVien(maSv);

        if (!sv) // Check if MaSV exists
        {
            std::cout << "MaSV khong ton tai.\n";
            return; // Exit if the student is not found
        }

        // Access the SinhVien object
        SinhVien &sinhVien = sv->get();

        // Update each field, showing the current value and allowing optional input
        std::optional<std::string> newHoTen = readLineWithTrimming("Nhap Ho Ten Sinh Vien (" + sinhVien.hoTenSv + "): ", true);
        if (newHoTen)
            sinhVien.hoTenSv = *newHoTen; // Update only if a new value is provided

        std::optional<std::string> newGioiTinh = readLineWithTrimming("Nhap Gioi Tinh (" + sinhVien.gioiTinh + "): ", true);
        if (newGioiTinh)
            sinhVien.gioiTinh = *newGioiTinh; // Update only if a new value is provided

        std::optional<int> newNamSinh = readInt("Nhap Nam Sinh (" + std::to_string(sinhVien.namSinh) + "): ", true);
        if (newNamSinh)
            sinhVien.namSinh = *newNamSinh; // Update only if a new value is provided

        // Check if DiemSo exists and handle updates
        auto optionalDiemSo = getDiemSo(maSv);
        DiemSo &diemSo = optionalDiemSo
                             ? optionalDiemSo->get()
                             : diemSoMap[maSv]; // Create and add a new DiemSo if not found

        // Ensure MaSV is set for the new DiemSo (in case of creation)
        diemSo.maSv = maSv;

        // Prompt for updates
        std::optional<float> newKtlt = readFloat("Nhap diem KTLT (" + std::to_string(diemSo.ktlt) + "): ", true);
        if (newKtlt)
            diemSo.ktlt = *newKtlt; // Update only if a new value is provided

        std::optional<float> newMmt = readFloat("Nhap diem MMT (" + std::to_string(diemSo.mmt) + "): ", true);
        if (newMmt)
            diemSo.mmt = *newMmt; // Update only if a new value is provided

        std::optional<float> newCtdl = readFloat("Nhap diem CTDL (" + std::to_string(diemSo.ctdl) + "): ", true);
        if (newCtdl)
            diemSo.ctdl = *newCtdl; // Update only if a new value is provided

        // Recalculate TrungBinhHK for SinhVien
        sinhVien.tinhTrungBinhHK(diemSo);

        auto optionalDangNhap = getDangNhap(maSv);

        DangNhap &dangNhap = optionalDangNhap
                                 ? optionalDangNhap->get()
                                 : dangNhapMap[maSv]; // Create and add a new DangNhap if not found

        // Ensure MaSV is set for the new DangNhap (in case of creation)
        dangNhap.maSv = maSv;

        // Prompt for password input
        std::string enteredPassword = readLineWithTrimming("Nhap mat khau cua sinh vien: ", true);

        // Check the password
        if (enteredPassword.empty())
        {
            std::cout << "Ban chua nhap mat khau. Khong thay doi.\n";
            return; // Exit without changing the password
        }

        // Update the password if provided
        std::string confirmPassword = readLineWithTrimming("Nhap lai mat khau de xac nhan: ", false);
        if (enteredPassword == confirmPassword)
        {
            dangNhap.matKhau = enteredPassword;
            std::cout << "Mat khau da duoc cap nhat thanh cong.\n";
        }
        else
        {
            std::cout << "Mat khau khong khop. Vui long thu lai.\n";
        }

        std::cout << "Cap nhat thong tin sinh vien thanh cong!\n";
        printSvAsGv(maSv);
    }

    bool askToExit()
    {
        std::string input = readLineWithTrimming("Do you want to exit? (Y/N): ");
        // Check if the input is "Y" or "y"
        return !input.empty() && std::toupper(input[0]) == 'Y';
        // User confirmed exit or
        // User does not want to exit
    }

    SinhVien &getSinhVien()
    {
        return getSinhVien(sinhVienDaDangNhap).value(); // throw if empty;
    }

    std::optional<std::reference_wrapper<SinhVien>> getSinhVien(const std::string &maSv)
    {
        auto it = sinhVienMap.find(maSv);
        if (it != sinhVienMap.end())
        {
            return it->second; // Wrap the reference in std::optional
        }
        return std::nullopt; // Return empty optional to signify "not found"
    }

    DiemSo &getDiemSo()
    {
        return getDiemSo(sinhVienDaDangNhap).value(); // throw if empty
    }

    std::optional<std::reference_wrapper<DiemSo>> getDiemSo(const std::string &maSv)
    {
        auto it = diemSoMap.find(maSv);
        if (it != diemSoMap.end())
        {
            return it->second; // Wrap the reference in std::optional
        }
        return std::nullopt; // Return empty optional to signify "not found"
    }

    DangNhap &getDangNhap()
    {
        return getDangNhap(sinhVienDaDangNhap).value(); // throw if empty;
    }

    std::optional<std::reference_wrapper<DangNhap>> getDangNhap(const std::string &maSv)
    {
        auto it = dangNhapMap.find(maSv);
        if (it != dangNhapMap.end())
        {
            return it->second; // Wrap the reference in std::optional
        }
        return std::nullopt; // Return empty optional to signify "not found"
    }

    inline bool laGiangVien() const
    {
        return sinhVienLaGiangVien(sinhVienDaDangNhap);
    }

    void printSvAsGv(const std::string &maSv) const
    {
        std::cout << sinhVienMap.find(maSv)->second << "\n"
                  << diemSoMap.find(maSv)->second << "\n"
                  << dangNhapMap.find(maSv)->second << "\n";
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
    dataManager.printAllMaps();
    // hien thi
    dataManager.giaoDien();

    std::cout << "Thoat chuong trinh.\n";

    return 0;
}
