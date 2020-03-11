#ifndef CIUDAD_CLASS
#define CIUDAD_CLASS

#include <iostream>
#include <list>

using namespace std;

class Ciudad
{
    private:
        string nombre; 
        int habitantes;
        int area_ciudad;
        int distancia; 
        list<Ciudad> ciudaristas; //ciudades que mantienen una arista (union) con esta ciudad correspondiente.
    public:
         //constructor
         Ciudad();
         //métodos de interfaz
         void setNombre(string n);
         string getNombre();
         void setHabitantes(int habitantes);
         int getHabitantes();
         void setAreaCiudad(int area);
         int getAreaCiudad();
         void setDistancia(int d);
         int getDistancia();
         list<Ciudad>& getCiudaristas();
         
         //métodos de la clase
         void insertarCiudarista(Ciudad ciudad, int distancia);
         void mostrarCiudaristas();
         void eliminarCiudaristas(string nombreCiudad);
         bool existeCiudarista(string nombreCiudad);
};

#endif