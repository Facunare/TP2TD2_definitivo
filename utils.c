#include "utils.h"

// strLen devuelve la longitud total de una cadena de caracteres.
int strLen(char* src) {
	int contador = 0;
	
	while(src[contador]!=NULL){
		contador++;
	}

    return contador;
}

// strDup genera una copia del string pasado por parametro
char* strDup(char* src) {

	char* copia = (char*)malloc((sizeof(char))*(strLen(src)+1)); 
	
	// copia caracter por caracter
	for (int i=0; i<strLen(src); i++){
		copia[i]=src[i];
	}
	
	copia[strLen(src)]=0;
	return copia;
}

// KeysPredictNew genera una nueva estructura keysPredict.

struct keysPredict* keysPredictNew() {
    struct keysPredict* kt = (struct keysPredict*)malloc(sizeof(struct keysPredict));
    kt->first = 0;
    kt->totalKeys = 0;
    kt->totalWords = 0;
    return kt;
}

// keysPredictAddWord agrega una nueva palabra a la estructura keysPredict
void keysPredictAddWord(struct keysPredict* kt, char* word) {
	
	// chequea que no sea una cadena vacia.
	if(strLen(word)==NULL){
		return NULL;
	}
	int index_palabra = 0;
	struct node** pp = &(kt->first);
	
	while(word[index_palabra] != NULL){
		
		struct node* founded = findNodeInLevel(pp, word[index_palabra]);
		
		// si no encontró el nodo, lo crea.
		if(founded==NULL){
			kt->totalKeys++;
			founded = addSortedNewNodeInLevel(pp, word[index_palabra]);
		}
		
		// si es el ultimo caracter, crea la palabra.
		if(index_palabra == strLen(word)-1){
			if(founded->end){
				return NULL;
			}
			founded->word = strDup(word);
			founded->end = 1;
		}
		
		pp = &(founded->down);
		index_palabra++;
	}
	kt->totalWords++;
}

// keysPredictRemoveWord se encarga de borrar una palabra de la estructura kt.
void keysPredictRemoveWord(struct keysPredict* kt, char* word) {
	if(kt->first != NULL){
		int index_palabra = 0;
		struct node** pp = &(kt->first);
		
		// navega hasta el ultimo caracter de la palabra
		while(word[index_palabra] != NULL){
			struct node* founded = findNodeInLevel(pp, word[index_palabra]);
			if(founded==NULL){
				return NULL;
			}
			// si llego al final de la palabra, la borra.
			if((strLen(word)-1)==index_palabra && founded->end == 1){
				founded->word = 0;
				founded->end = 0;
				free(founded->word);
				kt->totalWords--;
			}
			
			pp = &(founded->down);
			index_palabra++;
		}
	}
}

// keysPredictFind se encarga de encontrar una palabra en la estructura kt.
// retorna NULL si no la encuentra; si la encuentra retorno el puntero al nodo.
struct node* keysPredictFind(struct keysPredict* kt, char* word) {
	if(kt == NULL){
		return NULL;
	}
	int index_palabra = 0;
	struct node** pp = &(kt->first);

	// navega hasta el ultimo caracter de la palabra
	while(word[index_palabra] != NULL){
		struct node* founded = findNodeInLevel(pp, word[index_palabra]);
		
		// si no se encuentre ALGUN caracter, no se encontró la palabra, devuelve NULL. Caso contrario, devuelve el puntero. 
		if(founded==NULL){
			return NULL;
		}else{
			int equal = 1;
			if(founded->end == 1){
				// compara la palabra del nodo con la pasada por parametro
				for(int i = 0; i<strLen(word) || i<strLen(founded->word); i++){
					if(founded->word[i] != word[i]){
						equal = 0;
					}
				}
				
				// si son iguales retorna el puntero
				if(equal==1){
					return founded;	
				}				
			}
		}
		pp = &(founded->down);
		index_palabra++;
	}
}


// keysPredictRun retorna un arreglo de strings con todas las palabras comenzadas por un prefijo especifico.
char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount) {
	if(kt->first == NULL || strLen(partialWord) == NULL){
		return NULL;
	}
	int index_palabra = 0;
	struct node** pp = &(kt->first);
	struct node* founded = 0;
	
	// navegar hasta el nodo correspondiente al ultimo caracter del prefijo.
	while(partialWord[index_palabra] != NULL){
		founded = findNodeInLevel(pp, partialWord[index_palabra]);
		if(founded==NULL){
			return NULL;
		}else{
			pp = &(founded->down);
			index_palabra++;
		}

	}
	
	// comprueba si el prefijo es una palabra
	if(founded->end){
		(*wordsCount)++;
	}
	
	// contar todas las palabras comenzadas con el prefijo (reservar memoria con ese tamaño)
	*wordsCount = keysPredictCountWordAux(*pp, *wordsCount, 0);
	
	char** foundWords = (char**)malloc(*wordsCount * sizeof(char*));
	
	// agregar al arreglo las palabras encontradas.
	int cont = 0;
	if(founded->end){
		foundWords[cont] = strdup(founded->word);
		cont++;
	}
	keysPredictCountWordAux(*pp, cont, foundWords);
	
	return foundWords;
}

