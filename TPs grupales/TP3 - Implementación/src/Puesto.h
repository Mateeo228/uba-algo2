#ifndef TP_LOLLA_PUESTO_H
#define TP_LOLLA_PUESTO_H

#include <set>
#include "tipos.h"
#include <vector>
#include <list>

class Puesto{
public:

    Puesto(const Menu& menu_, const Stock& stock_, const Promociones& promociones);
    const Nat obtenerStock(Producto item) const;
    const Nat obtenerDescuento(Producto item, Nat cant) const;
    const Nat obtenerGastoPersona(Persona persona) const;
    const Nat vender(Persona persona, Producto item, Nat cant);
    void olvidarItem(Persona persona, Producto item);
    const bool perteneceAlMenu(Producto item) const;
    const Nat precioItem(Producto item) const;
    const bool esHackeable(Persona persona, Producto item) const;
    Puesto operator=(const Puesto& puesto);

private:

    void actualizarComprasPersona(Producto item, Nat cant, Nat descuento, Persona persona);
    vector<Nat> crearVectorDescuentos(const map<Nat, Nat> &dicc);

    Menu menu;
    Stock stock;
    map<Producto, vector<Nat>> descuentos;
    map<Persona, Nat> gastos_personas;
    map<Persona, list< pair< Producto, Nat>>> ventas_descuento;
    map<Persona, map<Producto, vector<Nat>>> ventas_sin_descuento;

};
#endif //TP_LOLLA_PUESTO_H
