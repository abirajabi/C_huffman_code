/*
Body Function	: huffmanCode.c
Deskripsi		: Body Function 
Author/NIM		: 	Icha Cahya Wulan	/181524013
					Naufal Rajabi		/181524025
Versi/Tanggal	: V.01/23_06_2019
IDE/Compiler	: DevC++ 5.11/TDM-GCC 4.9.2 64-bit Release
*/

#include "huffmanCode.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void init_prior_queue(priorty_queue_t *queue){
    queue->front = NULL;
    queue->size = 0;
}

int is_prior_queue_empty(priorty_queue_t queue){
    return queue.size == 0;
}

huffman_node_t* make_huffman_node(char letter, int frequency, huffman_node_t *left, huffman_node_t *right){
    huffman_node_t *new_node = (huffman_node_t *)malloc(sizeof (huffman_node_t));
    
    assert(new_node != NULL);

    //fill new node with info
    new_node->letter = letter;
    new_node->frequency = frequency;
    new_node->left = left;
    new_node->right = right; 
    new_node->next = NULL;

    return new_node;
}

void enqueue_prior(priorty_queue_t *queue, huffman_node_t *new_node){
    if (is_prior_queue_empty(*queue)){
        queue->front = new_node;
        new_node->next = NULL;
    } else {
        if (queue->front->frequency >= new_node->frequency){
            new_node->next = queue->front;
            queue->front = new_node;
        } else {
            huffman_node_t *help_pointer;
            help_pointer = queue->front;

            while (help_pointer->next != NULL && help_pointer->next->frequency < new_node->frequency){
                help_pointer = help_pointer->next;
            }

            if (help_pointer->next == NULL){
                new_node->next = NULL;
                help_pointer->next = new_node;
            } else {
                new_node->next = help_pointer->next;
                help_pointer->next = new_node;
            }
        }
    }
    queue->size++;
}

huffman_node_t* dequeue_prior(priorty_queue_t *queue){
    if (is_prior_queue_empty(*queue)){
        printf("Antrian kosong");
        exit(EXIT_FAILURE);
    }

    huffman_node_t *delete_node = queue->front;
    queue->front = queue->front->next;
    queue->size--;
    delete_node->next = NULL;
    return delete_node;
}

int prior_queue_size(priorty_queue_t queue){
    return queue.size;
}

void print_prior_queue(priorty_queue_t queue){
    printf("[");
    huffman_node_t *current = queue.front;
    while (current != NULL){
        printf("%c%d", current->letter, current->frequency);
        if (current->next != NULL) 
            printf(", ");
        current = current->next;
    }
    printf("]\n");
}

void printHierarchy(huffman_node_t *current, int level){
	for (int i = 0; i < level; i++){
		printf("\t");
	}
	
	huffman_node_t *help;
	if (current->letter != '\0'){
		printf("'%c'%d\n", current->letter, current->frequency);
	} else {
		printf("%c%d\n", current->letter, current->frequency);	
	}
	
	if (current->left != NULL){
		printHierarchy(current->left, level + 1);
	}
	if (current->right != NULL){
		printHierarchy(current->right, level + 1);
	}
}

bool is_leaf(huffman_node_t *node){
    return node->left == NULL && node->right == NULL;
}

void make_huffman_tree(priorty_queue_t *queue){
	if (is_prior_queue_empty(*queue)){
		printf("Gagal membuat forest...");
		exit(EXIT_FAILURE);
	}
    
    printf("\nMemulai proses pembentukan huffman tree...\n\n");
	if ((*queue).size == 1){
		//handling untuk teks hanya dengan satu karakter
		print_prior_queue(*queue);
	} else {
        int steps = 1;
		while ((*queue).size > 1){
			huffman_node_t *smallest, *nd_smallest;
	
			//ambil dua node dengan frequency terkecil
			smallest = dequeue_prior(queue);
			nd_smallest = dequeue_prior(queue);
	
			//buat node dengan frequency gabungan dari keduanya
			//dengan anak smallest sebagai anak kiri dan nd_smallest anak kanan
			huffman_node_t *new_node = make_huffman_node(BLANK_CHARACTER, smallest->frequency + nd_smallest->frequency, smallest, nd_smallest);
	
			//masukkan kembali ke queue untuk diurutkan
			enqueue_prior(queue, new_node);
            printf("Step %d:\t", steps++);
			print_prior_queue(*queue);
		}
	}
}

huffman_node_t* execute_huffman(int frequency_map[MAX_ASCII_CHARACTER], codewords_t *table, codewords_t code){
    int i;
    priorty_queue_t forest;
    init_prior_queue(&forest);

    for (i = 0; i < MAX_ASCII_CHARACTER; i++){
        if (frequency_map[i]){
            huffman_node_t *new_node = make_huffman_node(i, frequency_map[i], NULL, NULL);
            enqueue_prior(&forest, new_node);
        }
    }

    system("cls");
    printf("Kondisi awal setiap node dalam queue\n");
    print_prior_queue(forest);

    //buat huffman tree
    make_huffman_tree(&forest);
    compute_code_table(forest.front, table, code);
    print_code_table(table);
    
    printf("\nHierarki Huffman Tree\n");
    printHierarchy(forest.front, 0);
    return forest.front;
}

void print_code_table(codewords_t *table){
	printf("\nTabel Konversi\n");
    size_t x;
    for (x = 0; x < MAX_ASCII_CHARACTER; ++x) {
        if (table[x].code_length) {
            printf("\'%c\'\t", x);
            code_print(table + x);
            printf("\n");  
        }
    }
}

void compute_code_table(huffman_node_t *node, codewords_t *table, codewords_t code){
    if (is_leaf(node)){
        table[node->letter] = code;
    } else {
        code.code_length++;
        code.bit_code <<= 1;
        compute_code_table(node->left, table, code);
        code.bit_code += 1;
        compute_code_table(node->right, table, code);
    }
}

void code_print(codewords_t *code){
    size_t n = code->code_length;
    while (n-- > 0){
        putchar('0' + ((code->bit_code >> n) & 1));
    }
}

void write_code_to_file(codewords_t *code){
    size_t n = code->code_length;
    FILE *file_to_write = fopen("encodedString.txt", "a");
    
    //mengisi file dengan code setelah kompresi
    while (n-- > 0){
        fputc(('0' + ((code->bit_code >> n) & 1)), file_to_write);
    }
    fclose(file_to_write);
}

void decode_string(huffman_node_t *root){
    FILE *file_to_read = fopen("encodedString.txt", "r");

    assert(file_to_read != NULL);
    
    huffman_node_t *current = root;
    char letter;
    while ((letter = fgetc(file_to_read)) != EOF){
    	if (letter == '0'){
            current = current->left;
        } else {
            current = current->right;
        }
        if (is_leaf(current)){
            printf("%c", current->letter);
            current = root;
        }
    }    
}
