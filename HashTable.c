#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define LENGTH_INITIAL 256
#define hash(k, length) (key % length)

//Struct para HASH encadeada;
typedef struct data_chained {
	int key;
	int info;
	struct data_chained *next;
}HASH_CHAINED;

//Struct para HASH linear;
typedef struct data_open_addressing {
	int key;
	int info;
}Open_Addressing;

//Funções para HASH encadeada;
HASH_CHAINED *begin_chained(HASH_CHAINED *data); 								//Inicia o vetor
int insert_chained(HASH_CHAINED *data, int key, int info, int length);			//Insere novas chaves ao vetor
void destroy_chained(HASH_CHAINED *data, int *length);							//Destroi a data inteira
HASH_CHAINED *ReHASH_chained(HASH_CHAINED *data, int *length, int count);		//Expande o vetor quando necessario
void print_hash_chained(HASH_CHAINED *data, int length);						//Imprime toda a data
void Chained();																	//"main" encadeada

//Funções para HASH linear;
int insert_linear(Open_Addressing *data, int key, int info, int length);		//Insere novas chaves ao vetor
Open_Addressing *ReHASH_linear(Open_Addressing *data, int *length);				//Expande o vetor quando necessario
void Linear();																	//"main" linear;

//Funções para HASH quadratica
void Quadratic();																//"main" Quadratica
int insert_quadratic(Open_Addressing *data, int key, int info, int length);		//Insere novas chaves ao vetor
Open_Addressing *ReHASH_quadratic(Open_Addressing *data, int *length);			//Expande o vetor quando necessario

//Funções que os dois tipos de endereçamento aberto compartilham
Open_Addressing *begin_Open_Addressing(Open_Addressing *data);					//Inicia o vetor
void print_hash_Open_Addressing(Open_Addressing *data, int length);				//Imprime o vetor



//Comentar as funçãos que não irá utilizar.
int main() {
	Quadratic();
	Chained();
	Linear();
	return 0;
}

void Linear() {
	printf("\t\tTABELA HASH LINEAR \n");
	int length = LENGTH_INITIAL;
	double begin, end, time_test;
	int i, j, inserts = 0, count = 0;
	float FC;
	Open_Addressing *data;
	srand(time(NULL));
	data = begin_Open_Addressing(data);
	for(i = 0; i < 20; i++){	
		begin = clock();
		for(j = 0; j < 300; j++){
			count = insert_linear(data, rand(), rand(), length);
			inserts = inserts + count;
			FC = (inserts / length);
			if(FC > 0.7){
				data = ReHASH_linear(data, &length);
			}
		}
		end = clock();
		time_test = time_test + ((end - begin) / CLOCKS_PER_SEC);
		printf("\tDuracao: %lfs\ttamanho: %d\n", ((end - begin) / CLOCKS_PER_SEC), length);
	}
	time_test = time_test / 20;
	printf("Media: %lfs\n", time_test);
	//print_hash_Open_Addressing(data, length);							//Imprime o vetor
	free(data);
}
void print_hash_Open_Addressing(Open_Addressing *data, int length) {
	int i;
	for(i = 0; i < length; i++) {
		printf("%d. Endereço: %p\t Conteudo: %d, %d\n", (i+1), &data[i], data[i].key, data[i].info);
	}
}

Open_Addressing *begin_Open_Addressing(Open_Addressing *data) {
	int i;
	if((data = malloc(LENGTH_INITIAL * sizeof(Open_Addressing))) == NULL) {
		exit(1);
	}
	for(i = 0; i < LENGTH_INITIAL; i++) {
		data[i].key = 0;
		data[i].info = 0;
	}
	return data;
}
int insert_linear(Open_Addressing *data, int key, int info, int length) {
	int index, i;
	index = (key % length);
	if(data[index].key == 0) {
		data[index].key = key;
		data[index].info = info;
		return 1;
	} else {
		i = index;
		while(data[i].key != 0) {
			if(data[i].key == key) {
				return 0;
			}
			i++ % index;
		}
		data[i].key = key;
		data[i].info = info;
		return 1;
	}
}
Open_Addressing *ReHASH_linear(Open_Addressing *data, int *length){
	Open_Addressing *new_data;
	int new_length, i;
	new_length = (*length) * 2;
	if((new_data = malloc(new_length * sizeof(Open_Addressing))) == NULL) {
		exit(1);
	}
	for(i = 0; i < (*length); i++){
		if(data[i].key != 0){
			insert_linear(new_data, data[i].key, data[i].info, new_length);
		}
	}
	free(data);
	(*length) = new_length;
	return new_data;
}

