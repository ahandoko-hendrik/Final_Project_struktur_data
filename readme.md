# Dokumentasi Penggunaan Git (Kolaborasi)

Panduan singkat untuk bekerja secara kolaboratif menggunakan Git (GitHub/GitLab).

**Prasyarat**
- Git terpasang di mesin pengembang.
- Akun dan repository remote (GitHub/GitLab/Bitbucket) tersedia.

**Konfigurasi awal**
```bash
# set nama & email (sekali saja per mesin)
git config --global user.name "Nama Anda"
git config --global user.email "email@domain.com"
```

**Alur Kerja Rekomendasi (feature-branch + PR)**
- **Clone**: ambil repo lokal
```bash
git clone <repo-url>
cd <repo-folder>
```
- **Buat branch fitur**: tiap fitur/bugfix di branch terpisah
```bash
git checkout -b feature/nama-fitur
```
- **Kerja lokal & commit**
```bash
# cek status, tambahkan file, commit
git status
git add .
git commit -m "feat: tambah fitur X — penjelasan singkat"
```
- **Sinkronisasi branch utama** sebelum push/PR
```bash
# ambil pembaruan remote
git fetch origin
# update main lokal
git checkout main
git pull origin main
# kembali ke branch fitur dan rebase/merge
git checkout feature/nama-fitur
git rebase origin/main   # atau: git merge origin/main
```
- **Push dan buka Pull Request (PR)**
```bash
git push -u origin feature/nama-fitur
# lalu buka PR lewat GitHub/GitLab: pilih target branch (main/develop)
```

**Menangani konflik saat rebase/merge**
1. Git akan memberi tahu file yang konflik.
2. Buka file yang konflik, selesaikan perubahan secara manual.
3. Setelah memperbaiki, tandai sebagai terselesaikan dan lanjutkan:
```bash
git add <file-yang-diselesaikan>
git rebase --continue   # jika rebase
# atau
git commit              # jika merge dan perlu commit
```
4. Jika ingin membatalkan rebase/merge yang bermasalah:
```bash
git rebase --abort
git merge --abort
```

**Perintah berguna cepat**
- Lihat log singkat: `git log --oneline --graph --all`
- Cek perubahan: `git diff`
- Batalkan perubahan file lokal: `git restore <file>`
- Simpan sementara (stash): `git stash`, `git stash pop`

**Best practices**
- Buat branch kecil dan fokus (satu fitur / satu bugfix).
- Commit sering dengan pesan jelas: tipe: ringkasan — penjelasan singkat.
- Tarik (`pull`/`fetch` + `rebase`) perubahan main sebelum merge/PR.
- Gunakan PR untuk review kode dan diskusi; sertakan deskripsi & langkah pengujian.
- Jangan push kredensial atau file sensitif; gunakan `.gitignore`.

**Contoh pesan commit**
- `feat: tambah halaman login`
- `fix: perbaiki input validation pada form` 
- `docs: perbarui README`

Jika ingin, saya bisa menambahkan template PR, file `.gitignore` contoh, atau template pesan commit.

