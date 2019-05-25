/**
 *  @Programa: Ajedrez para dos.
 *
 *  @Proposito: Proyecto final del tercer parcial de la materia conceptos avanzados de programacion.
 *  @Docente: Raymundo Cornejo Garcia
 *
 *  @Descripcion: Programa para que dos personas disfruten del clasico juego de ajedrez, tomando turnos en un mismo dispositivo.
 *  @Fecha: 15 de abril del 2019
 *  @Autores:
 *  Gabriel Mar Barrio
 *  Julian Teran Vazquez
 *  Daniel Hermosillo Lopez
 *  Daniel Villareal Morales
 *  Luis Fernando Feliz Mata
**/

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

/**
 *  VARIABLES IMPORTANTES:
 *  struct coordenadas: Estructura para poder guardar las coordenadas de las piezas en el tablero.
 *  int arr: Representacion de nuestro tablero.
 *  int teamCount: Variable de control, nos sirve para saber de quien es el turno actual.
 *  coordenadas *elecciones: puntero que apunta a un arreglo de tipo estructura (creado en main, linea 65)
**/

struct coordenadas{
    int x;
    int y;
};
int arr[8][8] = {};
bool jaqueMate=false;
int teamCount = 1;
coordenadas *elecciones;
bool callCheck = 0;
int checkCount = 0;

/**
 *  Funciones:
 *  Mayor detalle de lo que hace cada funcion a partir de la linea 86
**/

bool isValidInt(string numeroEnCadena);
void imprimirLetra(int cordLetra);
int cinOpcionValida(int top);
bool mismoEquipo(int piezaAMover, int piezaTope);
void inicializarTablero();
void board_set();
void moverPieza(coordenadas *options);
void moverPeon(int iCord , int jCord, coordenadas *options);
void moverTorre(int iCord , int jCord, coordenadas *options);
void moverCaballo(int iCord , int jCord, coordenadas *options);
void moverAlfil(int iCord , int jCord, coordenadas *options);
void moverReina(int iCord , int jCord, coordenadas *options);
void moverRey(int iCord , int jCord, coordenadas *options);
void checkCheck(coordenadas* options );
bool piezaJaque(coordenadas* options,int iCord, int jCord, int reyX, int reyY);

int main()
{
    try{
    elecciones = new coordenadas[64];
    }catch(...){
        cout << "Error fatal, el programa terminara";
    }

    inicializarTablero();
    board_set();

    do{
    checkCheck(elecciones);
    board_set();
    moverPieza(elecciones);
    board_set();
    teamCount++;
    }while(!jaqueMate);

    system("pause");

    delete [] elecciones;

    return 0;
}

//FUNCIONES:

/**
 *  Funcion: boardSet
 *
 *  @Descripcion: Funcion que imprime el estado acutal del juego.
 *  @Parametros: Ninguno.
 *  @Retorna: Ninguno.
**/

void board_set(){
	system("CLS");
	bool check = true;
	std::cout << "   ";
	std::cout << " 1  2  3  4  5  6  7  8\n";
	for(int i=0; i<8; i++){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		switch(i){
			default:
				break;
			case 0:
				std::cout << " a ";
				break;
			case 1:
				std::cout << " b ";
				break;
			case 2:
				std::cout << " c ";
				break;
			case 3:
				std::cout << " d ";
				break;
			case 4:
				std::cout << " e ";
				break;
			case 5:
				std::cout << " f ";
				break;
			case 6:
				std::cout << " g ";
				break;
			case 7:
				std::cout << " h ";
				break;
		}
		for(int j=0; j<8; j++){
			if(check){
				if(arr[i][j] < 0)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 127);
			}
			else{
				if(arr[i][j] < 0)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 143);
			}
			check = !check;
			std::cout << std::setw(2);
			if(arr[i][j] == 0)
				std::cout << "   ";
			else
				switch(abs(arr[i][j])){
					default:
						break;
					case 1:
						std::cout << " P ";
						break;
					case 2:
						std::cout << " R ";
						break;
					case 3:
						std::cout << " N ";
						break;
					case 4:
						std::cout << " B ";
						break;
					case 5:
						std::cout << " Q ";
						break;
					case 6:
						std::cout << " K ";
						break;
				}
		}
		check = !check;
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if(teamCount % 2 != 0){
        cout << "Es turno de las piezas blancas" << endl;
    }else{
        cout << "Es turno de las piezas negras" << endl;
    }
}

