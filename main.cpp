#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <map>
#include "pair.h"
#include "ciudad.h"
#include <string>
using namespace std;

list<Ciudad> ciudades_grafo;

Ciudad capturarCiudad()
{
    int num_habitantes, area_ciudad;
    string nombre_ciudad;
    Ciudad nuevaCiudad;
    system("cls");
    cout << "\t\t\t CREAR UNA NUEVA CIUDAD"<<endl<<endl;
    cout << "Nombre de la ciudad: ";
    getline(cin, nombre_ciudad);
    do
    {
        cout << endl <<"Numero de habitantes: ";
        cin >> num_habitantes;
        if (num_habitantes <=0)
        {
            cout << endl << "** El numero de habitantes debe de ser mayor que 0 **";
        }
    } while (num_habitantes <= 0);
    do
    {
        cout << endl << "Area (en km2) de la ciudad: ";
        cin >> area_ciudad;
        if(area_ciudad < 15 || area_ciudad > 9000)
        {
            cout << endl << "** El area de la ciudad debe ser minimo de 15 km2 y maximo de 9000 km2**";
        }
    } while (area_ciudad < 15 || area_ciudad > 9000);
    nuevaCiudad.setNombre(nombre_ciudad);
    nuevaCiudad.setHabitantes(num_habitantes);
    nuevaCiudad.setAreaCiudad(area_ciudad);
    return nuevaCiudad;
}

void mostrarCiudades()
{
    cout << "\t\t\t LISTA DE CIUDADES "<<endl<<endl;
    cout << "@@@---------------------@@@" <<endl;
    for(auto it = ciudades_grafo.begin(); it != ciudades_grafo.end(); it++)
    {
        Ciudad aux = *it;
        cout << "nombre: " <<aux.getNombre()<<endl;
        cout << "habitantes: "<<aux.getHabitantes()<<endl;
        cout << "area de la ciudad (km2): "<<aux.getAreaCiudad()<<endl<<endl;
        aux.mostrarCiudaristas();
        cout << "@@@---------------------@@@" <<endl;
    }
}

Ciudad& recuperaCiudad(string nombreBuscar)
{
    Ciudad *aux;
    for (auto it = ciudades_grafo.begin(); it != ciudades_grafo.end(); it++)
    {
        aux = &(*it);
        if (nombreBuscar.compare((*aux).getNombre()) == 0)
        {
            return *it;
        }      
    }
    return *aux;
}

bool existeCiudad(string nombreBuscar)
{
    for (auto it = ciudades_grafo.begin(); it != ciudades_grafo.end(); it++)
    {
        Ciudad aux = *it;
        if (nombreBuscar.compare(aux.getNombre()) == 0)
        {
            return true;
        }      
    }
    return false;
}

string busquedaEnProdundidad(string recorrido, Ciudad c)
{
    string camino = recorrido + c.getNombre() + " -> ";
    for (auto it = c.getCiudaristas().begin() ; it != c.getCiudaristas().end(); it++)
    {
        Ciudad v = recuperaCiudad((*it).getNombre());

        if(camino.find(v.getNombre()) == string::npos)
        {
            camino = busquedaEnProdundidad(camino, v);
            camino = camino + c.getNombre () + " -> ";
        }
    }
    return camino;
}

string busquedaEnAmplitud(string recorrido, Ciudad &c, queue<Ciudad> &cola)
{
    cola.push(c);
    while (!cola.empty())
    {
        Ciudad frente = cola.front();
        recorrido = recorrido + frente.getNombre() + " -> ";
        for (auto it = frente.getCiudaristas().begin(); it != frente.getCiudaristas().end(); it++)
        {
            Ciudad v = recuperaCiudad((*it).getNombre());
            if(recorrido.find(v.getNombre()) == string::npos)
            {
                recorrido = recorrido + v.getNombre() + " -> ";
                recorrido = recorrido + frente.getNombre() + " -> ";
                cola.push(v);
            }
        }
        cola.pop();
    }
    recorrido = recorrido + "\b\b\b\b.   ";
    return recorrido;
}

