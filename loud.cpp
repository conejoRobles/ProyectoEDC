#include <iostream>
#include <algorithm>
#include <sdsl/bit_vectors.hpp>
#include <vector>
using namespace std;
using namespace sdsl;

//Clase Persona (PARA ORGANIGRAMA)
class Persona
{
private:
 string name;
 string cargo;

public:
 Persona(const string &_name, const string &_cargo)
 {
		name = _name;
		cargo = _cargo;
 }
 const string &getName() const { return name; }
 const string &getCargo() const { return cargo; }
};

//IMPLEMENTACIÓN DE LOUD CON BIT_VECTOR DE SDSL

/*First Child, este método devuelve un entero que representa el índice dentro del bit_vector con el primer hijo del un nodo dado, 
de no poseer un hijo el método retorna -1.*/
int first_child(bit_vector b, int i)
{
 rank_support_v<1> b_rank(&b);
 bit_vector::select_0_type b_sel(&b);
 int rank = b_rank(i);
 int firstChild;
 if (rank == 0)
 {
		firstChild = b_sel(1) + 1;
 }
 else
 {
		firstChild = (b_sel(b_rank(i + 1)) + 1);
 }
 if (b[firstChild] == 1)
 {
		return firstChild;
 }
 return -1;
}

/*Next Sibling, este método devuelve un entero que representa el indice dentro del bit_vector con el siguiente nodo hermano de un nodo dado,
de no poseer uno o más hermanos, el método retorna -1*/
int next_sibling(bit_vector b, int i)
{
 int next = b[i + 1];
 if (next == 1)
 {
		return (i + 1);
 }
 return -1;
}

/*Parent, este método devulve un entero que representa el indice dentro del bit_vector con el padre de un nodo dado,
al ser nodo raiz el método retorna el mismo nodo*/
int parent(bit_vector b, int i)
{
 rank_support_v<0> b_rank(&b);
 bit_vector::select_1_type b_sel(&b);
 if (b_rank(i) == 0)
 {
		return 0;
 }
 int parent = b_sel(b_rank(i));
 return parent;
}

//Child, este método devuelve un entero que representa el i-esimo hijo de un determinado nodo
int child(bit_vector b, int p, int i)
{
 rank_support_v<1> b_rank(&b);
 bit_vector::select_0_type b_sel(&b);
 int i_child = b_sel(b_rank(p + i - 1)) + i;
 return i_child;
}

//Data, este método es básicamente un toString a partir de un indice de referencia dentro del bit_vector
void data(bit_vector b, vector<Persona> personas, int i)
{
 rank_support_v<1> b_rank(&b);
 cout << " nombre : " << personas[b_rank(i)].getName() << "\n cargo  : " << personas[b_rank(i)].getCargo() << endl;
}

//Método para creación de bit_vector códificado en unario basado en "1"
bit_vector encoder_unario(vector<Persona> personas, string children)
{
 int sum = 0;
 for (int i = 0; i < children.length(); i++)
 {
		sum += (int)children.at(i) - 48;
 }
 //codificando con unarios
 int n = sum + children.length();
 bit_vector out(n, 1); //bitvetor de tama  o n lleno de
 int pos = 0;
 for (int i = 0; i < children.length(); i++)
 {
		out[(int)children.at(i) - 48 + pos] = 0;
		pos += (int)children.at(i) - 48 + 1;
 }
 return out;
}

