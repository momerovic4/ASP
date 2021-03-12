#include <iostream>
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
    return niz[brElemenata-1];
}

template<typename Tip>
int najveciAveciOdSume(Stek<Tip> &s, bool prviput=true){
    static int rez = 0;
    if(prviput)rez = s.vrh();
    static int suma = 0;
    if(s.brojElemenata()!=1){
        suma += s.skini();
        if(s.vrh() > suma) rez = s.vrh();
        najveciAveciOdSume(s, false);
    }
    return rez;
}

template<typename Tip>
void zadcetvrti(Stek<Tip> &a, Stek<Tip> &b){

}




int main() {
//    int brel;
//    Stek<int> s;
//    s.stavi(5);
//    s.stavi(2);
//    s.stavi(1);
//    s.stavi(1);
//    s.stavi(1);
//    s.stavi(3);
//    s.stavi(1);
//    std::cout<<najveciAveciOdSume(s);

    int n = 5;
    int k = 0;
    for (int i = 0; i < n; i++) {
        if(i%2 == 0 && k%2 == 0) {i--;k++;}
        for (int j = 0; j < n; j++) {
            int t= i*i*i;
            while (t >0 ) t--;
        }
        std::cout<<i<<" ";
    }
    return 0;
}
