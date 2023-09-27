#include "Puesto.h"

//Constructor del puesto
Puesto::Puesto(const Menu& menu_, const Stock& stock_, const Promociones& promociones){
    Promociones::const_iterator itItem = promociones.begin();
    //Iteramos y por cada item de promociones se crea un vector con los descuentos
    while(itItem != promociones.end()){
        map<Nat, Nat> diccPromos = itItem->second;
        vector<Nat> vectorPromos = crearVectorDescuentos(diccPromos);
        descuentos.insert({itItem->first, vectorPromos}); //Se inserta en descuentos {item, vectorPromos}
        itItem++;
    }
    menu = Menu(menu_); //Se copia menu
    stock = Stock(stock_); //Se copia stock
    //Las demas estructuras del Puesto inician vacías.
}

//A partir de un dicc de descuentos por cantidad, se crea un vector que cada pos representa la cantidad y cada elemento
//su descuento.
vector<Nat> Puesto::crearVectorDescuentos(const map<Nat, Nat> &dicc){
    vector<Nat> v1;
    int inicio = 0;
    int valor = 0;
    //se itera inorder el dicc. El iterador que da c++ ya recorre inorder.
    map<Nat, Nat>::const_iterator it1 = dicc.begin();
    while (it1 != dicc.end()) {
        for (int i = inicio; i < it1->first; i++) {
            v1.push_back(valor);
        }
        v1.push_back(it1->second);
        inicio = it1->first + 1; //empieza despues del ultimo elemento que se insertó en la linea anterior
        valor = it1->second;
        it1++;
    }
    return v1;
}

const Nat Puesto::obtenerStock(Producto item) const{
    return stock.at(item);
}

const Nat Puesto::obtenerDescuento(Producto item, Nat cant) const{
    //Si el ítem está definido, tiene descuentos
    if(descuentos.count(item) == 1){
        int longVec = descuentos.at(item).size();
        //Si la cantidad está en el rango del vector, se averigua el descuento por indexación. Si es mayor o igual
        //devuelve el descuento más alto.
        if(cant < longVec){
            return descuentos.at(item)[cant];
        } else return descuentos.at(item)[longVec-1];
    } else return 0; //Si no está definido, no tiene descuentos.
}

const Nat Puesto::obtenerGastoPersona(Persona persona) const{
    if (gastos_personas.count(persona) == 1){
        return gastos_personas.at(persona);
    } else return 0; //Si persona no está definida, no hizo ninguna compra.
}

//Actualiza los datos del puesto cuando ocurre una venta.
const Nat Puesto::vender(Persona persona, Producto item, Nat cant){
    Nat precio = precioItem(item);
    Nat descuento = obtenerDescuento(item,cant);
    stock.at(item) -= cant;
    Nat gasto = (precio * cant) - (precio * cant * descuento/100);
    //Si la persona estaba definido, sumo el nuevo gasto. Sino, la defino con su primer gasto.
    if(gastos_personas.count(persona) == 1){
        gastos_personas.at(persona) += gasto;
    } else gastos_personas.insert(pair< Persona, Nat>(persona,gasto));
    //La siguiente función actualiza el registro de compras de la persona.
    actualizarComprasPersona(item, cant, descuento, persona);
    return gasto;
}

void Puesto::actualizarComprasPersona(Producto item, Nat cant, Nat descuento, Persona persona) {
    if(descuento == 0){ //Si fue sin descuento, se actualiza ventas_sin_descuento de la persona.
        //Si la persona no había comprado antes sin descuento, se define en el diccionario.
        if(ventas_sin_descuento.count(persona) == 0){
            vector<Nat> compras = {cant}; //El primer elemento del vector es la cantidad de items comprados.
            map<Producto, vector<Nat>> comprasItems;
            comprasItems.insert({item, compras});
            //Se inserta un nuevo map<item, vector<nat> a las compras sin descuento de una persona.
            ventas_sin_descuento.insert({persona, comprasItems});
        } else {
            //Si ya había comprado, obtengo el diccionario con los items que compró.
            map<Producto, vector<Nat>>& itemsComprados = ventas_sin_descuento.at(persona);
            //Si no había comprado antes el item, se lo define en el diccionario.
            if(itemsComprados.count(item) == 0){
                vector<Nat> compras = {cant};
                itemsComprados.insert({item,compras});
            } else {
                itemsComprados.at(item).push_back(cant); //Sino, solo se agrega al final del vector la nueva cant.
            }
        }
    } else { //Si la compra fue con descuento, se actualiza ventas_descuento de la persona.
        if(ventas_descuento.count(persona) == 1){
            //Si la persona ya estaba definida en el diccionario solamente se agrega al final de la lista los datos
            //de la compra.
            ventas_descuento.at(persona).push_back({item,cant});
        } else {
            //Sino, se crea una nueva lista con los datos de la compra y se define la persona en el diccionario.
            list< pair< Producto, Nat>> listaVentas;
            listaVentas.push_back({item,cant});
            ventas_descuento.insert({persona,listaVentas});
        }
    }
}

//Actualiza los datos del puesto cuando se olvida la compra de un item debido a un hackeo.
void Puesto::olvidarItem(Persona persona, Producto item){
    stock.at(item)++; //Suma uno al stock del item
    map<Producto, vector<Nat>>& itemsSinDescuento = ventas_sin_descuento.at(persona);
    Nat precio = precioItem(item);
    //Se resta en los gastos de la persona el precio del item hackeado
    gastos_personas[persona] -= precio;
    vector<Nat>& vectorVentas = itemsSinDescuento.at(item);
    //En el registro de compras sin descuento de la persona, se resta en uno la cantidad comprada por última vez
    //del item
    vectorVentas[vectorVentas.size()-1]--;
    //Si la cantidad de la ultima compra ahora es 0, se borra el ultimo elemento del vector.
    if(vectorVentas[vectorVentas.size()-1] == 0){
        vectorVentas.pop_back();
        //Si el vector tiene ahora tamaño 0, se borra el item de las compras de la persona
        if(vectorVentas.size() == 0){
            itemsSinDescuento.erase(item);
            //Si el diccionario de compras de la persona no tiene elementos, se borra a la persona de la estructura
            //de ventas sin descuento.
            if(itemsSinDescuento.size() == 0){
                ventas_sin_descuento.erase(persona);
            }
        }
    }
}

const bool Puesto::perteneceAlMenu(Producto item) const{
    return menu.count(item) == 1;
}

const Nat Puesto::precioItem(Producto item) const{
    return menu.at(item);
}

const bool Puesto::esHackeable(Persona persona, Producto item) const{
    //Si está definida la persona en ventas_sin_descuento y si está definido el item en sus compras, entonces
    //es hackeable ese item de esa persona en el puesto.
    if(ventas_sin_descuento.count(persona) == 1){
        map<Producto, vector<Nat>> itemsComprados = ventas_sin_descuento.at(persona);
        return itemsComprados.count(item) == 1;
    } else return false;
}

Puesto Puesto::operator=(const Puesto& puesto){
    if(this == &puesto){
        return *this;
    }

    menu = Menu(puesto.menu);
    stock = Stock(puesto.stock);
    descuentos = map<Producto, vector<Nat>>(puesto.descuentos);
    ventas_sin_descuento = map<Persona, map<Producto, vector<Nat>>>(puesto.ventas_sin_descuento);
    ventas_descuento = map<Persona, list< pair< Producto, Nat>>>(puesto.ventas_descuento);
    gastos_personas = map<Persona, Nat>(puesto.gastos_personas);

    return *this;
}