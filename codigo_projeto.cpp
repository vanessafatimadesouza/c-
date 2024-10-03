/*	Projeto Pratico 2023_2 Introducao aos Algoritmos
  Tema: Gerenciamento de base de dados de componentes eletronicos, utilizados
  em um programa de desenvolvimento de circuitos eletronicos
  Grupo: Gustavo Rocha Morão Cherubim, Giovanni Ethel Alves Moscardini e
  Vanessa Fátima de Souza
  Turma: 22W */

#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
using namespace std;

const int tam = 200;
int cadastros = 0;

struct Eletronicos {
	//	Definição dos campos de componente eletrônico

	int id;            // Numero inteiro de identificacao de um componente
	string nome;       // Nome do componente
	string fabricante; // Fabricante do componente
	string tipo;       // Tipo de encapsulamento - PTH ou SMD
	float corrente;    // Corrente maxima do componente
	float tensao;      // Tensao maxima do componente
	int pinos;         // Quantidade de pinos do componente
};

void marca_idNegativo(Eletronicos componentes[], int tam) {
	for (int i = 0; i < tam; i++) 
	{
		componentes[i].id = -1; // Inicializa todos os IDs como -1
	}
}

void shellshort_corrente(Eletronicos componentes[], int tam) {
	int gap = tam / 2;

	while (gap > 0) 
	{
		for (int i = gap; i < tam; i++) 
		{
			if (componentes[i].id > -1) 
			{
				Eletronicos aux = componentes[i];
				int j = i;

				while (j >= gap and componentes[j - gap].corrente > aux.corrente) 
				{
					componentes[j] = componentes[j - gap];
					j -= gap;
				}
				componentes[j] = aux;
			}
		}
		gap /= 2;
	}
}

void shellshort_tensao(Eletronicos componentes[], int tam) {
	int gap = tam / 2;

	while (gap > 0) 
	{
		for (int i = gap; i < tam; i++) 
		{
			if (componentes[i].id > -1) 
			{
				Eletronicos aux = componentes[i];
				int j = i;

				while (j >= gap and componentes[j - gap].tensao > aux.tensao) 
				{
					componentes[j] = componentes[j - gap];
					j -= gap;
				}
				componentes[j] = aux;
			}
		}
		gap /= 2;
	}
}

void shellshort_ID(Eletronicos componentes[], int tam) {
	int gap = tam / 2;

	while (gap > 0) 
	{
		for (int i = gap; i < tam; i++) 
		{
			if (componentes[i].id > -1)
			{
				Eletronicos aux = componentes[i];
				int j = i;

				while (j >= gap and componentes[j - gap].id > aux.id) 
				{
					componentes[j] = componentes[j - gap];
					j -= gap;
				}
				componentes[j] = aux;
			}
		}
		gap /= 2;
	}
}

void converterCSVparaBinario(Eletronicos componentes[], int tam, const string &arquivoCSV, const string &arquivoBinario) {

	ifstream entrada(arquivoCSV);

	// Verifica se o arquivo CSV pode ser aberto
	if (!entrada.is_open()) 
	{
		cout << "Erro ao abrir o arquivo." << endl;
		return;
	}

	// Lê dados do arquivo CSV e passa-os para a struct
	string leitura;
	int i = 0;
	getline(entrada, leitura); // Cabeçalho
	while (getline(entrada, leitura) and i < tam)
	{
		string info;
		stringstream linha(leitura);

		getline(linha, info, ';');
		componentes[i].id = stoi(info);

		getline(linha, info, ';');
		componentes[i].nome = info;

		getline(linha, info, ';');
		componentes[i].fabricante = info;

		getline(linha, info, ';');
		componentes[i].tipo = info;

		getline(linha, info, ';');
		componentes[i].corrente = stof(info);

		getline(linha, info, ';');
		componentes[i].tensao = stof(info);

		getline(linha, info);
		componentes[i].pinos = stoi(info);

		i++;
		cadastros++;
	}

	entrada.close();

	ofstream saida(arquivoBinario, ios::binary);

	// Verifica se o arquivo Binario pode ser aberto
	if (!saida.is_open()) 
	{
		cout << "Erro ao abrir os arquivos." << endl;
		return;
	}

	// Escreve os dados no arquivo binario
	for (int i = 0; i < tam; i++) 
	{
		saida.seekp((i) * sizeof(int));
		saida.write(reinterpret_cast<const char *>(&componentes[i]), sizeof(int));
	}
	saida.close();

	cout << "Conversao concluida." << endl;
}

