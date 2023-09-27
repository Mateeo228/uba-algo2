#include "fachada_lollapatuza.h"

//Adapta la estructura de puestos de entrada a la solicitada por el constructor del lolla.
map<IdPuesto, Puesto> FachadaLollapatuza::convertirPuestos(const map<IdPuesto, aed2_Puesto>& infoPuestos){
    map<IdPuesto, Puesto> puestos;
    for (pair<IdPuesto,aed2_Puesto> x :infoPuestos) {
        puestos.insert({x.first, Puesto(x.second.menu,x.second.stock,x.second.promociones)});
    }
    return puestos;
}

FachadaLollapatuza::FachadaLollapatuza(const set<Persona> &personas, const map<IdPuesto, aed2_Puesto> &infoPuestos):
    lolla(convertirPuestos(infoPuestos),personas){}

void FachadaLollapatuza::registrarCompra(Persona persona, Producto producto, Nat cant, IdPuesto idPuesto) {
    lolla.registrarCompra(idPuesto, persona,producto, cant);
}

void FachadaLollapatuza::hackear(Persona persona, Producto producto) {
    lolla.hackear(persona, producto);
}

Nat FachadaLollapatuza::gastoTotal(Persona persona) const {
    return lolla.gastoTotal(persona);
}

Persona FachadaLollapatuza::mayorGastador() const {
    return lolla.masGasto();
}

IdPuesto FachadaLollapatuza::menorStock(Producto producto) const {
    return lolla.menorStock(producto);
}

const set<Persona> &FachadaLollapatuza::personas() const {
    return lolla.obtenerPersonas();
}

Nat FachadaLollapatuza::stockEnPuesto(IdPuesto idPuesto, const Producto &producto) const {
    map<IdPuesto, Puesto> diccPuestos = lolla.obtenerPuestos();
    for (pair<IdPuesto, Puesto> x: diccPuestos) {
        if(idPuesto == x.first){
            return x.second.obtenerStock(producto);
        }
    }
}

Nat FachadaLollapatuza::descuentoEnPuesto(IdPuesto idPuesto, const Producto &producto, Nat cantidad) const {
    map<IdPuesto, Puesto> diccPuestos = lolla.obtenerPuestos();
    for (pair<IdPuesto, Puesto> x: diccPuestos) {
        if(idPuesto == x.first){
            return x.second.obtenerDescuento(producto,cantidad);
        }
    }
}

Nat FachadaLollapatuza::gastoEnPuesto(IdPuesto idPuesto, Persona persona) const {
    map<IdPuesto, Puesto> diccPuestos = lolla.obtenerPuestos();
    for (pair<IdPuesto, Puesto> x: diccPuestos) {
        if(idPuesto == x.first){
            return x.second.obtenerGastoPersona(persona);
        }
    }
}

set<IdPuesto> FachadaLollapatuza::idsDePuestos() const {
    set<IdPuesto> p;
    map<IdPuesto, Puesto> diccPuestos = lolla.obtenerPuestos();
    for (pair<IdPuesto, Puesto> x: diccPuestos) {
        p.insert(x.first);
    }
    return p;
}