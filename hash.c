bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	char* clave_hash = malloc(sizeof(char));
	if (!clave_hash){
		return false;
	}
	clave_hash = clave;
	int pos = funcion_hash(???);
	if (hash->tabla[pos].estado != 0){ //Si la posicion esta ocupada/borrada
		while (pos ! = hash->tam || hash->tabla[pos].estado != 0){ //Vamos hasta el campo vacio o hasta el final
			pos++;
		}
		if (pos == hash->tam){ //Si llega al final y no lo encontre
			aux = pos;
			pos = 0;
			while(pos ! = aux || hash->tabla[pos].estado != 0){
				pos++;
			}
		}
	}
	hash->tabla[pos].clave = clave_hash;
	hash->tabla[pos].dato = dato;
	hash->tabla[pos].estado = 1;
	hash->cant += 1
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	int pos = funcion_hash(???);
	if (hash->tabla[pos].clave != clave){ //La clave no esta en la posicion que nos dio la funcion de hash
		while (pos ! = hash->tam || hash->tabla[pos].clave != clave || hash->tabla[pos].estado != 0){ //Vamos hasta el final, hasta el campo vacio o hasta hallar la clave
			pos++;
		}
		if (pos == hash->tam){ //Si llegue al final empiezo desde el principio y recorro hasta la posicion de la funcion
			aux = pos;
			pos = 0;
			while(pos != aux || hash->tabla[pos].clave != clave || hash->tabla[pos].estado != 0){
				pos++;
			}
			if (pos == aux){ //Si volvi a la posicion que genero la clave, no esta
				return NULL
			}
		}
		if (hash->tabla[pos].estado == 0){ //Si llegue al vacio, no esta
			return NULL;
		}
	}
	hash->tabla[pos].estado = 2;
	return hash->tabla[pos].dato;
}
