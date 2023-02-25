#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct registro_alumno {
	int legajo;
	char nombre[30];
	float nota;
	bool borrado;
};

void altaAlumno(FILE *archivo, registro_alumno &alumno);
void listarAlumno(FILE *archivo, registro_alumno &alumno);
void bajaFisicaAlumno(FILE *archivo, registro_alumno &alumno);
void bajaLogicaAlumno(FILE *archivo, registro_alumno &alumno);
void ordenarRegistrosAlumnos(FILE *archivo, registro_alumno &alumno);

int main() {
	FILE *archivo;
	registro_alumno alumno;
	int opcion;
	do {
		system("cls");
		printf("1 - Alta Alumno\n");
		printf("2 - Listar Alumnos\n");
		printf("3 - Baja Fisica\n");
		printf("4 - Baja Logica\n");
		printf("5 - Ordenar registros por legajo\n");
		printf("0 - Salir\n");
		printf("Ingrese opcion: ");
		scanf("%d", &opcion);
		switch(opcion) {
			case 1:
				system("cls");
				altaAlumno(archivo, alumno);
				break;
			case 2:
				system("cls");
				listarAlumno(archivo, alumno);
				break;
			case 3:
				system("cls");
				bajaFisicaAlumno(archivo, alumno);
				break;
			case 4:
				system("cls");
				bajaLogicaAlumno(archivo, alumno);
				break;
			case 5:
				system("cls");
				ordenarRegistrosAlumnos(archivo, alumno);
				break;
		}
	} while(opcion != 0);
	return(0);
}

void altaAlumno(FILE *archivo, registro_alumno &alumno) {
	int cantidad;
	archivo = fopen("alumno.dat", "wb");
	printf("Ingrese cantidad alumno: ");
	scanf("%d", &cantidad);
	for(int i = 0; i < cantidad; i++) {
		printf("--------------------------\n");
		printf("Ingrese Alumno %d\n", i);
		printf("Ingrese legajo: ");
		scanf("%d", &alumno.legajo);
		_flushall();
		printf("Ingrese nombre: ");
		gets(alumno.nombre);
		printf("Ingrese nota: ");
		scanf("%f", &alumno.nota);
		alumno.borrado = false;
		fwrite(&alumno, sizeof(registro_alumno), 1, archivo);
	}
	fclose(archivo);
	printf("\n");
	system("pause");
}

void listarAlumno(FILE *archivo, registro_alumno &alumno) {
	archivo = fopen("alumno.dat", "rb");
	fread(&alumno, sizeof(registro_alumno), 1, archivo);
	while(!feof(archivo)) {
		if(alumno.borrado == false) {
			printf("\n----------------------------------\n");
			printf("Legajo: %d\n", alumno.legajo);
			printf("Nombre: ");
			puts(alumno.nombre);
			printf("Nota: %.2f", alumno.nota);
		}
		fread(&alumno, sizeof(registro_alumno), 1, archivo);
	}
	fclose(archivo);
	printf("\n");
	system("pause");
}

void bajaFisicaAlumno(FILE *archivo, registro_alumno &alumno) {
	int legajo, bandera = 0;
	FILE *auxiliar;
	archivo = fopen("alumno.dat", "r+b");
	auxiliar = fopen("auxiliar.dat", "wb");
	printf("Ingrese legajo a dar de baja: ");
	scanf("%d", &legajo);
	fread(&alumno, sizeof(registro_alumno), 1, archivo);
	while(!feof(archivo)) {
		if(alumno.legajo != legajo) {
			fwrite(&alumno, sizeof(registro_alumno), 1, auxiliar);
		} else {
			bandera = 1;
		}
		fread(&alumno, sizeof(registro_alumno), 1, archivo);
	}
	if(bandera == 1) {
		printf("Registro con legajo %d dado de baja!", legajo);
	} else {
		printf("No existe el Registro con legajo %d\n", legajo);
	}
	fclose(archivo);
	fclose(auxiliar);
	remove("alumno.dat");
	rename("auxiliar.dat", "alumno.dat");
	printf("\n");
	system("pause");
}

void bajaLogicaAlumno(FILE *archivo, registro_alumno &alumno) {
	int legajo, bandera = 0;
	archivo = fopen("alumno.dat", "r+b");
	printf("Ingrese legajo para dar de baja logica: ");
	scanf("%d", &legajo);
	fread(&alumno, sizeof(registro_alumno), 1, archivo);
	while(!feof(archivo) && bandera == 0) {
		if(alumno.legajo == legajo && alumno.borrado == false) {
			alumno.borrado = true;
			fseek(archivo, -(long)sizeof(registro_alumno), SEEK_CUR);
			fwrite(&alumno, sizeof(registro_alumno), 1, archivo);
			bandera = 1;
		}
		fread(&alumno, sizeof(registro_alumno), 1, archivo);
	}
	if(bandera == 1) {
		printf("Registro con legajo %d dado de baja logica!", legajo);
	} else {
		printf("No existe el Registro con legajo %d\n", legajo);
	}
	fclose(archivo);
	printf("\n");
	system("pause");
}

void ordenarRegistrosAlumnos(FILE *archivo, registro_alumno &alumno) {
	registro_alumno alumnos[50], auxiliar;
	int contador = 0, bandera = 0;
	archivo = fopen("alumno.dat", "rb");
	fread(&alumno, sizeof(registro_alumno), 1, archivo);
	while(!feof(archivo)) {
		alumnos[contador] = alumno;
		fread(&alumno, sizeof(registro_alumno), 1, archivo);
		contador++;
	}

	fclose(archivo);

	do {
		bandera = 0;
		for(int i = 0; i < contador - 1; i++) {
			if(alumnos[i].legajo > alumnos[i + 1].legajo) {
				auxiliar = alumnos[i];
				alumnos[i] = alumnos[i + 1];
				alumnos[i + 1] = auxiliar;
				bandera = 1;
			}
		}
	} while(bandera == 1);

	archivo = fopen("alumno.dat", "wb");
	for(int i = 0; i < contador; i++) {
		fwrite(&alumnos[i], sizeof(registro_alumno), 1, archivo);
	}
	fclose(archivo);
	printf("Registros ordenados!\n");
	system("pause");
}