//Método para creación de Organigrama
vector<Persona> addEmpleado(vector<Persona> personas, string *children, string nombrePadre)
{
 string nombre;
 string cargo;
 string subordinados;
 string auxChild = (*children);
 if (nombrePadre.compare("start") != 0)
 {
		cout << "\nInserte el nombre del trabajador(a) subordinado de " << nombrePadre << "\n"
							<< endl;
		getline(cin, nombre);
		cout << "\nInserte el cargo del trabajador(a) " << nombre << "\n"
							<< endl;
		getline(cin, cargo);
		cout << "\nCuantos subordinados posee " << nombre << "?\n"
							<< endl;
		cin >> subordinados;
 }
 else
 {
		cout << "\nInserte el nombre del trabajador(a)\n"
							<< endl;
		getline(cin, nombre);
		cout << "\nInserte el cargo del trabajador(a)\n"
							<< endl;
		getline(cin, cargo);
		cout << "\nCuantos subordinados posee " << nombre << "?\n"
							<< endl;
		cin >> subordinados;
 }
 *children = auxChild.append(subordinados);

 Persona persona1 = Persona(nombre, cargo);
 personas.push_back(persona1);
 getline(cin, auxChild);
 for (int i = 0; i < stoi(subordinados); i++)
 {
		personas = addEmpleado(personas, children, nombre);
 }
 return personas;
}

int main()
{

 string children = "1";
 vector<Persona> personas;

 cout << "\n======= LOUD PARA ORGANIGRAMAS ========\n"
						<< endl;

 personas = addEmpleado(personas, &children, "start");

 cout << "\n========= Lista de Empleados ==========\n"
						<< endl;

 for (int i = 0; i < personas.size(); i++)
 {
		cout << " id     : " << (i + 1) << "\n nombre : " << personas[i].getName() << "\n cargo  : " << personas[i].getCargo() << "\n=======================================" << endl;
 }
 bit_vector unario = encoder_unario(personas, children);
 rank_support_v<1> b_rank(&unario);
 bit_vector::select_1_type b_sel(&unario);
 bool salir = false;
 int sal;
 int nodo = 0;
 int hijo = -1;
 while (!salir)
 {
		cout << "========== MENU =========" << endl;
		cout << "= 1) Subordinados       =" << endl;
		cout << "= 2) Jefe               =" << endl;
		cout << "= 3) Colegas            =" << endl;
		cout << "= 4) Cadena de mando    =" << endl;
		cout << "= 5) Buscar Nodo        =" << endl;
		cout << "= 6) Imprimir Árbol     =" << endl;
		cout << "= 7) Salir              =" << endl;
		cout << "=========================" << endl;
		cout << "Indique una opción: ";
		cin >> sal;
		cout << unario << endl;
		switch (sal)
		{
		case 1:
			cout << "Indique el nodo del cual desea saber sus subordinados: ";
			cin >> nodo;
			nodo = b_sel(nodo);
			hijo = first_child(unario, nodo);
			cout << nodo << "NODO" << endl;
			cout << "\n========= Lista de Subordinados ==========\n"
								<< endl;
			if (hijo == -1)
			{
				cout << "ESTE TRABAJADOR NO POSEE SUBORDINADOS\n";
				cout << "\n=========================================\n"
									<< endl;
			}
			while (hijo > -1)
			{
				data(unario, personas, hijo);
				cout << "\n==========================================\n"
									<< endl;
				hijo = next_sibling(unario, hijo);
			}
			break;
		case 2:
			cout << "Indique el nodo del cual desea saber su jefe: ";
			cin >> nodo;
			nodo = b_sel(nodo);
			hijo = parent(unario, nodo);
			data(unario, personas, hijo);
			break;
		case 3:
			cout << "Indique el nodo del cual desea saber sus colegas: ";
			cin >> nodo;
			nodo = b_sel(nodo);
			hijo = parent(unario, nodo);
			hijo = first_child(unario, hijo);
			cout << hijo << endl;
			if (nodo == 0)
			{
				cout << "\n=========================================\n";
				cout << "\nESTE TRABAJADOR NO POSEE COLEGAS\n";
				cout << "\n=========================================\n"
									<< endl;
			}
			else
			{
				cout << "\n============ Lista de Colegas ==============\n"
									<< endl;
				while (hijo > -1)
				{
					if (hijo != nodo)
					{
						data(unario, personas, hijo);
						cout << "\n==========================================\n"
											<< endl;
					}
					hijo = next_sibling(unario, hijo);
				}
			}
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			salir = !salir;
			break;
		deaful:
			break;
		}
 }
 return 0;
}