void moverPieza(coordenadas *options){
    string tempX;
    int coorX=0, coorY=0;
    bool ban=false;
    //Pide las coordenadas de la pieza a mover y verifica que sea coordenadas validas.
    do{
        try{
            cout << "Ingresa la fila de la pieza a mover (a-h)" << endl;
            cin>>tempX;

            if(tempX.size()>1)
                throw "NO EXISTE ESA COLUMNA (1-8)\n"; /** si introducen un string con longitud mayor a 1 se levanta una excepcion **/

            if(!(int(tempX[0])>= 97 && int(tempX[0])<=104))
                throw "NO EXISTE ESA COLUMNA (1-8)\n"; /** si introducen un char que no este entre a y h levanta una excepcion**/

            cout << "Ingresa la columna de la pieza a mover (1-8)" << endl;

            coorY = cinOpcionValida(8); /** Nos aseguramos de que ingrese un entero valido, consultar la funcion para mayor detalle **/

            if(coorY == 0)
                throw "NO EXISTE ESA COLUMNA (1-8)\n"; /** Si nos ingresan 0 levantamos una excepcion (la funcion anterior solo se asegura de que no sean numeros enormes, negativos o cadenas )**/

            coorY--;

            ban=true; //La bandera se convierte en verdadera, esto significa que las coordenadas son validas y por ende sale del do while
        }catch(const char* ex){ /** Si sucede alguna excepcion la imprimimos **/
            system("cls");
            cout << ex << endl << endl;
            system("pause");
            system("cls");
            board_set();
        }catch(out_of_range){  /** si nos introducen un valor que no cabe en un int, out_of_range se arroja de manera implicita **/
           system("cls");
            cout << "NO EXISTE ESA COLUMNA (1-8)\n" << endl << endl;
            system("pause");
            system("cls");
            board_set();
        }

    }while(!ban);

    switch(tempX[0]){ //Convierte las filas (a-h) en numeros para manejarlo en el arreglo
        case 'a':
            coorX=0;
        break;
        case 'b':
            coorX=1;
        break;
        case 'c':
            coorX=2;
        break;
        case 'd':
            coorX=3;
        break;
        case 'e':
            coorX=4;
        break;
        case 'f':
            coorX=5;
        break;
        case 'g':
            coorX=6;
        break;
        case 'h':
            coorX=7;
        break;
    }
    if(teamCount % 2 != 0){
        switch(arr[coorX][coorY]){ //Dependiendo de la pieza que desea mover llama la funcion correspondiente
            case 1:
                system("cls");
                board_set();
                cout << "SELECCIONASTE EL PEON (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverPeon(coorX, coorY, options);
            break;
            case 2:
                system("cls");
                board_set();
                cout << "SELECCIONASTE LA TORRE (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverTorre(coorX, coorY,options);
            break;
            case 3:
                system("cls");
                board_set();
                cout << "SELECCIONASTE EL CABALLO (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverCaballo(coorX, coorY, options);
            break;
            case 4:
                system("cls");
                board_set();
                cout << "SELECCIONASTE EL ALFIL (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverAlfil(coorX, coorY, options);
            break;
            case 5:
                system("cls");
                board_set();
                cout << "SELECCIONASTE LA REINA (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverReina(coorX, coorY, options);
            break;
            case 6:
                system("cls");
                board_set();
                cout << "SELECCIONASTE EL REY (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverRey(coorX, coorY, options);
            break;
            default:
                cout << "No existe pieza en esa coordenada" << endl;
                teamCount--;
                system("pause");
        }
    }else{
        switch(arr[coorX][coorY]){ //Dependiendo de la pieza que desea mover llama la funcion correspondiente
            case -1:
                system("cls");
                board_set();
                cout << "SELECCIONASTE EL PEON (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverPeon(coorX, coorY, options);
            break;
            case -2:
                system("cls");
                board_set();
                cout << "SELECCIONASTE LA TORRE (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverTorre(coorX, coorY,options);
            break;
            case -3:
                system("cls");
                board_set();
                cout << "SELECCIONASTE EL CABALLO (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverCaballo(coorX, coorY, options);
            break;
            case -4:
                system("cls");
                board_set();
                cout << "SELECCIONASTE EL ALFIL (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverAlfil(coorX, coorY, options);
            break;
            case -5:
                system("cls");
                board_set();
                cout << "SELECCIONASTE LA REINA (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverReina(coorX, coorY, options);
            break;
            case -6:
                system("cls");
                board_set();
                cout << "SELECCIONASTE EL REY (" << tempX << "," << coorY+1 << ")" << endl << endl;
                moverRey(coorX, coorY, options);
            break;
            default:
                cout << "No existe pieza en esa coordenada" << endl;
                teamCount--;
                system("pause");
        }
    }

}

/**
 *  Simple funcion para convertir un numero a una letra, la cual es equivalente al mismo en el tablero
 *  @Parametros: Un entero, que recibe de alguna funcion para mover una pieza
 *  @Retorna: Nada
**/

void imprimirLetra(int cordLetra){
    switch(cordLetra){
        case 0:
        cout << "a,";
        break;
        case 1:
        cout << "b,";
        break;
        case 2:
        cout << "c,";
        break;
        case 3:
        cout << "d,";
        break;
        case 4:
        cout << "e,";
        break;
        case 5:
        cout << "f,";
        break;
        case 6:
        cout << "g,";
        break;
        case 7:
        cout << "h,";
        break;
    }
}

