#include "hash.h"
#include "murmurhash.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_INICIAL 37

/* *****************************************************************
 *                      ESTRUCTURA DEL HASH
 * *****************************************************************/

typedef enum estado{
	vacio,
	ocupado,
	borrado
}estado_t;

typedef struct campo{
	char* clave;
	void* dato;
	estado_t estado;
}campo_t;

typedef struct hash{
	size_t tam;
	size_t cant;
	hash_destruir_dato_t* destruir_dato;
	campo_t* tabla;
}hash_t;

campo_t* crear_campo();
int buscar_posicion(const hash_t *hash, const char *clave);
hash_t* hash_redimensionar(hash_t* hash, size_t tam, hash_destruir_dato_t destruir_dato);

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t *hash_crear(void (*hash_destruir_dato_t)(void *)){
	hash_t* hash = malloc(sizeof(hash_t));
	hash->tabla = malloc(sizeof(campo_t*) * TAM_INICIAL);
	hash->cant = 0;
	hash->tam = TAM_INICIAL;
	hash->destruir_dato = destruir_dato;
	for (int i = 0; i != hash->tam; i++){
		hash->tabla[i].clave = '\0';
		hash->tabla[i].dato = NULL;
		hash->tabla[i].estado = 0;
	}
	if (!hash || !hash->tabla){
		return NULL;
	}
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	char* clave_hash = malloc(sizeof(char));
	if (!clave_hash){
		return false;
	}
	*clave_hash = *clave;
	int pos = (int)(murmurhash(clave, (uint32_t)hash->tam, 1));
	if (hash_pertenece(hash, clave)){
		free(clave_hash);
		return false;
	}
	if (hash->tabla[pos].estado != 0){ //Si la posicion esta ocupada/borrada
		pos = buscar_posicion(hash, clave); // Nos deberia devolver un vacio porque ya veridicamos que no este
	}
	hash->tabla[pos].clave = clave_hash;
	hash->tabla[pos].dato = dato;
	hash->tabla[pos].estado = 1;
	hash->cant += 1;
	if (hash->tam *(2/3) <= hash->cant) {
		hash = hash_redimensionar(hash, 2, hash->destruir_dato);
	}
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	int pos = (int)(murmurhash(clave, (uint32_t)hash->tam, 1)); 
	if (!hash_pertenece(hash, clave)){
		return NULL;
	}
	if (strcmp(hash->tabla[pos].clave, clave) != 0){ // La clave no esta en la posicion que nos dio la funcion de hash
		pos = buscar_posicion(hash, clave); // No verificamos que no sea vacio, porque ya lo verificamos antes. A menos de que debamos (?
	}
	hash->cant -= 1;
	hash->tabla[pos].estado = 2;
	if (hash->cant <= hash->tam * (1/4)){
		hash = hash_redimensionar(hash, 1/2, destruir_dato);
	}
	return hash->tabla[pos].dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	int pos = (int)(murmurhash(clave, (uint32_t)hash->tam, 1)); 
	if (!hash_pertenece(hash, clave)){
		return NULL;
	}
	if (strcmp(hash->tabla[pos].clave, clave) != 0){
		pos = buscar_posicion(hash, clave);
	}
	return hash->tabla[pos].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	int pos = buscar_posicion(hash,clave);
	return hash->tabla[pos].estado == 1;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant;
}

void hash_destruir(hash_t* hash){
	for (int i = 0; i != hash->tam; i++){
		if (hash->destruir_dato){
			destruir_dato(hash->tabla[i].dato);
		}
		else{
			free(hash->tabla[i].dato);
		}
		free(hash->tabla[i].clave);
	}
	free(hash);
}

/* *****************************************************************
 *                    ESTRUCTURA DEL ITER
 * *****************************************************************/

typedef struct hash_iter{
	const hash_t* hash;
	int actual;
}hash_iter_t;

/* *****************************************************************
 *                    PRIMITIVAS DEL ITER
 * *****************************************************************/


hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (iter == NULL) return NULL;
	iter->hash = hash;
	if (iter->hash->cant == 0) iter->actual = NULL; // Esto no copila. Si no hay nada, para mi habria que hacer free(iter) y return NULL. No tendriamos que verificar que avanzamos en un hash vacio porque directamente ni se creo
	else{
		int pos = 0;
		while (iter->hash->tabla[pos].estado != 0){
				pos++;
			}
		}
	if (pos != NULL) iter->actual = pos; // Pos sin declarar. Para mi no habria que hacer pos == NULL.
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return false;
	int nueva_pos = iter->actual + 1;
	while (iter->hash->tabla[nueva_pos].estado != 1){
		if (iter->hash->tabla[nueva_pos].estado == 0) return false;
		nueva_pos++;
	}
	iter->actual = nueva_pos;
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (iter->hash->tabla[iter->actual].estado != 1) return NULL;
	return iter->hash->tabla[iter->actual].clave;
}


bool hash_iter_al_final(const hash_iter_t *iter){
	if (iter->actual == NULL) return false;
	if (iter->hash->tabla[iter->actual + 1].estado == 1){
		return false;
	}
	else if (iter->hash->tabla[iter->actual + 1].estado == 0){
		return true;
	}
	else{
		int pos = iter->actual + 1;
		while (true){
			if (iter->hash->tabla[pos].estado == 2) pos++;
			else if (iter->hash->tabla[pos].estado == 1) return false;
			else{
				return true;
			}
		}
	}
}
			
bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter))return false;
	int nueva_pos=iter->actual + 1;
	while (iter->hash->tabla[nueva_pos].estado != 1){
		nueva_pos++;
	}
	iter->actual = nueva_pos;
	return true;
}

void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}

/* *****************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

int buscar_posicion(const hash_t *hash, const char *clave){
	int pos = (int)(murmurhash(clave, (uint32_t)hash->tam, 1));
	while (pos != hash->tam || hash->tabla[pos].estado != 0 || (strcmp(hash->tabla[pos].clave, clave) != 0 && hash->tabla[pos].estado != 1)){
		pos++;
	}
	if (pos == hash->tam){
		int aux = pos;
		pos = 0;
		while (pos != hash->tam || hash->tabla[pos].estado != 0 || (strcmp(hash->tabla[pos].clave, clave) != 0 && hash->tabla[pos].estado != 1)){
			pos++;
		}
	}
	return pos;
}

hash_t* hash_redimensionar(hash_t* hash, size_t tam, void (*hash_destruir_dato_t)(void *)){
	if (tam < TAM_INICIAL){
		tam = TAM_INICIAL;
	}
	hash* hash_nuevo = hash_crear(hash_destruir_dato_t);
	if (!hash_nuevo){
		return NULL;
	}
	hash_nuevo->cant = hash->cant;
	hash_nuevo->tam = hash->tam * tam;
	for (int i = 0; i != hash->tam; i++){ // Recorro el hash viejo para ir guardando los campos
		if (hash->tabla[pos].estado == 1){ // Guardo unicamente los ocupados
			char* clave = hash->tabla[pos].clave;
			void* dato = hash->tabla[pos];
			hash_guardar(hash_nuevo, clave, dato);
		}
	}
	hash_destruir(hash);
	return hash_nuevo;
}
