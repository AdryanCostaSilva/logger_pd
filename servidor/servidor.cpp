#include <iostream>
#include <fstream>
#include <string>
#include "LoggerI.h"
#include <orbsvcs/CosNamingC.h>

using namespace std;
using namespace CORBA;
using namespace PortableServer;
using namespace CosNaming;

ORB_var orb;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		cerr << "USD: " << argv[0] << " <nome_do_servidor>\n";
		return 1;
	}
	try{
		// 1. Inicia ORB
		orb = ORB_init(argc, argv, "ORB");

		// 2. Ativa RootPOA
		Object_ptr tmp = orb->resolve_initial_references("RootPOA");
		POA_var poa = POA::_narrow(tmp);
		POAManager_var ger = poa->the_POAManager();
		ger->activate();

		// 3. Instancia "servants"
		Logger_i li;

		// 4. Registra servos no POA, criando objetos distribuídos
		Logger_var logger = li._this();

		// 5. Publica IOR (servidor de nomes)
		tmp = orb->resolve_initial_references("NameService");
		NamingContext_var ns = NamingContext::_narrow(tmp);

		Name nome(1);
		nome.length(1);
		nome[0].id = string_dup(argv[1]);

		ns->rebind(nome, logger.in());

		String_var ior = orb->object_to_string(logger.in());
		ofstream arq("logger.ior");
		arq << ior;
		arq.close();
		cout << "IOR salva: logger.ior\n";

		// 6. Aguarda requisições
		cout << "Aguardando requisicoes...\n";
		orb->run();

		// 7. Finalizações
		poa->destroy(true, true);
		orb->destroy();
	} catch (const CORBA::Exception& e){
		cerr << "Erro CORBA: " << e << endl; 
	}
    return 0;
}
