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
 cout << "children : " << children << endl;
 cout << "unario : " << encoder_unario(personas, children) << endl;
 return 0;
}
