#include "Lollapatuza.h"

Lollapatuza::Lollapatuza(const map<IdPuesto,Puesto>& p, const set<Persona>& as){
    map< Persona, Nat*> diccPersonas;
    //Inicia el gasto de cada persona en 0
    for(Persona x:as){
        gasto_Personas.insert({x,0});
        diccPersonas.insert({x, nullptr});
    }
    //Inicia el diccionario de gastos en 0 con significado todas las personas.
    mas_gasto = gastos.insert({0,diccPersonas}).first;
    puestos = map<IdPuesto, Puesto>(p);
    personas = set<Persona>(as);
}

//En las funciones de obtener personas y puestos, realizamos un cambio con respecto al diseño del TP2.
//Antes entregabamos un iterador al primer elemento de cada estructura pero a la hora de respetar la fachada,no teniamos
//una cota que nos indique hasta donde recorrer, por lo tanto lo reemplazamos entregando las estructuras
//por referencia respetando la complejidad pedida O(1).
const set<Persona>& Lollapatuza::obtenerPersonas() const{
    return personas;
}

const map<IdPuesto, Puesto>& Lollapatuza::obtenerPuestos() const{
    return puestos;
}

void Lollapatuza::registrarCompra(IdPuesto idPuesto, Persona persona, Producto item, Nat cant){
    Puesto& p = puestos.at(idPuesto);
    Nat viejoGasto = gasto_Personas.at(persona);
    //Vender actualiza los datos del puesto con la nueva compra y devuelve el gasto de la misma.
    Nat gasto = p.vender(persona, item, cant);
    Nat descuento = p.obtenerDescuento(item,cant);
    //Suma el gasto al viejo gasto de la persona
    gasto_Personas[persona] = viejoGasto + gasto;
    //La siguiente función actualiza el diccionario de gastos.
    actualizarGastoPorCompra(viejoGasto, gasto, persona);
    if(descuento == 0){
        actualizarPuestosHackeables(idPuesto, p, item, persona);
    }
}

void Lollapatuza::actualizarGastoPorCompra(Nat viejoGasto, Nat gasto, Persona persona) {
    //Caso el nuevo gasto de la persona es mayor al mayor gasto del lolla.
    if(viejoGasto + gasto > mas_gasto->first){
        map<Persona , Nat*> personasMismoGasto;
        personasMismoGasto.insert({persona, nullptr});
        //Actualiza el iterador al nuevo mayor gasto del lolla.
        mas_gasto = gastos.insert({viejoGasto + gasto, personasMismoGasto}).first;
    } else {
        //Caso el nuevo gasto de la persona es igual al mayor gasto del lolla.
        if(viejoGasto + gasto == mas_gasto->first){
            //Se agrega la persona al diccionario de personas que tienen el mayor gasto.
            mas_gasto->second.insert({persona, nullptr});
        } else {
            //Caso el nuevo gasto de la persona es menor al mayor gasto del lolla.

            //Si ya existía alguien con ese gasto, se añade a la nueva persona al diccionario de personas
            //con mismo gasto. Sino, se define el nuevo gasto con un diccionario nuevo.
            if(gastos.count(viejoGasto + gasto) == 1){
                gastos.at(viejoGasto + gasto).insert({persona, nullptr});
            } else {
                map<Persona , Nat*> personasMismoGasto;
                personasMismoGasto.insert({persona, nullptr});
                gastos.insert({viejoGasto + gasto, personasMismoGasto});
            }
        }
    }

    map<Persona , Nat*> viejoGastoPersonas = gastos.at(viejoGasto);
    //Si en el viejo gasto solo había una persona, se borra el gasto de gastos.
    //Sino, solo se borra a la persona del diccionario de personas con el mismo viejo gasto.
    if(viejoGastoPersonas.size() == 1){
        gastos.erase(viejoGasto);
    } else {
        viejoGastoPersonas.erase(persona);
    }
}

