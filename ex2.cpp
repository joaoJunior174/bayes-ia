#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "readFile.cpp"
#include "matrixPrint.cpp"

using namespace std;

//classe que estrutura os dados reunidos
class GatheredClass
{
public:
    vector<vector<double>> data_class;
    string label;
    vector<double> mean;
    vector<double> dev;
};
//retorna o índice da palavra dentro do array das labels
int getIndex(string word, vector<string> labeledFields)
{
    for (int i = 0; i < labeledFields.size(); i++)
    {
        if (labeledFields[i] == word)
            return i;
    }
}
//junta os dados de acordo com as label
vector<GatheredClass> gatherDataByClass(vector<vector<double>> data, vector<string> data_label, vector<string> labeledFields)
{
    vector<GatheredClass> aux(labeledFields.size());
    for (int i = 0; i < data.size(); i++)
    {
        int index = getIndex(data_label[i], labeledFields);
        aux[index].data_class.push_back(data[i]);
        aux[index].label = data_label[i];
    }
    return aux;
}
//faz o calculo da meidia e desvio padrão
void calculateMean(vector<GatheredClass> &dt)
{

    for (int i = 0; i < dt.size(); i++)
    {
        
        vector<double> mean_aux(dt[i].data_class[0].size(), 0);
        vector<double> dev_aux(dt[i].data_class[0].size(), 0);

        dt[i].mean = mean_aux;
        dt[i].dev = dev_aux;
        
        //aqui eu somo os valores em cada dimensao para fazer as médias depois
        for (int j = 0; j < dt[i].data_class.size(); j++)
        {
            for (int k = 0; k < dt[i].data_class[j].size(); k++)
            {
                dt[i].mean[k] += dt[i].data_class[j][k];
            }
        }

        //calcula a media em cada dimensão da base de dados
        for (int c = 0; c < dt[i].data_class[0].size(); c++)
        {
            dt[i].mean[c] /= dt[i].data_class.size();
        }

        //aqui eu encontro o desvio padrão sem dividir por N
        for (int j = 0; j < dt[i].data_class.size(); j++)
        {
            for (int k = 0; k < dt[i].data_class[j].size(); k++)
            {
                dt[i].dev[k] += pow(dt[i].data_class[j][k] - dt[i].mean[k], 2);
            }
        }

        //calcula o desvio padrão em cada dimensão dividindo por N
        for (int c = 0; c < dt[i].data_class[0].size(); c++)
        {
            dt[i].dev[c] /= dt[i].data_class.size();
        }
    }
}
//encontra as probabilidades para a base de dados de teste
vector<double> findProbabilities(vector<GatheredClass> data, vector<double> test)
{
    vector<double> aux;

    for (int i = 0; i < data.size(); i++)
    {

        double p = 1;

        for (int j = 0; j < data[i].data_class[0].size(); j++)
        {
            p *= (1 / (sqrt(M_PI * 2 * data[i].dev[j]))) * (exp(-(pow(test[j] - data[i].mean[j], 2) / (2 * data[i].dev[j]))));
        }
        aux.push_back(p);
    }

    return aux;
}

int main()
{
    //esses são os únicos dados que você deve mexer professor
    //comente e descomente o que for necessário para o senhor ver o funcionamento
    // string file = "iris-data.txt";
    // string fileLabel = "iris-label.txt";
    // vector<string> labeledFields = {"Iris-setosa", "Iris-versicolor", "Iris-virginica"};

    //essa base teve 83% de acerto
    // string file_test = "test-iris-data.txt";
    // string fileLabel_test = "test-iris-data-label.txt";

    // string file = "cancer-data.txt";
    // string fileLabel = "cancer-label.txt";
    // vector<string> labeledFields = {"M", "B"};

    //essa teve 100% de acerto
    // string file_test = "cancer-data-test.txt";
    // string fileLabel_test = "cancer-data-test-label.txt";

    string file = "vinho-data.txt";
    string fileLabel = "vinho-data-label.txt";
    vector<string> labeledFields = {"1", "2","3"};

    //essa teve 100% de acerto
    string file_test = "vinho-data-test.txt";
    string fileLabel_test = "vinho-data-test-label.txt";

    //daqui para baixo deixa o programa se virar, não precisa mexer
    vector<vector<double>> data = readFile(file);
    vector<string> data_label = readFileLabel(fileLabel);
    //aqui são os dados para test
    vector<vector<double>> data_test = readFile(file_test);
    vector<string> data_test_label = readFileLabel(fileLabel_test);

    vector<GatheredClass> dataByClasses = gatherDataByClass(data, data_label, labeledFields);
    
    
    calculateMean(dataByClasses);
    double p_total=0;
    for (int f = 0; f < data_test.size(); f++)
    {

        vector<double> probabilities = findProbabilities(dataByClasses, data_test[f]);

        double maior=-10000;
        double index=-1;
        for (int i = 0; i < probabilities.size(); i++)
        {
            if(probabilities[i] > maior){
                maior=probabilities[i];
                index=i;
            }
        }
        if(data_test_label[f]==labeledFields[index]){
            p_total++;
        }
        cout<<"O dado "<<f<<" na base de dados de teste e da classe "<<labeledFields[index]<<endl;
    }

    cout<<"A porcentagem de acertos foi de "<<p_total*100/data_test.size()<<"%"<<endl;
    return 0;
}