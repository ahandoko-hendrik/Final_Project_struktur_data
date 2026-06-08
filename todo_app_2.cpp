#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <sstream>

using namespace std;

// ============================================================
//  STRUKTUR DATA
// ============================================================
struct Tugas {
    int id;
    string judul;
    string deskripsi;
    string kategori;
    string prioritas;
    string deadline;
    bool selesai;
    string tanggalDibuat;
};

// ============================================================
//  UTILITAS
// ============================================================
string intToStr(int n) {
    ostringstream ss;
    ss << n;
    return ss.str();
}

int strToInt(const string& s) {
    istringstream ss(s);
    int n = 0;
    ss >> n;
    return n;
}

string getTanggalHariIni() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20];
    sprintf(buf, "%04d-%02d-%02d",
            1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buf);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(10000, '\n');
    cin.get();
}

void cetakGaris(char c, int panjang) {
    for (int i = 0; i < panjang; i++) cout << c;
    cout << endl;
}

void cetakGaris() { cetakGaris('=', 60); }

void cetakJudul(const string& judul) {
    cetakGaris('=', 60);
    int spasi = (60 - (int)judul.size()) / 2;
    for (int i = 0; i < spasi; i++) cout << ' ';
    cout << judul << endl;
    cetakGaris('=', 60);
}

string warnaPrioritas(const string& p) {
    if (p == "Tinggi") return "[!!!]";
    if (p == "Sedang") return "[ ! ]";
    return "[   ]";
}

// ============================================================
//  SIMPAN & MUAT FILE
// ============================================================
const char* NAMA_FILE = "data_tugas.txt";

void simpanKeFile(const vector<Tugas>& daftar) {
    ofstream file(NAMA_FILE);
    if (!file.is_open()) {
        cout << "Gagal menyimpan file!\n";
        return;
    }
    for (int i = 0; i < (int)daftar.size(); i++) {
        const Tugas& t = daftar[i];
        file << t.id          << "|"
             << t.judul       << "|"
             << t.deskripsi   << "|"
             << t.kategori    << "|"
             << t.prioritas   << "|"
             << t.deadline    << "|"
             << (t.selesai ? 1 : 0) << "|"
             << t.tanggalDibuat << "\n";
    }
    file.close();
}

vector<Tugas> muatDariFile() {
    vector<Tugas> daftar;
    ifstream file(NAMA_FILE);
    if (!file.is_open()) return daftar;

    string baris;
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        Tugas t;
        vector<string> token;
        string tmp;
        for (int i = 0; i < (int)baris.size(); i++) {
            if (baris[i] == '|') {
                token.push_back(tmp);
                tmp.clear();
            } else {
                tmp += baris[i];
            }
        }
        token.push_back(tmp);
        if ((int)token.size() < 8) continue;
        t.id            = strToInt(token[0]);
        t.judul         = token[1];
        t.deskripsi     = token[2];
        t.kategori      = token[3];
        t.prioritas     = token[4];
        t.deadline      = token[5];
        t.selesai       = (token[6] == "1");
        t.tanggalDibuat = token[7];
        daftar.push_back(t);
    }
    file.close();
    return daftar;
}

// ============================================================
//  ID OTOMATIS
// ============================================================
int idBerikutnya(const vector<Tugas>& daftar) {
    int maxId = 0;
    for (int i = 0; i < (int)daftar.size(); i++)
        if (daftar[i].id > maxId) maxId = daftar[i].id;
    return maxId + 1;
}

// ============================================================
//  TAMPILKAN DAFTAR TUGAS
// ============================================================
void tampilkanDaftar(const vector<Tugas>& daftar, const string& filter) {
    cetakJudul("DAFTAR TUGAS");

    vector<Tugas> tampil;
    for (int i = 0; i < (int)daftar.size(); i++) {
        const Tugas& t = daftar[i];
        if (filter == "semua")                    tampil.push_back(t);
        else if (filter == "aktif"   && !t.selesai) tampil.push_back(t);
        else if (filter == "selesai" &&  t.selesai) tampil.push_back(t);
    }

    if (tampil.empty()) {
        cout << "  (Tidak ada tugas untuk ditampilkan)\n";
        cetakGaris('=', 60);
        return;
    }

    cout << left
         << setw(4)  << "ID"
         << setw(25) << "Judul"
         << setw(8)  << "Prior."
         << setw(10) << "Kategori"
         << setw(12) << "Deadline"
         << "Status" << "\n";
    cetakGaris('-', 60);

    for (int i = 0; i < (int)tampil.size(); i++) {
        const Tugas& t = tampil[i];
        string judulPendek = t.judul.size() > 23 ? t.judul.substr(0, 22) + "." : t.judul;
        cout << left
             << setw(4)  << t.id
             << setw(25) << judulPendek
             << setw(8)  << t.prioritas
             << setw(10) << t.kategori
             << setw(12) << (t.deadline.empty() ? "-" : t.deadline)
             << (t.selesai ? "[SELESAI]" : "[AKTIF]") << "\n";
    }
    cetakGaris('=', 60);
    cout << "Total: " << tampil.size() << " tugas\n";
}

