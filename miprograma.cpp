#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include <pthread.h>
#include <unistd.h>

/*
    ****************************************
    * Nombre: Francisco Gonzalez Placenca  *
    * Sistemas Operativos -  Tarea 2       *
    ****************************************
                                                */
using namespace std;

const int TAMANO_ALMACEN_IP = 10000; // aumentar si se cree que es mas grande

string *alamacenar_resultados_ping = new string[TAMANO_ALMACEN_IP]; // Estructura donde se almacenan los resultados
void* realizarPing(void *param);
int contarIpTotales(string nombre_archivo_txt);
string ejecutarPing(char* comando_ping);
void imprimirResultadosPing(int ip_totales);

int main(int argc, char *argv[])
{
  
    int i = 0;
    string nombre_archivo_txt = argv[1];
    
    int ip_totales = contarIpTotales(nombre_archivo_txt);
    pthread_t threads[ip_totales];

    /* Crea todos los hilos */
    for (i=0; i < ip_totales; i++)
    {
        pthread_create(&threads[i], NULL, realizarPing,argv);
    }

    /* Para esperar por el término de todos los hilos */
    for (i=0; i < ip_totales; i++)
    {
        pthread_join(threads[i], NULL);
    }
    imprimirResultadosPing(ip_totales);
    return 0;
}

void *realizarPing(void *param)
{
  
    char **aux_param = (char**)param;
   
    string nombre_archivo = aux_param[1];
    string paquetes = aux_param[2];

    int ip_totales = contarIpTotales(nombre_archivo); // contar cuantas ip tiene el archivo	

    ifstream listado_ip;

    listado_ip.open(nombre_archivo,ios::in); // abrir archivo en modo lectura

    if(listado_ip.fail())
    {
        cout<<"No se pudo abrir el archivo";
        pthread_exit(0);
        exit(1);
    }

    int contador = 0;

    string ip; //se guarda la ip
    string ping; // donde se guarda el comando ping
    char* comando_ping; // donde se guardad el comando ping en char*


    while(!listado_ip.eof() && contador != ip_totales)  //mientras no sea final del archivo y no supere el numero de ip totales
    {
        getline(listado_ip,ip);
        ping = "ping -q -c"+ paquetes +" "+ ip; // comando ping + la ip obtenida del archivo

        comando_ping = const_cast<char*>(ping.c_str());
        string resultado_ping = ejecutarPing(comando_ping);
        alamacenar_resultados_ping[contador] = resultado_ping;
        contador++; //siguiente posicion del arreglo de string

    }

    listado_ip.close(); //cerramos el archivo
    pthread_exit(0);
}


// Obtener el numero de cantidad de ip en el archivo
int contarIpTotales(string nombre_archivo_txt)
{

    ifstream listado_ip;

    listado_ip.open(nombre_archivo_txt,ios::in); // abrir archivo en modo lectura
    if(listado_ip.fail())
    {
        cout<<"No se pudo abrir el archivo";
        exit(1);
    }
    int contador_ip = 0;
    string linea;
    while(getline(listado_ip,linea))
    {
        contador_ip++;
    }

    listado_ip.close(); //cerramos el archivo
    return contador_ip;
}


// ejecuta el comando ping en consola y retorna el resultado
string ejecutarPing(char* comando_ping)
{
    FILE* pipe = popen(comando_ping, "r");
    char buffer[128];
    string resultado_ping = "";
    if (!pipe)
    {
        return "Error al ejecutar el comando";
    }

    while(!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
            resultado_ping += buffer;
    }
    pclose(pipe);
    return resultado_ping;
}

void imprimirResultadosPing(int ip_totales)
{


    for(int i = 0; i < ip_totales ; i++){
     cout<<alamacenar_resultados_ping[i];
    }

}