//Funcion que verifica si todos los vertices son definitivos en el ARM dijstra, si es asi entonces se ha
//acabado de construir el árbol
bool solucionVD(map<string, Trio> vector)
{
    for (auto it = ciudades_grafo.begin(); it != ciudades_grafo.end(); it++)
    {
        Ciudad aux = *it;
        if (!vector[aux.getNombre()].definitivo)
        {
            return false;
        }
    }
    return true;
}

//Selecciona al vertice cuya arista con el vertice del parametro es la menor posible
string seleccionaDefinitivo( map<string, Trio> vectorP ,Ciudad v_a)
{
    string seleccionando = "holi";
    Ciudad ciudadMenor;
    ciudadMenor.setDistancia(9999);
    list<Ciudad> aux = v_a.getCiudaristas();
    for(auto it = aux.begin(); it != aux.end(); it++)
    {
        Ciudad actual = *it;
        //si en el ciclo, la arista obtenida tiene menor disntancia que la previa, entonces se actualiza
        if((actual.getDistancia() < ciudadMenor.getDistancia()) && (!vectorP[actual.getNombre()].definitivo))
        {
            ciudadMenor.setNombre(actual.getNombre());
            ciudadMenor.setDistancia(actual.getDistancia());
            seleccionando = actual.getNombre();
        }
    }
    return seleccionando;
}

map<string, Trio> dijkstra(Ciudad c)
{
    map<string, Trio> vectorPesos;
    //inicializamos el vector de pesos
    for(auto it = ciudades_grafo.begin(); it != ciudades_grafo.end(); it++)
    {
        Ciudad aux = *it;
        if(aux.getNombre().compare(c.getNombre()) == 0)
        {
            vectorPesos[aux.getNombre()].definitivo = false;
            vectorPesos[aux.getNombre()].peso = 0;
            vectorPesos[aux.getNombre()].hizoDefinitvo = "";
        }
        else
        {
            vectorPesos[aux.getNombre()].definitivo = false;
            vectorPesos[aux.getNombre()].peso = 9999;
            vectorPesos[aux.getNombre()].hizoDefinitvo = "";
        }
    }
    //Aqui, antes de empezar a hacer dijstrka debemos de hacer nuestro vertice seleccionado el vertice inicial
    Ciudad v_s = c;
    while(!(solucionVD(vectorPesos)))
    {
        //Actualizamos el vector de pesos
        vectorPesos[v_s.getNombre()].definitivo = true;
        for(auto it = v_s.getCiudaristas().begin(); it != v_s.getCiudaristas().end(); it++)
        {
            //"actual" se refiere a cada arista para el vertice seleccionado
            Ciudad actual = *it;
            //si el peso de la arista mas el peso anteriormente actualizado del vertice seleccionado es menor entonces se actualiza en nuestro vector de pesos.
            if((actual.getDistancia() + vectorPesos[v_s.getNombre()].peso) < vectorPesos[actual.getNombre()].peso)
            {
                vectorPesos[actual.getNombre()].peso = actual.getDistancia() + vectorPesos[v_s.getNombre()].peso;
                vectorPesos[actual.getNombre()].hizoDefinitvo = v_s.getNombre();
            }
        }
        //Seleccionamos el siguiente vertice en base a la arista de menor peso del vertice actualizado
        v_s = recuperaCiudad(seleccionaDefinitivo(vectorPesos ,v_s));
    }
    return vectorPesos;
}

string imprimeARM(map<string, Trio> vectorPesos, string aImprimir, int faseRecursiva, string ciudadOrigen)
{
    int fR= faseRecursiva + 1;
    string estaFase = aImprimir + "\n|";
    for (auto it = ciudades_grafo.begin(); it != ciudades_grafo.end(); it++)
    {
        Ciudad aux = *it;
        if (vectorPesos[aux.getNombre()].hizoDefinitvo.compare(ciudadOrigen) == 0)
        {
            for(int i = 0; i < fR; i++)
            {
                estaFase = estaFase + "---";
            }
            estaFase = estaFase +" "+aux.getNombre() + " -- peso: " + to_string(vectorPesos[aux.getNombre()].peso) + " km";
            estaFase = imprimeARM(vectorPesos, estaFase, fR, aux.getNombre());
        }
    }
    return estaFase;
}

