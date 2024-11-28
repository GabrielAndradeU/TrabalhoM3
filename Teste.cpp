#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <chrono>

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
    int qtdDias;
    string situacaoLocacao; //  locado ou disponivel
    string dataDevolucao, cpf, codigoVeiculo;
} Locacao;

bool ehNumero(string);                                                                                          
bool ehLetra(string);                                                                                           
void maiusc(string &);                                                                                           
bool lerDadosVeiculo(Veiculo veipesquisa, string &codigo, string &marca_modelo, string &placa, char &categoria);
string escolherMarca();                                                                                         
bool pesquisarPlaca(Veiculo, string);
bool pesquisarDisponivel(Veiculo, string); // da pra ver se da para melhorar, para pesquisar aqueles que não estão disponiveis tbm, em vez de apenas disponivel
void exibirFrota(Veiculo);
void exibirFrotaDisponivel(Veiculo, char);
void atualizarSituacao(string, string);
double calcularValorLocacao(char, int);
void relatorioLocacoesAtivas(vector<Locacao> &locacao);
bool pesquisarCodigo(Veiculo, string);
string gerarCodigoAleatorio();
void atualizarQtd(string);
void devolucaoVeiculo(vector<Locacao> &locacoes, Veiculo &veiculo);
void salvarLocacoesEmArquivo(const vector<Locacao>& locacoes);
void carregarLocacoesDeArquivo(vector<Locacao>& locacoes);



int main()
{
    int opcao;
    string codigoMain, marca_modeloMain, placaMain, situacaoMain, codigoDevolucao;
    char categoriaMain;
    Veiculo veiculoDados;
    vector<Locacao> locacao;

    carregarLocacoesDeArquivo(locacao);

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
                    cout << "------Gravado com Sucesso------" << endl;
                }
            }
            break;

        case 2:
            if (pesquisarDisponivel(veiculoDados, "Disponivel")) // Verifica se tem veículos disponíveis
            {
                char categoriaBlocoIF;
                cout << "Tem carro disponível!" << endl;

                do
                {
                    cout << "Digite a categoria (B/I/S): ";
                    cin >> categoriaBlocoIF;
                    categoriaBlocoIF = toupper(categoriaBlocoIF);

                    if (categoriaBlocoIF != 'B' && categoriaBlocoIF != 'I' && categoriaBlocoIF != 'S')
                    {
                        cout << "Erro: a categoria precisa ser B, I ou S." << endl;
                    }
                } while (categoriaBlocoIF != 'B' && categoriaBlocoIF != 'I' && categoriaBlocoIF != 'S');


                exibirFrotaDisponivel(veiculoDados, categoriaBlocoIF);

                cin.ignore();
                
                string codigoMain;
                cout << "Digite o código do veículo: ";
                getline(cin, codigoMain); 

                if (pesquisarCodigo(veiculoDados, codigoMain)) 
                {
                    cout << "Veículo encontrado! Procedendo com a locação..." << endl;
                    string dataLocacao;
                    cout << "Digite a data da locação (DD/MM/AAAA): ";
                    getline(cin, dataLocacao);

                    string cpfLocatario;
                    cout << "Digite o CPF do locatário: ";
                    getline(cin, cpfLocatario);
                    int qtdDias;
                    cout << "Quantos dias será a locação? ";
                    cin >> qtdDias;
                    cin.ignore(); 

                    
                    string dataDevolucao = ""; 
                    Locacao novaLocacao;
                    novaLocacao.codigoVeiculo = codigoMain;
                    novaLocacao.situacaoLocacao = "Locado";
                    novaLocacao.dataDevolucao = dataDevolucao;
                    novaLocacao.cpf = cpfLocatario;
                    novaLocacao.qtdDias = qtdDias;

                    locacao.push_back(novaLocacao); 

                    
                    atualizarSituacao(codigoMain, "Locado"); 
                    atualizarQtd(codigoMain);

                    cout << "Locação realizada com sucesso!" << endl;
                }
                else
                {
                    cout << "Veículo não encontrado ou não disponível!" << endl;
                }
            }
            else
            {
                cout << "Não há veículos disponíveis no momento!" << endl;
            }
            break;

        case 3:
        
            devolucaoVeiculo(locacao, veiculoDados);

            break;
            
            
        case 4:
            if (locacao.empty())
            {
                cout << "Nenhuma locação encontrada." << endl;
            }
            else
            {
                relatorioLocacoesAtivas(locacao);
            }
            cout << endl;
            break;

        case 5: 
        {
            exibirFrota(veiculoDados);
        }
        break;
        case 6:

             salvarLocacoesEmArquivo(locacao);

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

    if (pesquisarPlaca(veiPesquisa, placateste))
    {
        cout << "Ja consta veiculo com essa placa." << endl;
        return false;
    }
    placa = placateste;

    marca_modelo = escolherMarca();
    cin.ignore();
    string nomeCarro;
    estaok = false;

    do
    {
        cout << "Digite agora o modelo do carro: ";
        getline(cin, nomeCarro);
        estaok = ehLetra(nomeCarro);
        if (not estaok)
            cout << "Nome invalido" << endl;
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

    do
    {
        estaok = false;
        codigo = gerarCodigoAleatorio();
        if (pesquisarCodigo(veiPesquisa, codigo))
        {
        }
        else
        {
            estaok = true; 
        }
    } while (!estaok);

    cout << "Codigo do veiuculo: " << codigo << endl;

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

        if (frotapesquisa)
        {

            if (string(pesquisa.placa) == placaProcurada)
            { // transforma o char em string
                frotapesquisa.close();
                return true;
            }
        }
        else
        {

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

        if (frotapesquisa)
        {

            if (string(pesquisa.situacao) == EncontrarDisponivel)
            { // transforma o char em string
                frotapesquisa.close();
                return true;
            }
        }
        else
        {

            cout << "Erro ao ler os dados do veiculo no registro " << i + 1 << endl;
            break;
        }
    }
    return false;
}

