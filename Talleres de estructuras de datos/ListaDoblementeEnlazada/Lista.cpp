#include "Lista.h"
#include <cassert>

Lista::Lista(): _size(0), _primero(nullptr), _ultimo(nullptr) {}

Lista::Lista(const Lista& l) : Lista() {
    //Inicializa una lista vacía y luego utiliza operator= para no duplicar el código de la copia de una lista.
    *this = l;
}

Lista::~Lista() {
    destruirNodos();
}

Lista& Lista::operator=(const Lista& aCopiar) {
    destruirNodos();
    copiarNodos(aCopiar);
    return *this;
}

void Lista::destruirNodos() {
    Nodo* actual = _primero;
    while ( actual != nullptr ) {
        Nodo* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
        _size--;
    }
    _primero = nullptr;
    _ultimo = nullptr;
}

void Lista::copiarNodos(const Lista &o) {
    Nodo* actual = o._primero;
    while ( actual != nullptr ) {
        agregarAtras(actual->valor);
        actual = actual->siguiente;
    }
}

void Lista::agregarAdelante(const int& elem) {
    // Completar
    Nodo* nuevo = new Nodo(elem);
    if ( _primero != nullptr ) {
        _primero->anterior = nuevo;
        nuevo->siguiente = _primero;
        _primero = nuevo;
    } else {
        _ultimo = nuevo;
        _primero = nuevo;
    }
    _size++;
}

void Lista::agregarAtras(const int& elem) {
    // Completar
    Nodo* nuevo = new Nodo(elem);
    if ( _ultimo != nullptr ) {
        _ultimo->siguiente = nuevo;
        nuevo->anterior = _ultimo;
        _ultimo = nuevo;
    } else {
        _ultimo = nuevo;
        _primero = nuevo;
    }
    _size++;
}

void Lista::eliminar(Nat i) {
    // Completar
    Nodo* actual = _primero;
    Nodo* previo = nullptr;
    Nodo* next = nullptr;
    for(int j = 0; j < i; ++j) {
        previo = actual;
        actual = actual->siguiente;
        next = actual->siguiente;
    }
    if(previo != nullptr){
        previo->siguiente = actual->siguiente;
    } else _primero = actual->siguiente; //Si previo es null, entonces estoy eliminando
                                         //la primera posición.
    if(next != nullptr){
        next->anterior = actual->anterior;
    } else _ultimo = actual->anterior; //Si next es null, entonces estoy eliminando
                                       //la última posición.
    delete(actual);
    _size--;
}

int Lista::longitud() const {
    return _size;
}

const int& Lista::iesimo(Nat i) const {
    // Completar
    Nodo* actual = _primero;
    for(int j = 0; j < i; ++j) {
        actual = actual->siguiente;
    }
    return actual->valor;
    //assert(false);
}

int& Lista::iesimo(Nat i) {
    // Completar (hint: es igual a la anterior...)
    Nodo* actual = _primero;
    for(int j = 0; j < i; ++j) {
        actual = actual->siguiente;
    }
    return actual->valor;
    //assert(false);
}

void Lista::mostrar(ostream& o) {
    // Completar
}
