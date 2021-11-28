/**
 * @file matrix.cc
 * @author Yago Pérez Molanes (alu0101254678@ull.edu.es)
 * @brief implementacion de la clase Matrix, esta contiene los elementos
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

#include "../include/matrix.h"

Matrix::Matrix(char* argv[]) {
  crear_matriz(argv);
}

Matrix::~Matrix() {}

void Matrix::crear_matriz(char* argv[]) {

  opcion_metrica = atoi(argv[2]); //opcion de la metrica
  vecinos = atoi(argv[3]); //numero de vecinos
  opcion_prediccion = atoi(argv[4]); //prediccion usada

  if (opcion_metrica == 0 || vecinos == 0 || opcion_prediccion == 0) {
    throw "Ha especificado de forma incorrecta algun de los argumentos";
  }

  if (opcion_metrica > 3 || opcion_prediccion > 2 ) {
    throw "Las opciones correctas son, 1(Pearson)-2(Coseno)-3(Euclidea), para la opcion de la metrica, y, 1(Simple)-2(Media) para la prediccion";
  }
  
  
  std::ifstream fi(argv[1]); //fichero de la matriz
  std::ifstream fi_aux(argv[1]); //flujo de entrada auxiliar para obtener las filas y columnas
  std::string linea;

  if (!fi || !fi_aux) {
    throw "Ha habido un problema en la apertura del fichero de entrada";
  }
  
  int fil = 0;
  int col = 0;

  while (getline(fi_aux, linea)) {
    fil++;
    int col_aux = 0;
    for (size_t i = 0; i < linea.size(); i++) {
      if (linea[i] == ' ') {
        col_aux += 0;
      } else {
        col_aux ++;
      }
    }
    col = col_aux;
  }

  filas = fil; //le decimos las filas y las columnas
  columnas = col;
 
  matriz.resize((double)filas); //redimensionamos la matriz

  for (int i = 0; i < filas; i++) {
    matriz[i].resize((double)columnas);
  }

  double elemento; //valor de un elemento de la matriz
  std::string elemento_aux; //lo que esta leyendo, luego lo pasa a double

  //lee los elementos
  for (int i = 0; i < filas; i++) { 
    for (int j = 0; j < columnas; j++) {
      fi >> elemento_aux;
      try {
        elemento = stod(elemento_aux);
        //std::cout << elemento << std::endl;
      } catch (...) {
        elemento = -1.0; //un elemento es nulo cuando es -1.0
        //std::cout << "hola";
      }
      matriz[i][j] = elemento;
    }
  } 
}

void Matrix::mostrar_matriz() {
  std::cout << "--Matriz de utilidad: --" << std::endl;
  for (int i = 0; i < filas; i++) {
    for (int j = 0; j < columnas; j++) {
      std::cout << matriz[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n\n";
}

bool Matrix::es_nulo(int i, int j) {
  if (matriz[i][j] == -1.0) { 
    return true;
  } else {
    return false;
  }
}

double Matrix::calcular_media(int i) {
  double media_ = 0;
  int cont_ = 0; //contar el numero de elementos no vacios
  for (size_t j = 0; j < matriz[i].size(); j++) {
    if (es_nulo(i, j)) {
      media_ += 0;
    } else {
      media_ += matriz[i][j];
      cont_++;
    }
  }
  media_ = media_ / cont_;
  
  media[i] = media_; //actualiza el map
  return media_;
}

void Matrix::mostrar_media() {
  for (const auto &iter : media) {
    std::cout << "Media del usuario " << iter.first << " : " << iter.second << "\n"; 
  }
  std::cout << "\n\n";
}

double Matrix::similitud_pearson(int i, int j) {
  
  double media_primer_elemento = calcular_media(i);
  double media_segundo_elemento = calcular_media(j);

  double desviacion_primer_elemento = 0;
  double desviacion_segundo_elemento = 0;

  double numerador = 0;
  double denominador = 0;
  double resultado = 0;
  
  for (size_t z = 0; z < matriz[i].size(); z++) {
    if (!es_nulo(i, z) && !es_nulo(j, z)) { //se fila la fila, se fija el usuario a y b
      double resultado_resta_primer_elemento = matriz[i][z] - media_primer_elemento;
      double resultado_resta_segundo_elemento = matriz[j][z] - media_segundo_elemento;
      desviacion_primer_elemento += pow(resultado_resta_primer_elemento, 2);
      desviacion_segundo_elemento += pow(resultado_resta_segundo_elemento, 2);
    } else {
      desviacion_primer_elemento += 0;
      desviacion_segundo_elemento += 0;
    }
  }

  desviacion_primer_elemento = sqrt(desviacion_primer_elemento);
  desviacion_segundo_elemento = sqrt(desviacion_segundo_elemento);
  denominador = desviacion_primer_elemento * desviacion_segundo_elemento;

  for (size_t z = 0; z < matriz[i].size(); z++) {
    if (!es_nulo(i, z) && !es_nulo(j,z)) {
      numerador += (matriz[i][z] - media_primer_elemento) * (matriz[j][z] - media_segundo_elemento);  
    } else {
      numerador += 0;
    }
  }
  
  resultado = numerador / denominador;

  resultado = (resultado + 1) / 2; //normalizacion de los valores para que de entre 0 y 1

  //std::pair<int, int> similitud_usuario_a_b;

  similitudes[{i, j}] = resultado; //actualizamos el map de las similitudes
  return resultado;
  
}

double Matrix::similitud_coseno(int i, int j) {
  double numerador = 0;
  double denominador = 0;
  double resultado = 0;

  double sumatorio_primer_elemento = 0;
  double sumatorio_segundo_elemento = 0;
  
  for (size_t z = 0; z < matriz[i].size(); z++) {
    if (!es_nulo(i, z) && !es_nulo(j, z)) { //se fija las filas, y se avanzan las columnas
      numerador += matriz[i][z] * matriz[j][z];
    } else {
      numerador += 0;
    }
  }

  for (size_t z = 0; z < matriz[i].size(); z++) {
    if (!es_nulo(i, z) && !es_nulo(j, z)) {
      sumatorio_primer_elemento += pow(matriz[i][z], 2);
      sumatorio_segundo_elemento += pow(matriz[j][z], 2);
    } else {
      sumatorio_primer_elemento += 0;
      sumatorio_segundo_elemento += 0;
    }
  }

  sumatorio_primer_elemento = sqrt(sumatorio_primer_elemento);
  sumatorio_segundo_elemento = sqrt(sumatorio_segundo_elemento);
  denominador = sumatorio_primer_elemento * sumatorio_segundo_elemento;

  resultado = numerador / denominador; //no hace falta normalizar, porque siempre da entre 0 y 1

  similitudes[{i, j}] = resultado; //actualizamos el map de las similitudes
  return resultado;
  
}

double Matrix::similitud_euclidea(int i, int j) {
  double resultado = 0;
  double resultado_resta = 0;
  
  for (size_t z = 0; z < matriz[i].size(); z++) {
    if (!es_nulo(i, z) && !es_nulo(j, z)) {
      resultado_resta = matriz[i][z] - matriz[j][z];
      resultado_resta = pow(resultado_resta, 2);
      resultado += resultado_resta;
    } else {
      resultado += 0;
    }
  }
  resultado = sqrt(resultado);

  resultado = 1 / (1 + resultado); //normalizamos el valor obtenido

  similitudes[{i, j}] = resultado; //actualizamos el map de las similitudes
  return resultado;
  
}

double Matrix::prediccion_simple(int i, int j) {
  double resultado = 0;
  int vecinos_proximos = vecinos;
  int usuario = i;
  int producto = j;
  
  //similitudes del usuario i con el resto
  for (int j = 0; j < filas; j++) {
    if (usuario != j) {
      similitudes_aux[{usuario, j}] = similitudes[{usuario, j}];
      //std::cout << similitudes[{usuario, j}] << std::endl;
    }
  }

  std::vector<double> vect;

  //ponemos todas las similitudes en un vector y las ordenamos
  for (int k = 0; k < filas; k++) {
    if (usuario != k) {
      vect.push_back(similitudes_aux[{usuario, k}]);
    }
  }

  std::sort(vect.begin(), vect.end());

  /*for (size_t i = 0; i < vect.size(); i++) {
    std::cout << vect[i] << " ";
  }
  std::cout << "\n";*/

  std::vector<double> vect_ord_mayor;
  
  for (int i = vect.size() - 1; i >= 0; i--) {
    vect_ord_mayor.push_back(vect[i]);    
  }
  
  std::cout << "Vecinos considerados del usuario: " << usuario << " -> " << vecinos_proximos << " vecinos" << std::endl << std::endl;
  for (int i = 0; i < vecinos_proximos; i++) {
    for (size_t j = 0; j < filas; j++) {
      if (usuario != j && !es_nulo(j, producto)) {
        if (vect_ord_mayor[i] == similitudes_aux[{usuario, j}]) {
          std::cout << "usuario " << j << " : " << vect_ord_mayor[i] << std::endl;
        }
      }
    }
  }
  
  double numerador = 0;
  double denominador = 0;
  for (int i = 0; i < vecinos_proximos; i++) {
    for (size_t j = 0; j < filas; j++) {
      if (usuario != j && !es_nulo(j, producto)) { //no puede ser el mismo usuario ni ser nulo
        if (vect_ord_mayor[i] == similitudes_aux[{usuario, j}]) {
          //std::cout << vect_ord_mayor[i] << "---" << matriz[j][producto] << std::endl;
          numerador += vect_ord_mayor[i] * matriz[j][producto];
        }
      }
    }
    denominador += vect_ord_mayor[i];
  }
  resultado = numerador / denominador;
  valores_completar[{usuario, producto}] = resultado; //para ponerlo en el map de valores a completar
  std::cout << "\n";
  std::cout << "Predicción de la matriz de utilidad: " << "usuario -> " << usuario << " ," << 
  " producto -> " << producto << " , ([" << usuario << "," << producto << "]) -> " << resultado << std::endl << std::endl; 
  return resultado;
}

