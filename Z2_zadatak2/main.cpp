#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ostream>
using namespace std;

template<typename Tip>
class Stek {
private:
    int kapacitet;
    Tip* niz;
    int brElemenata;
public:
    Stek();
    Stek(const Stek &s);
    Stek operator =(Stek &s);
    ~Stek();
    void brisi();
    void stavi(const Tip& el);
    Tip skini();
    Tip& vrh();
    int brojElemenata() {return brElemenata;};
};
template<typename Tip>
Stek<Tip> Stek<Tip>::operator =(Stek &s) {
    if(this == &s) return *this;
    else {
        if(s.brElemenata > this->kapacitet) {
            delete [] niz;
            niz = new Tip[s.kapacitet];
            for (int i = 0; i < s.brElemenata; i++)
                stavi(s.niz[i]);
        }else{
            for (int i = 0; i < s.brElemenata; i++) {
                stavi(s.niz[i]);
            }
        }

    }
    return *this;
}
template<typename Tip>
Stek<Tip>::Stek(const Stek &s) {
    brElemenata = s.brElemenata;
    kapacitet = s.kapacitet;
    niz = new Tip[kapacitet];
    for (int i = 0; i < brElemenata; i++) {
        niz[i] = s.niz[i];
    }
}
template<typename Tip>
Stek<Tip>::Stek():kapacitet(1) {
    brElemenata=0;
    niz= new Tip[kapacitet];
}
template<typename Tip>
Stek<Tip>::~Stek() {
    delete [] niz;
}
template<typename Tip>
void Stek<Tip>::brisi() {
    brElemenata=0;
}
template<typename Tip>
void Stek<Tip>::stavi(const Tip &el) {
    if(brElemenata == kapacitet) {
        Tip* pomocni = new Tip[kapacitet*2];
        kapacitet*=2;
        for (int i = 0; i < brElemenata; i++) {
            pomocni[i] = niz[i];
        }
        pomocni[brElemenata] = el;
        brElemenata++;
        delete [] niz;
        niz = pomocni;
        pomocni=nullptr;
    }else {
        niz[brElemenata] = el;
        brElemenata++;
    }
}
template<typename Tip>
Tip Stek<Tip>::skini() {
    if(brElemenata == 0) throw "Prazan Stek!";
    Tip povratni = niz[brElemenata-1];
    brElemenata--;
    return povratni;
}
template<typename Tip>
Tip& Stek<Tip>::vrh() {
    if(brElemenata <= 0) throw "Prazan Stek!";
    return niz[brElemenata-1];
}

void pretraga(Stek<vector<int> >& s, int trazeni, bool novi = false){
    bool pronadjen = false;
    bool pronadjenRaspon = false;
    int indexBroja(-1);
    static int indexVektora = s.brojElemenata();
    if(novi) {indexVektora=s.brojElemenata(); novi=!novi;} //parametar novi dodan samo zbog autotestova
    vector<int> pomocni;
    if(s.brojElemenata() != 0 && !pronadjen){
        pomocni = s.skini();
        indexVektora--;
        if(pomocni.size()!=0 && pomocni.at(0) <= trazeni && trazeni<=pomocni.at(pomocni.size()-1)){
            pronadjenRaspon = true;
            int l=0;
            int r=pomocni.size()-1;
            while (l <= r) {
                int m = l + (r - l) / 2;
                if (pomocni.at(m) == trazeni) {
                    pronadjen = true;
                    indexBroja = m;
                    break;
                } if (pomocni.at(m) < trazeni)
                    l = m + 1;
                else
                    r = m - 1;
            }

        }
        if(!pronadjen && !pronadjenRaspon) pretraga(s,trazeni);
        else if(pronadjen){
            std::cout<< indexBroja << " " ;
            if(indexVektora==-1) std::cout<< indexVektora+1;
            else std::cout<< indexVektora;
        }else if(!pronadjen && pronadjenRaspon) std::cout<< "Nema elementa";
    }
    if(!s.brojElemenata() && !pronadjenRaspon) std::cout<< "Nema elementa";
    s.stavi(pomocni);
}


//Testne Funckije

bool TestPretrage1(){
    Stek<vector<int> > s;
    vector<int> a;
    for (int i(10); i<1000000; i++)
        a.push_back(i);
    s.stavi(a);
    int trazeni = 999999;
    std::string poredbeni = "999989 0";

    std::stringstream buffer;
    std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

    pretraga(s,trazeni,true);

    std::string text = buffer.str();
    int result = text.compare(poredbeni);

    std::cout.rdbuf(prevcoutbuf);

    if(result == 0) return true;
    return false;
}

bool TestPretrage2(){
    Stek<vector<int> > s;
    int trazeni=546;
    std::string poredbeni = "Nema elementa";

    std::stringstream buffer;
    std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

    pretraga(s,trazeni,true);

    std::string text = buffer.str();
    int result = text.compare(poredbeni);

    std::cout.rdbuf(prevcoutbuf);
    std::cout.flush();

    if(result == 0) return true;
    return false;
}

bool TestPretrage3(){
    Stek<vector<int> > s;
    vector<int> a;
    for (int i(10); i<10000; i++)
        a.push_back(i);
    vector<int> b;
    b.push_back(10006);b.push_back(10008);
    vector<int> c;
    c.push_back(10009);c.push_back(10010);c.push_back(10130);
    vector<int> d;
    d.push_back(10157);d.push_back(10226);d.push_back(13424);
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);

    int trazeni = 254;
    std::string poredbeni = "244 0";

    std::stringstream buffer;
    std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

    pretraga(s,trazeni,true);

    std::string text = buffer.str();
    int result = text.compare(poredbeni);

    std::cout.rdbuf(prevcoutbuf);
    std::cout.flush();

    if(result == 0) return true;
    return false;
}


int main() {
    std::cout<<"Testovi za pretragu: \n\n";
    if(TestPretrage1()) std::cout<<"Test1 Dobar\n";
    else std::cout<<"Test1 nije Dobar\n";
    if(TestPretrage2()) std::cout<<"Test2 Dobar\n";
    else std::cout<<"Test2 nije Dobar\n";
    if(TestPretrage3()) std::cout<<"Test3 Dobar\n";
    else std::cout<<"Test3 nije Dobar\n";
    return 0;
}