void exibirFrota(Veiculo veiculoDados)
{

    ifstream frota1("frota.bin", ios::binary);

    if (!frota1)
    {
        cout << "Erro ao abrir o arquivo FROTA.bin para leitura." << endl;
        return;
    }

    frota1.seekg(0, ios::end);
    double cont = frota1.tellg();
    frota1.seekg(0, ios::beg);

    if (cont == 0)
    {
        cout << "O arquivo esta vazio!" << endl;
        frota1.close();
        return;
    }

    cout << "Lista de Veículos cadastrados:\n";
    cout << "Codigo\tMarca-Modelo\tPlaca\tCategoria\tQtd Locacoes\tSituazao\n";
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
                 << veiculoDados.qtdLocacoes << "\t"
                 << veiculoDados.situacao << endl;
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

void atualizarSituacao(string codigo, string novaSituacao)
{

    vector<Veiculo> frota;
    Veiculo aux;
    ifstream frotaArq("frota.bin", ios::binary);
    if (!frotaArq)
    {
        cout << "Erro ao abrir o arquivo frota.bin para leitura" << endl;
        return;
    }

    frotaArq.seekg(0, ios::end);
    double tamanhoArq = frotaArq.tellg();
    frotaArq.seekg(0, ios::beg);
    for (double i = 0; i < tamanhoArq / sizeof(Veiculo); i++)
    {
        frotaArq.read((char *)&aux, sizeof(Veiculo));
        if (frotaArq)
        {
            frota.push_back(aux);
        }
    }

    frotaArq.close();

    bool encontrado = false;
    for (size_t i = 0; i < frota.size(); i++)
    {
        if (string(frota[i].codigo) == codigo)
        {                                                    
            strcpy(frota[i].situacao, novaSituacao.c_str()); 
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
    {
        cout << "Veículo com o código " << codigo << " não encontrado." << endl;
        return;
    }

    ofstream frotaArqOut("frota.bin", ios::binary | ios::trunc);
    if (!frotaArqOut)
    {
        cout << "Erro ao abrir o arquivo frota.bin para escrita!" << endl;
        return;
    }

    for (double i = 0; i < frota.size(); i++)
    {
        frotaArqOut.write((const char *)&frota[i], sizeof(Veiculo));
    }

    frotaArqOut.close();
    cout << "Atualização feita com sucesso!" << endl;
}

double calcularValorLocacao(char categoria, int qtdDias)
{
    double diaria;
    switch (categoria)
    {
    case 'B':
        diaria = diaria_Basico;
        break;
    case 'I':
        diaria = diaria_Intermediario;
        break;
    case 'S':
        diaria = diaria_Super;
        break;
    }
    return diaria * qtdDias;
}
void relatorioLocacoesAtivas(vector<Locacao> &locacao)
{
    for (int i = 0; i < locacao.size(); i++)
    {
        if (locacao[i].situacaoLocacao == "Locado")
        {
            cout << "CPF: " << locacao[i].cpf << " | "
                 << "Código Veículo: " << locacao[i].codigoVeiculo << " | "
                 << "Dias de Locação: " << locacao[i].qtdDias << " | "
                 << "Data de Devolução: " << locacao[i].dataDevolucao << endl;

            cout << "-----------------------------------" << endl;
            cout << endl;
        }
    }
}

bool pesquisarCodigo(Veiculo pesquisa, string codigoProcurado)
{
    ifstream frotapesquisa("frota.bin", ios::binary);
    frotapesquisa.seekg(0, ios::end);
    double cont = frotapesquisa.tellg();
    frotapesquisa.seekg(0, ios::beg);

    for (double i = 0; i < cont / sizeof(Veiculo); i++)
    {
        frotapesquisa.read((char *)&pesquisa, sizeof(Veiculo));

        if (frotapesquisa)
        {
            if (string(pesquisa.codigo) == codigoProcurado)
            { // Comparando pelo código
                frotapesquisa.close();
                return true;
            }
        }
        else
        {
            cout << "Erro ao ler os dados do veiculo no registro " << i + 1 << endl;
            break;
        }
    }
    return false;
}

string gerarCodigoAleatorio()
{
    int codigo = rand() % 9000 + 1000; 
    return to_string(codigo);
}

void atualizarQtd(string codigo)
{

    vector<Veiculo> frota;
    Veiculo aux;

    ifstream frotaArq("frota.bin", ios::binary);
    if (!frotaArq)
    {
        cout << "Erro ao abrir o arquivo frota.bin para leitura" << endl;
        return;
    }

    frotaArq.seekg(0, ios::end);
    double tamanhoArq = frotaArq.tellg();
    frotaArq.seekg(0, ios::beg);
    for (double i = 0; i < tamanhoArq / sizeof(Veiculo); i++)
    {
        frotaArq.read((char *)&aux, sizeof(Veiculo));
        if (frotaArq)
        {
            frota.push_back(aux);
        }
    }

    frotaArq.close();

    bool encontrado = false;
    for (size_t i = 0; i < frota.size(); i++)
    {
        if (string(frota[i].codigo) == codigo)
        {                              
            frota[i].qtdLocacoes +=1; 
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
    {
        cout << "Veiculo com o codigo " << codigo << " nao encontrado." << endl;
        return;
    }
    ofstream frotaArqOut("frota.bin", ios::binary | ios::trunc);
    if (!frotaArqOut)
    {
        cout << "Erro ao abrir o arquivo frota.bin para escrita!" << endl;
        return;
    }

    for (double i = 0; i < frota.size(); i++)
    {
        frotaArqOut.write((const char *)&frota[i], sizeof(Veiculo));
    }

    frotaArqOut.close();
    cout << "Atualizacao feita com sucesso." << endl;
}


void devolucaoVeiculo(vector<Locacao> &locacoes, Veiculo &veiculo)
{
    string codigoDevolucao;
    cout << "Digite o codigo do veiculo a ser devolvido: ";
    cin >> codigoDevolucao;

    bool locacaoEncontrada = false;
    for (auto &locacao : locacoes)
    {
        if (locacao.codigoVeiculo == codigoDevolucao && locacao.situacaoLocacao == "Locado")
        {
            locacaoEncontrada = true;
            string dataDevolucao;
            cout << "Digite a data de devolução: ";
            cin >> dataDevolucao; 
            locacao.situacaoLocacao = "Devolvido";
            locacao.dataDevolucao = dataDevolucao;
            atualizarSituacao(codigoDevolucao, "Disponivel");

            double valorTotal = calcularValorLocacao(veiculo.categoria, locacao.qtdDias);
            cout << "Valor total da locacao: R$ " << fixed << setprecision(2) << valorTotal << endl;
            cout << "Devolucao registrada com sucesso!" << endl;
            break;
        }
    }

    if (!locacaoEncontrada)
    {
        cout << "Nao foi encontrada uma locação ativa com o codigo informado." << endl;
    }
}


void salvarLocacoesEmArquivo(const vector<Locacao>& locacoes)
{
    ofstream arquivo("locacoes.txt");

    if (!arquivo)
    {
        cout << "Erro ao abrir o arquivo para salvar os dados" << endl;
        return;
    }

    for (const auto& locacao : locacoes)
    {
        arquivo << locacao.codigoVeiculo << ","
                << locacao.situacaoLocacao << ","
                << locacao.dataDevolucao << ","
                << locacao.cpf << ","
                << locacao.qtdDias << endl;
    }

    arquivo.close();
    cout << "Dados de locações salvos com sucesso." << endl;
}


void carregarLocacoesDeArquivo(vector<Locacao>& locacoes)
{
    ifstream arquivo("locacoes.txt");

    if (not arquivo)
    {
        cout << "Erro ao abrir o arquivo locacoes.txt!" << endl;
        return;
    }

    string linha;

    while (getline(arquivo, linha))  
    {
        Locacao locacao;

        size_t pos = linha.find(",");
        if (pos == string::npos) break;  
        locacao.codigoVeiculo = linha.substr(0, pos);
        linha.erase(0, pos + 1); 
        pos = linha.find(",");
        if (pos == string::npos) break;
        locacao.situacaoLocacao = linha.substr(0, pos);
        linha.erase(0, pos + 1);
        pos = linha.find(",");
        if (pos == string::npos) break;
        locacao.dataDevolucao = linha.substr(0, pos);
        linha.erase(0, pos + 1);


        pos = linha.find(",");
        if (pos == string::npos) break;
        locacao.cpf = linha.substr(0, pos);
        linha.erase(0, pos + 1);

        locacao.qtdDias = 0; 
        while (not linha.empty() and linha[0] >= '0' and linha[0] <= '9') {
            locacao.qtdDias = locacao.qtdDias * 10 + (linha[0] - '0'); 
            linha.erase(0, 1); 
        }

    
        locacoes.push_back(locacao);
    }

    arquivo.close();
    cout << "Dados carregados com sucesso." << endl;
}