void tampilkanDaftar(const vector<Tugas>& daftar) {
    tampilkanDaftar(daftar, "semua");
}

// ============================================================
//  TAMBAH TUGAS
// ============================================================
void tambahTugas(vector<Tugas>& daftar) {
    cetakJudul("TAMBAH TUGAS BARU");
    Tugas t;
    t.id = idBerikutnya(daftar);
    t.selesai = false;
    t.tanggalDibuat = getTanggalHariIni();

    cin.ignore(10000, '\n');

    cout << "Judul tugas    : ";
    getline(cin, t.judul);
    if (t.judul.empty()) { cout << "Judul tidak boleh kosong!\n"; return; }

    cout << "Deskripsi      : ";
    getline(cin, t.deskripsi);

    cout << "Kategori\n  1. Kerja\n  2. Belajar\n  3. Pribadi\n  4. Lainnya\nPilih (1-4): ";
    int pil; cin >> pil;
    string katArr[] = {"Kerja","Belajar","Pribadi","Lainnya"};
    t.kategori = (pil >= 1 && pil <= 4) ? katArr[pil-1] : "Lainnya";

    cout << "Prioritas\n  1. Tinggi\n  2. Sedang\n  3. Rendah\nPilih (1-3): ";
    cin >> pil;
    string priArr[] = {"Tinggi","Sedang","Rendah"};
    t.prioritas = (pil >= 1 && pil <= 3) ? priArr[pil-1] : "Sedang";

    cin.ignore(10000, '\n');
    cout << "Deadline (DD-MM-YYYY, kosongkan jika tidak ada): ";
    getline(cin, t.deadline);

    daftar.push_back(t);
    simpanKeFile(daftar);
    cout << "\nTugas berhasil ditambahkan! (ID: " << t.id << ")\n";
}

// ============================================================
//  UBAH STATUS
// ============================================================
void ubahStatusTugas(vector<Tugas>& daftar) {
    cetakJudul("UBAH STATUS TUGAS");
    tampilkanDaftar(daftar);

    cout << "Masukkan ID tugas: ";
    int id; cin >> id;

    for (int i = 0; i < (int)daftar.size(); i++) {
        if (daftar[i].id == id) {
            daftar[i].selesai = !daftar[i].selesai;
            simpanKeFile(daftar);
            cout << "Status tugas \"" << daftar[i].judul << "\" diubah menjadi "
                 << (daftar[i].selesai ? "[SELESAI]" : "[AKTIF]") << "\n";
            return;
        }
    }
    cout << "ID tidak ditemukan!\n";
}

// ============================================================
//  EDIT TUGAS
// ============================================================
void editTugas(vector<Tugas>& daftar) {
    cetakJudul("EDIT TUGAS");
    tampilkanDaftar(daftar);

    cout << "Masukkan ID tugas yang akan diedit: ";
    int id; cin >> id;

    for (int i = 0; i < (int)daftar.size(); i++) {
        if (daftar[i].id == id) {
            Tugas& t = daftar[i];
            cin.ignore(10000, '\n');

            cout << "Judul baru [" << t.judul << "]: ";
            string tmp; getline(cin, tmp);
            if (!tmp.empty()) t.judul = tmp;

            cout << "Deskripsi baru [" << t.deskripsi << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) t.deskripsi = tmp;

            cout << "Deadline baru [" << (t.deadline.empty() ? "-" : t.deadline) << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) t.deadline = tmp;

            cout << "Prioritas baru (Tinggi/Sedang/Rendah) [" << t.prioritas << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) t.prioritas = tmp;

            simpanKeFile(daftar);
            cout << "Tugas berhasil diperbarui!\n";
            return;
        }
    }
    cout << "ID tidak ditemukan!\n";
}

// ============================================================
//  HAPUS TUGAS
// ============================================================
void hapusTugas(vector<Tugas>& daftar) {
    cetakJudul("HAPUS TUGAS");
    tampilkanDaftar(daftar);

    cout << "Masukkan ID tugas yang akan dihapus: ";
    int id; cin >> id;

    for (int i = 0; i < (int)daftar.size(); i++) {
        if (daftar[i].id == id) {
            cout << "Yakin hapus \"" << daftar[i].judul << "\"? (y/n): ";
            char konfirm; cin >> konfirm;
            if (konfirm == 'y' || konfirm == 'Y') {
                daftar.erase(daftar.begin() + i);
                simpanKeFile(daftar);
                cout << "Tugas berhasil dihapus!\n";
            } else {
                cout << "Penghapusan dibatalkan.\n";
            }
            return;
        }
    }
    cout << "ID tidak ditemukan!\n";
}

