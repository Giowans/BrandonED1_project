#include "ciudad.h"
Ciudad::Ciudad()
{
    //do nothing;
}
void Ciudad::setNombre(string n)
{
    nombre = n;
}

void Ciudad::setAreaCiudad(int ac)
{
    area_ciudad = ac;
}

void Ciudad::setHabitantes(int nh)
{
    habitantes = nh;
}

string Ciudad::getNombre()
{
    return nombre;
}

int Ciudad::getHabitantes()
{
    return habitantes;
}

int Ciudad::getAreaCiudad()
{
    return area_ciudad;
}

int Ciudad::getDistancia()
{
    return distancia;
}

void Ciudad::setDistancia(int d)
{
    distancia = d;
}

list<Ciudad>& Ciudad::getCiudaristas()
{
    return ciudaristas;
}

void Ciudad::mostrarCiudaristas()
{
    if(ciudaristas.size() > 0)
    {
        cout << "\t CARRETERAS DE "<< nombre <<endl<<endl;
        for(auto it = ciudaristas.begin(); it != ciudaristas.end(); it++)
        {
            Ciudad aux = *it;
            cout << "  -.carretera a: " <<aux.getNombre()<<endl;
            cout << "  -.distancia: "<<aux.getDistancia()<< " km"<<endl<<endl;
        }
    }
    else
    {
        cout << "** Esta ciudad aun no tiene carreteras con otras ciudades **" <<endl;
    }
}

void Ciudad::insertarCiudarista(Ciudad ciudad, int distancia)
{
    Ciudad aux;
    aux.setNombre(ciudad.getNombre());
    aux.setAreaCiudad(ciudad.getAreaCiudad());
    aux.setHabitantes(ciudad.getHabitantes());
    aux.setDistancia(distancia);
    ciudaristas.push_back(aux);
}

void Ciudad::eliminarCiudaristas(string nombreCiudad)
{
    for (auto it = ciudaristas.begin(); it != ciudaristas.end(); it++)
    {
        Ciudad aux = *it;
        if (nombreCiudad.compare(aux.getNombre())==0)
        {
            ciudaristas.erase(it);
        }      
    }
}

bool Ciudad::existeCiudarista(string nombreCiudad)
{
    for (auto it = ciudaristas.begin(); it != ciudaristas.end(); it++)
    {
        Ciudad aux = *it;
        if (nombreCiudad.compare(aux.getNombre())==0)
        {
            return true;
        }      
    }
    return false;
}