void crearJalisco()
{
    Ciudad nuevaCiudad;
    vector<Ciudad> v;
    nuevaCiudad.setNombre("Guadalajara");
    nuevaCiudad.setAreaCiudad(4000);
    nuevaCiudad.setHabitantes(15000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Tlajomulco");
    nuevaCiudad.setAreaCiudad(5000);
    nuevaCiudad.setHabitantes(9000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Zapopan");
    nuevaCiudad.setAreaCiudad(6000);
    nuevaCiudad.setHabitantes(18000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Tonala");
    nuevaCiudad.setAreaCiudad(8000);
    nuevaCiudad.setHabitantes(12000);
    v.push_back(nuevaCiudad);
    //Añadimos carreteras entre las ciudades
    //Guadalajara - Tlajomulco
    v[0].insertarCiudarista(v[1], 50);
    v[1].insertarCiudarista(v[0], 50);
    //Guadalajara - Zapopan
    v[0].insertarCiudarista(v[2], 40);
    v[2].insertarCiudarista(v[0], 40);
    //Guadalajara - Tonalá
    v[0].insertarCiudarista(v[3], 30);
    v[3].insertarCiudarista(v[0], 30);
    //Tonalá - Tlajomulco
    v[3].insertarCiudarista(v[1], 60);
    v[1].insertarCiudarista(v[3], 60);
    //Zapopan - Tlajomulco
    v[2].insertarCiudarista(v[1], 70);
    v[1].insertarCiudarista(v[2], 70);
    //Añadimos el contenido del vector a la lista de listas que es nuestro grafo
    for (size_t i = 0; i < v.size(); i++)
    {
        ciudades_grafo.push_back(v[i]);
    }
}

void crearSinaloa()
{
    Ciudad nuevaCiudad;
    vector<Ciudad> v;
    nuevaCiudad.setNombre("Culiacan");
    nuevaCiudad.setAreaCiudad(8900);
    nuevaCiudad.setHabitantes(19000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Mazatlan");
    nuevaCiudad.setAreaCiudad(5000);
    nuevaCiudad.setHabitantes(9000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Los Mochis");
    nuevaCiudad.setAreaCiudad(4500);
    nuevaCiudad.setHabitantes(5000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("El Fuerte");
    nuevaCiudad.setAreaCiudad(8000);
    nuevaCiudad.setHabitantes(12000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Rosario");
    nuevaCiudad.setAreaCiudad(3500);
    nuevaCiudad.setHabitantes(7500);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Guasave");
    nuevaCiudad.setAreaCiudad(6000);
    nuevaCiudad.setHabitantes(10000);
    v.push_back(nuevaCiudad);
    //Añadimos carreteras entre las ciudades
    //Culiacán - Los Mochis
    v[0].insertarCiudarista(v[2], 40);
    v[2].insertarCiudarista(v[0], 40);
    //Culiacán - Mazatlán
    v[0].insertarCiudarista(v[1], 20);
    v[1].insertarCiudarista(v[0], 20);
    //Culiacan - El Fuerte
    v[0].insertarCiudarista(v[2], 35);
    v[2].insertarCiudarista(v[0], 35);
    //Culiacan - Guasave
    v[0].insertarCiudarista(v[5], 30);
    v[5].insertarCiudarista(v[0], 30);
    //Los Mochis - Mazatlán
    v[2].insertarCiudarista(v[1], 50);
    v[1].insertarCiudarista(v[2], 50);
    //Rosario - El Fuerte
    v[4].insertarCiudarista(v[3], 10);
    v[3].insertarCiudarista(v[4], 10);
    //Rosario - Guasave
    v[4].insertarCiudarista(v[5], 25);
    v[5].insertarCiudarista(v[4], 25);
    //Añadimos el contenido del vector a la lista de listas que es nuestro grafo
    for (size_t i = 0; i < v.size(); i++)
    {
        ciudades_grafo.push_back(v[i]);
    }
}

void crearTexas()
{
    Ciudad nuevaCiudad;
    vector<Ciudad> v;
    nuevaCiudad.setNombre("Austin");
    nuevaCiudad.setAreaCiudad(8900);
    nuevaCiudad.setHabitantes(19000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("San Antonio");
    nuevaCiudad.setAreaCiudad(5000);
    nuevaCiudad.setHabitantes(9000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Houston");
    nuevaCiudad.setAreaCiudad(4500);
    nuevaCiudad.setHabitantes(5000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("Dallas");
    nuevaCiudad.setAreaCiudad(8000);
    nuevaCiudad.setHabitantes(12000);
    v.push_back(nuevaCiudad);
    nuevaCiudad.setNombre("El Paso");
    nuevaCiudad.setAreaCiudad(3500);
    nuevaCiudad.setHabitantes(7500);
    v.push_back(nuevaCiudad);
    //Añadimos carreteras entre las ciudades
    //Austin - San Antonio
    v[0].insertarCiudarista(v[1], 10);
    v[1].insertarCiudarista(v[0], 10);
    //Austin - Houston 
    v[0].insertarCiudarista(v[2], 30);
    v[2].insertarCiudarista(v[0], 30);
    //Austin - Dallas
    v[0].insertarCiudarista(v[3], 20);
    v[3].insertarCiudarista(v[0], 20);
    //Austin - El Paso
    v[0].insertarCiudarista(v[4], 70);
    v[4].insertarCiudarista(v[0], 70);
    //Houston - Dallas
    v[2].insertarCiudarista(v[3], 35);
    v[3].insertarCiudarista(v[2], 35);
    //Austin - San Antonio
    v[2].insertarCiudarista(v[1], 25);
    v[1].insertarCiudarista(v[2], 25);
    //El Paso - Dallas
    v[4].insertarCiudarista(v[3], 90);
    v[3].insertarCiudarista(v[4], 90);
    //El Paso - San Antonio
    v[4].insertarCiudarista(v[1], 80);
    v[1].insertarCiudarista(v[4], 80);
    //Añadimos el contenido del vector a la lista de listas que es nuestro grafo
    for (size_t i = 0; i < v.size(); i++)
    {
        ciudades_grafo.push_back(v[i]);
    }
}

int main()
{
    int opc;
    do
    {
        system("cls");
        cout << "Antes de continuar, dime si quieres inicializar tu grafo: "<<endl<<endl;
        cout << "\t1-.) Inicializar con ciudades de Jalisco"<<endl;
        cout << "\t2-.) Inicializar con ciudades de Sinaloa"<<endl;
        cout << "\t3-.) Inicializar con ciudades de Texas"<<endl;
        cout << "\t4-.) Prefiero iniciar desde cero un estado/region..."<<endl<<endl;
        cout << "Ingrese una opci"<<char(162)<<"n del menu: ";
        cin >> opc;
        switch (opc)
        {
            case 1:
                crearJalisco();
                cout <<endl<< "Estado de Jalisco creado :D"<<endl;
                break;
            
            case 2:
                crearSinaloa();
                cout <<endl<< "Estado de Sinaloa creado :D"<<endl;
                break;
            
            case 3:
                crearTexas();
                cout <<endl<< "Estado de Jalisco creado :D"<<endl;
                break;
            
            case 4:
                cout <<endl<< "Ok, decidido..."<<endl;
                break;

            default:
                opc = 0;
                cout <<endl<< "Por favor, ingresa una opcion de la lista ----***"<<endl;
                break;
        }
        system("pause");
    } while (opc < 1);
    opc = 0;
    do
    {
        system("cls");
        cout <<"\t\t\t\t EL GRAFO DE LAS CIUDADES by BRANDON ;U"<<endl<<endl;
        cout << "\tMenu: "<<endl;
        cout << "\t\t1-.) Mostrar ciudades existentes."<<endl;
        cout << "\t\t2-.) Eliminar ciudad."<<endl;
        cout << "\t\t3-.) Agregar ciudad."<<endl;
        cout << "\t\t4-.) A"<<char(164)<<"adir carretera entre ciudades."<<endl;
        cout << "\t\t5-.) Busqueda en profundidad."<<endl;
        cout << "\t\t6-.) Busqueda en amplitud."<<endl;
        cout << "\t\t7-.) ARM de una ciudad." << endl;
        cout << "\t\t8-.) Salir."<<endl<<endl;
        cout << "Ingrese una opci"<<char(162)<<"n del menu: ";
        cin >> opc;
        cin.ignore();
        switch(opc)
        {
            case 1:
                if (ciudades_grafo.size() > 0)
                {
                    system("cls");
                    mostrarCiudades();
                    cout <<endl<<endl;
                }
                else
                {
                    cout << endl << endl << " ** No existen ciuades en el estado-grafo aun **";
                }
                system("pause");
                break;
            case 2:
            {
                if(ciudades_grafo.size() > 0)
                {
                    system("cls");
                    cout << "\t\t\t ELIMINAR CIUDAD"<<endl<<endl;
                    string nombreAbuscar = "";
                    cout << "nombre de la ciudad en la lista: ";
                    getline(cin, nombreAbuscar);
                    if(existeCiudad(nombreAbuscar))
                    {
                        Ciudad ciudad_encontrada = recuperaCiudad(nombreAbuscar);
                        for (auto it = ciudades_grafo.begin(); it != ciudades_grafo.end() ; it++)
                        {
                            Ciudad aux = *it;
                            aux.eliminarCiudaristas(nombreAbuscar);
                            if (aux.getNombre().compare(ciudad_encontrada.getNombre()) == 0)
                            {
                                ciudades_grafo.erase(it);
                            }
                        }
                        cout << endl << "la ciudad se ha eliminado :D.  " << endl;
                    }
                    else
                    {
                        cout << endl << "No se pudo encontrar la ciudad a eliminar :(.  " << endl;
                    }
                }
                else
                {
                    cout << endl << endl << "** No existen ciudades que eliminar **" <<endl;
                }
                system("pause");
                break;
            }
            case 3:
                ciudades_grafo.push_back(capturarCiudad());
                cout << endl << endl<< "Se ha añadido una nueva ciudad :) ---"<<endl;
                system("pause");
                break;
            case 4:
            {
                if (ciudades_grafo.size() > 1)
                {
                    bool flag;
                    Ciudad c1, c2;
                    string nombre_c1, nombre_c2;
                    int distancia;
                    system("cls");
                    cout << "\t\t\t AÑADIR CARRETERA ENTRE CIUDADES" <<endl;
                    do
                    {
                        cout << endl << "nombre de la primer ciudad: ";
                        getline(cin, nombre_c1);
                        flag = existeCiudad(nombre_c1);
                        if (!flag)
                        {
                            cout << endl<< "No existe una ciudad con ese nombre. Intenta nuevamente";
                        }
                    } while (!flag);
                    c1 = recuperaCiudad(nombre_c1);
                    do
                    {
                        cout << endl << "nombre de la segunda ciudad: ";
                        getline(cin, nombre_c2);
                        flag = existeCiudad(nombre_c2);
                        if (!flag)
                        {
                            cout << endl<< "No existe una ciudad con ese nombre. Intenta nuevamente";
                        }
                    } while (!flag);
                    c2 = recuperaCiudad(nombre_c2);
                    if(c1.existeCiudarista(nombre_c2))
                    {
                        cout << endl << "** Actualmente ya existe una carretera enntre estas dos ciudades **" << endl;
                    }
                    else
                    {
                        do
                        {
                            cout << endl <<"distancia entre las ciudades (longitud de la carretera en KM): ";
                            cin >> distancia;
                            if (distancia <=0)
                            {
                                cout << endl << "** la carretera debe de tener como minimo 1 KM de longitud **";
                            }
                        } while (distancia <= 0);
                        //si los datos pasan sobre las validaciones entonces insertamos la nueva carretera en ambas ciudades
                        recuperaCiudad(nombre_c1).insertarCiudarista(c2, distancia);
                        recuperaCiudad(nombre_c2).insertarCiudarista(c1, distancia);
                        cout << endl << "Se ha creado la nueva carretera " << c1.getNombre() << " - " << c2.getNombre() << " :D ----"<<endl;
                    }               

                }
                else
                {
                    cout << endl << " ** Deben de existir al menos 2 ciudades para hacer una carretera **"<<endl;
                }
                
            }
            system("pause");   
            break;
            case 5:
            {
                if(ciudades_grafo.size() > 2)
                {
                    bool flag;
                    Ciudad c;
                    string nombre_ciudad, recorrido;
                    int distancia;
                    system("cls");
                    cout << "\t\t\t BUSQUEDA EN PROFUNDIDAD" <<endl;
                    do
                    {
                        cout << endl << "nombre de la ciudad que sera el punto de partida: ";
                        getline(cin, nombre_ciudad);
                        flag = existeCiudad(nombre_ciudad);
                        if(!flag)
                        {
                            cout << endl<< "No existe una ciudad con ese nombre. Intenta nuevamente";
                        }
                    } while (!flag);
                    c = recuperaCiudad(nombre_ciudad);
                    recorrido = busquedaEnProdundidad("", c);
                    recorrido = recorrido + "\b\b\b\b.   ";
                    cout << endl << "-----**-------**------**" <<endl;
                    cout << "La busqueda ha finalizado. El recorrido fue el siguiente:"<<endl<<recorrido<<endl<<endl;
                }
                else
                {
                    cout << endl << " ** Deben de existir al menos 2 ciudades y una carretera para poder hacer la busqueda **"<<endl;
                }
                
            }
            system("pause");
            break;
            case 6:
            {
                if(ciudades_grafo.size() > 2)
                {
                    bool flag;
                    Ciudad c;
                    string nombre_ciudad, recorrido;
                    int distancia;
                    system("cls");
                    cout << "\t\t\t BUSQUEDA EN AMPLITUD" <<endl;
                    do
                    {
                        cout << endl << "nombre de la ciudad que sera el punto de partida: ";
                        getline(cin, nombre_ciudad);
                        flag =  existeCiudad(nombre_ciudad);
                        if (!flag)
                        {
                            cout << endl<< "No existe una ciudad con ese nombre. Intenta nuevamente";
                        }
                    } while (!flag);
                    c = recuperaCiudad(nombre_ciudad);
                    queue<Ciudad> cola;
                    recorrido = busquedaEnAmplitud("", c, cola);
                    cout << endl << "-------.....------_______------......---------"<< endl;
                    cout << "La busqueda ha finalizado. El recorrido fue el siguiente:"<<endl<< endl <<recorrido<<endl<<endl;
                }
                else
                {
                    cout << endl << " ** Deben de existir al menos 2 ciudades y una carretera para poder hacer la busqueda **"<<endl;
                }
                
            }
            system("pause");
            break;
            case 7:
            {
                if(ciudades_grafo.size() > 2)
                {
                    bool flag;
                    Ciudad c;
                    string nombre_ciudad;
                    system("cls");
                    cout << "\t\t\t BUSQUEDA DEL CAMINO MAS CORTO (DIJKSTRA)" <<endl;
                    do
                    {
                        cout << endl << "nombre de la ciudad que sera el punto de partida: ";
                        getline(cin, nombre_ciudad);
                        flag = existeCiudad(nombre_ciudad);
                        if (!flag)
                        {
                            cout << endl<< "No existe una ciudad con ese nombre. Intenta nuevamente";
                        }
                    } while (!flag);
                    c = recuperaCiudad(nombre_ciudad);
                    cout << "------- ARBOL DE RECUBRIMIENTO MINIMO ------ "<<endl<<endl;
                    map<string, Trio> dijkstraResultado = dijkstra(c); 
                    string arm = imprimeARM(dijkstraResultado, "^^^^^ raiz: " + c.getNombre() + "\n|", 0, c.getNombre());
                    cout << arm << endl;
                }
                else
                {
                    cout << endl <<  " ** Deben existir al menos 2 ciudades y una carretera entre ellas para generar el ARM" <<endl;
                }    
            }
            system("pause");
            break;
            case 8:
            {
                cout << endl << "Vuelve cuando quieras :D ----." <<endl;
                system("pause");
            }
        }
    } while (opc != 8);
    
    return 0;
}