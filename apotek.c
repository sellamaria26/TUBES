#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Obat {
    char kode[5];
    char nama[30];
    int harga; 
    int stok;
    int terjual;
};

struct Riwayat {
    char namaObat[30];
    int jumlah;
    int subtotal;
};

void tampilDaftarObat(struct Obat daftar[], int n);
int cariObat(struct Obat daftar[], int n, char kode[]);
int hitungSubtotal(int harga, int jumlah);
void tampilNotaStruk(char namaPembeli[], char metode[], struct Riwayat rwt[], int jumlahRwt, int total);
void sortingNota(struct Riwayat dataAsli[], struct Riwayat dataUrut[], int jumlah);
void laporanTerlarisDanStok(struct Obat daftar[], int n);

int main() {

    struct Obat daftar[5] = {
        {"A01", "Paracetamol", 5000, 20, 0},
        {"A02", "Amoxicillin", 8000, 15, 0},
        {"A03", "Vitamin C",   7000, 25, 0},
        {"A04", "OBH",        10000, 10, 0},
        {"A05", "Salep Luka", 12000,  8, 0}
    };
    int nObat = 5;

    struct Riwayat riwayat[50];
    int riwayatJumlahPerObat[5];

    for (int i = 0; i < nObat; i++) riwayatJumlahPerObat[i] = 0;

    int menu;

    do {
        printf("\n================================================\n");
        printf("         SELAMAT DATANG DI APOTEK SELMAR\n");
        printf("================================================\n");
        printf("                   MENU UTAMA\n");
        printf("------------------------------------------------\n");
        printf("| 1 | Tampilkan Daftar Obat                    |\n");
        printf("| 2 | Buat Transaksi Baru                      |\n");
        printf("| 3 | Laporan Penjualan                        |\n");
        printf("| 4 | Keluar                                   |\n");
        printf("------------------------------------------------\n");
        printf("Pilih menu (1-4): ");
        if (scanf("%d", &menu) != 1) {
            printf("Input tidak valid!\n");
            return 0;
        }

        if (menu == 1) {
            tampilDaftarObat(daftar, nObat);
            system("pause");
        }

        else if (menu == 2) {
            int jumlahJenis;
            char namaPembeli[50];
            int total = 0;
            int jumlahRiwayat = 0;

            for (int i = 0; i < nObat; i++) riwayatJumlahPerObat[i] = 0;

            tampilDaftarObat(daftar, nObat);

            printf("\nMasukkan nama pembeli: ");
            scanf(" %[^\n]s", namaPembeli);
            printf("Berapa jenis obat yang ingin dibeli? ");
            if (scanf("%d", &jumlahJenis) != 1 || jumlahJenis <= 0) {
                printf("Jumlah tidak valid.\n");
                system("pause");
                continue;
            }

            for (int i = 0; i < jumlahJenis; i++) {
                char kode[10];
                int idx = -1;
                int qty = 0;

                do {
                    printf("\nMasukkan kode obat ke-%d: ", i+1);
                    scanf(" %s", kode);

                    idx = cariObat(daftar, nObat, kode);
                    if (idx == -1) printf("Kode tidak ditemukan.\n");

                } while (idx == -1);

                printf("Nama obat : %s\n", daftar[idx].nama);
                printf("Harga     : Rp%d\n", daftar[idx].harga);
                printf("Stok      : %d\n", daftar[idx].stok);

                do {
                    printf("Jumlah beli: ");
                    if (scanf("%d", &qty) != 1 || qty <= 0) {
                        printf("Input salah.\n");
                        qty = -1;
                        continue;
                    }
                    if (qty > daftar[idx].stok) {
                        printf("Stok tidak cukup!\n");
                        qty = -1;
                    }
                } while (qty <= 0);

                strcpy(riwayat[jumlahRiwayat].namaObat, daftar[idx].nama);
                riwayat[jumlahRiwayat].jumlah = qty;
                riwayat[jumlahRiwayat].subtotal = hitungSubtotal(daftar[idx].harga, qty);

                jumlahRiwayat++;
                total += riwayat[jumlahRiwayat-1].subtotal;

                daftar[idx].stok -= qty;
                riwayatJumlahPerObat[idx] += qty;
            }

            system("pause");
            printf("\n=== DETAIL PEMBELIAN ===\n");
            for (int i = 0; i < jumlahRiwayat; i++) {
                printf("%d. %s x%d = Rp%d\n",
                    i+1,
                    riwayat[i].namaObat,
                    riwayat[i].jumlah,
                    riwayat[i].subtotal
                );
            }

            printf("TOTAL BAYAR = Rp%d\n", total);

            char konf;
            printf("\nKonfirmasi pembelian? (y/n): ");
            scanf(" %c", &konf);

            if (konf == 'y' || konf == 'Y') {
                int pilihMetode;
                char metode[20];

                printf("\nPilih metode pembayaran (1/2):\n");
                printf("1. Tunai\n2. Non-tunai\nMasukkan pilihan: ");
                scanf("%d", &pilihMetode);

                if (pilihMetode == 1) {
                    strcpy(metode, "Tunai");
                } else {
                    strcpy(metode, "Non-tunai");
                }

                for (int i = 0; i < nObat; i++)
                    daftar[i].terjual += riwayatJumlahPerObat[i];

                struct Riwayat temp[50];
                sortingNota(riwayat, temp, jumlahRiwayat);

                tampilNotaStruk(namaPembeli, metode, temp, jumlahRiwayat, total);
                system("pause");
            }
            else {
                printf("Transaksi dibatalkan.\n");

                for (int i = 0; i < jumlahRiwayat; i++) {
                    for (int j = 0; j < nObat; j++) {
                        if (strcmp(riwayat[i].namaObat, daftar[j].nama) == 0) {
                            daftar[j].stok += riwayat[i].jumlah;
                        }
                    }
                }
                system("pause");
            }
        }

        else if (menu == 3) {
            laporanTerlarisDanStok(daftar, nObat);
            system("pause");
        }

        else if (menu == 4) printf("Keluar...\n");
        else {
            printf("Pilihan tidak dikenal.\n");
            system("pause");
        }

    } while (menu != 4);

    return 0;
}

