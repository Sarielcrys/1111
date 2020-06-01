/*

nom: CHEN
prénom: Nuo

date: 12/12/2019

Dans ce tp noté, on a utilisé presque toutes les choses que l'on avait apprises dans les cours.
Pour décompresser le code, on a besoin le tableau qui contient chaque mots dans le texte(un dictionnaire qui a déjà été réalisé par ce programme)
et on peut lire le fichier compressé, par comparer les codes et les mots, on peut donc décompresser le document.


Je vous prie de m'excuser, monsieur, mon expression de la langue francaise n'est pas fort.
Merci beaucoup.

*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include <cmath>


//-----------------------------------------------------------------------------------
std::vector<std::string> load()  //fonction load lit le texte, et elle stocke tous les mots dans un verteur de string 'result'
{
    std::vector<std::string> result;    //读文件的步骤 容器vector(string)储存字符串
    std::ifstream fin("1984.txt");      //ifstream输入流读取文件
    std::string word;                   
    while (std::getline(fin, word)) {   //循环 如果可以getline 从文件到word，那么pushpuck这个词到result中

        result.push_back(word);
    }
    fin.close();                           //关闭文件
    return result;
}

//-----------------------------------------------------------------------------------
typedef std::map<std::string, int> occs;    //自定义类型 occs为 map前者和后者的查询关系，string和int型对应

//-----------------------------------------------------------------------------------
occs count(std::vector<std::string> const &words)  //fonction count prend le vecteur qui est rempli par les mots
{
    occs result;                                   //on utilise 'result' à stoker un map

    for (const std::string &word: words) {
        if (result.find(word) != result.end()) {    //chaque fois, lire un mots dans words, s'il existe dans result, le nombre de ce mot plus 1
            result[word]++;
        }
        else {                                      //s'il existe pas, on insert ce mot dans result, et on initialise le nombre de ce mot
            result.insert(std::make_pair(word, 1)); //等于end也就是没有找到，新建一个词汇和它的数量加入到result中
        }
    }
    return result;                                  //map result表明的是word中每个词汇的出现次数。
}

//-----------------------------------------------------------------------------------
typedef std::map<std::string, std::string> codes;   //新定义map codes 开始编码

//-----------------------------------------------------------------------------------
std::pair<std::string, std::string>                 //
prefix(std::pair<std::string, std::string> const &root, std::string const &pref) // fonction prefix fusionne pair et string, obtenir un nouveau pair
{
    std::pair<std::string, std::string> result = std::make_pair(root.first, pref + root.second);
    return result;
}

//------------------------------------------------------------------------------------
codes merge(codes const &x, codes const &y)
{
    codes result;

    for(auto it=x.begin(); it!=x.end();it++) {  //dans la fonction merge, on utilise codes(2 string de map) à coder le mot
        result[it->first] = "0"+it->second;     //pour chaque mot dans x, on ajoute un 0 et pour ce qui est dans y un 1, et fusionne les 2 codes dans result
    }
    for(auto it=y.begin(); it!=y.end();it++) {
        result[it->first] = "1"+it->second;
    }
    return result;
}

//------------------------------------------------------------------------------------
typedef std::multimap<int, codes> partial_codes;     //nouveau type partial_codes de multimap composé par int et codes 

//------------------------------------------------------------------------------------
auto extract(partial_codes &pc) 
{
    int minimal_int = 999999;
    codes mincodes;
    auto miniter=pc.begin();

    for(auto it=pc.begin(); it!=pc.end();it++) {     // on cherche le plus petit élémemt par utiliser l'itérateur
        if(it->first < minimal_int){
            minimal_int = it->first;
            mincodes = it->second;
            miniter = it;
        }
    }

    pc.erase(miniter);                              // on supprime le plus petit ici et le renvoit
    return std::make_pair(minimal_int, mincodes);
}

//--------------------------------------------------------------------------------------
void reduce(partial_codes &res)                     // on supprime 2 mots qui sont plus petits via des appel à la donction extracte
{
    auto p1 = extract(res);
    auto p2 = extract(res);
    auto p = std::make_pair(p1.first+p2.first, merge(p1.second, p2.second)); // ajouter un nouveau élément selon la consigne
    res.insert(p);
}

//--------------------------------------------------------------------------------------
codes create(occs const &occ) 
{
    partial_codes pc;
    for(auto it=occ.begin();it!=occ.end();it++) { // boucle tansfert tous les élément dans table 'occ'
        int c = it->second;
        std::string word = it->first;
        codes code;
        code.insert(std::make_pair(word, ""));
        pc.insert(std::make_pair(c, code));
    }
    while (pc.size()>1){                             // on appel à la fonction reduce plusieurs fois jusqu'à ce qu'elle ne contient qu'un seul élémemt
        reduce(pc);
    }
    return pc.begin()->second;                      // renvoyer le membre de droite
}


//--------------------------------------------------------------------------------------
std::string compress(std::vector<std::string> const &words, codes &c) 
{
    std::string result;
    for(auto it = c.begin();it!=c.end();it++){
        for(const std::string word:words){          //fonction compress peut transfert les mots dans verteur words, par utiliser les code dans le map codes
            if(word.find(it->second)){              //chercher chaque mot par l'itérateur
                result += it->second;
                result += '\n';
            }
        break;
        }
    }

return result;
}

//--------------------------------------------------------------------------------------
codes shorten(codes const &c)
{
    codes result;
    for(auto it= c.begin();it!=c.end();it++) {                     //fonction shorten remplace des codes 1 avec autre chiffre 
        std::string code = it->second;
        int sum = 0;
        for (int i = 0; i < code.size(); ++i) {
            if(code[code.size()-1-i]=='1'){
                sum+=1*std::pow(2, i);
            }
        }
        result.insert(std::make_pair(it->first, std::to_string(sum)));
    }
    return result;
}

//======================================================================================
#include "check_1984.hpp"


int main() 
{

    run_tests();

    const std::vector<std::string> &words = load();
    occs occ = count(words);
    codes c = create(occ);
    codes shortc = shorten(c);
    const std::string &r = compress(words, c);
    
    std::ofstream fout_codes("codes.txt");                      //tester. 'codes.txt' est créé par la fonction shorten  
    std::ofstream fout_content("compressed.txt");               //        'compressed.txt' est créé par la fonction compress

    for(auto it=shortc.begin(); it!=shortc.end(); it++) {
        fout_codes<<it->second<<std::endl;
    }
    fout_content<<r;

    fout_content.close();
    fout_codes.close();

    getchar();
    getchar();
    
    return 0;
}