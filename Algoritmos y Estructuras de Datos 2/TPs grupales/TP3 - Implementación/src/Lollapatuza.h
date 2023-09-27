#ifndef TP_LOLLA_LOLLAPATUZA_H
#define TP_LOLLA_LOLLAPATUZA_H

#include <set>
#include "tipos.h"
#include "Puesto.h"

class Lollapatuza{
public:

    Lollapatuza(const map<IdPuesto,Puesto>& puestos, const set<Persona>& personas);
    const set<Persona>& obtenerPersonas() const;
    const map<IdPuesto, Puesto>& obtenerPuestos() const;
    void registrarCompra(IdPuesto idPuesto, Persona persona, Producto item, Nat cant);
    void hackear(Persona persona, Producto item);
    const Nat gastoTotal(Persona persona) const;
    const Persona masGasto() const;
    const IdPuesto menorStock(Producto item) const;

private:

    void actualizarGastoPorCompra(Nat viejoGasto, Nat gasto, Persona persona);
    void actualizarGastoPorHackeo(Nat gastoTotal, Nat precio, Persona persona);
    void actualizarPuestosHackeables(IdPuesto idPuesto, Puesto& p, Producto item, Persona persona);

    map<IdPuesto, Puesto> puestos;
    set<Persona> personas;
    map<Persona, Nat> gasto_Personas;
    map<Nat, map< Persona, Nat*>> gastos;
    map<Nat, map< Persona, Nat*>>::iterator mas_gasto;
    map<Persona, map< Producto, map< IdPuesto, map< IdPuesto, Puesto>::iterator>>> compro_sin_descuento;

};

#endif //TP_LOLLA_LOLLAPATUZA_H
