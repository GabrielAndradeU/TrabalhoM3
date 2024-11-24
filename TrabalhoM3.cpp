#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <iomanip>

using namespace std;

#define diaria_Basico 10
#define diaria_Intermediario 20
#define diaria_Super 30

typedef struct
{
    char codigo[5], marca_modelo[30], placa[8];
    char categoria;
    char situacao[11]; //  locado ou disponivel
    int qtdLocacoes = 0;
} Veiculo;

typedef struct
{
    char cpf, codigoVeiculo;
    int qtdDias;
    char situacaoLocacao; //  locado ou disponivel
    string dataDevolucao;
} Locacao;

bool ehNumero(string); // Validação de número
bool ehLetra(string);  // Conferir se é letra
void maiusc(string &); // Tranformar em maiúsculo (Lembrar se vamos usar. Caso não usarmos. RETIRAR)
bool lerDadosVeiculo(Veiculo veipesquisa, string &codigo, string &marca_modelo, string &placa, char &categoria); // é bool pois só vai gravar se a placa não estiver cadastrada.
string escolherMarca(); // limitei as marcas
bool pesquisarPlaca(Veiculo, string);
bool pesquisarDisponivel(Veiculo, string); // da para melhorar depois, para pesquisar aqueles que não estão disponiveis tbm, em vez de apenas disponivel
void exibirFrota(Veiculo);
void exibirFrotaDisponivel(Veiculo, char);
void atualizarSituacao (string,string);

