#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"


ST_medicion *crearMedicion(int hora, float min, float max, float prom, int contador)
{
    ST_medicion *medicion = (ST_medicion*) malloc(sizeof(ST_medicion));
    medicion->hora = hora;
    medicion->humedadMin = min;
    medicion->humedadMax = max;
    medicion->humedadProm = prom;
    medicion->contador = contador;
    return medicion;
}

FILE* OpenTxt(const char *mode){

    FILE *pArchivo=NULL;
    if((pArchivo = fopen("Archivo.txt",mode))==NULL)
            exit(EXIT_FAILURE);
    return pArchivo;
}
FILE* OpenDat(const char *mode){

    FILE *pArchivo=NULL;
    if((pArchivo = fopen("Archivo.dat",mode))==NULL)
            exit(EXIT_FAILURE);

    return pArchivo;
}


void resolverParcial()
{
    FILE *pArchivo = OpenTxt("r+");
    ST_archivoTxt arcTxt;

    char linea[100];
    fgets(linea, 100, pArchivo);

    while(!feof(pArchivo)){

        parcearLinea(linea,&arcTxt);

        float newMedicion = strAFloat(arcTxt);
        int hora = atoi(arcTxt.hora);

        calcula(hora, newMedicion);
        fgets(linea,100, pArchivo);
    }

    fclose(pArchivo);
}

void parcearLinea(char linea[100], ST_archivoTxt *datos)
{
    char *token = strtok(linea, ";");// token,id;
    token = strtok(NULL, ";"); //token,fecha
    strncpy(datos->fecha, token,L_fecha);

    token = strtok(NULL, ";");//token,hora
    strncpy(datos->hora, token,L_time);

    token = strtok(NULL, ";");//token, hasta coordenadas
    token = strtok(NULL, ",");// tomo el 0  de la medicion xq si lo paso 0,01 atof lo convierte en 0,00

    token = strtok(NULL, ";");//token, medicion despues de la coma
    strncpy(datos->medicion, token, L_medicion);
}


float strAFloat(ST_archivoTxt arcTxt)
{
    float aux = 0;
    float medicion = 0;

    aux = atof(arcTxt.medicion);
    medicion = aux/100;

    return medicion;
}

void calcula(int hora, float newMedicion)
{

    FILE *pArchivoDat = OpenDat("rb+");
    ST_medicion *medicion = (ST_medicion*) malloc(sizeof(ST_medicion));

    fseek(pArchivoDat, hora*sizeof(ST_medicion), SEEK_SET);
    fread(medicion, sizeof(ST_medicion), 1, pArchivoDat);

    if(newMedicion > medicion->humedadMax)
        medicion->humedadMax = newMedicion;

    if(newMedicion < medicion->humedadMin)
        medicion->humedadMin = newMedicion;

    medicion->contador += 1;
    medicion->humedadProm += newMedicion;

    if(medicion->contador >= mediciones_x_hora)
        medicion->humedadProm /= medicion->contador;

    fseek(pArchivoDat, hora*sizeof(ST_medicion), SEEK_SET);
    fwrite(medicion, sizeof(ST_medicion), 1, pArchivoDat);

    fclose(pArchivoDat);
}

void crearDat()
{
    FILE *pArchivoDat = OpenDat("wb");

    for(int i = 0; i < cant_horas; i++)
    {
        fwrite(crearMedicion(i,99,0,0,0), sizeof(ST_medicion), 1, pArchivoDat);
    }
    fclose(pArchivoDat);
}

void leerDat()
{
    FILE *pArchivoDat = OpenDat("r+b");
    ST_medicion *medicion = (ST_medicion*) malloc(sizeof(ST_medicion));
    fread(medicion, sizeof(ST_medicion), 1, pArchivoDat);
    while(!feof(pArchivoDat))
    {
        printf("\nHora: %i",medicion->hora);
        printf("\nHumedad Max: %.2f",medicion->humedadMax);
        printf("\nHumedad Min: %.2f",medicion->humedadMin);
        printf("\nProm x Hora: %.2f",medicion->humedadProm);
        printf("\n");
        fread(medicion, sizeof(ST_medicion), 1, pArchivoDat);
    }
    fclose(pArchivoDat);
}

