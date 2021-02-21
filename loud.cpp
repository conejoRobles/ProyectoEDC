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
al ser nodo raiz el método retorna -1*/
int parent(bit_vector b, int i)
{
 rank_support_v<0> b_rank(&b);
 bit_vector::select_1_type b_sel(&b);
 if (b_rank(i) == 0)
 {
		return -1;
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
void data(bit_vector b, vector<Persona> personas, int i, bool cargo)
{
 rank_support_v<1> b_rank(&b);
 if (cargo)
 {
		cout << " nombre : " << personas[b_rank(i)].getName() << "\n cargo  : " << personas[b_rank(i)].getCargo() << endl;
 }
 else
 {
		cout << " nombre : " << personas[b_rank(i)].getName() << endl;
 }
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
vector<Persona> addEmpleado(vector<Persona> personas, string *children, string nombrePadre, int indice)
{
 string nombre;
 string cargo;
 string subordinados;
 string auxChild = (*children);

 if (indice == 0)
 {
		cout << "\nInserte el nombre del trabajador(a)\n"
							<< endl;
		getline(cin, nombre);
 }
 else
 {
		nombrePadre = personas[indice].getName();
		getline(cin, nombre);
		cout << "\nInserte el nombre del trabajador(a) subordinado de " << nombrePadre << "\n"
							<< endl;
		getline(cin, nombre);
 }
 cout << "\nInserte el cargo del trabajador(a) " << nombre << "\n"
						<< endl;
 getline(cin, cargo);
 cout << "\nCuantos subordinados posee " << nombre << "?\n"
						<< endl;
 cin >> subordinados;

 *children = auxChild.append(subordinados);
 Persona persona1 = Persona(nombre, cargo);
 personas[indice] = persona1;

 for (int i = 0; i < stoi(subordinados); i++)
 {
		Persona persona1 = Persona(nombre, cargo);
		personas.push_back(persona1);
 }
 if ((indice + 1) < personas.size())
 {
		personas = addEmpleado(personas, children, nombre, (indice + 1));
 }
 return personas;
}

void soutPersonas(vector<Persona> personas)
{
 cout << "\n========= Lista de Empleados ==========\n"
						<< endl;
 for (int i = 0; i < personas.size(); i++)
 {
		cout << " id     : " << (i + 1) << "\n nombre : " << personas[i].getName() << "\n cargo  : " << personas[i].getCargo() << "\n=======================================" << endl;
 }
}

void tree(bit_vector b, vector<Persona> personas, int i, int tab)
{
 bit_vector::select_1_type b_sel(&b);
 int nodo;
 int hijo;
 tab++;
 if (i == 0)
 {
		nodo = b_sel(i + 1);
 }
 else
 {
		nodo = b_sel(i);
 }
 if (nodo < b.size())
 {
		hijo = first_child(b, nodo);
		if (hijo > -1)
		{
			for (i = 0; i < tab; i++)
			{
				cout << "\t";
			}
			cout << "|\n";
			for (i = 0; i < tab; i++)
			{
				cout << "\t";
			}
			cout << "- ";
			data(b, personas, hijo, false);
			tree(b, personas, hijo, tab);
			hijo = next_sibling(b, hijo);
			while (hijo > -1)
			{
				for (i = 0; i < tab; i++)
				{
					cout << "\t";
				}
				cout << "|\n";
				for (i = 0; i < tab; i++)
				{
					cout << "\t";
				}
				cout << "- ";
				data(b, personas, hijo, false);
				tree(b, personas, hijo, tab);
				hijo = next_sibling(b, hijo);
			}
		}
 }
}

int main()
{

 string children = "1";
 vector<Persona> personas;

 cout << "\n======= LOUD PARA ORGANIGRAMAS ========\n"
						<< endl;
 personas.push_back({"asdlkfj", "aksdfjalsdf"});
 personas = addEmpleado(personas, &children, "start", 0);

 bit_vector unario = encoder_unario(personas, children);
 rank_support_v<1> b_rank(&unario);
 bit_vector::select_1_type b_sel(&unario);
 vector<int> jefes;
 bool salir = false;
 bool pillado = false;
 string nombre;
 int sal;
 int nodo = 0;
 int hijo = -1;
 soutPersonas(personas);
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
		cout << "\nbit vector: " << unario << endl;
		switch (sal)
		{
		case 1:
			soutPersonas(personas);
			cout << "Indique el nodo del cual desea saber sus subordinados: ";
			cin >> nodo;
			nodo = b_sel(nodo);
			hijo = first_child(unario, nodo);
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
				data(unario, personas, hijo, true);
				cout << "\n==========================================\n"
									<< endl;
				hijo = next_sibling(unario, hijo);
			}
			break;
		case 2:
			soutPersonas(personas);
			cout << "Indique el nodo del cual desea saber su jefe: ";
			cin >> nodo;
			if (nodo == 1)
			{
				cout << "\n=========================================\n";
				cout << "Este trabajador no posee jefes";
				cout << "\n=========================================\n"
									<< endl;
			}
			else
			{
				nodo = b_sel(nodo);
				hijo = parent(unario, nodo);
				cout << "\n=========================================\n";
				data(unario, personas, hijo, true);
				cout << "\n=========================================\n"
									<< endl;
			}
			break;
		case 3:
			soutPersonas(personas);
			cout << "Indique el nodo del cual desea saber sus colegas: ";
			cin >> nodo;
			if (nodo == 1)
			{
				cout << "\n=========================================\n";
				cout << "\nESTE TRABAJADOR NO POSEE COLEGAS\n";
				cout << "\n=========================================\n"
									<< endl;
			}
			else
			{
				nodo = b_sel(nodo);
				hijo = parent(unario, nodo);
				hijo = first_child(unario, hijo);
				cout << "\n============ Lista de Colegas ==============\n"
									<< endl;
				while (hijo > -1)
				{
					if (hijo != nodo)
					{
						data(unario, personas, hijo, true);
						cout << "\n==========================================\n"
											<< endl;
					}
					hijo = next_sibling(unario, hijo);
				}
			}
			break;
		case 4:
			soutPersonas(personas);
			cout << "Indique el nodo del cual desea saber su cadena de mando: ";
			cin >> nodo;
			if (nodo == 1)
			{
				cout << "\n=========================================\n";
				cout << "Este trabajador no posee jefes";
				cout << "\n=========================================\n"
									<< endl;
			}
			else
			{
				nodo = b_sel(nodo);
				hijo = parent(unario, nodo);
				cout << "\n"
									<< endl;
				while (hijo > -1)
				{
					jefes.push_back(hijo);
					hijo = parent(unario, hijo);
				}
				if (!jefes.empty())
				{
					cout << "\n============ Cadena de Mando =============\n"
										<< endl;
					for (int i = (jefes.size()); i > 0; i--)
					{
						data(unario, personas, jefes[i - 1], true);
						cout << "\n==========================================\n"
											<< endl;
					}
					jefes.clear();
				}
			}
			break;
		case 5:
			getline(cin, nombre);
			cout << "Indique el nombre del nodo a buscar: ";
			getline(cin, nombre);
			for (int i = 0; i < personas.size(); i++)
			{
				if (!personas[i].getName().compare(nombre))
				{
					cout << "\n==========================================\n";
					cout << " id     : " << (i + 1) << "\n nombre : " << personas[i].getName() << "\n cargo  : " << personas[i].getCargo() << "\n=======================================" << endl;
				}
			}
			break;
		case 6:
			cout << "-";
			data(unario, personas, 0, false);
			tree(unario, personas, nodo, 0);
			// soutPersonas(personas);
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
