template <typename T>
string_map<T>::string_map(): raiz(nullptr), _size(0) {}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    if(raiz != nullptr){
        destructorNodos(raiz);
        raiz = nullptr;
    }
    if(d.raiz != nullptr){
        raiz = new Nodo();
        copiarNodos(d.raiz, raiz);
    }
}

template <typename T>
void string_map<T>::copiarNodos(Nodo* actual, Nodo* copia){
    if(actual->definicion != nullptr){
        copia->definicion = new T(*actual->definicion);
    }
    if(actual->hijos > 0){
        copia->hijos = actual->hijos;
        for(int i = 0; i < 256; i++){
            if(actual->siguientes[i] != nullptr){
                copia->siguientes[i] = new Nodo();
                copiarNodos(actual->siguientes[i], copia->siguientes[i]);
            }
        }
    }
}

template <typename T>
string_map<T>::~string_map(){
    if(raiz != nullptr){
        destructorNodos(raiz);
        raiz = nullptr;
    }
}

template <typename T>
void string_map<T>::destructorNodos(Nodo* actual){
    if(actual->definicion != nullptr){
        delete actual->definicion;
        actual->definicion = nullptr;
    }
    for(int i = 0; actual->hijos != 0 && i<256; i++){
        if(actual->siguientes[i] != nullptr){
            Nodo* siguiente = actual;
            siguiente = siguiente->siguientes[i];
            actual->siguientes[i] = nullptr;
            destructorNodos(siguiente);
            actual->hijos = actual->hijos - 1;
        }
    }
    delete actual;
}

template <typename T>
T& string_map<T>::operator[](const string& clave){
    // COMPLETAR
}


template <typename T>
int string_map<T>::count(const string& clave) const{
    Nodo* actual = raiz;
    int i = 0;
    while(i < clave.size()){
        if(actual == nullptr){
            return 0;
        }
        int index = int(clave[i]);
        actual = actual->siguientes[index];
        i++;
    }
    if(actual->definicion == nullptr){
        return 0;
    }
    return 1;
}

template <typename T>
void string_map<T>::insert(const pair<string, T> & p) {
    if(raiz == nullptr){
        raiz = new Nodo();
    }
    Nodo* actual = raiz;
    string clave = p.first;
    int i = 0;
    while(i < clave.size()){
        int index = int(clave[i]);
        if (actual->siguientes[index] == nullptr){
            actual->siguientes[index] = new Nodo();
            actual->hijos = actual->hijos + 1;
        }
        actual = actual->siguientes[index];
        i++;
    }
    if(actual->definicion == nullptr){
        _size++;
    }
    delete actual->definicion;
    T* significado = new T(p.second);
    actual->definicion = significado;
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    Nodo* actual = raiz;
    int i = 0;
    while(i < clave.size()){
        int index = int(clave[i]);
        actual = actual->siguientes[index];
        i++;
    }
    return *actual->definicion;
}

template <typename T>
T& string_map<T>::at(const string& clave) {
    Nodo* actual = raiz;
    int i = 0;
    while(i < clave.size()){
        int index = int(clave[i]);
        actual = actual->siguientes[index];
        i++;
    }
    return *actual->definicion;
}

template <typename T>
void string_map<T>::erase(const string& clave) {
    Nodo* actual = raiz;
    Nodo* ultimo = raiz;
    int ult_indice = 0;
    for(int i = 0; i < clave.size(); i++){
        if((actual->hijos > 1 || actual->definicion != nullptr) && i != clave.size() - 1){
            ultimo = actual;
            ult_indice = i;
        }
        int index = int(clave[i]);
        actual = actual->siguientes[index];
    }
    delete actual->definicion;
    actual->definicion = nullptr;
    if(actual->hijos == 0){
        ultimo->hijos = ultimo->hijos - 1;
        Nodo* siguiente = ultimo->siguientes[int(clave[ult_indice])];
        ultimo->siguientes[int(clave[ult_indice])] = nullptr;
        ultimo = siguiente;
        ult_indice++;
        for(int k = ult_indice; k < clave.size(); k++){
            siguiente = ultimo->siguientes[int(clave[k])];
            delete ultimo;
            ultimo = siguiente;
        }
        delete ultimo;
    }
    if(raiz->hijos == 0){
        delete raiz;
        raiz = nullptr;
    }
}

template <typename T>
int string_map<T>::size() const{
    return _size;
}

template <typename T>
bool string_map<T>::empty() const{
    return raiz == nullptr;
}