int main()
{
    int opcao;
    string codigoMain, marca_modeloMain, placaMain, situacaoMain;
    char categoriaMain;
    Veiculo veiculoDados;
    vector<Locacao>locacao;

    do
    {
        cout << "1. Inclusao de veiculo" << endl;
        cout << "2. Locacao de veiculo" << endl; 
        cout << "3. Devolucao de veiculo" << endl;
        cout << "4. Relatorio de locacoes ativas" << endl;
        cout << "5. Relatorio de veiculos" << endl; // Se der tempo, fazer uma Sub rotina para ficar organizado.
        cout << "6. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore();
        switch (opcao)
        {
        case 1:
            // Inclusão de veículo
            if (lerDadosVeiculo(veiculoDados, codigoMain, marca_modeloMain, placaMain, categoriaMain)) // se der true, vai continua com o codigo e gravar. Se der false, é porque já tem a placa cadastrada.
            {
                {
                    ofstream frota2("frota.bin", ios::binary | ios::app); 
                    if (!frota2)
                    {
                        cout << "Erro ao abrir o arquivo frota.bin para escrita!" << endl;
                        break;
                    }
                    strcpy(veiculoDados.codigo, codigoMain.c_str());
                    strcpy(veiculoDados.marca_modelo, marca_modeloMain.c_str());
                    strcpy(veiculoDados.placa, placaMain.c_str());
                    veiculoDados.categoria = categoriaMain; 
                    situacaoMain = "Disponivel";
                    strcpy(veiculoDados.situacao, situacaoMain.c_str());

                    frota2.write((const char *)(&veiculoDados), sizeof(Veiculo));
                    frota2.close();
                    cout<<"------Gravado com Sucesso------"<<endl;
                }
            }
            break;

        case 2:
            if (pesquisarDisponivel(veiculoDados, "Disponivel"))
            {

                char categoriaBlocoIF;
                cout << "Tem carro disponivel" << endl;
                do
                {
                    cout << "Digite a categoria (B/I/S): ";
                    categoriaBlocoIF = toupper(cin.get());
                    if (categoriaBlocoIF != 'B' and categoriaBlocoIF != 'I' and categoriaBlocoIF != 'S')
                        {
                            cout << "Erro: a categoria precisa ser B, I ou S." << endl;
                        }
                } while (categoriaBlocoIF != 'B' and categoriaBlocoIF != 'I' and categoriaBlocoIF != 'S'); // corrigir depois para voltar ao menu, para não pedir novamente
                exibirFrotaDisponivel(veiculoDados, categoriaBlocoIF);
                cout<<"Digite a placa do veiculo"; // validar a placa e procurar


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

        case 5: // Se der tempo, fazer uma sub rotina. Melhorar visualização
                {
                exibirFrota(veiculoDados);    
                /*ifstream frota1("frota.bin", ios::binary); 
                if (!frota1)
                {
                    cout << "Erro ao abrir o arquivo FROTA.bin para leitura." << endl;
                    break;
                }

              
                frota1.seekg(0, ios::end);
                double cont = frota1.tellg();
                frota1.seekg(0, ios::beg); 

                if (cont == 0)
                {
                    cout << "O arquivo esta vazio!" << endl;
                    frota1.close();
                    break;
                }

                cout << "Lista de Veículos cadastrados:\n";
                cout << "Codigo\tMarca-Modelo\tPlaca\tCategoria\tQtd Locacoes\n";
                cout << "---------------------------------------------------------------\n";

                for (double i = 0; i < cont / sizeof(Veiculo); i++)  
                {
                    frota1.read((char *)&veiculoDados, sizeof(Veiculo));
                    if (frota1)
                    {
                      
                        cout << veiculoDados.codigo << "\t"
                             << veiculoDados.marca_modelo << "\t"
                             << veiculoDados.placa << "\t"
                             << veiculoDados.categoria << "\t"
                             << veiculoDados.qtdLocacoes << "\t";
                             cout<<endl;
                    }
                    else
                    {
                        cout << "Erro ao ler os dados do veículo no registro " << i + 1 << endl;
                        break;
                    }
                }

                frota1.close();
                cout << endl;
                */
            }
            break;
        case 6:
            break;            

        default:
            cout << endl;
            break;
        }

    } while (opcao != 6);

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

bool lerDadosVeiculo(Veiculo veiPesquisa, string &codigo, string &marca_modelo, string &placa, char &categoria)
{
    bool estaok = false;
    string placateste;
    do
    {
        cout << "Digite a placa do veiculo: ";
        getline(cin, placateste);

        if (placateste.size() != 7)
        {
            cout << "Placa invalida. O formato deve ser 3 letras seguidas de 4 numeros." << endl;
        }
        else
        {
            bool valida = true;

            for (int i = 0; i < 3; i++)
            {   
                maiusc(placateste);
                if (!isalpha(placateste[i]))
                {
                    valida = false;
                    break;
                }
            }
            for (int i = 3; i < 7; i++)
            {
                if (!isdigit(placateste[i]))
                {
                    valida = false;
                    break;
                }
            }

            if (valida)
            {
                break;
            }
            else
            {
                cout << "Placa invalida. O formato deve ser 3 letras seguidas de 4 numeros." << endl;
            }
        }
    } while (true);

    if (pesquisarPlaca(veiPesquisa, placateste)){
        cout<<"Ja consta veicula com essa placa."<<endl;
        return false;
    }
    placa = placateste;
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
    cin.ignore();
    string nomeCarro;
    estaok = false;

    do{
        cout<<"Digite agora o modelo do carro: ";
        getline (cin,nomeCarro);
        estaok = ehLetra(nomeCarro);
        if (not estaok)
            cout<<"Nome invalido"<<endl;
    } while (not estaok);
    maiusc(nomeCarro);
    marca_modelo = marca_modelo + " - " + nomeCarro;
    do
    {
        cout << "Digite a categoria (B/I/S): ";
        categoria = toupper(cin.get());
        if (categoria != 'B' and categoria != 'I' and categoria != 'S')
        {
            cout << "Erro: a categoria precisa ser B, I ou S." << endl;
        }
    } while (categoria != 'B' and categoria != 'I' and categoria != 'S');

    return true;
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

bool pesquisarPlaca(Veiculo pesquisa, string placaProcurada)
{
    ifstream frotapesquisa("frota.bin", ios::binary);
    frotapesquisa.seekg(0, ios::end);
    double cont = frotapesquisa.tellg();  
    frotapesquisa.seekg(0, ios::beg);
    for (double i = 0; i < cont / sizeof(Veiculo); i++)
    {
        frotapesquisa.read((char *)&pesquisa, sizeof(Veiculo));  
        
        if (frotapesquisa) {
          
            if (string(pesquisa.placa) == placaProcurada) { //transforma o char em string
                frotapesquisa.close();
                return true;  
            }
        } else {
            
            cout << "Erro ao ler os dados do veiculo no registro " << i + 1 << endl;
            break;  
        }
    }
    return false;  
}

bool pesquisarDisponivel(Veiculo pesquisa, string EncontrarDisponivel)
{
    ifstream frotapesquisa("frota.bin", ios::binary);
    frotapesquisa.seekg(0, ios::end);
    double cont = frotapesquisa.tellg();  
    frotapesquisa.seekg(0, ios::beg);   

    for (double i = 0; i < cont / sizeof(Veiculo); i++)
    {
        frotapesquisa.read((char *)&pesquisa, sizeof(Veiculo));  
        
        if (frotapesquisa) {
          
            if (string(pesquisa.situacao) == EncontrarDisponivel) { //transforma o char em string
                frotapesquisa.close();
                return true;  
            }
        } else {
            
            cout << "Erro ao ler os dados do veiculo no registro " << i + 1 << endl;
            break;  
        }
    }
    return false;  
}

void exibirFrota(Veiculo veiculoDados) {

    ifstream frota1("frota.bin", ios::binary);  

    if (!frota1) {
        cout << "Erro ao abrir o arquivo FROTA.bin para leitura." << endl;
        return;  
    }

   
    frota1.seekg(0, ios::end);
    double cont = frota1.tellg();  
    frota1.seekg(0, ios::beg);  

    if (cont == 0) {
        cout << "O arquivo esta vazio!" << endl;
        frota1.close();
        return;  
    }

  
    cout << "Lista de Veículos cadastrados:\n";
    cout << "Codigo\tMarca-Modelo\tPlaca\tCategoria\tQtd Locacoes\tSituazao\n";
    cout << "---------------------------------------------------------------\n";

    
    for (double i = 0; i < cont / sizeof(Veiculo); i++) {
        frota1.read((char *)&veiculoDados, sizeof(Veiculo));  

        if (frota1) {
          
            cout << veiculoDados.codigo << "\t"
                 << veiculoDados.marca_modelo << "\t"
                 << veiculoDados.placa << "\t"
                 << veiculoDados.categoria << "\t"
                 << veiculoDados.qtdLocacoes << "\t"
                 <<veiculoDados.situacao<<endl;
        } else {
            cout << "Erro ao ler os dados do veículo no registro " << i + 1 << endl;
            break;
        }
    }

    frota1.close(); 
    cout << endl;
}

void exibirFrotaDisponivel(Veiculo veiculoDados, char categoria)
{
    ifstream frota1("frota.bin", ios::binary);
    frota1.seekg(0, ios::end);
    double cont = frota1.tellg();
    frota1.seekg(0, ios::beg);

    cout << "Lista de Veiculos Disponiveis:\n";
    cout << "Codigo\tMarca-Modelo\tPlaca\tCategoria\tQtd Locacoes\tSituazao\n";
    cout << "---------------------------------------------------------------\n";

    for (double i = 0; i < cont / sizeof(Veiculo); i++)
    {
        frota1.read((char *)&veiculoDados, sizeof(Veiculo));

        if (frota1)
        {
            // Verifique se a situação é exatamente "Disponivel"
            if (string(veiculoDados.situacao) == "Disponivel" and veiculoDados.categoria == categoria)
            {
                cout << veiculoDados.codigo << "\t"
                     << veiculoDados.marca_modelo << "\t"
                     << veiculoDados.placa << "\t"
                     << veiculoDados.categoria << "\t"
                     << veiculoDados.qtdLocacoes << "\t"
                     << veiculoDados.situacao << endl;
            }
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


void atualizarSituacao(string placa, string novaSituacao) {
    
    vector<Veiculo> frota; 
    Veiculo aux;  

    ifstream frotaArq("frota.bin", ios::binary);
    if (!frotaArq) {
        cout << "Erro ao abrir o arquivo frota.bin para leitura" << endl;
        return;
    }

   
    frotaArq.seekg(0, ios::end);  
    double tamanhoArq = frotaArq.tellg();  
    frotaArq.seekg(0, ios::beg);  

    for (double i = 0; i<tamanhoArq; i++) {
        frotaArq.read((char *)&aux, sizeof(Veiculo)); 
        if (frotaArq) {
            frota.push_back(aux);  
        }
    }

    frotaArq.close(); 

    bool encontrado = false;
    for (size_t i = 0; i < frota.size(); i++) {  
        if (string(frota[i].placa) == placa) {
         
            strcpy(frota[i].situacao, novaSituacao.c_str());
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Veiculo com a placa " << placa << " nao encontrado." << endl;
        return;
    }

    ofstream frotaArqOut("frota.bin", ios::binary | ios::trunc);
    if (!frotaArqOut) {
        cout << "Erro ao abrir o arquivo frota.bin para escrita!" << endl;
        return;
    }

    for (double i = 0; i < frota.size(); i++) {
        frotaArqOut.write((const char *)&frota[i], sizeof(Veiculo));
    }

    frotaArqOut.close();
    cout << "Atualizacao feita" << endl;
}