// ============================================================
//  CARI TUGAS
// ============================================================
void strToLower(string& s) {
    for (int i = 0; i < (int)s.size(); i++)
        s[i] = tolower(s[i]);
}

void cariTugas(const vector<Tugas>& daftar) {
    cetakJudul("CARI TUGAS");
    cin.ignore(10000, '\n');
    cout << "Masukkan kata kunci: ";
    string kw; getline(cin, kw);
    string kwLower = kw;
    strToLower(kwLower);

    vector<Tugas> hasil;
    for (int i = 0; i < (int)daftar.size(); i++) {
        string j = daftar[i].judul;
        string d = daftar[i].deskripsi;
        strToLower(j); strToLower(d);
        if (j.find(kwLower) != string::npos || d.find(kwLower) != string::npos)
            hasil.push_back(daftar[i]);
    }

    if (hasil.empty()) {
        cout << "Tidak ada tugas yang cocok dengan \"" << kw << "\"\n";
    } else {
        cout << "Ditemukan " << hasil.size() << " tugas:\n";
        tampilkanDaftar(hasil);
    }
}

// ============================================================
//  STATISTIK
// ============================================================
void tampilkanStatistik(const vector<Tugas>& daftar) {
    cetakJudul("STATISTIK TUGAS");
    int total = (int)daftar.size();
    int selesai = 0, aktif = 0, tinggi = 0, sedang = 0, rendah = 0;
    for (int i = 0; i < total; i++) {
        if (daftar[i].selesai) selesai++; else aktif++;
        if (daftar[i].prioritas == "Tinggi") tinggi++;
        else if (daftar[i].prioritas == "Sedang") sedang++;
        else rendah++;
    }
    cout << "  Total Tugas     : " << total   << "\n";
    cout << "  Tugas Aktif     : " << aktif   << "\n";
    cout << "  Tugas Selesai   : " << selesai << "\n";
    if (total > 0)
        cout << "  Progress        : " << (selesai * 100 / total) << "%\n";
    cetakGaris('-', 60);
    cout << "  Prioritas Tinggi: " << tinggi << "\n";
    cout << "  Prioritas Sedang: " << sedang << "\n";
    cout << "  Prioritas Rendah: " << rendah << "\n";
    cetakGaris('=', 60);
}

// ============================================================
//  FILTER
// ============================================================
void menuFilter(const vector<Tugas>& daftar) {
    cetakJudul("FILTER TUGAS");
    cout << "1. Tampilkan semua tugas\n";
    cout << "2. Hanya tugas aktif\n";
    cout << "3. Hanya tugas selesai\n";
    cout << "Pilihan: ";
    int pil; cin >> pil;
    if      (pil == 1) tampilkanDaftar(daftar, "semua");
    else if (pil == 2) tampilkanDaftar(daftar, "aktif");
    else if (pil == 3) tampilkanDaftar(daftar, "selesai");
    else cout << "Pilihan tidak valid!\n";
}

// ============================================================
//  MAIN MENU
// ============================================================
void tampilkanMenu() {
    cetakJudul("APLIKASI MANAJEMEN TUGAS");
    cout << "  1. Lihat Semua Tugas\n";
    cout << "  2. Tambah Tugas Baru\n";
    cout << "  3. Edit Tugas\n";
    cout << "  4. Tandai Selesai / Aktifkan Kembali\n";
    cout << "  5. Hapus Tugas\n";
    cout << "  6. Cari Tugas\n";
    cout << "  7. Filter Tugas\n";
    cout << "  8. Statistik\n";
    cout << "  0. Keluar\n";
    cetakGaris('=', 60);
    cout << "Pilihan Anda: ";
}

int main() {
    vector<Tugas> daftar = muatDariFile();

    cout << "\n";
    cetakJudul("SELAMAT DATANG");
    cout << "  Data dimuat: " << daftar.size() << " tugas\n";
    cout << "  Tanggal    : " << getTanggalHariIni() << "\n";
    cetakGaris('=', 60);
    pauseScreen();

    int pilihan;
    do {
        clearScreen();
        tampilkanMenu();
        cin >> pilihan;

        clearScreen();
        switch (pilihan) {
            case 1: tampilkanDaftar(daftar);      break;
            case 2: tambahTugas(daftar);           break;
            case 3: editTugas(daftar);             break;
            case 4: ubahStatusTugas(daftar);       break;
            case 5: hapusTugas(daftar);            break;
            case 6: cariTugas(daftar);             break;
            case 7: menuFilter(daftar);            break;
            case 8: tampilkanStatistik(daftar);    break;
            case 0:
                cetakJudul("SAMPAI JUMPA!");
                cout << "  Data tersimpan otomatis.\n";
                cetakGaris('=', 60);
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }

        if (pilihan != 0) pauseScreen();

    } while (pilihan != 0);

    return 0;
}
