/**
 * @file main.cc
 * @author Yago Pérez Molanes (alu0101254678@ull.edu.es)
 * @brief Programa principal de la practica de sistemas de
 * recomendacion de gestion del conocimiento, este se encarga de
 * leer los argumentos que recibe desde la linea de comandos, y
 * crear el objeto asociado a la matriz, realizar los calculos de 
 * la media de los usuarios (filas de la matriz) asi como de la similitud de
 * cada usuario con el resto segun la opcion elegida, y por ultimo el 
 * calculo de las predicciones, es decir, de los valores nulos de la matriz
 * 
 * Argumentos:
 * 	1 -> fichero donde se encuentra la matriz de utilidad
 * 	2 -> opcion de la matrica elegida: 
 * 		correlacion de Pearson
 * 		distancia coseno
 * 		distancia euclidea
 * 	3 -> numero de vecinos considerados
 * 	4 -> opcion de la prediccion
 * 		prediccion simple
 * 		diferencia con la media
 * 
 * @version 0.1
 * @date 2021-11-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../include/matrix.h"

int main(int argc, char* argv[]) {
	try {
		if (argc == 2) {
			std::string ayuda = argv[1];
			if (ayuda == "--help") {
				std::cout << "--------------------------------------------------------------------" << std::endl;
				std::cout << "MODO DE USO DEL PROGRAMA" << std::endl;
				std::cout << "Para poder ejecutar el programa debe indicar el nombre\n"
				"del ejecutable, del fichero de entrada de la matriz de utilidad, la opcion de la\n"
				"metrica elegida, el numero de vecinos considerados, y el tipo de prediccion\n";
				std::cout << "Ejemplo: ./bin/main matriz_utilidad.txt 1 3 2\n";
				std::cout << "En este caso estamos eligiendo la similitud de la correlacion de Pearson(1)\n"
				"3 vecinos mas proximos(3), y la distancia con la media como prediccion(2)\n";
				std::cout << "---------------------------------------------------------------------" << std::endl;
				return 0;
			}
		}

		if (argc != 5) {
			std::cerr << "Falta algun argumento por especificar, o ha escrito de mas, pruebe ./bin/nombre ejecutable --help para mas informacion" << std::endl;
			return 1;
		}

		std::cout << "\n";
		int opcion_metrica;
		int opcion_prediccion;

		Matrix matriz_relacion = Matrix(argv); //creamos el objeto de la matriz pasandole los argumentos
		matriz_relacion.mostrar_matriz(); //mostramos la matriz original
	
		for (int i = 0; i < matriz_relacion.get_filas(); i++) { //calculamos la media de cada usuario
			matriz_relacion.calcular_media(i);
		}
		std::cout << "Media de los usuarios: " << std::endl << std::endl;
		matriz_relacion.mostrar_media();

		opcion_metrica = matriz_relacion.get_opcion_metrica();
		opcion_prediccion = matriz_relacion.get_opcion_prediccion();

		if (opcion_metrica == 1) {
			std::cout << "Métrica elegida: " << "Correlacion de Pearson" << std::endl << std::endl;
			for (int i = 0; i < matriz_relacion.get_filas(); i++) {
				std::cout << "Similitud del usuario " << i << " respecto a: " << std::endl;
				for (int j = 0; j < matriz_relacion.get_filas(); j++) { //porque es ususario - usuario
					if (i != j) { //no se puede calcular la similitud con el mismo usuario
						std::cout << "usuario " << j << ": ";
						std::cout << matriz_relacion.similitud_pearson(i, j) << std::endl; //calculamos similitud de pearson
					}
				}
				std::cout << "\n\n";
			}
		} else if (opcion_metrica == 2){
				std::cout << "Métrica elegida: " << "Distancia Coseno" << std::endl << std::endl;
				for (int i = 0; i < matriz_relacion.get_filas(); i++) {
					std::cout << "Similitud del usuario " << i << " respecto a: " << std::endl;
					for (int j = 0; j < matriz_relacion.get_filas(); j++) {
						if (i != j) {
							std::cout << "usuario " << j << ": ";
							std::cout << matriz_relacion.similitud_coseno(i, j) << std::endl; //calculamos similitud coseno
						}
					}
					std::cout << "\n\n";
				}
		} else if(opcion_metrica == 3) {
				std::cout << "Métrica elegida: " << "Distancia Euclídea" << std::endl << std::endl;
				for (int i = 0; i < matriz_relacion.get_filas(); i++) {
					std::cout << "Similitud del usuario " << i << " respecto a: " << std::endl;
					for (int j = 0; j < matriz_relacion.get_filas(); j++) {
						if (i != j) {
							std::cout << "usuario " << j << ": ";
							std::cout << matriz_relacion.similitud_euclidea(i, j) << std::endl; //calculamos similitud euclidea
						}
					}
					std::cout << "\n\n";
				}
		}

		if (opcion_prediccion == 1) { //calculamos los valores segun la prediccion elegida
			for (int i = 0; i < matriz_relacion.get_filas(); i++) {
				for (int j = 0; j < matriz_relacion.get_columnas(); j++) {
					if (matriz_relacion.es_nulo(i, j)) {
						matriz_relacion.prediccion_simple(i, j);
					}
				}
			}
		} else if(opcion_prediccion == 2) {
			for (int i = 0; i < matriz_relacion.get_filas(); i++) {
				for (int j = 0; j < matriz_relacion.get_columnas(); j++) {
					if (matriz_relacion.es_nulo(i, j)) {
						matriz_relacion.diferencia_con_la_media(i, j);
					}
				}
			} 
		}

		matriz_relacion.completar_matriz(); //completamos la matriz con las predicciones
		matriz_relacion.mostrar_matriz(); //y la mostramos de nuevo	
		return 0;
	}
	catch(const char* mensaje) {
		std::cerr << mensaje << '\n';		
		return 1;
	}
			
}
