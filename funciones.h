#define mediciones_x_hora 3
#define cant_horas 4
#define L_fecha 20
#define L_time 2
#define L_coord 30
#define L_medicion 15

typedef struct
{
    char   fecha[L_fecha];
    char   hora[L_time];
    char   coordenadas[L_coord];
    char   medicion[L_medicion];
} ST_archivoTxt;

typedef struct
{
    int  hora;
    float humedadMin;
    float humedadMax;
    float humedadProm;
    int contador;
} ST_medicion;

void crearDat();
void leerTxt();
void parcearLinea(char linea[50], ST_archivoTxt *datos);
float strAFloat(ST_archivoTxt arcTxt);
void calcula(int hora, float newMedicion);
void leerDat();

