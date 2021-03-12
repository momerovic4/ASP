#include <iostream>
using namespace std;

template<typename Tip>
class Red {
private:
    int pocetak;
    int kapacitet;
    Tip* niz;
    int brElemenata;
public:
    Red();
    Red(const Red &s);
    Red operator =(Red &s);
    ~Red();
    void brisi();
    void stavi(const Tip& el);
    Tip skini();
    Tip& celo();
    int brojElemenata() {return brElemenata;};
};

template<typename Tip>
Red<Tip> Red<Tip>::operator =(Red &s) {
    if(this == &s) return *this;
    else {
        if(s.brElemenata > this->kapacitet) {
            delete [] niz;
            niz = new Tip[s.kapacitet];
            for (int i = 0; i < s.brElemenata; i++)
                stavi(s.niz[s.pocetak + i]);
            pocetak = 0;
        }else{
            for (int i = 0; i < s.brElemenata; i++) {
                stavi(s.niz[s.pocetak + i]);
            }
            pocetak = 0;
        }
    }
    return *this;
}

template<typename Tip>
Red<Tip>::Red(const Red &s) {
    brElemenata = s.brElemenata;
    kapacitet = s.kapacitet;
    pocetak = s.pocetak;
    niz = new Tip[kapacitet];
    for (int i = 0; i < brElemenata; i++) {
        niz[i] = s.niz[s.pocetak + i];
    }
}

template<typename Tip>
Red<Tip>::Red():kapacitet(1) {
    brElemenata=0;
    pocetak = 0;
    niz= new Tip[kapacitet];
}

template<typename Tip>
Red<Tip>::~Red() {
    delete [] niz;
}

template<typename Tip>
void Red<Tip>::brisi() {
    brElemenata=0;
    pocetak=0;
}

template<typename Tip>
void Red<Tip>::stavi(const Tip &el) {
    if(brElemenata == kapacitet) {
        Tip* pomocni = new Tip[kapacitet*2];
        kapacitet*=2;
        for (int i = 0; i < brElemenata; i++) {
            pomocni[i] = niz[pocetak + i];
        }
        pomocni[brElemenata] = el;
        brElemenata++;
        delete [] niz;
        niz = pomocni;
        pocetak=0;
        pomocni=nullptr;
    }else {
        if(brElemenata==0) pocetak=0;
        niz[brElemenata] = el;
        brElemenata++;
    }
}

template<typename Tip>
Tip Red<Tip>::skini() {
    if(brElemenata == 0) throw "Prazan Red!";
    Tip povratni = niz[pocetak];
    pocetak++;
    brElemenata--;
    return povratni;
}

template<typename Tip>
Tip& Red<Tip>::celo() {
    return niz[pocetak];
}


int main() {
    int brel;
    Red<int> r;
    for (int i(1); i<=5; i++)
        r.stavi(i);
    {
        // Konstruktor kopije
        Red<int> r2(r);
        Red<int> r3;
        // Operator dodjele
        (r3=r).brojElemenata();
        // Destruktivna samododjela
        (r=r).brojElemenata();

        r.skini();
        brel = r2.brojElemenata();
        for (int i(0); i<brel; i++)
            std::cout << r2.skini() << " ";
        brel = r3.brojElemenata();
        for (int i(0); i<brel; i++)
            std::cout << r3.skini() << " ";
    }
    brel = r.brojElemenata();
    for (int i(0); i<brel; i++)
        std::cout << r.skini() << " ";
    return 0;
}
