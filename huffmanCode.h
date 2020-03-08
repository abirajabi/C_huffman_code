/*
Header      	: huffmanCode.h
Deskripsi		: Simulasi Kompresi HUffman Code
Author/NIM		: 	Icha Cahya Wulan	/181524013
					Naufal Rajabi		/181524025
Versi/Tanggal	: V.01/23_06_2019
IDE/Compiler	: DevC++ 5.11/TDM-GCC 4.9.2 64-bit Release
*/

#ifndef huffman_coding_tree_h
#define huffman_coding_tree_h
#include <stdbool.h>
#include <stdint.h>

#define BLANK_CHARACTER '\0'
#define MAX_ASCII_CHARACTER 128

/* Struktur huffman node */
typedef struct huffman_node{
    char letter;
    int frequency;
    struct huffman_node *left;
    struct huffman_node *right;
    struct huffman_node *next;
} huffman_node_t;

/* Antrian huffman node */
typedef struct huffman_queue{
    huffman_node_t *front;
    int size;
} priorty_queue_t;

/*  Struktur untuk menyimpan binary code dari huffman node */
typedef struct codewords{
    size_t code_length;
    uint32_t bit_code;
} codewords_t;

/* 
    Mencetak tree
 */
void printHierarchy(huffman_node_t *current, int level);

/*
    Inisiasi antrian huffman node
 */
void init_prior_queue(priorty_queue_t *queue);

/*
    Memeriksa apakah ada node dalam antrian atau tidak
 */
int is_prior_queue_empty(priorty_queue_t queue);

/*
    Memasukkan node ke antrian sesuai dengan urutan prioritas
    Node dengan frekuensi lebih kecil didahulukan
    Sehingga muncul antrian dengan urutan ascending
 */
void enqueue_prior(priorty_queue_t *queue, huffman_node_t *new_node);

/*
    Mengeluarkan node dari antrian
 */
huffman_node_t* dequeue_prior(priorty_queue_t *queue);

/*
    Membuat huffman node
 */
huffman_node_t* make_huffman_node(char letter, int frequency, huffman_node_t *left, huffman_node_t *right);

/*
    Menghitung besar antrian
 */
int prior_queue_size(priorty_queue_t queue);

/*
    Mencetak antrian
 */
void print_prior_queue(priorty_queue_t queue);

/*
    Memeriksa apakah node merupakan daun atau bukan
 */
bool is_leaf(huffman_node_t *node);

/*
    Membuat huffman tree dari kumpulan huffman node (forest)
 */
void make_huffman_tree(priorty_queue_t *queue);

/*
    Menjalankan pembentukan huffman tree dari map frequency yang ada
    dilanjut inisiasi forest sampai dengan print huffman tree
 */
huffman_node_t* execute_huffman(int frequency_map[MAX_ASCII_CHARACTER], codewords_t *table, codewords_t code);

/*
    Komputasi bit code
    Diambil dari: //https://stackoverflow.com/questions/47421732/how-can-i-generate-a-binary-code-table-of-a-huffman-tree
 */
void compute_code_table(huffman_node_t *node, codewords_t *table, codewords_t code);

/*
    Mencetak bit code ke layar
    Diambil dari: //https://stackoverflow.com/questions/47421732/how-can-i-generate-a-binary-code-table-of-a-huffman-tree
 */
void code_print(codewords_t *code);

/*
    Menyimpan string yang telah diterjemahkan menjadi kode biner ke dalam encodedString.txt
 */
void write_code_to_file(codewords_t *code);

/*
    Menerjemahkan string yang telah dikompresi ke string awal
 */
void decode_string(huffman_node_t *root);

void print_code_table(codewords_t *table);

#endif