void Lollapatuza::actualizarPuestosHackeables(IdPuesto idPuesto, Puesto &p, Producto item, Persona persona) {
    //Se guarda un iterador del elemento donde se encuentra definido idpuesto en la estructura puestos.
    map<IdPuesto, Puesto>::iterator itPuesto = puestos.insert({idPuesto, p}).first;
    //Caso la persona no compró sin descuento anteriormente en el lolla.
    if(compro_sin_descuento.count(persona) == 0){
        //Se crea un diccionario de idpuestos hackeables con el iterador a puestos.
        map<IdPuesto, map<IdPuesto , Puesto>::iterator> puestosSinDescuento;
        puestosSinDescuento.insert({idPuesto, itPuesto});
        //Se crea un diccionario de items con los puestos hackeables para ese item
        map< Producto, map< IdPuesto, map< IdPuesto, Puesto>::iterator>> itemsSinDescuento;
        itemsSinDescuento.insert({item, puestosSinDescuento});
        //Se inserta la persona con sus items y puestos hackeables.
        compro_sin_descuento.insert({persona, itemsSinDescuento});
    } else {
        //Caso la persona sí compró con descuento anteriormente en el lolla.
        map< Producto, map< IdPuesto, map< IdPuesto, Puesto>::iterator>> itemsSinDescuento = compro_sin_descuento.at(persona);
        //Caso la persona no compró ese item sin descuento anteriormente.
        if(itemsSinDescuento.count(item) == 0){
            //Se crea un diccionario de idpuestos hackeables con el iterador a puestos.
            map<IdPuesto, map<IdPuesto , Puesto>::iterator> puestosSinDescuento;
            puestosSinDescuento.insert({idPuesto, itPuesto});
            //Se inserta el item con sus puestos donde puede ser hackeado.
            itemsSinDescuento.insert({item, puestosSinDescuento});
        } else{
            //Caso la persona sí compró ese item sin descuento anteriormente.
            map<IdPuesto, map<IdPuesto , Puesto>::iterator>& puestosSinDescuento = itemsSinDescuento.at(item);
            //Se inserta el nuevo puesto hackeable en el diccionario de items hackeables de la persona.
            puestosSinDescuento.insert({idPuesto, itPuesto});
        }
    }
}

void Lollapatuza::hackear(Persona persona, Producto item){
    //Obtiene un iterador a los puestos hackeables para esa persona y ese item.
    map< IdPuesto, map< IdPuesto, Puesto>::iterator>& diccPuestos = compro_sin_descuento.at(persona).at(item);
    //Con begin se obtiene un iterador al primer elemento de los puestos hackeables. Como el iterador de map
    //es inorder, el primer elemento es el menor de todos.
    auto it = diccPuestos.begin();
    IdPuesto menorIDPUESTO = it->first;
    Puesto& puesto = it->second->second; //Accede al puesto a hackear de menor id.
    Nat precio = puesto.precioItem(item);
    //OlvidarItem actualiza los datos del puesto luego del hackeo a la persona y el item.
    puesto.olvidarItem(persona, item);
    Nat gastoTotal = gasto_Personas.at(persona);
    //Se reduce el gasto total de la persona restando el costo del item hackeado.
    gasto_Personas[persona] = gastoTotal - precio;
    //La siguiente función actualiza el diccionario de gastos.
    actualizarGastoPorHackeo(gastoTotal, precio, persona);
    //Si el puesto ya no es hackeable, se lo elimina de los puestos hackeables.
    if(!(puesto.esHackeable(persona, item))){
        diccPuestos.erase(menorIDPUESTO);
    }
}

