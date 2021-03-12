#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <utility>

using namespace std;

template <typename K, typename V>
class Mapa {
public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual V& operator[](const K&) = 0;
    virtual V operator[](const K&) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const K&) = 0;
};

template <typename K, typename V>
class Cvor {
public:
    K kljuc;
    V vrijednost;
    Cvor *roditelj, *lijevo, *desno;
    Cvor(const K& kljuc, const V& vrijednost, Cvor* roditelj = nullptr,
         Cvor* lijevo = nullptr, Cvor* desno = nullptr) :
            kljuc(kljuc), vrijednost(vrijednost), roditelj(roditelj),
            lijevo(lijevo), desno(desno) {}
};

template <typename K, typename V>
class BinStabloMapa : public Mapa<K, V> {
    mutable Cvor<K, V> *korijen;
    int velicina;
    Cvor<K, V>*& Nadji(const K& kljuc, Cvor<K, V>* &cvor);
    V& NadjiIUbaci(const K& kljuc, Cvor<K, V>* &cvor, Cvor<K, V> * roditelj = nullptr);
    Cvor<K, V>*& NadjiConst(const K& kljuc, Cvor<K, V>* &cvor) const;
    Cvor<K, V>* Ubaci(const K& kljuc, const V& vrijednost, Cvor<K, V> *&cvor, Cvor<K, V> * const& roditelj = nullptr);
    void BrisiCvor(const K& kljuc, Cvor<K, V>* &cvor);
    void Brisi(Cvor<K, V> *&cvor);
    void Kopiraj(const Cvor<K, V>* stari, Cvor<K, V>* &novi, Cvor<K, V>* roditelj = nullptr);
public:
    BinStabloMapa() : korijen(nullptr), velicina(0) {}
    BinStabloMapa(const BinStabloMapa& rhs);
    BinStabloMapa(BinStabloMapa&& rhs);
    ~BinStabloMapa();
    V& operator[](const K&);
    V operator[](const K&) const;
    BinStabloMapa& operator=(const BinStabloMapa& rhs);
    BinStabloMapa& operator=(BinStabloMapa&& rhs);
    int brojElemenata() const { return velicina; }
    void obrisi() { Brisi(korijen); velicina = 0; }
    void obrisi(const K& kljuc) { BrisiCvor(kljuc, korijen); }
};

template <typename K, typename V>
V& BinStabloMapa<K, V>::NadjiIUbaci(const K& kljuc, Cvor<K, V>* &cvor, Cvor<K, V> * roditelj) {
    if(korijen == nullptr) return Ubaci(kljuc, V(), korijen)->vrijednost;
    if(roditelj== nullptr) roditelj = korijen;
    if(cvor == nullptr){
        return Ubaci(kljuc, V(),cvor,roditelj)->vrijednost;
    }
    if(cvor->kljuc == kljuc) return cvor->vrijednost;
    else if(cvor->kljuc < kljuc) {roditelj=cvor; return NadjiIUbaci(kljuc, cvor->desno,roditelj);}
    else {roditelj =cvor; return NadjiIUbaci(kljuc, cvor->lijevo,roditelj);}
}

template <typename K, typename V>
Cvor<K, V>*& BinStabloMapa<K, V>::Nadji(const K& kljuc, Cvor<K, V>* &cvor) {
    if(cvor == nullptr || cvor->kljuc == kljuc) return cvor;
    else if(cvor->kljuc < kljuc) return Nadji(kljuc, cvor->desno);
    else return Nadji(kljuc, cvor->lijevo);
}

template <typename K, typename V>
Cvor<K, V>*& BinStabloMapa<K, V>::NadjiConst(const K& kljuc, Cvor<K, V>* &cvor) const {
    if(cvor == nullptr || cvor->kljuc == kljuc) return cvor;
    else if(cvor->kljuc < kljuc) return NadjiConst(kljuc, cvor->desno);
    else return NadjiConst(kljuc, cvor->lijevo);
}

