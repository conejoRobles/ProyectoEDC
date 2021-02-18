#include <iostream>
#include <algorithm>
#include <sdsl/bit_vectors.hpp>
#include <vector>
using namespace std;
using namespace sdsl;

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

int first_child(bit_vector b, int i)
{
 rank_support_v<1> b_rank(&b);
 bit_vector::select_0_type b_sel(&b);
 int firstChild = (b_sel(b_rank(i)) + 1);
 return firstChild;
}

int parent(bit_vector b, int i)
{
 rank_support_v<0> b_rank(&b);
 bit_vector::select_0_type b_sel(&b);
 bit_vector::select_1_type b_sel1(&b);
 int parent = (b_sel(b_rank(b_sel1(b_rank(i)))) + 1);
 return parent;
}

int next_sibling(bit_vector b, int i)
{
 int next = b[i + 1];
 if (next == 1)
 {
		return (i + 1);
 }
 return 0;
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
		cout << " nombre : " << personas[i].getName() << "\n cargo  : " << personas[i].getCargo() << "\n=======================================" << endl;
 }
 bit_vector unario = encoder_unario(personas, children);
 cout << "LOUD : " << unario << endl;
 int fistc = first_child(unario, 1);
 cout << "first Child : " << fistc << endl;
 cout << "LOUD[First Child] : " << unario[fistc] << endl;
 int siblin = next_sibling(unario, 2);
 cout << "nextSibling : " << siblin << endl;
 cout << "LOUD[siblin] : " << unario[siblin] << endl;
 // int parentS = parent(unario, 3);
 // cout << "parent : " << parentS << endl;
 // cout << "LOUD[parent] : " << unario[parentS] << endl;
 return 0;
}
