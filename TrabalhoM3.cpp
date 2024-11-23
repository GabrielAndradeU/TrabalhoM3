#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

#define diaria_Basico 10
#define diaria_Intermediario 20
#define diaria_Super 30

struct
{
    string codigo, marca_modelo, placa, nomeCarro;
    char categoria;
    bool situacao; // false para locado e true para disponivel
    int qtdLocacoes = 0;
} Veiculo;

struct
{
    string cpf, codigoVeiculo;
    int qtdDias;
    bool situacaoLocacao; // True para Ativo e False para Inativo
    string dataDevolucao;
} Locacao;

bool ehNumero(string); // Validação de número
bool ehLetra(string);  // Conferir se é letra
void maiusc(string &); // Tranformar em maiUsculo (Lembrar se vamos usar. Caso não usarmos. RETIRAR)
void lerDadosVeiculo(string, string, string, char);
string escolherMarca();

int main()
{
    int opcao;
    do
    {
        cout << "1. Inclusao de veiculo" << endl;
        lerDadosVeiculo(Veiculo.codigo, Veiculo.marca_modelo, Veiculo.placa, Veiculo.categoria);
        
        cout << "2. Locacao de veiculo" << endl;
        cout << "3. Devolucao de veiculo" << endl;
        cout << "4. Relatório de locacaes ativas" << endl;
        cout << "5. Sair" << endl;
        cout << "Escolha uma opção: ";
        cin >> opcao;

    } while (opcao != 5);

    return 0;
}

bool ehNumero(string numero)
{
    if (numero[0] == '-' or not isdigit(numero[0]))
        return false;
    for (int i = 1; i < numero.size(); i++)
        if (not isdigit(numero[i]))
            return false;

    return true;
}

bool ehLetra(string letra)
{
    if (letra[0] == ' ')
    {
        cout << "Entrada inválida" << endl;
        return false;
    }
    for (int i = 0; i < letra.size(); i++)
    {
        if (not(isalpha(letra[i])) and (letra[i] != ' ') or (i > 0 and isspace(letra[i]) and isspace(letra[i - 1])))
        {
            cout << "Entrada inválida" << endl;
            return false;
        }
    }
    return true;
}

void maiusc(string &frase)
{
    for (int i = 0; i < frase.size(); i++)
    {
        frase[i] = toupper(frase[i]);
    }
}

void lerDadosVeiculo(string codigo, string marca_modelo, string placa, char categoria)
{
    bool estaok = false; // Rever depois, para utilizar apenas uma variavel bool na sub rotina
    do
    {
        bool estaok = false;
        cout << "Digite o código do veículo: ";
        getline(cin, codigo);
        if (codigo.size() != 4)
            cout << "Erro: o Codigo precisa conter apenas numeros com ate 4 digitos" << endl;
        continue; // Aprendemos em python. Da para usar nesse caso
        estaok = ehNumero(codigo);
        if (not estaok)
            cout << "Erro: o Codigo precisa conter apenas numeros com ate 4 digitos" << endl;
    } while (not estaok);
    marca_modelo = escolherMarca();

    do
    {
        cout << "Digite a placa do veiculo: ";
        getline(cin, placa);

        if (placa.size() != 7)
        {
            cout << "Placa precisa conter 7 digitos" << endl;
        }
        else
        {
            bool valida = true;

            for (int i = 0; i < 3; i++)
            {
                if (!isalpha(placa[i]))
                {
                    valida = false;
                    break;
                }
            }
            for (int i = 3; i < 7; i++)
            {
                if (!isdigit(placa[i]))
                {
                    valida = false;
                    break;
                }
            }

            if (valida)
            {
                cout << "Placa valida: " << placa << endl;
                break;
            }
            else
            {
                cout << "Placa invalida. O formato deve ser 3 letras seguidas de 4 numeros." << endl;
            }
        }
    } while (true);

    do
    {
        cout << "Digite a categoria (B/I/S): ";
        categoria = toupper(cin.get());
        if (categoria != 'B' && categoria != 'I' && categoria != 'S')
        {
            cout << "Erro: categoria inválida! Digite apenas B, I ou S." << endl;
        }
    } while (categoria != 'B' && categoria != 'I' && categoria != 'S');
}

string escolherMarca()
{
    int opcao;
    string marca;
    do
    {
        cout << "Escolha a marca do carro:" << endl;
        cout << "1 - Toyota" << endl;
        cout << "2 - Ford" << endl;
        cout << "3 - Honda" << endl;
        cout << "4 - Chevrolet" << endl;
        cout << "Digite o número da marca escolhida: ";
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            marca = "Toyota";
            return marca;
        case 2:
            marca = "Ford";
            return marca;
        case 3:
            marca = "Honda";
            return marca;
        case 4:
            marca = "Chevrolet";
            return marca;
        default:
            cout << "Marca invalida. Escolha um numero entre 1 e 4." << endl;
        }
    } while (true);
}