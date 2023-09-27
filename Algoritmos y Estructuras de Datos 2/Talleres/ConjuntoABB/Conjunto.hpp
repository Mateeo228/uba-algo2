
template <class T>
Conjunto<T>::Conjunto(): _raiz(nullptr), _cardinal(0) {}

template <class T>
Conjunto<T>::~Conjunto() {
    int size = _cardinal;
    for(int i = 1; i <= size ; i++){
        remover(_raiz->valor);
    }
}

template <class T>
bool Conjunto<T>::pertenece(const T& clave) const {
    Nodo* actual = _raiz;
    while(actual != nullptr){
        if(clave == actual->valor){
            return true;
        }
        if(clave > actual->valor){
            actual = actual->der;
        } else actual = actual->izq;
    }
    //assert(false);
    return false;
}

template <class T>
void Conjunto<T>::insertar(const T& clave) {
    if( _raiz == nullptr ){
        _raiz = new Nodo(clave);
        _cardinal++;
        return;
    }
    Nodo* actual = _raiz;
    while(actual != nullptr){
        if(clave == actual->valor){
            return;
        }
        if(clave > actual->valor){
            if(actual->der == nullptr){
                actual->der = new Nodo(clave);
                _cardinal++;
                return;
            } else actual = actual->der;
        } else {
            if(actual->izq == nullptr){
                actual->izq = new Nodo(clave);
                _cardinal++;
                return;
            } else actual = actual->izq;
        }
    }
}

template <class T>
void Conjunto<T>::remover(const T& clave) {
    if (!pertenece(clave)) {
        return;
    }
    Nodo* actual = _raiz;
    if (actual->valor == clave){ // CASO ELIMINAR RAIZ
        if (actual->izq == nullptr && actual->der == nullptr) { //RAIZ UNICO ELEMENTO
            _raiz = nullptr;
            delete actual;
            _cardinal--;
            return;
        }
        else if (actual->izq == nullptr && actual->der != nullptr) { //RAIZ HIJO A LA DERECHA
            _raiz = actual->der;
            delete actual;
            _cardinal--;
            return;
        }
        else if (actual->izq != nullptr && actual->der == nullptr) { //RAIZ HIJO A LA IZQUIERDA
            _raiz = actual->izq;
            delete actual;
            _cardinal--;
            return;
        }
        else { //RAIZ CON DOS HIJOS
            Nodo* sucesor = actual->der;
            Nodo* prevSucesor = actual;
            string sucesorDir = "der";
            while (sucesor->izq != nullptr) {
                prevSucesor = sucesor;
                sucesor = sucesor->izq;
                sucesorDir = "izq";
            }
            actual->valor = sucesor->valor;
            removerSinDosHijos(sucesor, prevSucesor, sucesorDir);
        }
    } else {
        string direction;
        Nodo* prev = nullptr;
        while (actual != nullptr) {
            if (clave == actual->valor) {
                if (actual->izq != nullptr && actual->der != nullptr) { //CASO DOS HIJOS
                    Nodo* sucesor = actual->der;
                    Nodo* prevSucesor = actual;
                    string sucesorDir = "der";
                    while (sucesor->izq != nullptr) {
                        prevSucesor = sucesor;
                        sucesor = sucesor->izq;
                        sucesorDir = "izq";
                    }
                    actual->valor = sucesor->valor;
                    removerSinDosHijos(sucesor, prevSucesor, sucesorDir);
                } else { //CASO SIN DOS HIJOS
                    removerSinDosHijos(actual, prev, direction);
                    return;
                }
            }

            prev = actual;
            if (clave > actual->valor) {
                actual = prev->der;
                direction = "der";
            } else {
                actual = prev->izq;
                direction = "izq";
            }

        }
    }

}

template <class T>
void Conjunto<T>::removerSinDosHijos(Nodo* actual, Nodo* prev, string direction) {
    if (actual->izq == nullptr && actual->der == nullptr) { //Sin hijos
        if (direction == "der"){
            prev->der = nullptr;
        } else prev->izq = nullptr;
    }
    else if (actual->izq == nullptr && actual->der != nullptr) { //Solo hijo derecho
        if (direction == "der"){
            prev->der = actual->der;
        } else prev->izq = actual->der;
    }
    else { //Solo hijo izquierdo
        if (direction == "der"){
            prev->der = actual->izq;
        } else prev->izq = actual->izq;
    }
    delete actual;
    _cardinal--;
}

template <class T>
const T& Conjunto<T>::siguiente(const T& clave) {
    Nodo* actual = _raiz;
    T* sig = &_raiz->valor;
    while (actual != nullptr) {
        if (actual->valor > clave){
            sig = &actual->valor;
            actual = actual->izq;
        } else actual = actual->der;
    }
    return *sig;
}

template <class T>
const T& Conjunto<T>::minimo() const {
    Nodo* actual = _raiz;
    while (actual->izq != nullptr) {
        actual = actual->izq;
    }
    return actual->valor;
}

template <class T>
const T& Conjunto<T>::maximo() const {
    Nodo* actual = _raiz;
    while (actual->der != nullptr) {
        actual = actual->der;
    }
    return actual->valor;
}

template <class T>
unsigned int Conjunto<T>::cardinal() const {
    return _cardinal;
}

template <class T>
void Conjunto<T>::mostrar(std::ostream&) const {
    //assert(false);
}