string lerBinario(Eletronicos componentes[], const string &arquivoBinario,int posicao, string nome) 
{

	ifstream entrada(arquivoBinario, ios::binary);

	if (!entrada.is_open()) 
	{
		cout << "Erro ao abrir os arquivos." << endl;
	}

	entrada.seekg((posicao) * sizeof(int));
	entrada.read(reinterpret_cast<char *>(&componentes[posicao]),sizeof(int)); // Lê o binário

	entrada.close();
	return componentes[posicao].nome;
}

void imprimirDados(Eletronicos componentes[], int pos) {
	// Imprime as informações do registro

	cout << endl;
	cout << "ID: " << componentes[pos].id << endl;
	cout << "Nome do componente: " << componentes[pos].nome << endl;
	cout << "Fabricante: " << componentes[pos].fabricante << endl;
	cout << "Tipo de encapsulamento: " << componentes[pos].tipo << endl;
	cout << "Corrente maxima: " << componentes[pos].corrente << endl;
	cout << "Tensao maxima: " << componentes[pos].tensao << endl;
	cout << "Quantidade de pinos: " << componentes[pos].pinos << endl;
	cout << endl;
}

int testeId(Eletronicos componentes[], int id, int tam) {
	int teste = -1; // Variável para teste
	for (int i = 0; i < tam; i++) 
	{
		if (componentes[i].id == id) 
		{
			teste = i; // Se encontrar id a variável assume o valor de i
		}
	}
	if (teste > -1) // Se variável é maior que -1 return o valor dela
	{
		return teste;
	} 
	else // Caso não encontrado, retorna -1
	{
		return -1;
	}
}

int novoId(Eletronicos componentes[], int cadastros) {
	// Código para geração de id aleatóio com auxílio da biblioteca random e ctime
	time_t seed = time(nullptr); //
	mt19937 generator(seed);
	uniform_int_distribution<int> distribution(1000, 9999);
	int numeroGerado = distribution(generator);
	if (cadastros == 0) 
	{
		return numeroGerado;
	} 
	else if (testeId(componentes, numeroGerado, tam) == -1) 
	{
		return numeroGerado;
	} 
	else if (testeId(componentes, numeroGerado, tam) > -1) // Id já existente, retorna novoId novamente
	{
		return novoId(componentes, cadastros);
	}
	return 0;
}

void cadastrarComponente(Eletronicos componentes[], int *cadastros, int tam) {
	int j; // Será a posição
	bool disponivel = false;
	string aux;
	for (int i = 0; i < tam; i++) 
	{
    // Condicional avaliando se a posição está disponível e uma variável booleana encerrando o laço se disponível
		if ((componentes[i].id < 0) and (disponivel == false)) 
		{
			j = i;
			disponivel = true;
		}
	}

	cout << "Insira o nome do componente: ";
	cin.ignore();
	getline(cin, aux);

	componentes[j].nome = aux;
	cout << "Insira o Fabricante: ";
	getline(cin, componentes[j].fabricante);
	cout << "Insira o tipo: ";
	getline(cin, componentes[j].tipo);
	cout << "Insira a Corrente Maxima: ";
	cin >> componentes[j].corrente;
	cout << "Insira a Tensao Maxima: ";
	cin >> componentes[j].tensao;
	cout << "Insira a quantidade de Pinos: ";
	cin >> componentes[j].pinos;
	componentes[j].id = novoId(componentes, tam);
	cout << "Componente registrado com sucesso! Seu numero de identificacao e: " << componentes[j].id << endl;
	*cadastros = *cadastros + 1;
}

void removerComponente(Eletronicos componentes[], int *cadastros) {
	int id;
	cout << "Insira o Id do componente que deseja excluir: ";
	cin >> id;

	int posicao = -1; // Variável auxiliar
	for (int i = 0; i < tam; i++) 
	{
		if (componentes[i].id == id) 
		{
			posicao = i; // Se encontrado o id, posição assume o valor de i
		}
	}

	if (posicao > -1) 
	{
		componentes[posicao].id = -1; // Marcação para que a função salvarAlterações reconheça e remova suas informações
		cout << "Componente excluido com sucesso!" << endl;
		*cadastros = *cadastros - 1;
	} 
	else 
	{
		cout << "Não foi possível localizar o componente!" << endl;
	}
}