double Matrix::diferencia_con_la_media(int i, int j) {
  double resultado = 0;
  int vecinos_proximos = vecinos;
  int usuario = i;
  int producto = j;
  
  std::map<std::pair<int, int>, double> similitudes_aux;

  //similitudes del usuario i con el resto
  for (int j = 0; j < filas; j++) {
    if (usuario != j) {
      similitudes_aux[{usuario, j}] = similitudes[{usuario, j}];
      //std::cout << similitudes[{usuario, j}] << std::endl;
    }
  }

  std::cout << "\n";

  //ponemos todas las similitudes en un vector y las ordenamos
  std::vector<double> vect;

  for (int k = 0; k < filas; k++) {
    if (usuario != k) {
      vect.push_back(similitudes_aux[{usuario, k}]);
    }
  }

  std::vector<double> vect_ord_mayor;

  std::sort(vect.begin(), vect.end());
  
  for (int i = vect.size() - 1; i >= 0; i--) {
    vect_ord_mayor.push_back(vect[i]);    
  }

  //mostramos por pantalla los vecinos considerados, el numero y para cada uno de ellos
  //su similitud con el usuario a
  std::cout << "Vecinos considerados del usuario: " << usuario << " -> " << vecinos_proximos << " vecinos" << std::endl << std::endl;
  for (int i = 0; i < vecinos_proximos; i++) {
    for (size_t j = 0; j < filas; j++) {
      if (usuario != j && !es_nulo(j, producto)) {
        if (vect_ord_mayor[i] == similitudes_aux[{usuario, j}]) {
          std::cout << "usuario " << j << " : " << vect_ord_mayor[i] << std::endl;
        }
      }
    }
  }
  
  double numerador = 0;
  double denominador = 0;
  for (int i = 0; i < vecinos_proximos; i++) {
    for (size_t j = 0; j < filas; j++) {
      if (usuario != j && !es_nulo(j, producto)) {
        if (vect_ord_mayor[i] == similitudes_aux[{usuario, j}]) {
          //std::cout << vect_ord_mayor[i] << "---" << matriz[j][producto] << std::endl;
          numerador += vect_ord_mayor[i] * (matriz[j][producto] - media[j]);
        }
      }
    }
    denominador += vect_ord_mayor[i];
  }
  resultado = numerador / denominador;
  resultado += media[usuario];
  valores_completar[{usuario, producto}] = resultado; //lo metemos en el map de valores a completar
  std::cout << "\n";
  std::cout << "Predicción de la matriz de utilidad: " << "usuario -> " << usuario << " ," << 
  " producto -> " << producto << " , ([" << usuario << "," << producto << "]) -> " << resultado << std::endl << std::endl; 
  return resultado;
}

int Matrix::get_filas() {
  return filas;
}

int Matrix::get_columnas() {
  return columnas;
}

int Matrix::get_vecinos() {
  return vecinos;
}

int Matrix::get_opcion_metrica() {
  return opcion_metrica;
}

int Matrix::get_opcion_prediccion() {
  return opcion_prediccion;
}

void Matrix::completar_matriz() {
  for (int i = 0; i < get_filas(); i++) {
    for (int j = 0; j < get_columnas(); j++) {
      if (es_nulo(i, j)) {
        matriz[i][j] = valores_completar[{i, j}];
      }
    }
  }
}
