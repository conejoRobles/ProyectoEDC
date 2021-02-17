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

 string children;
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
 return 0;
}