void buscaNome(Eletronicos componentes[], int tam, const string &arquivoBinario) {
	// Realiza busca por nome de componente

	string nomeBuscado, busca, nome;
	int contador = 0;
	bool identificado = false;
	cout << "Digite o nome do componente buscado: ";
	cin.ignore();
	getline(cin, nomeBuscado);

	for (char &caractere : nomeBuscado)
	{
		caractere = toupper(caractere); // Coloca todos os caracteres em letra maiúscula
	}

	ifstream entrada(arquivoBinario, ios::binary);

	if (!entrada.is_open()) 
	{
		cout << "Erro ao abrir os arquivos." << endl;
	}

	for (int i = 0; i < tam; i++) 
	{
		busca = lerBinario(
        componentes, arquivoBinario, i, nome); // Variável irá receber o nome dos componentes do arq para busca
		for (char &caractere : busca) 
		{
			caractere = toupper(caractere); // Coloca todos os caracteres em letra maiúscula
		}
		if ((busca == nomeBuscado) and (componentes[i].id > -1)) // Se encontrado irá imprimir seus dados
		{
			imprimirDados(componentes, i);
			contador++;
			identificado = true;
		}
	}
	cout << endl << "Quantidade de " << nomeBuscado << " encontrado: " << contador << endl;

	if (identificado == false) 
	{
		cout << "Componente com esse nome nao encontrado!" << endl;
	}
	entrada.close();
}

void buscaTensao(Eletronicos componentes[], int tam, const string &arquivoBinario) {
	// Realiza busca por tensão de componente

	float tensaoBuscada, busca;
	int contador = 0;
	bool identificado = false;
	cout << "Digite a Tensao buscada (casas decimais separadas por ponto,ex: 2.2): ";
	cin >> tensaoBuscada;

	ifstream entrada (arquivoBinario, ios::binary);

	if (!entrada.is_open())
	{
		cout << "Erro ao abrir os arquivos." << endl;
	}

	for (int i = 0; i < tam; i++) 
	{
		entrada.seekg((i)*sizeof(int));
		entrada.read(reinterpret_cast < char *> (&componentes[i]), sizeof(int)); // Lê o binário
		busca = componentes[i].tensao; 
		if ((busca == tensaoBuscada) and (componentes[i].id > -1)) // Se encontrado irá imprimir seus dados
		{
			imprimirDados(componentes, i);
			contador++;
			identificado = true;
		}
	}

	cout << endl << "Quantidade de componentes com a Tensao maxima " << tensaoBuscada << " encontrados: " << contador << endl;

	if (identificado == false) 
	{
		cout << "Componente com essa tensao nao encontrado!" << endl;
	}
	entrada.close();
}

void imprimeComponentes(Eletronicos componentes[], int tam) {
	for (int i = 0; i < tam; i++) 
	{
		if (componentes[i].id > -1) 
		{
			imprimirDados(componentes, i);
		}
	}
}

void imprimeIntervalo_corrente(Eletronicos componentes[], int tam) {
	float comeco_corrente, fim_corrente;
	
	shellshort_corrente(componentes, tam);
	cout << "Inicio do intervalo de Corrente Maxima desejada: ";
	cin >> comeco_corrente;
	cout << "Fim do intervalo de Corrente Maxima desejada: ";
	cin >> fim_corrente;

	for (int i = 0; i < tam; i++) 
	{
		if (componentes[i].id > -1) 
		{
			if ((componentes[i].corrente >= comeco_corrente) and (componentes[i].corrente <= fim_corrente)) 
			{
				imprimirDados(componentes, i);
			}
		}
	}
}

void imprimeIntervalo_tensao(Eletronicos componentes[], int tam) {
	float comeco_tensao, fim_tensao;

	shellshort_tensao(componentes, tam);
	cout << "Inicio do intervalo de Tensao Maxima desejada: ";
	cin >> comeco_tensao;
	cout << "Fim do intervalo de Tensao Maxima desejada: ";
	cin >> fim_tensao;

	for (int i = 0; i < tam; i++) 
	{
		if (componentes[i].id > -1) 
		{
			if ((componentes[i].tensao >= comeco_tensao) and (componentes[i].tensao <= fim_tensao)) 
			{
				imprimirDados(componentes, i);
			}
		}
	}
}