void Lollapatuza::actualizarGastoPorHackeo(Nat gastoTotal, Nat precio, Persona persona) {
    map<Persona, Nat*> personasMaxGasto = mas_gasto->second;
    //Si la persona era la única que más gastó en el lolla, se debe buscar a la siguiente persona que más haya
    //gastado.
    if(personasMaxGasto.size() == 1 && personasMaxGasto.count(persona) == 1){
        //Guarda el maximo anterior del diccionario, pues el iterador siempre apunta al mayor y el dicc es inorder.
        Nat MaximoAnterior = prev(mas_gasto)->first;
        //Caso el nuevo gasto de la persona es igual al próximo mayor gasto del lolla
        if(gastoTotal - precio == MaximoAnterior){
            //Se agrega la persona al diccionario de personas con el mismo gasto
            prev(mas_gasto)->second[persona] = nullptr;
            mas_gasto--; //Se retrocede el iterador para que apunte al nuevo máximo gasto del lolla.
        } else {
            //Caso la persona sigue siendo la única de mayor gasto
            if(gastoTotal - precio > MaximoAnterior){
                map<Persona, Nat*> tienenNuevoMaxGasto; //Nuevo dicc de personas que comparten mayor gasto
                tienenNuevoMaxGasto.insert({persona, nullptr}); //Se inserta a la persona
                //Se inserta el nuevo gasto en gastos y guardamos el iterador en mas_gasto
                mas_gasto = gastos.insert({gastoTotal - precio, tienenNuevoMaxGasto}).first;
            } else {
                //Caso la persona ya no es la de mayor gasto.
                mas_gasto--; //Se retrocede el iterador para que apunte al nuevo máximo gasto del lolla.
                //Si el nuevo gasto ya estaba definido, se agrega a la persona
                if(gastos.count(gastoTotal - precio) == 1){
                    gastos.at(gastoTotal - precio)[persona] = nullptr;
                } else {
                    //Si el nuevo gasto no estaba definido, se define con un dicc que contiene a la persona.
                    map<Persona , Nat*> personasMismoGasto;
                    personasMismoGasto[persona] = nullptr;
                    gastos[gastoTotal - precio] = personasMismoGasto;
                }
            }
        }
    } else{
        //Si la persona no era la de mayor gasto, simplemente se agrega el nuevo gasto al diccionario si no estaba def.
        //o se agrega a la persona si lo estaba.
        if(gastos.count(gastoTotal - precio) == 1){
            gastos.at(gastoTotal - precio)[persona] = nullptr;
        } else {
            map<Persona , Nat*> personasMismoGasto;
            personasMismoGasto[persona] = nullptr;
            gastos[gastoTotal - precio] = personasMismoGasto;
        }
    }

    map<Persona , Nat*> viejoGastoPersonas = gastos.at(gastoTotal);
    //Si en el viejo gasto solo había una persona, se borra el gasto de gastos.
    //Sino, solo se borra a la persona del diccionario de personas con el mismo viejo gasto.
    if(viejoGastoPersonas.size() == 1 ){
        gastos.erase(gastoTotal);
    } else {
        viejoGastoPersonas.erase(persona);
    }
}

const Nat Lollapatuza::gastoTotal(Persona persona) const{
    return gasto_Personas.at(persona);
}

const Persona Lollapatuza::masGasto() const{
    map< Persona, Nat*> diccPersonas = mas_gasto->second; //Obtiene el dicc de las personas con mayor gasto.
    map< Persona, Nat*>::iterator it = diccPersonas.begin(); // .begin() entrega el iterador a la de menor id.
    return it->first;
}

const IdPuesto Lollapatuza::menorStock(Producto item) const{
    auto it = puestos.begin();
    bool hayAlguno = false;
    IdPuesto res = it->first; //Como el map es inorder, el primer puesto es el de mínima id.
    Puesto puestoRes = it->second;
    //Itera los puestos del lolla buscando el de menor stock
    while(it != puestos.end()){
        if(it->second.perteneceAlMenu(item)){
            if(hayAlguno == false){ //Caso es el primer puesto que se encuentra que venda el item.
                res = it->first;
                puestoRes = it->second;
                hayAlguno = true;
            } else { //Si encuentra un nuevo puesto que venda el item, comparo los stocks del viejo y del nuevo.
                //obtenerStock se indefine si el item no pertenece al menú.
                Nat stockRes = puestoRes.obtenerStock(item);
                Nat stockNuevo = it->second.obtenerStock(item);
                //Se queda con el puesto de menor stock o, si son iguales, con el de menor id, pues se itera inorder.
                if(stockNuevo < stockRes){
                    res = it->first;
                    puestoRes = it->second;
                }
            }
        }
        it++;
    }
    return res;
}