void Chained() {
	printf("\t\tTABELA HASH ENCADEADA \n");
	HASH_CHAINED *data, *p;
	double begin, end, time_test = 0;
	int i, j, count;
	int length = LENGTH_INITIAL;
	srand(time(NULL));
	data = begin_chained(data);
	for(i = 0; i < 20; i++) {
		begin = clock();
		for(j = 0; j < 300; j++) {
			count = insert_chained(data, rand(), rand(), length);
			data = ReHASH_chained(data, &length, count);
		}
		end = clock();
		time_test = time_test + ((end - begin) / CLOCKS_PER_SEC);
		printf("\tDuracao: %lfs\ttamanho: %d\n", ((end - begin) / CLOCKS_PER_SEC), length);
	}
	//print_hash_chained(data, length);
	destroy_chained(data, &length);
	time_test = time_test / 20;
	printf("\nMedia: %lfs\n", time_test);
}
HASH_CHAINED *begin_chained(HASH_CHAINED *data) {
	int i;
	if((data = malloc(LENGTH_INITIAL * sizeof(HASH_CHAINED))) == NULL) {
		exit(1);
	}
	for(i = 0; i < LENGTH_INITIAL; i++) {
		data[i].key = 0;
		data[i].info = 0;
		data[i].next = NULL;
	}
	return data;
}
int insert_chained(HASH_CHAINED *data, int key, int info, int length) {
	int index;
	int count = 0;
	HASH_CHAINED *aux, *p;
	index = hash(k, length);
	p = &(data[index]);
	if(p->key == 0) {
		p->key = key;
		p->info = info;
	}
	else if((p->key != 0) && (p->key != key)) {
		if((aux = malloc(sizeof(HASH_CHAINED))) == NULL) {
			exit(1);
		}
		aux->key = key;
		aux->info = info;
		aux->next = NULL;
		if(p->next == NULL) {
			p->next = aux;
			count++;
		} 
		else if((p->next != NULL) && (p->key != key) && (p->next->key != key)) {
			count++;
			aux->next = p->next;
			p->next = aux;
			while(p->next != NULL) {
				p = p->next;
				count++;
			}
		}
	}
	return count;
}
void destroy_chained(HASH_CHAINED *data, int *length) {
	HASH_CHAINED *aux, *p;
	int i;
	for(i = 0; i < (*length); i++) {
		p = data[i].next;
		for(p = data[i].next; p != NULL; p = aux) {
			aux = p->next;
			free(p);
		}
	}
	free(data);
}
HASH_CHAINED *ReHASH_chained(HASH_CHAINED *data, int *length, int count) {
	HASH_CHAINED *p, *new;
	int new_length, i;
	if(count > 4) {
		new_length = (*length) * 2;
		if((new = malloc(new_length * sizeof(HASH_CHAINED))) == NULL) {
			exit(1);
		}
		for(i = 0; i < (*length); i++){
			insert_chained(new, data[i].key, data[i].info, new_length);
			for(p = data[i].next; p != NULL; p = p->next) {
				insert_chained(new, p->key, p->info, new_length);
			}
		}
		destroy_chained(data, length);
		(*length) = new_length;
		return new;
	} else {
		return data;
	}
}
void print_hash_chained(HASH_CHAINED *data, int length) {
	HASH_CHAINED *p;
	int i;
	for(i = 0; i < length; i++){
		printf("%d. Endereço: %p\tConteudo: %d, %d\n", (i + 1), &data[i], data[i].key, data[i].info);
		for(p = data[i].next; p != NULL; p = p->next) {
			printf("\t--Endereço: %p\tConteudo: %d, %d\n", p, p->key, p->info);
		}
		printf("\n");
	}
}

void Quadratic(){
	printf("\t\tTABELA HASH QUADRATICA \n");
	Open_Addressing *data;
	int length = LENGTH_INITIAL, i, key, info, count = 0, j;
	float inserts = 0, FC;
	double time_test, begin, end;
	srand(time(NULL));
	data = begin_Open_Addressing(data);
	for (i = 0; i < 20; i++){
		begin = clock();
		for(j = 0; j < 300; j++){
			count = insert_quadratic(data, rand(), rand(), length);
			inserts = inserts + count;
			FC = (inserts / length);
			if(FC > 0.7){
				data = ReHASH_quadratic(data, &length);
			}
		}
		end = clock();
		time_test = time_test + ((end - begin) / CLOCKS_PER_SEC);
		printf("\tDuracao: %lfs\ttamanho: %d\n", ((end - begin) / CLOCKS_PER_SEC), length);
	}
	time_test = time_test / 20;
	printf("Media: %lfs\n", time_test);
	//print_hash_Open_Addressing(data, length);							//Imprime o vetor
	free(data);
}

int insert_quadratic(Open_Addressing *data, int key, int info, int length){
	int index, i = 0, indexOriginal;
	indexOriginal = key % length;
	if(data[indexOriginal].key == 0){
		data[indexOriginal].key = key;
		data[indexOriginal].info = info;
		return 1;
	}
	else{
		index = indexOriginal;
		while(data[index].key != 0){
			if(data[index].key == key){
				return 0;
			}
			i++;
			index = (indexOriginal + (((pow(i, 2)) + i)/2));
			index = index % length;
		}
		data[index].key = key;
		data[index].info = info;
		return 1;
	}
}

Open_Addressing *ReHASH_quadratic(Open_Addressing *data, int *length){
	Open_Addressing *new_data;
	int new_length, i;
	new_length = (*length) * 2;
	if((new_data = malloc(new_length * sizeof(Open_Addressing))) == NULL){
		exit(1);
	}
	for(i = 0; i < (*length); i++){
		if(data[i].key != 0){
			insert_quadratic(new_data, data[i].key, data[i].info, new_length);
		}
	}
	free(data);
	*length = new_length;
	return new_data;
}