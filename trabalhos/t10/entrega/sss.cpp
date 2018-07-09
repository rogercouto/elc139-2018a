#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp> 
#include <sys/time.h>
#include <omp.h>

using namespace std;
using namespace boost;

/**
 * Lê um arquivo de texto e divide em um vetor de strings
 * */
vector<string> readFile(string file){
    vector<string> in;
    ifstream is;
    is.open (file, ios::in);
    if (is.is_open()){
        while(!is.eof()){
            string line;
            getline(is, line);
            vector<string> ss;
            split(ss,line,is_any_of(" "));
            for (int i = 0; i < ss.size(); i++)
                in.push_back(ss[i]);
        }
    }
    return in;
}

/*
* Imprime um vetor
*/
void print(vector<string> v){
    cout << "{";
    for (int i  = 0; i < v.size()-1; i++){
        if (i > 0)
            cout << ", ";
        cout << v[i];
    }
    cout << "}\n";
}

/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

/*
* Conta o máximo de caracteres em comum entre o início e o fim de uma string
*/
int countMaxCommon(string str1, string str2){
    string s1;//menor string
    string s2;//maior string
    if (str1.size() <= str2.size()){
        s1 = str1;
        s2 = str2;
    }else{
        s1 = str2;
        s2 = str1;
    }
    int desl = 0;
    int maxSize = -1;
    while (maxSize < 0){
        //procurar se a parte inicial da menor está no final da string maior
        int tam = s1.size()-desl; //tamanho da parte a ser verificada (vai diminuindo)
        int dif = s2.size()-s1.size()+desl; //inicio da string maior
        if (s1.substr(0,tam) == s2.substr(dif, tam)){
            maxSize = tam;
        }else if (s1.substr(desl, tam) == s2.substr(0, tam)){
            maxSize = tam;
        }
        desl++;
    }
    return maxSize;
}

/*
* Junta 2 strings conforme o número de caracteres em comum
*/
string mix(string str1, string str2, int maxCommon){
    if (maxCommon == 0)
        return str1+str2;
    string s1;//menor string
    string s2;//maior string
    if (str1.size() <= str2.size()){
        s1 = str1;
        s2 = str2;
    }else{
        s1 = str2;
        s2 = str1;
    }
    string s1b = s1.substr(0,maxCommon);
    string s2e = s2.substr(s2.size()-maxCommon, maxCommon);
    if (s1b==s2e){
        return s2+s1.substr(maxCommon, s1.size()-maxCommon);
    }
    string s1e = s1.substr(s1.size()-maxCommon ,maxCommon);
    string s2b = s2.substr(0,maxCommon);
    if (s1e==s2b){
        return s1.substr(0, s1.size()-maxCommon)+s2;
    }
    return "{ERRO}";
}

/*
* Transforma o vetor de strigns na menor superstring
*/
string getShortestSuperstring(vector<string> t){
    while (t.size() > 1){
        int i, j, max, maxI, maxJ;
        max = -1;
        #pragma omp parallel for schedule(dynamic, 1) shared(max, maxI, maxJ) private(i, j)
        for (i  = 0; i < t.size()-1; i++){
            for (j = i+1; j < t.size(); j++){
                int tam, pos;
                int tst = countMaxCommon(t[i], t[j]);
                #pragma omp critical
                {
                    if (tst > max){
                        max = tst;
                        maxI = i;
                        maxJ = j;
                    }
                }
            }
        }
        string mixed = mix(t[maxI], t[maxJ], max);
        t[maxI] = mixed;
        t.erase (t.begin()+maxJ);
    }
    return t[0];
}

int main(int argc, char** argv){
    string file;
    vector<string> s;
    string sss;
    long start_time, end_time;
    if (argc < 3){
        cout << "usage ./sss <threads> <filename>\n";
        return -1;
    }
    omp_set_num_threads(atoi(argv[1]));
    file = argv[2];
    s = readFile(file);
    start_time = wtime();
    sss = getShortestSuperstring(s);
    end_time = wtime();
    cout << "Shortest superstring: " << sss << '\n';
    cout << "superstring size: " << sss.size() << '\n';
    float tc = (float)(end_time - start_time)/1000000.0;
	cout << "Tempo de calculo = " << tc << " sec\n";
    return 0;
}