/**
 *  Funcion: Mismo equipo
 *
 *  @Descripcion: Funcion que verifica si una pieza cualquiera y una pieza que se encuentra en su trayectoria son del mismo equipo
 *  @Parametros: Un par de enteros, que representan ambas piezas.
 *  @Retorna: Una booleana, verdadero en caso de que sean del mismo equipo.
**/

bool mismoEquipo(int piezaAMover, int piezaTope){
    if(piezaAMover < 0){
        return (piezaTope < 0);
    }else{
        return (piezaTope > 0);
    }
}

/**
 *  Funcion: inicializarTablero
 *
 *  @Descripcion: Funcion que se encarga de colocar las piezas en el tablero, dandole valor a lugares especificos de nuestra matriz, los valores del -1 al -6 representan
 *  las piezas negras, del 1 al 6 representan las blancas y un 0 es un espacio vacio.
 *  @Parametros: Ninguno.
 *  @Retorna: Ninguno.
**/

void inicializarTablero(){
    arr[8][8] = {};
    /**
    for(int i = 0 ; i < 8 ; i++){
        arr[5][i] = -1;
        arr[2][i] = -1;
    }
    arr[0][0] = arr[0][7] = -2;
    arr[0][1] = arr[0][6] = -3;
    arr[0][2] = arr[0][5] = -4;
    arr[0][3] = -5;
    arr[0][4] = -6;
    arr[7][0] = arr[7][7] = 2;
    arr[7][1] = arr[7][6] = 3;
    arr[7][2] = arr[7][5] = 4;
    arr[7][3] = 5;
    arr[7][4] = 6;
    arr[5][1] = -1;
    arr[2][1] = 1;
    **/
    //arr[3][3] = arr[3][4] = -2;
    arr[4][3] = 6;
    arr[3][4] = arr[3][3] = arr[3][2] = -1;
    arr[0][0] = -6;
    arr[7][0] = 2;
}

/**
 *  Funcion: isValidInt
 *  Autor: Raymundo Cornejo
 *
 *  @Descripcion: Funcion que se encarga de que el usuario no ingrese algun valor que no sea un entero positivo.
 *  @Parametros: Una cadena, para poder guardar cualquier tipo de dato en forma de caracter.
 *  @Retorna: Un entero positivo.
**/

bool isValidInt(string numeroEnCadena){
    int inicio = 0;
    int i=0;
    bool valido = true;
    bool signo = false;

    int tamanioDeCadena = numeroEnCadena.size();

    if(tamanioDeCadena == 0)
        valido = 0;

    if(numeroEnCadena.at(0)=='+'){
        inicio=1;
        signo = true;
    }

    if(signo && tamanioDeCadena ==1)
        valido = false;

    for(i=inicio; valido && i<tamanioDeCadena; i++){
        if(!isdigit(numeroEnCadena.at(i)))
        valido = false;
    }

    return valido;
}

/**
 *  Funcion: moverTorre
 *
 *  @Descripcion: Funcion que se encarga de que el usuario pueda mover a la torre con respecto a las reglas del ajedrez.
 *  @Parametros: Un par de enteros, los cuales representan las coordenadas de la torre en el tablero, y el arreglo de posibles elecciones creado en main.
 *  @Retorna: Nada, simplemente modifica nuestra matriz que representa el estado del tablero.
**/

