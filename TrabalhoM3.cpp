#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>

using namespace std;

#define diaria_Basico 10
#define diaria_Intermediario 20
#define diaria_Super 30

typedef struct
{
    char codigo[4], marca_modelo[50], placa[7];
    char categoria;
    bool situacao; // false para locado e true para disponivel
    int qtdLocacoes = 0;
} Veiculo;

typedef struct
{
    char cpf, codigoVeiculo;
    int qtdDias;
    bool situacaoLocacao; // True para Ativo e False para Inativo
    string dataDevolucao;
} Locacao;

bool ehNumero(string); // Validação de número
bool ehLetra(string);  // Conferir se é letra
void maiusc(string &); // Tranformar em maiúsculo (Lembrar se vamos usar. Caso não usarmos. RETIRAR)
void lerDadosVeiculo(string &codigo, string &marca_modelo, string &placa, char &categoria);
string escolherMarca();

int main()
{
    int opcao;
    string codigoMain, marca_modeloMain, placaMain, situacaoMain;
    char categoriaMain;
    Veiculo veiculoDados;

    do
    {
        cout << "1. Inclusao de veiculo" << endl;
        cout << "2. Locacao de veiculo" << endl;
        cout << "3. Devolucao de veiculo" << endl;
        cout << "4. Relatorio de locacoes ativas" << endl;
        cout << "5. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore();
        switch (opcao)
        {
        case 1:
            // Inclusão de veículo
            lerDadosVeiculo(codigoMain, marca_modeloMain, placaMain, categoriaMain);
            {
                ofstream frota2("frota.bin", ios::binary | ios::app); // Usar ofstream para escrita
                if (!frota2)
                {
                    cout << "Erro ao abrir o arquivo frota.bin para escrita!" << endl;
                    break;
                }

                strcpy(veiculoDados.codigo, codigoMain.c_str());
                strcpy(veiculoDados.marca_modelo, marca_modeloMain.c_str());
                strcpy(veiculoDados.placa, placaMain.c_str());
                frota2.write((const char *)(&veiculoDados), sizeof(Veiculo));
                frota2.close();
            }
            break;

        case 2:
            {
                ifstream frota1("frota.bin", ios::binary); // Abrir o arquivo em modo binário para leitura
                if (!frota1)
                {
                    cout << "Erro ao abrir o arquivo FROTA.bin para leitura!" << endl;
                    break;
                }

                // Obtendo o número total de registros (pulos para o final e uso de tellg)
                frota1.seekg(0, ios::end);
                double cont = frota1.tellg(); // Corrigido o tipo da variável 'cont' para long long
                frota1.seekg(0, ios::beg); // Coloca o ponteiro de leitura no começo do arquivo

                if (cont == 0)
                {
                    cout << "O arquivo está vazio!" << endl;
                    frota1.close();
                    break;
                }

                cout << "Lista de Veículos cadastrados:\n";
                cout << "Codigo\tMarca-Modelo\tPlaca\tCategoria\tSituacao\tQtd Locacoes\n";
                cout << "---------------------------------------------------------------\n";

                for (double i = 0; i < cont / sizeof(Veiculo); i++)  // Ajustando para percorrer corretamente os registros
                {
                    frota1.read((char *)&veiculoDados, sizeof(Veiculo));
                    if (frota1)
                    {
                        // Mostrar os dados do veículo
                        cout << veiculoDados.codigo << "\t"
                             << veiculoDados.marca_modelo << "\t"
                             << veiculoDados.placa << "\t"
                             << veiculoDados.categoria << "\t"
                             << (veiculoDados.situacao ? "Disponivel" : "Locado") << "\t"
                             << veiculoDados.qtdLocacoes << endl;
                    }
                    else
                    {
                        cout << "Erro ao ler os dados do veículo no registro " << i + 1 << endl;
                        break;
                    }
                }

                frota1.close();
                cout << endl;
            }
            break;

        case 3:
            // Devolução de veículo (adicionar implementação aqui)
            cout << endl;
            break;

        case 4:
            // Relatório de locações ativas (adicionar implementação aqui)
            cout << endl;
            break;

        case 5:
            cout << endl;
            break;

        default:
            cout << endl;
            break;
        }

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

void lerDadosVeiculo(string &codigo, string &marca_modelo, string &placa, char &categoria)
{
    bool estaok = false;
    do
    {
        cout << "Digite o codigo do veiculo: ";
        getline(cin, codigo);
        if (codigo.size() != 4)
        {
            cout << "Erro: o Codigo precisa conter apenas numeros com ate 4 digitos" << endl;
        }
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
        if (categoria != 'B' and categoria != 'I' and categoria != 'S')
        {
            cout << "Erro: a categoria precisa ser B, I ou S." << endl;
        }
    } while (categoria != 'B' and categoria != 'I' and categoria != 'S');
}

string escolherMarca()
{
    int opcao;
    string marca;

    do
    {
        cout << "Escolha uma marca:" << endl;
        cout << "1 - Toyota" << endl;
        cout << "2 - Ford" << endl;
        cout << "3 - Honda" << endl;
        cout << "4 - Chevrolet" << endl;
        cout << "Digite o numero da marca escolhida: ";
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
