/*
Program 		: HUffman Code
Deskripsi		: Simulasi Kompresi HUffman Code
Author/NIM		: 	Icha Cahya Wulan	/181524013
					Naufal Rajabi		/181524025
Versi/Tanggal	: V.01/23_06_2019
IDE/Compiler	: DevC++ 5.11/TDM-GCC 4.9.2 64-bit Release
*/

#include "huffmanCode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define READ_MAP_CHAR 1
#define READ_STRING 2
#define READ_FILE 3
#define EXIT 4
#define MAX_DIRECTORY_LENGTH 1000

//list of prototype
int choose_read_type();
void run_huffman();
void run_huffman_read_char();
void run_huffman_read_string();
void run_huffman_read_file();
void exit_huffman();
void ask_for_exit();

int choose_read_type(){
	system("cls");
	printf("\tProgram Simulasi Huffman Coding\t\n\n");
	printf("Pilih salah satu metode baca karakter: \n");
	printf("\n1. Memasukkan karakter dan frekuensinya ");
	printf("\n2. Memasukkan kalimat ");
	printf("\n3. Membaca dari file");
	printf("\n4. Keluar Program \n");
	printf("\nPilihan: ");
	
	int answer;
	scanf("%d", &answer);
	return answer;
}

void ask_for_exit(){
	int answer;
	printf("\nApakah Anda ingin mensimulasikan kembali? 0: Tidak, 1: Ya\n");
	scanf("%d", &answer);

	switch (answer){
		case 1: {
			run_huffman();
			break;
		}
		case 0: {
			exit_huffman();
			break;
		}
		default: {
			printf("\nPilihan tidak valid! Pilih sesuai pilihan yang tersedia!\n");
			ask_for_exit();
			break;
		}
	}
}

void run_huffman_read_char(){
	system("cls");
	getchar();			//pembuang karakter enter

	int i, sum_of_character, frequency_map[MAX_ASCII_CHARACTER] = {0};

	//meminta inputan jumlah karakter yang akan dikompresi
	printf("Masukkan banyak karakter yang akan dikompresi : ");
	scanf("%d", &sum_of_character);
	printf("\n");
	
	//membaca karakter dan frekuensinya
	for(i = 0; i < sum_of_character; i++){
		char letter;
		printf("Karakter ke - %d\n", i+1);
		printf("Masukkan karakter\t: ");
		scanf(" %c", &letter);
		
		//handle ketika ada karakter yang telah ada diinput kembali
		//karakter akan ditambah frekuensinya
		if (frequency_map[letter]){
			int frequency;
			scanf("%d", &frequency);
			frequency_map[letter] += frequency;
		} else {
			printf("Masukkan frekuensi\t: ");
			scanf("%d", &frequency_map[letter]);
		}	
		printf("\n");
	}
	codewords_t code = { 0 };
    codewords_t table[MAX_ASCII_CHARACTER] = { 0 };
	huffman_node_t *root = execute_huffman(frequency_map, table, code);
}

void run_huffman_read_string(){
	system("cls");
	getchar();			//pembuang karakter enter

	//Asumsi maksimal kalimat berjumlah 10.000 karakter
	char sentence[10000];
	printf("Masukkan kalimat yang akan dikompresi: ");		
	scanf("%[^\n]", sentence);
	
	//kalimat diubah dari array menjadi array constant
	const char *string = (const char *)sentence;
	const char *to_encode_string = (const char *)sentence;
	
	//jumlah frekuensi ditaruh ke dalam array, dengan indeks
	//array sebagai representasi karakter yang dihitung
	int frequency_map[MAX_ASCII_CHARACTER] = { 0 };
	while (*string){
		frequency_map[(int)*string++]++;
	}
	
	//membuat huffman tree, mengembalikan root untuk proses decode nantinya
    codewords_t code = { 0 };
    codewords_t table[MAX_ASCII_CHARACTER] = { 0 };
	huffman_node_t *root = execute_huffman(frequency_map, table, code);
	
	//menampilkan hasil kompresi dari stirng yang telah diterjemahkan
	//hasil kompresi ditaruh ke dalam encodedString.txt
	printf("\nString setelah kompresi\n");
    while (*to_encode_string){
		int letter = (int) *to_encode_string++;
		code_print(table + letter);
		write_code_to_file(table + letter);
	}
    printf("\n");

	//menampilkan string setelah dikembalikan dari file teks encodedString.txt
	printf("\nString setelah dekompresi\n");
    decode_string(root);
    printf("\n");

	//membersihkan file encodedString.txt agar bisa dipakai ullang tanpa tercampur
	//dengan kode sebelumnya
	fclose(fopen("encodedString.txt", "w"));
}

void run_huffman_read_file(){
	system("cls");
	char new_line_remover;
	getchar();

	//asumsi file muat ditambung dalam 1000 karakter
	char letter, filename[MAX_DIRECTORY_LENGTH];
	printf("Masukkan nama file beserta directory (jika bukan satu folder) dan format filenya: ");
	scanf("%s", filename);

	char *file_pointer = (char *)filename;
	FILE *file_to_read;
	file_to_read = fopen(file_pointer, "r");

	if (!file_to_read){
		printf("File tidak ditemukan");
		ask_for_exit();
	}

	//jumlah frekuensi ditaruh ke dalam array, dengan indeks
	//array sebagai representasi karakter yang dihitung
	int frequency_map[MAX_ASCII_CHARACTER] = {0};
	while ((letter = fgetc(file_to_read)) != EOF){
		frequency_map[(int)letter++]++;
	}
	fclose(file_to_read);

	//membuat huffman tree, mengembalikan root untuk proses decode nantinya
	codewords_t code = { 0 };
    codewords_t table[MAX_ASCII_CHARACTER] = { 0 };
	huffman_node_t *root = execute_huffman(frequency_map, table, code);

	//menampilkan hasil kompresi dari stirng yang telah diterjemahkan
	//hasil kompresi ditaruh ke dalam encodedString.txt
	printf("\nString setelah dikompresi\n");
	file_to_read = fopen(filename, "r");
	while ((letter = fgetc(file_to_read)) != EOF){
		code_print(table + letter);
		write_code_to_file(table + letter);
	}
	printf("\n");
	fclose(file_to_read);

	//menampilkan string setelah dikembalikan dari file teks encodedString.txt
	printf("\nString setelah dekompresi\n");
    decode_string(root);
    printf("\n");

	//membersihkan file encodedString.txt agar bisa dipakai ullang tanpa tercampur
	//dengan kode sebelumnya
	fclose(fopen("encodedString.txt", "w"));
}

void exit_huffman(){
	system("cls");
	printf("\tMenutup program...\n");
	printf("\tProgram berakhir...\n");
	exit(EXIT_SUCCESS);
}

void run_huffman(){
	int read_t = choose_read_type();
	switch (read_t) {
		case READ_MAP_CHAR:{
			run_huffman_read_char();
			break;
		}
		case READ_STRING: {
			run_huffman_read_string();
			break;
		}
		case READ_FILE:{
			run_huffman_read_file();
			break;
		}
		case EXIT: {
			exit_huffman();
			break;
		}
		default: {
			printf("\nPilihan tidak valid! Pilih sesuai pilihan yang tersedia!\n");
			printf("Tekan apapun untuk melanjutkan");
			getchar();
			run_huffman;
		}
	}
	ask_for_exit();
}

int main (){
	run_huffman();
    return 0;
}
