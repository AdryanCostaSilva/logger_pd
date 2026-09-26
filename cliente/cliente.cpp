#include <iostream>
#include <string>
#include <LoggerC.h>
#include <orbsvcs/CosNamingC.h>
#include <ctime>

using namespace std;
using namespace CORBA;
using namespace CosNaming;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		cerr << "USO: " << argv[0] << " <nome_do_servidor>\n";
		return 1;
	}

	try {
		// 1. Inicializa ORB
		ORB_var orb = ORB_init(argc, argv, "ORB");

		// 2. Obtém referência para objeto distirbuído (da IOR)
		Object_ptr tmp = orb->resolve_initial_references("NameService");
		NamingContext_var ns = NamingContext::_narrow(tmp);

		Name nome(1);
		nome.length(1);
		nome[0].id = string_dup(argv[1]);

		tmp = ns->resolve(nome); 

		Logger_var logger = Logger::_narrow(tmp);

		// 3. Usa objeto (chama métodos)
		CORBA::ULong hora = static_cast<CORBA::ULong>(time(nullptr));

		logger->log(Logger::DEBUG, "196.23.43.11:5051", 1, hora, "Bugado");

		logger->log(Logger::WARNING, "196.23.43.12:5052", 2, hora, "Warnado");
		
		logger->log(Logger::ERROR, "196.23.43.13:5053", 3, hora, "Errado");

		// Teste de excecao
		try {
			cout << logger->locate(Logger::CRITICAL);
		}
		catch (const SemEventosComNivel&){
			cout << "No existem eventos com nivel CRITICAL" << endl;
		}

		logger->log(Logger::CRITICAL, "196.23.43.14:5054", 4, hora, "Criticado");

		try {
			cout << logger->locate(Logger::DEBUG) << endl;
		}
		catch (const SemEventosComNivel&){
			cout << "No existem eventos com nivel DEBUG" << endl;
		}
		
		try {
			cout << logger->locate(Logger::WARNING) << endl;
		}
		catch (const SemEventosComNivel&){
			cout << "No existem eventos com nivel WARNING" << endl;
		}

		try {
			cout << logger->locate(Logger::ERROR) << endl;
		}
		catch (const SemEventosComNivel&){
			cout << "No existem eventos com nivel ERROR" << endl;
		}

		try {
			cout << logger->locate(Logger::CRITICAL) << endl;
		}
		catch (const SemEventosComNivel&){
			cout << "No existem eventos com nivel CRITICAL" << endl;
		}

		// 4. Finalizações
		orb->destroy();
	} catch (const CORBA::Exception& e) {
		cerr << "Erro CORBA: " << e << endl;
	}

	return 0;
}