template <typename K, typename V>
Cvor<K, V> *BinStabloMapa<K, V>::Ubaci(const K& kljuc, const V& vrijednost, Cvor<K, V> *&cvor, Cvor<K, V> * const& roditelj) {
    if(cvor == nullptr) {
        velicina++;
        return cvor = new Cvor<K, V>(kljuc, vrijednost, roditelj);
    }
    else if(cvor->kljuc < kljuc)
        return Ubaci(kljuc, vrijednost, cvor->desno, cvor);
    else
        return Ubaci(kljuc, vrijednost, cvor->lijevo, cvor);
}

template <typename K, typename V>
void BinStabloMapa<K, V>::BrisiCvor(const K& kljuc, Cvor<K, V>* &cvor) {
    Cvor<K, V> *p(Nadji(kljuc, korijen)), *rp(nullptr);
    if (p == nullptr)
        return;
    if (velicina == 1) {
        korijen = nullptr;
        velicina--;
        delete p;
        p= nullptr;
        return;
    }
    if(p->desno== nullptr && p->lijevo == nullptr) {
        if (p->roditelj->lijevo != nullptr && p->roditelj->lijevo->kljuc == p->kljuc) p->roditelj->lijevo = nullptr;
        else p->roditelj->desno = nullptr;
        delete p;
        p= nullptr;
        velicina--;
        return;
    }
    if(p->roditelj == nullptr && p->lijevo != nullptr && p->desno != nullptr){
        rp = p->desno;
        while (rp->lijevo != nullptr) rp = rp->lijevo;
        rp->lijevo=p->lijevo;
        p->lijevo->roditelj = rp;
        p->desno->roditelj = nullptr;
        korijen = p->desno;
        korijen->roditelj = nullptr;
        delete p;
        p = nullptr;
        velicina--;
        return;
    }
    if(p->roditelj == nullptr) {
        if (p->lijevo != nullptr) korijen = p->lijevo;
        else korijen = p->desno;
        korijen->roditelj = nullptr;
        delete p;
        p = nullptr;
        velicina--;
        return;
    }
    if (p->lijevo == nullptr) {
        p->desno->roditelj = p->roditelj;
        if (p->roditelj->lijevo != nullptr && p->roditelj->lijevo->kljuc == p->kljuc) p->roditelj->lijevo = p->desno;
        else p->roditelj->desno = p->desno;
        delete p;
        p = nullptr;
        velicina--;
        return;
    }
    if (p->desno == nullptr){
        p->lijevo->roditelj = p->roditelj;
        if (p->roditelj->lijevo != nullptr &&  p->roditelj->lijevo->kljuc == p->kljuc) p->roditelj->lijevo = p->lijevo;
        else p->roditelj->desno = p->lijevo;

        delete p;
        p = nullptr;
        velicina--;
        return;
    }
    rp = p->desno;
    while (rp->lijevo != nullptr) rp = rp->lijevo;
    rp->lijevo=p->lijevo;
    p->lijevo->roditelj = rp;
    p->desno->roditelj = p->roditelj;
    if (p->roditelj->lijevo != nullptr &&  p->roditelj->lijevo->kljuc == p->kljuc) p->roditelj->lijevo = p->desno;
    else p->roditelj->desno = p->desno;
    delete p;
    p = nullptr;
    velicina--;
}

template <typename K, typename V>
void BinStabloMapa<K, V>::Brisi(Cvor<K, V> *&cvor) {
    if(cvor != nullptr) {
        Brisi(cvor->desno);
        Brisi(cvor->lijevo);
        delete cvor;
        cvor = nullptr;
    }
}

template <typename K, typename V>
void BinStabloMapa<K, V>::Kopiraj(const Cvor<K, V>* stari, Cvor<K, V>* &novi, Cvor<K, V>* roditelj) {
    if(stari == nullptr)
        return;
    novi = new Cvor<K, V>(stari->kljuc, stari->vrijednost, roditelj);
    Kopiraj(stari->lijevo, novi->lijevo, novi);
    Kopiraj(stari->desno, novi->desno, novi);
}

template <typename K, typename V>
BinStabloMapa<K, V>::BinStabloMapa(const BinStabloMapa& rhs)
        : velicina(rhs.velicina), korijen(nullptr) {
    Kopiraj(rhs.korijen, korijen);
}

template <typename K, typename V>
BinStabloMapa<K, V>::BinStabloMapa(BinStabloMapa&& rhs)
        : velicina(rhs.velicina), korijen(rhs.korijen) {
    rhs.korijen = nullptr;
}