void tampilDaftarObat(struct Obat daftar[], int n) {
    printf("\n============================================================\n");
    printf("                       DAFTAR OBAT\n");
    printf("============================================================\n");
    printf("| Kode | Nama Obat            |   Harga   | Stok | Terjual |\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("| %-4s | %-20s | Rp%-7d | %4d | %7d |\n",
            daftar[i].kode, daftar[i].nama, daftar[i].harga,
            daftar[i].stok, daftar[i].terjual);
    }

    printf("============================================================\n");
}

int cariObat(struct Obat daftar[], int n, char kode[]) {
    for (int i = 0; i < n; i++)
        if (strcmp(daftar[i].kode, kode) == 0) return i;
    return -1;
}

int hitungSubtotal(int harga, int jumlah) {
    return harga * jumlah;
}

void tampilNotaStruk(char namaPembeli[], char metode[], struct Riwayat rwt[], int jumlahRwt, int total) {
    printf("\n======= APOTEK SELMAR ========\n");
    printf("  JL. ASIA AFRIKA NO. 26 BDG  \n");
    printf("-----------------------------\n");
    printf("Pembeli : %s\n", namaPembeli);
    printf("Bayar   : %s\n", metode);
    printf("-----------------------------\n");

    for (int i = 0; i < jumlahRwt; i++) {
    int hargaPerItem;

    if (rwt[i].jumlah > 0) {
        hargaPerItem = rwt[i].subtotal / rwt[i].jumlah;
    } else {
        hargaPerItem = 0;
    }

    printf("%-15s %2d x Rp%d\n",
           rwt[i].namaObat, rwt[i].jumlah, hargaPerItem);
}
    printf("-----------------------------\n");
    printf("TOTAL           Rp%d\n", total);
    printf("-----------------------------\n");
    printf("Terima kasih :) \n");
    printf("=============================\n");
}

void sortingNota(struct Riwayat dataAsli[], struct Riwayat dataUrut[], int jumlah) {
    for (int i = 0; i < jumlah; i++)
        dataUrut[i] = dataAsli[i];

    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (strcmp(dataUrut[j].namaObat, dataUrut[j+1].namaObat) > 0) {
                struct Riwayat tmp = dataUrut[j];
                dataUrut[j] = dataUrut[j+1];
                dataUrut[j+1] = tmp;
            }
        }
    }
}

void laporanTerlarisDanStok(struct Obat daftar[], int n) {

    int totalTerjualTransaksi = 0;
    for (int i = 0; i < n; i++)
        totalTerjualTransaksi += daftar[i].terjual;

    if (totalTerjualTransaksi == 0) {
        printf("\nBelum ada penjualan.\n");
        return;
    }

    int idxMaks = 0;
    for (int i = 1; i < n; i++) {
        if (daftar[i].terjual > daftar[idxMaks].terjual)
            idxMaks = i;
        }

    struct Temp {
        int idx;
        int terjual;
    } temp[10];

    for (int i = 0; i < n; i++) {
        temp[i].idx = i;
        temp[i].terjual = daftar[i].terjual;

    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (temp[j].terjual > temp[i].terjual) {
                struct Temp t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }

    printf("\n===============================================================================\n");
    printf("                       LAPORAN PENJUALAN & STOK\n");
    printf("===============================================================================\n");
    printf("| %-3s | %-25s | %-10s | %-10s | %-15s |\n",
           "No", "Nama Obat", "Terjual", "Stok", "Keterangan");
    printf("-------------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
    int id = temp[i].idx;
    char keterangan[20];

    if (id == idxMaks) {
        strcpy(keterangan, "OBAT TERLARIS");
    } else {
        strcpy(keterangan, "-");
    }

    printf("| %-3d | %-25s | %-10d | %-10d | %-15s |\n",
           i + 1,
           daftar[id].nama, temp[i].terjual, daftar[id].stok, keterangan
    );
}
   
printf("===============================================================================\n");

} 