void moverTorre(int iCord , int jCord, coordenadas *options){
    int iCordTemp = iCord;
    int jCordTemp = jCord;
    int counter = 0;
    int eleccion;

    bool check = 1;
    while(check){ /** Movimiento hacia arriba**/
        iCordTemp--;
        if(iCordTemp < 0){ /** Nos aseguramos que no se salga del los limites del tablero **/
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){ /** checamos que sea una casilla vacia, si esta condicion no se cumple, llegamos a una pieza **/
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){ /** si es una pieza del mismo equipo de la que vamos a mover, se detiene**/
            check = 0;
        }else{  /** si es del equipo contrario, la podemos comer y se detiene **/
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord; /** Restauramos nuestra temporal despues de haberla modificado en el while anterior **/
    check = 1;
    while(check){ /** Movimiento hacia la derecha**/
        jCordTemp++;
        if(jCordTemp > 7){
            check = 0;
        }else if(arr[iCord][jCordTemp]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCord][jCordTemp])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    jCordTemp = jCord;
    check = 1;
    while(check){ /** Movimiento hacia abajo**/
        iCordTemp++;
        if(iCordTemp > 7){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    check = 1;
    while(check){ /** Movimiento hacia la izquierda**/
        jCordTemp--;
        if(jCordTemp < 0){
            check = 0;
        }else if(arr[iCord][jCordTemp]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCord][jCordTemp])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    if(callCheck == 1){
        checkCount = counter;
        return;
    }

    if(counter == 0){
        cout << "Esta pieza no se puede mover hacia ninguna casilla" << endl; /** Si el counter no aumento, quiere decir que no hay movimientos posibles**/
        system("pause");
        teamCount--;
        return;
    }
    cout << "Se puede mover a las siguientes opciones:" << endl;
    cout << "Seleccione 0 para cambiar de jugada" << endl;
    for(int i = 0 ; i < counter ; i++){ /** for para imprimir las opciones que tiene el usuario **/
        cout << "#" << i+1 << ":";
        imprimirLetra(options[i].x);
        cout << (options[i].y)+1 << endl;
    }

    cout << "Escoge tu opcion ";
    check = 1; /** variable de control **/
    while(check == 1){ /** ciclo para asegurarnos de recibir datos validos **/
        try{
            check = 0;
            eleccion = cinOpcionValida(counter); /** nos aseguramos que el dato de entrada cumpla con varias caracterizticas, consultar la funcion para mayor detalle **/
        }catch(out_of_range){  /** si nos introducen un valor que no cabe en un int, out_of_range se arroja de manera implicita **/
            cout << "Favor de introducir un numero menor o igual a las opciones disponibles \n";
            check = 1;
        }catch(const char* ex){ /** Si sucede alguna excepcion la imprimimos **/
            cout << ex << endl;
            check = 1;
        }catch(...){
            cout << "Error desconocido";
            exit(1);
        }
    }
    if(eleccion != 0){ /** si introducen 0 cancelan su jugada **/
        arr[options[eleccion-1].x][options[eleccion-1].y] = arr[iCord][jCord];
        arr[iCord][jCord] = 0;
    }else{
        cout << "Has cancelado la jugada" << endl;
        system("pause");
        system("cls");
        board_set();
        moverPieza(options);
    }
}

/**
 *  Funcion: moverCaballo
 *
 *  @Descripcion: Funcion que se encarga de que el usuario pueda mover al caballo con respecto a las reglas del ajedrez.
 *  @Parametros: Un par de enteros, los cuales representan las coordenadas del caballo en el tablero, y el arreglo de posibles elecciones creado en main.
 *  @Retorna: Nada, simplemente modifica nuestra matriz que representa el estado del tablero.
**/

void moverCaballo(int iCord , int jCord, coordenadas *options){

    int iCordTemp = iCord;
    int jCordTemp = jCord;
    int counter = 0;
    int eleccion;

    iCordTemp -= 2; /** El caballo se mueve 2 hacia arriba y 1 a la derecha **/
    jCordTemp--;

    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){ // checa que no se salga del tablero
       if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp; // le da valor a la cordenada i de nuestra primer opcion
           options[counter].y = jCordTemp; // le da valor a la cordenada j de nuestra primer opcion
           counter++; // aumentamos el contador
       }
    }

    jCordTemp = jCord + 1; /** el caballo se mueve 2 hacia arriba y 1 a la derecha **/

    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp = iCord - 1; /** el caballo se mueve 2 hacia la derecha y 1 hacia arriba **/
    jCordTemp = jCord + 2;

    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp = iCord + 1; /** el caballo se mueve 2 hacia la derecha y 1 hacia abajo **/
    jCordTemp = jCord + 2;

    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
       if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp = iCord + 2; /** el caballo se mueve 2 hacia abajo y 1 a la derecha **/
    jCordTemp = jCord + 1;

    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
       if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp = iCord + 2; /** el caballo se mueve 2 hacia abajo y 1 a la izquierda **/
    jCordTemp = jCord - 1;

    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
       if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp = iCord + 1; /** el caballo se mueve 2 hacia la izquierda y 1 hacia abajo **/
    jCordTemp = jCord - 2;

    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){ // checa que no se salg
        if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp = iCord - 1; /** el caballo se mueve 2 hacia la izquierda y 1 hacia arriba **/
    jCordTemp = jCord - 2;

    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
       if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    if(callCheck == 1){
        checkCount = counter;
        return;
    }

    if(counter == 0){
        cout << "Esta pieza no se puede mover hacia ninguna casilla" << endl; /** Si el counter no aumento, quiere decir que no hay movimientos posibles**/
        system("pause");
        teamCount--;
        return;
    }
    cout << "Se puede mover a las siguientes opciones:" << endl;
    cout << "Seleccione 0 para cambiar de jugada" << endl;
    for(int i = 0 ; i < counter ; i++){ /** for para imprimir las opciones que tiene el usuario **/
        cout << "#" << i+1 << ":";
        imprimirLetra(options[i].x);
        cout << (options[i].y)+1 << endl;
    }

    cout << "Escoge tu opcion ";
    bool check = 1; /** variable de control **/
    while(check == 1){ /** ciclo para asegurarnos de recibir datos validos **/
        try{
            check = 0;
            eleccion = cinOpcionValida(counter); /** nos aseguramos que el dato de entrada cumpla con varias caracterizticas, consultar la funcion para mayor detalle **/
        }catch(out_of_range){  /** si nos introducen un valor que no cabe en un int, out_of_range se arroja de manera implicita **/
            cout << "Favor de introducir un numero menor o igual a las opciones disponibles \n";
            check = 1;
        }catch(const char* ex){ /** Si sucede alguna excepcion la imprimimos **/
            cout << ex << endl;
            check = 1;
        }catch(...){
            cout << "Error desconocido";
            exit(1);
        }
    }
    if(eleccion != 0){ /** si introducen 0 cancelan su jugada **/
        arr[options[eleccion-1].x][options[eleccion-1].y] = arr[iCord][jCord];
        arr[iCord][jCord] = 0;
    }else{
        cout << "Has cancelado la jugada" << endl;
        system("pause");
        system("cls");
        board_set();
        moverPieza(options);
    }

}

/**
 *  Funcion: moverPeon
 *
 *  @Descripcion: Funcion que se encarga de que el usuario pueda mover al peon con respecto a las reglas del ajedrez.
 *  @Parametros: Un par de enteros, los cuales representan las coordenadas del peon en el tablero, y el arreglo de posibles elecciones creado en main.
 *  @Retorna: Nada, simplemente modifica nuestra matriz que representa el estado del tablero.
**/

void moverPeon(int iCord , int jCord, coordenadas *options){

    int iCordTemp = iCord;
    int jCordTemp = jCord;
    int counter = 0;
    int eleccion;

    if(arr[iCord][jCord]>0){
        iCordTemp--;
        jCordTemp--; /** checar arriba a la izquierda del peon**/
        if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
            if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp]) && arr[iCordTemp][jCordTemp]!= 0 ){ // checa si es el mismo equipo
               options[counter].x = iCordTemp;
               options[counter].y = jCordTemp;
               counter++;
            }
        }
        iCordTemp = iCord - 1;
        jCordTemp = jCord; /** checar justo arriba **/

        if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
            if(arr[iCordTemp][jCordTemp] == 0){ // checa si es el mismo equipo
               options[counter].x = iCordTemp;
               options[counter].y = jCordTemp;
               counter++;
            }
        }

        if(iCord == 6){ /** solo entra si es la primera vez que el peon blanco se mueve **/

            iCordTemp = iCord - 2; /** checar 2 hacia arriba **/
            jCordTemp = jCord;

            if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
                if(arr[iCordTemp][jCordTemp] == 0){ // checa si es el mismo equipo
                   options[counter].x = iCordTemp;
                   options[counter].y = jCordTemp;
                   counter++;
                }
            }
        }

        iCordTemp = iCord - 1;
        jCordTemp = jCord + 1;

        if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
            if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp]) && arr[iCordTemp][jCordTemp]!= 0 ){ // checa si es el mismo equipo
               options[counter].x = iCordTemp;
               options[counter].y = jCordTemp;
               counter++;
            }
        }
    }else{

        iCordTemp++; /** checas abajo a la derecha **/
        jCordTemp++;

        if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
            if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp]) && arr[iCordTemp][jCordTemp]!= 0 ){ // checa si es el mismo equipo
               options[counter].x = iCordTemp;
               options[counter].y = jCordTemp;
               counter++;
            }
        }

        iCordTemp = iCord + 1; /** checas abajo **/
        jCordTemp = jCord;

        if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
            if(arr[iCordTemp][jCordTemp] == 0){ // checa si es el mismo equipo
               options[counter].x = iCordTemp;
               options[counter].y = jCordTemp;
               counter++;
            }
        }

        if(iCord == 1){ /** solo entra si es la primera vez que el peon blanco se mueve **/

            iCordTemp = iCord + 2; /** checar 2 hacia aabajo **/
            jCordTemp = jCord;

            if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
                if(arr[iCordTemp][jCordTemp] == 0){ // checa si es el mismo equipo
                   options[counter].x = iCordTemp;
                   options[counter].y = jCordTemp;
                   counter++;
                }
            }
        }

        iCordTemp = iCord + 1; /** checas abajo y a la izquierda **/
        jCordTemp = jCord - 1;

        if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
            if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp]) && arr[iCordTemp][jCordTemp]!= 0 ){ // checa si es el mismo equipo
                options[counter].x = iCordTemp;
                options[counter].y = jCordTemp;
                counter++;
            }
        }
    }

    if(callCheck == 1){
        checkCount = counter;
        return;
    }

    if(counter == 0){
        cout << "Esta pieza no se puede mover hacia ninguna casilla" << endl; /** Si el counter no aumento, quiere decir que no hay movimientos posibles**/
        system("pause");
        teamCount--;
        return;
    }
    cout << "Se puede mover a las siguientes opciones:" << endl;
    cout << "Seleccione 0 para cambiar de jugada" << endl;
    for(int i = 0 ; i < counter ; i++){ /** for para imprimir las opciones que tiene el usuario **/
        cout << "#" << i+1 << ":";
        imprimirLetra(options[i].x);
        cout << (options[i].y)+1 << endl;
    }

    cout << "Escoge tu opcion ";
    bool check = 1; /** variable de control **/
    while(check == 1){ /** ciclo para asegurarnos de recibir datos validos **/
        try{
            check = 0;
            eleccion = cinOpcionValida(counter); /** nos aseguramos que el dato de entrada cumpla con varias caracterizticas, consultar la funcion para mayor detalle **/
        }catch(out_of_range){  /** si nos introducen un valor que no cabe en un int, out_of_range se arroja de manera implicita **/
            cout << "Favor de introducir un numero menor o igual a las opciones disponibles \n";
            check = 1;
        }catch(const char* ex){ /** Si sucede alguna excepcion la imprimimos **/
            cout << ex << endl;
            check = 1;
        }catch(...){
            cout << "Error desconocido";
            exit(1);
        }
    }
    if(eleccion != 0){ /** si introducen 0 cancelan su jugada **/
        arr[options[eleccion-1].x][options[eleccion-1].y] = arr[iCord][jCord];
        arr[iCord][jCord] = 0;

    }else{
        cout << "Has cancelado la jugada" << endl;
        system("pause");
        system("cls");
        board_set();
        moverPieza(options);
    }
}

