/**
 * @file matrix.h
 * @author Yago Pérez Molanes (alu0101254678@ull.edu.es)
 * @brief declaracion de la clase Matrix, esta contiene los elementos
 * necesarios para el sistema de recomendacion, se realizan operaciones
 * sobre una matriz que el usuario pasa por linea de comandos, asi como
 * otros argumentos, el objetivo principal es completar o predecir los valores
 * que aparecen en blanco, es decir aquellos para los que el usuario no les ha
 * dado todavia valoracion y que en base a las valoraciones de usuarios 
 * parecidos a el se podria saber
 * @version 0.1
 * @date 2021-11-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

/**
 * @brief Clase Matriz, engloba los elementos del sistema de
 * recomendacion, estos serian el numero de filas y columnas
 * de la matriz, el numero de vecinos, la metrica y la prediccion,
 * una matriz que seria un vector de vectores, un mapa para guardar
 * los valores de las similitudes, esto seria para cada par de usuarios
 * el valor en double
 * 
 * similitudes_aux seria para el calculo de las predicciones, para cuando
 * se quiere predecir el valor de i, j, pues las similitudes del usuario i
 * con el resto de usuario
 * por ultimo un mapa para guardar los valores a completar
 */
class Matrix {
  private:
    int filas;
    int columnas;
    int vecinos;
    int opcion_metrica;
    int opcion_prediccion;
    std::vector<std::vector<double>> matriz;
    std::map<int, double> media;
    std::map<std::pair<int, int>, double> similitudes;
    std::map<std::pair<int, int>, double> similitudes_aux;
    std::map<std::pair<int, int>, double> valores_completar;
    
    /**
     * @brief es un metodo auxiliar que crea la matriz, es
     * llamada por el constructor
     * @param argv lista de argumentos
     */
    void crear_matriz(char* argv[]);

  public:
    /**
     * @brief Constructor de la clase
     * @param argv se le pasa la lista de argumentos
     */
    Matrix(char* argv[]);
    /**
     * @brief El destructor no hace nada
     */
    ~Matrix();

    /**
     * @brief recorre la matriz y la muestra
     */
    void mostrar_matriz();

    /**
     * @brief calcula la media de un usuario, de un valor de la fila
     * @param i usuario o valor de la fila
     * @return double el valor de la media
     */
    double calcular_media(int i);

    /**
     * @brief dice si el elemento de la matriz esta vacio
     * @param i valor de la fila, ususario
     * @param j valor de la columna, item
     * @return true 
     * @return false 
     */
    bool es_nulo(int i, int j);

    /**
     * @brief muestra la media obtenida de todos los
     * usuarios
     */
    void mostrar_media();
    
    /**
     * @brief calcula la similitud de un usuario con otro
     * a traves de la correlacion de Pearson
     * @param i usuario a
     * @param j usuario b
     * @return double resultado
     */
    double similitud_pearson(int i, int j);

    /**
     * @brief calcula la similitud de un usuario con otro
     * a traves de la distancia coseno
     * @param i usuario a
     * @param j usuario b
     * @return double resultado
     */
    double similitud_coseno(int i, int j);

    /**
     * @brief calcula la similitud de un usuario con otro
     * a traves de la distancia euclidea
     * @param i usuario a
     * @param j usuario b
     * @return double resultado
     */
    double similitud_euclidea(int i, int j);

    /**
     * @brief calcula el valor de un elemento de la matriz
     * que es nulo a traves de la prediccion simple
     * @param i usuario a
     * @param j item
     * @return double resultado 
     */
    double prediccion_simple(int i, int j);

    /**
     * @brief calcula el valor de un elemento de la matriz
     * que es nulo a traves de la prediccion con la media
     * @param i usuario a
     * @param j item
     * @return double resultado 
     */
    double diferencia_con_la_media(int i, int j);

    /**
     * @brief obtiene el numero de filas
     * @return int atributo filas
     */
    int get_filas();

    /**
     * @brief obtiene el numero de columnas
     * @return int atributo columnas
     */
    int get_columnas();

    /**
     * @brief obtiene el numero de vecinos
     * @return int atributo vecinos
     */
    int get_vecinos();

    /**
     * @brief obtiene la opcion de la metrica elegida
     * @return int atributo opcion metrica 
     */
    int get_opcion_metrica();

    /**
     * @brief obtiene la opcion de la prediccion elegida
     * @return int atributo opcion prediccion
     */
    int get_opcion_prediccion();

    /**
     * @brief completa la matriz a traves del mapa (valores_completar)
     * que tiene los elementos que se han predicho y que se van
     * a sustituir en los valores nulos
     */
    void completar_matriz();
};
