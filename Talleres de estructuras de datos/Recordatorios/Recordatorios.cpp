#include <iostream>
#include <list>

using namespace std;

using uint = unsigned int;

// Pre: 0 <= mes < 12
uint dias_en_mes(uint mes) {
    uint dias[] = {
        // ene, feb, mar, abr, may, jun
        31, 28, 31, 30, 31, 30,
        // jul, ago, sep, oct, nov, dic
        31, 31, 30, 31, 30, 31
    };
    return dias[mes - 1];
}

// Ejercicio 7, 8, 9 y 10

// Clase Fecha
class Fecha {
  public:
    Fecha(int mes, int dia);
    int mes();
    int dia();
    #if EJ >= 9 // Para ejercicio 9
    bool operator==(Fecha o);
    #endif
    void incrementar_dia();

  private:
    int _mes;
    int _dia;
};

Fecha::Fecha(int mes, int dia): _mes(mes), _dia(dia) {}

int Fecha::dia() {
    return _dia;
}

int Fecha::mes() {
    return _mes;
}

ostream& operator<<(ostream& os, Fecha f) {
    os << f.dia() << "/" << f.mes() ;
    return os;
}

#if EJ >= 9
bool Fecha::operator==(Fecha o) {
    bool igual_dia = this->dia() == o.dia();
    bool igual_mes = this->mes() == o.mes();
    return igual_dia && igual_mes;
}
#endif

void Fecha::incrementar_dia() {
    uint diasMax = dias_en_mes(_mes);
    if (_dia < diasMax){
        _dia++;
    } else {
        _mes++;
        _dia=1;
    }
}

// Ejercicio 11, 12
class Horario {
    public:
        Horario(uint hora, uint min);
        uint hora();
        uint min();
        bool operator==(Horario o);
        bool operator<(Horario h);
    private:
        uint _hora;
        uint _min;
};

Horario::Horario(uint hora, uint min): _hora(hora), _min(min) {}

uint Horario::min() {
    return _min;
}

uint Horario::hora() {
    return _hora;
}

ostream& operator<<(ostream& os, Horario h) {
    os << h.hora() << ":" << h.min() ;
    return os;
}

bool Horario::operator==(Horario h) {
    bool igual_hora = this->hora() == h.hora();
    bool igual_min = this->min() == h.min();
    return igual_hora && igual_min;
}

bool Horario::operator<(Horario h) {
    if(this->hora() < h.hora()){
        return true;
    }
    else if (this->hora() == h.hora() && this->min() < h.min()) {
        return true;
    }
    else return false;
}

// Ejercicio 13

// Clase Recordatorio
class Recordatorio {
    public:
        Recordatorio(Fecha f, Horario h, string s);
        Fecha f();
        Horario h();
        string s();
    private:
        Fecha _f;
        Horario _h;
        string _s;
};

Recordatorio::Recordatorio(Fecha f, Horario h, std::string s): _f(f), _h(h), _s(s) {}

Fecha Recordatorio::f() {
    return _f;
}

Horario Recordatorio::h() {
    return _h;
}

string Recordatorio::s() {
    return _s;
}

ostream& operator<<(ostream& os, Recordatorio r) {
    os << r.s() << " @ " << r.f() << " " << r.h() ;
    return os;
}

// Ejercicio 14

// Clase Agenda

class Agenda {
    public:
        Agenda(Fecha fecha_inicial);
        void agregar_recordatorio(Recordatorio rec);
        void incrementar_dia();
        list<Recordatorio> recordatorios_de_hoy();
        Fecha hoy();
    private:
        Fecha _fecha;
        list<Recordatorio> _list;
};

Agenda::Agenda(Fecha fecha_inicial): _fecha(fecha_inicial) {}

void Agenda::agregar_recordatorio(Recordatorio rec) {
    _list.push_back(rec);
}

void Agenda::incrementar_dia() {
    uint diasMax = dias_en_mes(_fecha.mes());
    uint dia = _fecha.dia();
    uint mes = _fecha.mes();

    if (dia < diasMax){
        _fecha = Fecha(mes, dia + 1);
    } else {
        _fecha = Fecha(mes + 1, 1);
    }
}

Fecha Agenda::hoy() {
    return _fecha;
}

list <Recordatorio> Agenda::recordatorios_de_hoy() {
    list<Recordatorio> rec_de_hoy;
    for ( Recordatorio recordatorio: _list) {
        if (recordatorio.f() == hoy()) {
            rec_de_hoy.push_back(recordatorio);
        }
    }
    return rec_de_hoy;
}

bool comparar(Recordatorio a, Recordatorio b){
    if ( a.h() < b.h()) {
        return true;
    }
    else return false;
}

ostream& operator<<(ostream& os, Agenda a) {
    list<Recordatorio> rec_de_hoy = a.recordatorios_de_hoy();
    rec_de_hoy.sort(comparar);

    os << a.hoy() << "\n";
    os << "=====" << "\n";
    for(Recordatorio recordatorio: rec_de_hoy){
        os << recordatorio << "\n";
    }
    return os;
}