// keysPredictCountWordAux es una funcion recursiva que cuenta el numero de palabras o inserta las palabras 
// encontradas en el arreglo de strings, segun se especifique. 
int keysPredictCountWordAux(struct node* n, int cont, char** foundWords) {
	struct node* current = n;
	
	// contar palabras o inserlas en el arreglo de punteros a strings segun corresponda
	while(current) {
		if(current->end) {
			if(foundWords != NULL){
				foundWords[cont] = strDup(current->word);	
			}
			cont++;
		}
		
		// llamada recursiva para contar palabras en el nivel inferior al nodo actual
		cont = keysPredictCountWordAux(current->down, cont, foundWords);
		current = current->next; // avanzar al siguiente nodo de un mismo nivel
	}
	
	return cont;
}

// keysPredictListAll se encarga de listar todas las palabras almacenadas
char** keysPredictListAll(struct keysPredict* kt, int* wordsCount) {
	
	struct node** pp = &(kt->first);

	// reservar espacio de memoria segun cantidad de palabras encontradas
	*wordsCount = keysPredictCountWordAux(*pp, *wordsCount, 0);
	char** foundWords = (char**)malloc(*wordsCount * sizeof(char*));
	
	// insertar las palabras en el array
	keysPredictCountWordAux(*pp, 0, foundWords);
	
	return foundWords;
}

// keysPredictDelete se encarga de eliminar recursivamente toda la estructura keysPredict, nodo por nodo.
void keysPredictDelete(struct keysPredict* kt) {
	keysPredictDeleteAux(kt->first, kt);
	kt->first = NULL;
	free(kt->first);
	free(kt);
	
}

// keysPredictDeleteAux es una funcion auxiliar que se encarga de recorrer toda la estructura recursivamente eliminando cada nodo del kt
// (si es un nodo correspondiente a una palabra, eliminar la misma)
void keysPredictDeleteAux(struct node* n, struct keysPredict* kt) {
	if(!n) return;
	struct node* current = n;
	while (current) {
		struct node* temp = current->next;
		keysPredictDeleteAux(current->down, kt);
	
		current->down = NULL;
		// si es una palabra, eliminarla.
		if (current->end) {
			free(current->word);
			kt->totalWords--;
		}
		// elimina el nodo
		current->next = NULL;
		free(current);
		
		kt->totalKeys--;
		current = temp;
	}

}

// keysPredictPrint imprime una vista jerarquica de todas las palabras almacenadas en
// la estructura keysPredict, mostrando el caracter de cada nodo. 
// Tambien muestra el total de llaves y palabras.
void keysPredictPrint(struct keysPredict* kt) {
    printf("--- Predict --- Keys: %i Words: %i\n", kt->totalKeys, kt->totalWords);
    keysPredictPrintAux(kt->first, 0);
    printf("---\n");
}

// keysPredictPrintAux es una funcion recursiva que recorre cada nodo del keysPredict, y lo imprime
// Si el nodo es el final de una palabra, el caracter se muestra entre corchetes.
void keysPredictPrintAux(struct node* n, int level) {
    if(!n) return;
    struct node* current = n;
    while(current) {
        for(int i=0; i<level;i++) printf(" |   ");
        if(current->end) {
            printf("[%c]\n",current->character);
        } else  {
            printf(" %c \n",current->character);
        }
        keysPredictPrintAux(current->down, level+1);
        current = current->next;
    }
}

// Auxiliar functions

// findNodeInLevel se encarga de comprobar si un nodo se encuentra presente en un nivel especifico del keysPredict
// Si lo encuentra, retorna el puntero al mismo; caso contrario devuelve NULL.
struct node* findNodeInLevel(struct node** list, char character) {
	
	// comprobar si la lista esta vacia
	struct node* temp = *list;
	if (temp == NULL) {
		return NULL;
	}
	
	// buscar el nodo pedido segun el caracter especificado.
	while(temp != NULL){
		if(temp->character == character){
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

// addSortedNewNodeInLevel se engarga de agregar un nodo en un nivel especifico segun orden alfabetico
// Si la lista esta vacia o el nuevo nodo es el primero, lo coloca al inicio. En otros casos
// lo inserta en la posicion correcta.
struct node* addSortedNewNodeInLevel(struct node** list, char character){
	
	// creacion y seteo de nuevo nodo
	struct node* newNode = (struct node*) malloc(sizeof(struct node));
	newNode->character = character;
	newNode->next = 0;
	newNode->end = 0;
	newNode->word = 0;
	newNode->down = 0;
	
	// comprobar si la lista esta vacia o si el nodo es el primero de la lista
	if(*list == NULL || (*list)->character > character){
		newNode->next = *list;
		*list = newNode;       
		return newNode;
	}
	
	// insertar el nodo en la lista segun orden alfabetico
	struct node* current = *list;
	while (current->next != NULL && current->next->character < character) {
		current = current->next;
	}
	
	newNode->next = current->next;
	current->next = newNode;
	
	return newNode;
	
};
	
// deleteArrayOfWords se encarga de liberar la memoria reservada para un array de palabras
// (libera cada palabra individualmente y luego libera el array que las contiene)
void deleteArrayOfWords(char** words, int wordsCount) {
	// libera palabra por palabra
	for(int i = 0; i < wordsCount; i++){
		free(words[i]);
	}
	
	// libera arreglo completo
	free(words);	
}