void salvarAlteracao(Eletronicos componentes[], int tam, const string &arquivoBinario, const string &nomeArquivo) {
	ofstream arquivo(nomeArquivo);

	if (!arquivo.is_open()) 
	{
		cout << "Erro ao abrir o arquivo." << endl;
		return;
	}

	arquivo << "ID;Nome;Fabricante;Tipo;Corrente;Tensao;Pinos" << endl; // Cabeçalho do arquivo CSV

	for (int i = 0; i < tam; i++) 
	{
		if (componentes[i].id > -1) 
		{
			arquivo << componentes[i].id << ";";
			arquivo << componentes[i].nome << ";";
			arquivo << componentes[i].fabricante << ";";
			arquivo << componentes[i].tipo << ";";
			arquivo << componentes[i].corrente << ";";
			arquivo << componentes[i].tensao << ";";
			arquivo << componentes[i].pinos << endl;
		}
	}

	ofstream saida(arquivoBinario, ios::binary);

	// Escreve os dados no arquivo binario
	for (int i = 0; i < tam; i++) 
	{
		saida.seekp((i) * sizeof(int));
		saida.write(reinterpret_cast<const char *>(&componentes[i]), sizeof(int)); // Escreve no arquivo binário
	}

	saida.close();
	arquivo.close();
}