/**
 *  Funcion: moverAlfil
 *
 *  @Descripcion: Funcion que se encarga de que el usuario pueda mover al alfil con respecto a las reglas del ajedrez.
 *  @Parametros: Un par de enteros, los cuales representan las coordenadas del alfil en el tablero, y el arreglo de posibles elecciones creado en main.
 *  @Retorna: Nada, simplemente modifica nuestra matriz que representa el estado del tablero.
**/

void moverAlfil(int iCord , int jCord, coordenadas *options){

    int iCordTemp = iCord;
    int jCordTemp = jCord;
    int counter = 0;
    int eleccion;

    bool check = 1;

    while(check){ /** Movimiento hacia arriba y la izquierda **/
        iCordTemp--;
        jCordTemp--;
        if(iCordTemp < 0 || jCordTemp < 0){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
		check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia arriba y la derecha**/
        iCordTemp--;
        jCordTemp++;
        if(iCordTemp < 0 || jCordTemp > 7){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia abajo y derecha **/
        iCordTemp++;
        jCordTemp++;
        if(iCordTemp > 7 || jCordTemp > 7){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia abajo e izquierda **/
        iCordTemp++;
        jCordTemp--;
        if(iCordTemp > 7 || jCordTemp < 0){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    if(callCheck == 1){
        checkCount = counter;
        return;
    }

    if(counter == 0){
        cout << "Esta pieza no se puede mover hacia ninguna casilla" << endl; /** Si el counter no aumento, quiere decir que no hay movimientos posibles**/
        system("pause");
        teamCount--;
        return;
    }
    cout << "Se puede mover a las siguientes opciones:" << endl;
    cout << "Seleccione 0 para cambiar de jugada" << endl;
    for(int i = 0 ; i < counter ; i++){ /** for para imprimir las opciones que tiene el usuario **/
        cout << "#" << i+1 << ":";
        imprimirLetra(options[i].x);
        cout << (options[i].y)+1 << endl;
    }

    cout << "Escoge tu opcion ";
    check = 1; /** variable de control **/
    while(check == 1){ /** ciclo para asegurarnos de recibir datos validos **/
        try{
            check = 0;
            eleccion = cinOpcionValida(counter); /** nos aseguramos que el dato de entrada cumpla con varias caracterizticas, consultar la funcion para mayor detalle **/
        }catch(out_of_range){  /** si nos introducen un valor que no cabe en un int, out_of_range se arroja de manera implicita **/
            cout << "Favor de introducir un numero menor o igual a las opciones disponibles \n";
            check = 1;
        }catch(const char* ex){ /** Si sucede alguna excepcion la imprimimos **/
            cout << ex << endl;
            check = 1;
        }catch(...){
            cout << "Error desconocido";
            exit(1);
        }
    }
    if(eleccion != 0){ /** si introducen 0 cancelan su jugada **/
        arr[options[eleccion-1].x][options[eleccion-1].y] = arr[iCord][jCord];
        arr[iCord][jCord] = 0;

    }else{
        cout << "Has cancelado la jugada" << endl;
        system("pause");
        system("cls");
        board_set();
        moverPieza(options);
    }

}

/**
 *  Funcion: moverReina
 *
 *  @Descripcion: Funcion que se encarga de que el usuario pueda mover a la reina con respecto a las reglas del ajedrez.
 *  @Parametros: Un par de enteros, los cuales representan las coordenadas de la reina en el tablero, y el arreglo de posibles elecciones creado en main.
 *  @Retorna: Nada, simplemente modifica nuestra matriz que representa el estado del tablero
**/

void moverReina(int iCord , int jCord, coordenadas *options){

    int iCordTemp = iCord;
    int jCordTemp = jCord;
    int counter = 0;
    int eleccion;

    bool check = 1;

    while(check){ /** Movimiento hacia arriba**/
        iCordTemp--;
        if(iCordTemp < 0){ /** Nos aseguramos que no se salga del los limites del tablero **/
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){ /** checamos que sea una casilla vacia, si esta condicion no se cumple, llegamos a una pieza **/
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){ /** si es una pieza del mismo equipo de la que vamos a mover, se detiene**/
            check = 0;
        }else{  /** si es del equipo contrario, la podemos comer y se detiene **/
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia arriba y la derecha**/
        iCordTemp--;
        jCordTemp++;
        if(iCordTemp < 0 || jCordTemp > 7){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia la derecha**/
        jCordTemp++;
        if(jCordTemp > 7){
            check = 0;
        }else if(arr[iCord][jCordTemp]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCord][jCordTemp])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia abajo y derecha **/
        iCordTemp++;
        jCordTemp++;
        if(iCordTemp > 7 || jCordTemp > 7){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;
    while(check){ /** Movimiento hacia abajo**/
        iCordTemp++;
        if(iCordTemp > 7){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia abajo e izquierda **/
        iCordTemp++;
        jCordTemp--;
        if(iCordTemp > 7 || jCordTemp < 0){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia la izquierda**/
        jCordTemp--;
        if(jCordTemp < 0){
            check = 0;
        }else if(arr[iCord][jCordTemp]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCord][jCordTemp])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    iCordTemp = iCord;
    jCordTemp = jCord;
    check = 1;

    while(check){ /** Movimiento hacia arriba y la izquierda **/
        iCordTemp--;
        jCordTemp--;
        if(iCordTemp < 0 || jCordTemp < 0){
            check = 0;
        }else if(arr[iCordTemp][jCord]==0){
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
        }else if(mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCord])){
            check = 0;
        }else{
            options[counter].x = iCordTemp;
            options[counter].y = jCordTemp;
            counter++;
            check = 0;
        }
    }

    if(callCheck == 1){
        checkCount = counter;
        return;
    }

    if(counter == 0){
        cout << "Esta pieza no se puede mover hacia ninguna casilla" << endl; /** Si el counter no aumento, quiere decir que no hay movimientos posibles**/
        system("pause");
        teamCount--;
        return;
    }
    cout << "Se puede mover a las siguientes opciones:" << endl;
    cout << "Seleccione 0 para cambiar de jugada" << endl;
    for(int i = 0 ; i < counter ; i++){ /** for para imprimir las opciones que tiene el usuario **/
        cout << "#" << i+1 << ":";
        imprimirLetra(options[i].x);
        cout << (options[i].y)+1 << endl;
    }

    cout << "Escoge tu opcion ";
    check = 1; /** variable de control **/
    while(check == 1){ /** ciclo para asegurarnos de recibir datos validos **/
        try{
            check = 0;
            eleccion = cinOpcionValida(counter); /** nos aseguramos que el dato de entrada cumpla con varias caracterizticas, consultar la funcion para mayor detalle **/
        }catch(out_of_range){  /** si nos introducen un valor que no cabe en un int, out_of_range se arroja de manera implicita **/
            cout << "Favor de introducir un numero menor o igual a las opciones disponibles \n";
            check = 1;
        }catch(const char* ex){ /** Si sucede alguna excepcion la imprimimos **/
            cout << ex << endl;
            check = 1;
        }catch(...){
            cout << "Error desconocido";
            exit(1);
        }
    }
    if(eleccion != 0){ /** si introducen 0 cancelan su jugada **/
        arr[options[eleccion-1].x][options[eleccion-1].y] = arr[iCord][jCord];
        arr[iCord][jCord] = 0;

    }else{
        cout << "Has cancelado la jugada" << endl;
        system("pause");
        system("cls");
        board_set();
        moverPieza(options);
    }
}

/**
 *  Funcion: moverRey
 *
 *  @Descripcion: Funcion que se encarga de que el usuario pueda mover al rey con respecto a las reglas del ajedrez.
 *  @Parametros: Un par de enteros, los cuales representan las coordenadas del rey en el tablero, y el arreglo de posibles elecciones creado en main.
 *  @Retorna: Nada, simplemente modifica nuestra matriz que representa el estado del tablero.
**/

void moverRey(int iCord , int jCord, coordenadas *options){

    int iCordTemp = iCord;
    int jCordTemp = jCord;
    int counter = 0;
    int eleccion;

    bool check = 1;

    iCordTemp--;
    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    jCordTemp++;
    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp++;
    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp++;
    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    jCordTemp--;
    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    jCordTemp--;
    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp--;
    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    iCordTemp--;
    if(iCordTemp >= 0 && iCordTemp < 8 && jCordTemp >= 0 && jCordTemp < 8){
      if(!mismoEquipo(arr[iCord][jCord],arr[iCordTemp][jCordTemp])){ // checa si es el mismo equipo
           options[counter].x = iCordTemp;
           options[counter].y = jCordTemp;
           counter++;
       }
    }

    if(callCheck == 1){
        checkCount = counter;
        return;
    }

    if(counter == 0){
        cout << "Esta pieza no se puede mover hacia ninguna casilla" << endl; /** Si el counter no aumento, quiere decir que no hay movimientos posibles**/
        system("pause");
        teamCount--;
        return;
    }
    cout << "Se puede mover a las siguientes opciones:" << endl;
    cout << "Seleccione 0 para cambiar de jugada" << endl;
    for(int i = 0 ; i < counter ; i++){ /** for para imprimir las opciones que tiene el usuario **/
        cout << "#" << i+1 << ":";
        imprimirLetra(options[i].x);
        cout << (options[i].y)+1 << endl;
    }

    cout << "Escoge tu opcion ";
    check = 1; /** variable de control **/
    while(check == 1){ /** ciclo para asegurarnos de recibir datos validos **/
        try{
            check = 0;
            eleccion = cinOpcionValida(counter); /** nos aseguramos que el dato de entrada cumpla con varias caracterizticas, consultar la funcion para mayor detalle **/
        }catch(out_of_range){  /** si nos introducen un valor que no cabe en un int, out_of_range se arroja de manera implicita **/
            cout << "Favor de introducir un numero menor o igual a las opciones disponibles \n";
            check = 1;
        }catch(const char* ex){ /** Si sucede alguna excepcion la imprimimos **/
            cout << ex << endl;
            check = 1;
        }catch(...){
            cout << "Error desconocido";
            exit(1);
        }
    }
    if(eleccion != 0){ /** si introducen 0 cancelan su jugada **/
        arr[options[eleccion-1].x][options[eleccion-1].y] = arr[iCord][jCord];
        arr[iCord][jCord] = 0;

    }else{
        cout << "Has cancelado la jugada" << endl;
        system("pause");
        system("cls");
        board_set();
        moverPieza(options);
    }


}

/**
 *  Funcion cinOpcionValida:
 *
 *  @Descripcion: Funcion la cual al momento de escoger una opcion para mover una pieza se asegura que esta opcion sea valida
 *  @Parametros: Un entero, el cual representa la cantidad de elementos en la lista de elecciones
 *  @Retorna: Un netero el cual es una eleccion valida
**/

int cinOpcionValida(int top){
    string dummy;
    cin >> dummy;
    if(!isValidInt(dummy))
        throw "Favor de solo introducir numeros positivos ";

    int eleccion = stoi(dummy);

    if(eleccion == 0)
        return eleccion;

    if(eleccion > top)
        throw "Favor de introducir un numero menor o igual a las opciones disponibles ";

    return eleccion;
}

void checkCheck(coordenadas* options){
    callCheck = 1;
    coordenadas rey;

    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            if(teamCount%2 != 0){
                if(arr[i][j] == 6){
                rey.x = i;
                rey.y = j;
                }
            }else{
                if(arr[i][j] == -6){
                rey.x = i;
                rey.y = j;
                }
            }
        }
    }

    cout << rey.x << " " << rey.y << endl;

    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            if(piezaJaque(options , i , j , rey.x , rey.y)){
                if(arr[rey.x][rey.y] > 0){
                    cout << "Estan en jaque las blancas" << endl;
                }else{
                    cout << "Estan en jaque las negras" << endl;
                }
            }
        }
    }


    system("pause");
    system("cls");


    callCheck = 0;
}

bool piezaJaque(coordenadas* options,int iCord, int jCord, int reyX, int reyY){

    if(arr[iCord][jCord] == 0){
            return false;
    }

    if(arr[reyX][reyY] > 0){
        if(arr[iCord][jCord] == -1){
            if(arr[iCord+1][jCord-1] == 6){
                return true;
            }
            if(arr[iCord+1][jCord+1] == 6){
                return true;
            }
        }

        if(arr[iCord][jCord] == -2){
            moverTorre(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }

        if(arr[iCord][jCord] == -3){
            moverCaballo(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }

        if(arr[iCord][jCord] == -4){
            moverAlfil(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }

        if(arr[iCord][jCord] == -5){
            moverReina(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }

        if(arr[iCord][jCord] == -6){
            moverRey(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }
    }else{
        if(arr[iCord][jCord] == 1){
            if(arr[iCord-1][jCord-1] == -6){
                return true;
            }
            if(arr[iCord-1][jCord+1] == -6){
                return true;
            }
        }

        if(arr[iCord][jCord] == 2){
            moverTorre(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }

        if(arr[iCord][jCord] == 3){
            moverCaballo(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }

        if(arr[iCord][jCord] == 4){
            moverAlfil(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }

        if(arr[iCord][jCord] == 5){
            moverReina(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }

        if(arr[iCord][jCord] == 6){
            moverRey(iCord,jCord,options);
            for(int i = 0 ; i < checkCount ; i++){
                if(options[i].x == reyX){
                    if(options[i].y == reyY){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


