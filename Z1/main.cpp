#include <iostream>

template <typename Tip>
class Lista{
private:
    void operator = (const Lista&);
    Lista(const Lista&);
public:
    Lista() {}
    virtual ~Lista() {};
    virtual int brojElemenata () const = 0;
    virtual Tip& trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip& operator [](int broj) = 0;
};

template<typename Tip>
class DvostrukaLista : public Lista<Tip> {
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
    DvostrukaLista() : pocetni(0),krajnji(0),tekuci(0), brelemenata(0) {} //Uradio
    DvostrukaLista(const DvostrukaLista &jdlista); //Uradio
    DvostrukaLista &operator = (const DvostrukaLista &jdlista); //Uradio
    ~DvostrukaLista(); //Uradio
    void pocetak() { tekuci = pocetni;} //Uradio
    void kraj() { tekuci = krajnji; } //Uradio
    bool prethodni(); //Uradio
    bool sljedeci(); //Uradio
    void obrisi(); //Uradio
    Tip& trenutni(); //Uradio
    Tip& trenutni() const; //Uradio
    void dodajIspred(const Tip& el); //Uradio
    void dodajIza(const Tip& el) ; //Uradio
    int brojElemenata() const ; //Uradio
    Tip& operator [](int broj); //Uradio
    Tip const& operator [](int broj) const; //Uradio
};

template<typename Tip>
Tip& DvostrukaLista<Tip>::operator[](int broj) {
    if(broj<0 || broj>(brelemenata-1)) throw ("Neispravan index");
    if(brelemenata-broj-1 < broj){
        int j=brelemenata-1;
        Cvor* p(krajnji);
        for (; p->prethodni != nullptr; p=p->prethodni)
            if(j-- == broj) break;
        return p->element;
    }else {
        int j = 0;
        Cvor *p(pocetni);
        for (; p->sljedeci != nullptr; p = p->sljedeci)
            if (j++ == broj) break;
        return p->element;
    }
}

template<typename Tip>
Tip const& DvostrukaLista<Tip>::operator[](int broj) const {
    if(broj<0 || broj>(brelemenata-1)) throw ("Neispravan index");
    if(brelemenata-broj-1 < broj){
        int j=brelemenata-1;
        Cvor* p(krajnji);
        for (; p->prethodni != nullptr; p=p->prethodni)
            if(j-- == broj) break;
        return p->element;
    }else {
        int j = 0;
        Cvor *p(pocetni);
        for (; p->sljedeci != nullptr; p = p->sljedeci)
            if (j++ == broj) break;
        return p->element;
    }
}

template<typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &jdlista) : pocetni(0), krajnji(0) , brelemenata(0) {
    Cvor *p(jdlista.pocetni);
    while (p!=0) {
        dodajIza(p->element);
        if(brelemenata > 1) sljedeci();
        p=p->sljedeci;
    }
}

template<typename Tip>
DvostrukaLista<Tip>::~DvostrukaLista() {
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

template<typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator = (const DvostrukaLista &jdlista) {
    if(&jdlista==this) return *this;
    brelemenata=0;
    Cvor *p(jdlista.pocetni);
    while (p!=0) {
        dodajIza(p->element);
        if(brelemenata > 1) sljedeci();
        p=p->sljedeci;
    }
    return *this;
}

template<typename Tip>
bool DvostrukaLista<Tip>::prethodni() {
    if(tekuci == pocetni) return false;
    tekuci = tekuci->prethodni;
    return true;
}

template<typename Tip>
bool DvostrukaLista<Tip>::sljedeci() {
    if(tekuci == krajnji) return false;
    tekuci = tekuci->sljedeci;
    return true;
}

template<typename Tip>
Tip& DvostrukaLista<Tip>::trenutni() {
    if (pocetni==0) throw "Prazno!\n";
    else return tekuci->element;
}

template<typename Tip>
Tip& DvostrukaLista<Tip>::trenutni() const{
    if (pocetni==0) throw "Prazno!\n";
    else return tekuci->element;
}

template<typename Tip>
int DvostrukaLista<Tip>::brojElemenata() const {
    return brelemenata;
}

template<typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip &el) {
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

template<typename Tip>
void DvostrukaLista<Tip>::dodajIza(const Tip &el) {
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

template<typename Tip>
void DvostrukaLista<Tip>::obrisi() {
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

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n){
    DvostrukaLista<Tip> *kopija =  (DvostrukaLista<Tip> *) &n;
    if(n.brojElemenata() <= 0) std::cout<<"Nema elemenata";
    else if(n.brojElemenata() == 1) return (*kopija)[0];
    else {
        Tip maxElement = (*kopija)[0];
        int brojSljedecih=0;
        int j=n.brojElemenata();
        while(i!=j || j!=(i-1)){
            if((*kopija)[i] > maxElement) maxElement=(*kopija)[i];
            if((*kopija)[j] > maxElement) maxElement=(*kopija)[j];
            i++;j++;
        }
        return maxElement;
    }
    return 0;
}

int main() {
    Lista<int>* l;
    l = new DvostrukaLista<int>;
    for (int i(0); i<100000; i++)
        l->dodajIspred(i);
    std::cout <<dajMaksimum(*l);
    delete l;
    return 0;
}
