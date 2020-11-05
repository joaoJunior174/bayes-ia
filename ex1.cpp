#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <map>
#include "readFile.cpp"
#include "matrixPrint.cpp"

using namespace std;

//classe para guardar as tabelas das probabilidades de cada classe
class Table
{
public:
    map<string, pair<double, double>> tab;
    string _class;
};

//função resposável por separar os dados e calcular as probabilidades
//das classes
Table populateTable(vector<vector<string>> data, string c)
{

    Table aux = Table();

    aux._class = c;

    for (int i = 0; i < data.size(); i++)
    {
        aux.tab[data[i][0]] = make_pair(0, 0);
    }

    for (int i = 0; i < data.size(); i++)
    {
        if (data[i][1] == "no")
        {
            aux.tab[data[i][0]] = make_pair(aux.tab[data[i][0]].first + 1, aux.tab[data[i][0]].second);
        }
        else if (data[i][1] == "yes")
        {
            aux.tab[data[i][0]] = make_pair(aux.tab[data[i][0]].first, aux.tab[data[i][0]].second + 1);
        }
    }

    for (int i = 0; i < data.size(); i++)
    {
        if (aux.tab[data[i][0]].first == 0)
        {
            aux.tab[data[i][0]] = make_pair(0.000001, aux.tab[data[i][0]].second);
        }
        else if (aux.tab[data[i][0]].second == 0)
        {
            aux.tab[data[i][0]] = make_pair(aux.tab[data[i][0]].first, 0.000001);
        }
    }

    map<string, pair<double, double>>::iterator it;

    for (it = aux.tab.begin(); it != aux.tab.end(); it++)
    {
        aux.tab[it->first] = make_pair(aux.tab[it->first].first / (aux.tab[it->first].first + aux.tab[it->first].second),
                                       aux.tab[it->first].second / (aux.tab[it->first].first + aux.tab[it->first].second));
    }

    return aux;
}
//função que printa as probabilidades e tabelas
void printTables(vector<Table> tb)
{
    for (int i = 0; i < tb.size(); i++)
    {
        map<string, pair<double, double>>::iterator it;
        cout << "Tabela da classe: " << tb[i]._class << endl;
        cout << "Class - (No) - (Yes)" << endl;
        for (it = tb[i].tab.begin(); it != tb[i].tab.end(); it++)
        {
            cout << it->first << " - " << it->second.first << " - " << it->second.second << endl;
        }
        cout << "----------------------------------------------" << endl;
    }
}

//retorna uma resposta de no ou yes para uma determinada pergunta
//que o usuario faz
string findProbabilities(vector<Table> tb, vector<string> q)
{
    double py = 1;
    double pn = 1;

    for (int i = 0; i < q.size(); i = i + 2)
    {
        for (int j = 0; j < tb.size(); j++)
        {
            if (tb[j]._class == q[i])
            {
                //aqui é calculado a probabilidade (tanto para o no e yes)
                pn *= tb[j].tab[q[i + 1]].first;
                py *= tb[j].tab[q[i + 1]].second;
            }
        }
    }

    return pn > py ? "No" : "Yes";
}

int main()
{

    //variáveis para guardar o nome dos arquivos de texto
    string file_age = "age-table-ex1.txt";
    string file_income = "income-table-ex1.txt";
    string file_student = "student-table-ex1.txt";
    string file_credit = "credit-table-ex1.txt";

    //lê os dados dos arquivos txt
    vector<vector<string>> data_age = readFileEx1(file_age);
    vector<vector<string>> data_income = readFileEx1(file_income);
    vector<vector<string>> data_student = readFileEx1(file_student);
    vector<vector<string>> data_credit = readFileEx1(file_credit);

    vector<Table> tb;

    tb.push_back(populateTable(data_age, "age"));
    tb.push_back(populateTable(data_income, "income"));
    tb.push_back(populateTable(data_student, "student"));
    tb.push_back(populateTable(data_credit, "credit"));

    printTables(tb);

    //se o senhor quiser testar outras possibilidades é só fazer uma pergunta
    //no array. Primeiro você coloca o nome da coluna seguido da classe
    //faça quantas perguntas quiser com quantas varáveis quiser
    //ex: Se a pessoa tem uma idade de 35 anos
    //então a classe que vai ser usada é 31_40 (entre 31 e 40 anos)
    //se a idade for menor que trinta, então a classe é 30 (simboliza menor que 30)
    vector<string> question1 = {"age", "31_40", "income", "medium"};

    string answer = findProbabilities(tb, question1);

    cout << "A resposta da pergunta 1 eh " << answer << endl;

    vector<string> question2 = {"student", "yes", "credit", "fair"};

    string answer2 = findProbabilities(tb, question2);

    cout << "A resposta da pergunta 2 eh " << answer2 << endl;

    return 0;
}