void alterarComponente (Eletronicos componentes[], int tam, const string &arquivoBinario) {
	cout << endl;
	cout << "----- Funcao Alterar Componente -----" << endl << endl;
	cout << "Qual o Id do componente desejado para alterar?" << endl<<endl;

	// Realiza a busca do id
	int id, opcao=0;
	cout<<"Id do Componente: ";
	cin>>id;
	int posicaoComponente=0;
	bool identificado = false;

	ifstream entrada(arquivoBinario, ios::binary);

	if (!entrada.is_open()) 
	{
		cout << "Erro ao abrir os arquivos." << endl;
	}

	else
	{
		for (int i = 0; i < tam; i++) 
		{
			entrada.seekg((i) * sizeof(int));
			entrada.read(reinterpret_cast<char *>(&componentes[i]),sizeof(int)); // Lê o binário

			if (componentes[i].id == id) 
			{
				identificado = true;
				posicaoComponente=i;
				i=tam;
			}
		}

		if (identificado == false) 
		{
			cout <<endl<< "Componente com esse id nao foi encontrado!" << endl;
		}
		else 
		{
			cout <<endl<<"Componente encontrado!"<<endl;
			imprimirDados(componentes, posicaoComponente);
		}
		while((opcao < 1) or (opcao > 6)) 
		{
			cout<<"Qual propriedade gostaria de alterar deste componente?"<<endl
			<< "1. Fabricante "<<endl
			<< "2. Tipo "<<endl
			<< "3. Corrente Maxima "<<endl
			<< "4. Tensao Maxima "<<endl
			<< "5. Quantidade de Pinos "<<endl
			<< "6. Todas as propriedades acima"<<endl
			<<"Opcao: ";
			cin>>opcao;

			if ((opcao < 1) or (opcao > 6)) 
			{
				cout << "Opcao invalida!" << endl;
			}
		}
		if(opcao == 1)
		{
			cout << "Insira o Fabricante: ";
			cin.ignore();
			getline(cin, componentes[posicaoComponente].fabricante);
		}
		if(opcao == 2)
		{
			cout << "Insira o tipo: ";
			cin.ignore();
			getline(cin, componentes[posicaoComponente].tipo);
		}
		if(opcao == 3)
		{
			cout << "Insira a Corrente Maxima: ";
			cin >> componentes[posicaoComponente].corrente;
		}
		if(opcao == 4)
		{
			cout << "Insira a Tensao Maxima: ";
			cin >> componentes[posicaoComponente].tensao;
		}
		if(opcao == 5)
		{
			cout << "Insira a quantidade de Pinos: ";
			cin >> componentes[posicaoComponente].pinos;
		}
		if(opcao == 6)
		{
			cout << "Insira o Fabricante: ";
			cin.ignore();
			getline(cin, componentes[posicaoComponente].fabricante);
			cout << "Insira o tipo: ";
			getline(cin, componentes[posicaoComponente].tipo);
			cout << "Insira a Corrente Maxima: ";
			cin >> componentes[posicaoComponente].corrente;
			cout << "Insira a Tensao Maxima: ";
			cin >> componentes[posicaoComponente].tensao;
			cout << "Insira a quantidade de Pinos: ";
			cin >> componentes[posicaoComponente].pinos;
		}
		entrada.close();
	}
}
int main() {
	// Configurações iniciais e menu de opções
	Eletronicos componentes[tam];
	marca_idNegativo(componentes, tam);
	converterCSVparaBinario(componentes, tam, "Componentes.csv", "Componentes.bin");

	int opcao = 0;
	while (opcao != 7) 
	{
		cout << endl;
		cout << ". . . . . . MENU . . . . . ." << endl << endl;
		cout << "1. Cadastrar componente" << endl;
		cout << "2. Remover componente" << endl;
		cout << "3. Buscar componente" << endl;
		cout << "4. Ordenar componentes" << endl;
		cout << "5. Alterar componente" << endl;
		cout << "6. Imprimir" << endl;
		cout << "7. Sair" << endl << endl;
		cout << ". . . . . . . . . . . . . ." << endl;

		cin >> opcao;

		// Realiza a operação de acordo com a opção escolhida pelo usuário

		if ((opcao < 1) or (opcao > 7)) 
		{
		cout << "Opcao invalida!" << endl;
		}

		if (opcao == 1) 
		{
			cadastrarComponente(componentes, &cadastros, tam);
			salvarAlteracao(componentes, tam, "Componentes.bin", "Componentes.csv");
			cout << "Possuem " << cadastros << " componentes cadastrados.";
		}

		if (opcao == 2) 
		{
			removerComponente(componentes, &cadastros);
			salvarAlteracao(componentes, tam, "Componentes.bin", "Componentes.csv");
			cout << "Possuem " << cadastros << " componentes cadastrados.";
		}

		if (opcao == 3) 
		{
			int op_busca;
			cout << endl;
			cout << "----- Opcoes para Busca -----" << endl << endl;
			cout << "1. Busca por Nome" << endl;
			cout << "2. Busca por Tensao" << endl << endl;
			cout << ". . . . . . . . . . . . . ." << endl;
			cin >> op_busca;

			if (op_busca == 1) 
			{
				buscaNome(componentes, tam, "Componentes.bin");
			}
			if (op_busca == 2) 
			{
				buscaTensao(componentes, tam, "Componentes.bin");
			}
		}

		if (opcao == 4) 
		{
			int opcao_ordenar;
			cout << "----- Opcoes para ordenacao -----" << endl;
			cout << "1. Ordenar por ID em ordem crescente" << endl;
			cout << "2. Ordenar por corrente(Amperes) em ordem crescente" << endl;
			cout << "3. Ordenar por tensão (Volts) em ordem crescente" << endl;
			cin >> opcao_ordenar;
			
			if (opcao_ordenar == 1) 
			{
				shellshort_ID(componentes, tam);
				salvarAlteracao(componentes, tam, "Componentes.bin", "Componentes.csv");
				cout << endl << "Ordenado com sucesso!" << endl;
			}
			if (opcao_ordenar == 2) 
			{
				shellshort_corrente(componentes, tam);
				salvarAlteracao(componentes, tam, "Componentes.bin", "Componentes.csv");
				cout << endl << "Ordenado com sucesso!" << endl;
			}
			if (opcao_ordenar == 3) 
			{
				shellshort_tensao(componentes, tam);
				salvarAlteracao(componentes, tam, "Componentes.bin", "Componentes.csv");
				cout << endl << "Ordenado com sucesso!" << endl;
			}
		}
		
		if (opcao == 5) 
		{
			alterarComponente(componentes, tam, "Componentes.bin");
			salvarAlteracao(componentes, tam, "Componentes.bin", "Componentes.csv");
			cout<<"Alteracoes realizadas e salvas com sucesso!"<<endl<<endl;
		}
		if (opcao == 6) 
		{
			int opcao_impressao;
			cout << "----- Opcoes para impressao -----" << endl;
			cout << "1. Imprimir todos os componentes" << endl;
			cout << "2. Imprimir intervalo" << endl;
			cin >> opcao_impressao;
			
			if (opcao_impressao == 1) 
			{
				imprimeComponentes(componentes, tam);
				cout << "Possuem " << cadastros << " componentes cadastrados.";
			}
			if (opcao_impressao == 2) 
			{
				cout << "----- Opcoes de intervalo -----" << endl;
				cout << "1. Intervalo de Corrente Maxima: " << endl;
				cout << "2. Intervalo de Tensao Maxima: " << endl;

				int op;
				cin >> op;

				if (op == 1) 
				{
					imprimeIntervalo_corrente(componentes, tam);
				}
				if (op == 2) 
				{
					imprimeIntervalo_tensao(componentes, tam);
				}
			}
		}
	}
}
