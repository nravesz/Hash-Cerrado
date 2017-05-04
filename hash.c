typedef struct hash{
	size_t tam;
	size_t cant;
	destruir_dato_t destrir_dato;
	campo_hash_t tabla;
}hash_t;

typedef struct campo_hash{
	char* clave;
	void* dato;
	estado_t estado;
}campo_t;

campo_t* crear_campo(){
	campo.clave = '\0';
	campo.dato = NULL;
	campo.estado = 1; // vacio = 1
	return campo;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	campo_t* hash->tabla = malloc(sizeof(campo_t*) * TAM_INICIAL);
	hash->cant = 0;
	hash->tam = TAM_INICIAL;
	hash->destruir_dato = destruir_dato;
	for (int i = 0; i != hash->tam, i++){
		hash->tabla[i] = crear_campo; //Habria que poner alguna condicion de que si un campo es NULL, hay que borrar todo, si es que pedimos memoria ajja
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
	clave_hash = clave;
	int pos = funcion_hash(???);
	if (hash_pertenece(hash, clave)){
		free(clave_hash);
		return false;
	}
	if (hash->tabla[pos].estado != 0){ //Si la posicion esta ocupada/borrada
		int pos = buscar_posicion(hash, clave); // Nos deberia devolver un vacio porque ya veridicamos que no este
	}
	hash->tabla[pos].clave = clave_hash;
	hash->tabla[pos].dato = dato;
	hash->tabla[pos].estado = 1;
	hash->cant += 1
	if (hash->tam *(2/3) <= hash->cant) {
		bool resultado= redimensionar(hash);
		if (resultado==false) return false;
	}
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	int pos = funcion_hash(???);
	if (!hash_pertenece(hash, clave)){
		return NULL;
	}
	if (hash->tabla[pos].clave != clave){ // La clave no esta en la posicion que nos dio la funcion de hash
		int pos = buscar_posicion(hash, clave); // No verificamos que no sea vacio, porque ya lo verificamos antes. A menos de que debamos (?
	}
	hash->cant -= 1;
	hash->tabla[pos].estado = 2;
	return hash->tabla[pos].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave);
	int pos = buscar_posicion(hash,clave);
	return hash->tabla[pos].estado == 1;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant;
}

void hash_destruir(hash_t* hash){
}

/* FUNCIONES AUXILIARES*/

/* Recorre el hash. Busca la posicion en la que se encuentre la clave pasada por parametro o hasta halla el
 * primer vacio. */
int buscar_posicion(hash_t* hash, const char *clave){
	int pos = funcion_hash(???);
	while (pos != hash->tam || hash->tabla[pos].estado != 0 || (hash->tabla[pos].clave != clave && hash->tabla[pos].estado != 1)){
		pos++;
	}
	if (pos == hash->tam){
		aux = pos;
		pos = 0;
		while (pos != hash->tam || hash->tabla[pos].estado != 0 || (hash->tabla[pos].clave != clave && hash->tabla[pos].estado != 1)){
			pos++;
		}
	}
	return pos;
}