template <typename K, typename V>
BinStabloMapa<K, V>::~BinStabloMapa() {
    Brisi(korijen);
}

template <typename K, typename V>
V& BinStabloMapa<K, V>::operator[](const K& kljuc) {
    return NadjiIUbaci(kljuc,korijen);
}

template <typename K, typename V>
V BinStabloMapa<K, V>::operator[](const K& kljuc) const {
    auto rezultat(NadjiConst(kljuc, korijen));
    if(rezultat != nullptr)
        return rezultat->vrijednost;
    return V();
}

template <typename K, typename V>
BinStabloMapa<K, V>& BinStabloMapa<K, V>::operator=(const BinStabloMapa<K, V>& rhs) {
    if(this == &rhs)
        return *this;
    Brisi(korijen);
    velicina = rhs.velicina;
    korijen = nullptr;
    Kopiraj(rhs.korijen, korijen);
    return *this;
}

template <typename K, typename V>
BinStabloMapa<K, V>& BinStabloMapa<K, V>::operator=(BinStabloMapa<K, V>&& rhs) {
    if(this == &rhs)
        return *this;
    Brisi(korijen);
    velicina = rhs.velicina;
    korijen = rhs.korijen;
    rhs.korijen = nullptr;
    return *this;
}


template <typename K,typename V>
class NizMapa : public Mapa<K, V> {
    static const int INICIJALNA_VELICINA = 32;
    int brEl, prostor;
    pair<K, V> **niz, **kraj;
    void Prosiri();
public:
    NizMapa() : brEl(0), prostor(INICIJALNA_VELICINA), niz(new pair<K, V>*[prostor]{}), kraj(niz) {}
    NizMapa(const NizMapa& rhs);
    NizMapa(NizMapa&& rhs);
    ~NizMapa();
    V& operator[](const K& kljuc);
    V operator[](const K& kljuc) const;
    NizMapa& operator=(const NizMapa& rhs);
    NizMapa& operator=(NizMapa&& rhs);
    int brojElemenata() const { return brEl; }
    void obrisi(const K& kljuc);
    void obrisi();
};

template <typename K, typename V>
NizMapa<K, V>::NizMapa(const NizMapa& rhs) : brEl(rhs.brEl), prostor(rhs.prostor), niz(new pair<K, V>*[prostor]{}), kraj(niz) {
    for(pair<K, V> **it = rhs.niz, **it2 = niz; it != rhs.kraj; it++)
        if(*it) {
            (*it2++) = new pair<K, V>(**it);
            kraj++;
        }
}

template <typename K, typename V>
NizMapa<K, V>::NizMapa(NizMapa&& rhs) : brEl(rhs.brEl), prostor(rhs.prostor), niz(rhs.niz), kraj(rhs.kraj) {
    rhs.niz = rhs.kraj = nullptr;
}

template <typename K, typename V>
NizMapa<K, V>& NizMapa<K, V>::operator=(const NizMapa& rhs) {
    if(this == &rhs)
        return *this;
    obrisi();
    delete[] niz;
    brEl = rhs.brEl;
    prostor = rhs.prostor;
    niz = new pair<K, V>*[prostor]{};
    kraj = niz;
    for(pair<K, V> **it = rhs.niz, **it2 = niz; it != rhs.kraj; it++)
        if(*it) {
            (*it2++) = new pair<K, V>(**it);
            kraj++;
        }
    return *this;
}

template <typename K, typename V>
NizMapa<K, V>& NizMapa<K, V>::operator=(NizMapa&& rhs) {
    if(this == &rhs)
        return *this;
    obrisi();
    delete[] niz;
    brEl = rhs.brEl;
    prostor = rhs.prostor;
    niz = rhs.niz;
    kraj = rhs.kraj;
    rhs.niz = rhs.kraj = nullptr;
    return *this;
}

template <typename K, typename V>
void NizMapa<K, V>::Prosiri() {
    pair<K, V> **noviNiz = new pair<K, V>*[brEl * 2]{};
    pair<K, V> **it, **it2;
    for(it = niz, it2 = noviNiz; it != kraj; it++)
        if(*it != nullptr)
            (*it2++) = new pair<K, V>(**it++);
    int brEl2(brEl * 2);
    obrisi();
    delete[] niz;
    prostor = brEl = brEl2;
    kraj = it2;
    niz = noviNiz;
}

