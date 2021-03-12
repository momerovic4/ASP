#include <iostream>
using namespace std;

template<typename Tip>
class DvostraniRed {
private:
    struct Cvor {
        Tip element;
        Cvor* prethodni;
        Cvor* sljedeci;
        Cvor( const Tip &element1, Cvor *sljedeci1, Cvor *prethodni1): element(element1) , sljedeci(sljedeci1), prethodni(prethodni1) {}
    };
    Cvor *pocetni;
    Cvor *krajnji;
    Cvor *tekuci;
    int brelemenata;
public:
    DvostraniRed() : pocetni(0),krajnji(0),tekuci(0), brelemenata(0) {}
    DvostraniRed(const DvostraniRed &jdlista);
    DvostraniRed &operator = (const DvostraniRed &jdlista);
    ~DvostraniRed();
    void brisi() ;
    void pocetak() { tekuci = pocetni;}
    void kraj() { tekuci = krajnji; }
    bool prethodni();
    bool sljedeci();
    void obrisi();
    Tip& trenutni();
    Tip& trenutni() const;
    void dodajIspred(const Tip& el);
    void dodajIza(const Tip& el) ;
    int brojElemenata() const ;
    void staviNaVrh(const Tip& el);
    Tip skiniSaVrha();
    Tip& vrh();
    void staviNaCelo(const Tip& el);
    Tip skiniSaCela();
    Tip& celo();
};
//O(1)
template<typename Tip>
void DvostraniRed<Tip>::staviNaVrh(const Tip& el) {
    kraj();
    dodajIza(el);
}
//O(1)
template<typename Tip>
Tip DvostraniRed<Tip>::skiniSaVrha() {
    kraj();
    Tip povratni = trenutni();
    obrisi();
    return povratni;
}
//O(1)
template<typename Tip>
Tip& DvostraniRed<Tip>::vrh() {
    if(brelemenata <= 0) throw "Prazan DvostrukiRed!";
    return krajnji->element;
}
//O(1)
template<typename Tip>
void DvostraniRed<Tip>::staviNaCelo(const Tip& el) {
    pocetak();
    dodajIspred(el);
}
//O(1)
template<typename Tip>
Tip DvostraniRed<Tip>::skiniSaCela() {
    pocetak();
    Tip povratni = trenutni();
    obrisi();
    return povratni;
}
//O(1)
template<typename Tip>
Tip& DvostraniRed<Tip>::celo() {
    if(brelemenata <= 0) throw "Prazan DvostrukiRed!";
    return pocetni->element;
}
//O(n)
template<typename Tip>
DvostraniRed<Tip>::DvostraniRed(const DvostraniRed &jdlista) : pocetni(0), krajnji(0) , brelemenata(0) {
    Cvor *p(jdlista.pocetni);
    while (p!=0) {
        dodajIza(p->element);
        if(brelemenata > 1) sljedeci();
        p=p->sljedeci;
    }
}
//O(n)
template<typename Tip>
void DvostraniRed<Tip>::brisi() {
    if(brelemenata == 1) {
        delete tekuci;
        krajnji = nullptr;
        pocetni = nullptr;
        tekuci = nullptr;
    }
    else {
        pocetak();
        while (sljedeci()) {
            delete tekuci->prethodni;
            tekuci->prethodni=nullptr;
        }
        delete tekuci;
        krajnji=nullptr;
        tekuci=nullptr;
        pocetni=nullptr;
    }
    brelemenata=0;
}
//O(n)
template<typename Tip>
DvostraniRed<Tip>::~DvostraniRed() {
    if(brelemenata == 1) {
        delete tekuci;
        krajnji = nullptr;
        pocetni = nullptr;
        tekuci = nullptr;
    }
    else {
        pocetak();
        while (sljedeci()) {
            delete tekuci->prethodni;
            tekuci->prethodni=nullptr;
        }
        delete tekuci;
        krajnji=nullptr;
        tekuci=nullptr;
        pocetni=nullptr;
    }
}
//O(n)
template<typename Tip>
DvostraniRed<Tip> &DvostraniRed<Tip>::operator = (const DvostraniRed &jdlista) {
    if(&jdlista==this) return *this;
    obrisi();
    pocetni=0;krajnji=0;tekuci=0;
    Cvor *p(jdlista.pocetni);
    while (p!=0) {
        dodajIza(p->element);
        if(brelemenata > 1) sljedeci();
        p=p->sljedeci;
    }
    return *this;
}
//O(1)
template<typename Tip>
bool DvostraniRed<Tip>::prethodni() {
    if(tekuci == pocetni) return false;
    tekuci = tekuci->prethodni;
    return true;
}
//O(1)
template<typename Tip>
bool DvostraniRed<Tip>::sljedeci() {
    if(tekuci == krajnji) return false;
    tekuci = tekuci->sljedeci;
    return true;
}
//O(1)
template<typename Tip>
Tip& DvostraniRed<Tip>::trenutni() {
    if (pocetni==0) throw "Prazno!\n";
    else return tekuci->element;
}
//O(1)
template<typename Tip>
Tip& DvostraniRed<Tip>::trenutni() const{
    if (pocetni==0) throw "Prazno!\n";
    else return tekuci->element;
}
//O(1)
template<typename Tip>
int DvostraniRed<Tip>::brojElemenata() const {
    return brelemenata;
}
//O(1)
template<typename Tip>
void DvostraniRed<Tip>::dodajIspred(const Tip &el) {
    if(pocetni == nullptr) { //Slucaj kada nema elemenata u listi
        Cvor* p = new Cvor(el,0,0);
        pocetni = p;
        krajnji = p;
        tekuci = p;
        p = nullptr;
    }else if(brelemenata==1 || pocetni == tekuci){
        //Slucaj kada je samo jedan element u listi i kada je tekuci jednak pocetnom
        Cvor* novi = new Cvor(el,tekuci,0);
        pocetni = novi;
        tekuci->prethodni = novi;
        novi = nullptr;
    }else { //Slucaj za sve ostalo
        Cvor* p = (pocetni);
        if(tekuci != pocetni) p=tekuci->prethodni;
        Cvor* novi= new Cvor(el, tekuci, p);
        p->sljedeci = novi;
        tekuci->prethodni = novi;
        if(tekuci == pocetni) pocetni = novi;
        p = nullptr;
        novi = nullptr;
    }
    brelemenata++;
}
//O(1)
template<typename Tip>
void DvostraniRed<Tip>::dodajIza(const Tip &el) {
    if(pocetni == nullptr) {
        Cvor* p = new Cvor(el,0,0);
        pocetni = p;
        krajnji = p;
        tekuci = p;
        p = nullptr;
    }else if(pocetni == krajnji || tekuci == krajnji){
        Cvor* novi = new Cvor(el,0,tekuci);
        tekuci->sljedeci = novi;
        krajnji = novi;
        krajnji->prethodni = tekuci;
        novi = nullptr;
    }else if(tekuci == krajnji->prethodni){
        Cvor* novi = new Cvor(el,krajnji,tekuci);
        tekuci->sljedeci = novi;
        krajnji->prethodni = novi;
        novi = nullptr;
    }else {
        Cvor* p = tekuci->sljedeci;
        Cvor* novi= new Cvor(el, tekuci->sljedeci, tekuci);
        p->prethodni = novi;
        tekuci->sljedeci = novi;
        p = nullptr;
        novi = nullptr;
    }
    brelemenata++;
}
//O(1)
template<typename Tip>
void DvostraniRed<Tip>::obrisi() {
    if(krajnji == pocetni) { delete pocetni; pocetni= nullptr; krajnji= nullptr; tekuci= nullptr;}
    else if(tekuci == krajnji){
        prethodni();
        tekuci->sljedeci= nullptr;
        krajnji->prethodni = nullptr;
        delete krajnji; krajnji= nullptr;
        krajnji=tekuci;
    }
    else if(tekuci == pocetni){
        Cvor* p(pocetni);
        sljedeci(); p->sljedeci= nullptr;
        delete p; p= nullptr;
        tekuci->prethodni = nullptr;
        pocetni=tekuci;
    }
    else {
        Cvor* p = tekuci->prethodni;
        Cvor* q = tekuci->sljedeci;
        tekuci->sljedeci = nullptr;
        tekuci->prethodni = nullptr;
        delete tekuci;
        p->sljedeci = q;
        q->prethodni = p;
        tekuci = q;
    }
    brelemenata--;
}

int main() {
    DvostraniRed<int> dr;
    for (int i(100); i<110; i+=2) {
        dr.staviNaVrh(i);
        dr.staviNaCelo(i);
    }
    std::cout << dr.brojElemenata() << " " << dr.vrh() << " " << dr.celo() << std::endl;
    for (int i(0); i<10; i++)
        std::cout << dr.skiniSaVrha() << " ";
    std::cout << std::endl;
    try { dr.skiniSaVrha(); } catch(...) {
        std::cout << "ok ";
    }
    try { dr.skiniSaCela(); } catch(...) {
        std::cout << "ok ";
    }
    try { dr.vrh(); } catch(...) {
        std::cout << "ok ";
    }
    try { dr.celo(); } catch(...) {
        std::cout << "ok ";
    }
    for (int i(100); i<110; i+=2) {
        dr.staviNaVrh(i);
        dr.staviNaCelo(i);
    }
    std::cout << dr.brojElemenata() << " " << dr.vrh() << " " << dr.celo();
    return 0;
}