template <typename K, typename V>
V& NizMapa<K, V>::operator[](const K& kljuc) {
    srand(0);
    bool pronadjen(false);
    pair<K, V> **it;
    for(it = niz; it != kraj; it++)
        if(*it && (*it)->first == kljuc) {
            pronadjen = true;
            break;
        }
    if(pronadjen)
        return (*it)->second;
    else {
        if(kraj - niz == prostor)
            Prosiri();
        *kraj = new pair<K, V>(kljuc, V());
        brEl++;
        return (*kraj++)->second;
    }
}

template <typename K, typename V>
V NizMapa<K, V>::operator[](const K& kljuc) const {
    bool pronadjen(false);
    pair<K, V> **it;
    for(it = niz; it != kraj; it++)
        if(*it && (*it)->first == kljuc) {
            pronadjen = true;
            break;
        }
    if(pronadjen)
        return (*it)->second;
    else
        return V();
}

template <typename K, typename V>
void NizMapa<K, V>::obrisi(const K& kljuc) {
    bool pronadjen(false);
    for(pair<K, V> **it = niz; it != kraj; it++)
        if(*it && (*it)->first == kljuc) {
            delete *it;
            *it = nullptr;
            if(it + 1 == kraj)
                kraj = it;
            brEl--;
            pronadjen = true;
            break;
        }
    if(!pronadjen)
        throw "Kljuc nije pronadjen";
}

template <typename K, typename V>
void NizMapa<K, V>::obrisi() {
    for(pair<K, V> **it = niz; it != kraj; it++) {
        delete *it;
        *it = nullptr;
    }
    brEl = 0;
    kraj = niz;
}

template <typename K, typename V>
NizMapa<K, V>::~NizMapa() {
    obrisi();
    delete[] niz;
}

double avgTime(clock_t t1, clock_t t2, int broj_el) {
    return ((t2 - t1) / (CLOCKS_PER_SEC / 1000.)) / broj_el;
}



int main()
{
//
//    BinStabloMapa <int, int> m;
//    int vel(10000);
//    for (int i(-vel); i<vel; i++) {
//        m[i] = i;
//    }
//    NizMapa<int, int> niz_test;
//    BinStabloMapa<int, int> stablo_test;
//    vector<int> kljucevi;
//    int broj_el = 10000;
//    for(int i = 0; i < broj_el; i++)
//        kljucevi.push_back(rand());
//    clock_t vrijeme1 = clock();
//    for(auto k : kljucevi)
//        niz_test[k];
//    clock_t vrijeme2 = clock();
//    cout << "Prosjecno vrijeme umetanja u niz mapu: " << avgTime(vrijeme1, vrijeme2, broj_el) << " ms." << endl;
//    vrijeme1 = clock();
//    for(auto k : kljucevi)
//        stablo_test[k];
//    vrijeme2 = clock();
//    cout << "Prosjecno vrijeme umetanja u binarno stablo mapu: " << avgTime(vrijeme1, vrijeme2, broj_el) << " ms." << endl;
//    vrijeme1 = clock();
//    for(auto k : kljucevi)
//        niz_test[k];
//    vrijeme2 = clock();
//    cout << "Prosjecno vrijeme citanja niz mape: " << avgTime(vrijeme1, vrijeme2, broj_el) << " ms." << endl;
//    vrijeme1 = clock();
//    for(auto k : kljucevi)
//        stablo_test[k];
//    vrijeme2 = clock();
//    cout << "Prosjecno vrijeme citanja binarno stablo mape: " << avgTime(vrijeme1, vrijeme2, broj_el) << " ms." << endl;

// Malo jaci test performansi BinStabloMape gdje se vidi
// da li je zaista O(log n)
    BinStabloMapa <int, int> m;
    int vel(100000);
    for (int i(0); i<vel; i++) {
        m[rand()%(2*vel)-vel] = i;
    }
    for (int i(-vel); i<vel; i++)
        m[i] = i;
    for (int i(-vel); i<vel; i++)
        m.obrisi(i);
    //m.obrisi(-9995);
    cout << "OK";

}