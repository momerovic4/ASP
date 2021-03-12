#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

template <typename K, typename V>
class Pair {
public:
    K first;
    V second;
    bool brisati;
    Pair() : brisati(false) {}
    Pair(const K& first, const V& second, bool brisati = false) : first(first), second(second), brisati(brisati) {}
};

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
class HashMapa : public Mapa<K, V> {
    int brEl, max;
    Pair<K, V> **niz, **krajNiza;
    static const int pocetni_kapacitet = 100000;
    unsigned int hash(const K& kljuc, unsigned int i) const { return (h(kljuc, max) + i) % max; }
    std::function<unsigned int(const K&, unsigned int)>* h;
    //Pomocne funkcije za pregledniji kod
    int searchHashelement(const K& k) const;
    int addHashelement(const K& k);
    bool deleteHashelement(const K& k);
    void prosiri();
public:
    HashMapa() : brEl(0), max(pocetni_kapacitet), niz(new Pair<K, V>*[max]{}), krajNiza(niz + max) {}
    HashMapa(const HashMapa& hashmapa);
    HashMapa(HashMapa&& hashMapa);
    ~HashMapa();
    HashMapa& operator=(const HashMapa& rhs);
    HashMapa& operator=(HashMapa&& rhs);
    V& operator[](const K& kljuc);
    V operator[](const K& kljuc) const;
    int brojElemenata() const { return brEl; }
    void obrisi(const K& kljuc);
    void obrisi();
    void definisiHashFunkciju(std::function<unsigned int(const K&, unsigned int)>* funkcija) { this->h = funkcija; }
};
//kopirajuci konstruktor
template <typename K, typename V>
HashMapa<K, V>::HashMapa(const HashMapa& hashmapa) : brEl(hashmapa.brEl), max(hashmapa.max), niz(new Pair<K, V>*[max] {}), krajNiza(niz + max), h(hashmapa.h) {
    for (int i = 0; i < hashmapa.max; ++i) if (hashmapa.niz[i]) niz[i] = new Pair<K, V>(*hashmapa.niz[i]);
}
//kopirajuci konstruktor
template <typename K, typename V>
HashMapa<K, V>::HashMapa(HashMapa&& hashMapa) : brEl(hashMapa.brEl), max(hashMapa.max), niz(hashMapa.niz), krajNiza(hashMapa.krajNiza), h(hashMapa.h) {
    hashMapa.niz = hashMapa.krajNiza = nullptr;
}
//Destruktor
template <typename K, typename V>
HashMapa<K, V>::~HashMapa() {
    obrisi();
    delete[] niz;
}
//Brisanje elementa sa tim hashiranim kljucem
template <typename K, typename V>
bool HashMapa<K, V>::deleteHashelement(const K& k) {
    int indeks(searchHashelement(k));
    if (indeks != -1) {
        brEl--;
        niz[indeks]->brisati = true;
        return true;
    }
    return false;
}
//trazenje indexa elementa sa ovim kljucem
template <typename K, typename V>
int HashMapa<K, V>::searchHashelement(const K& k) const {
    unsigned int indeks, i(0);
    do {
        indeks = hash(k, i);
        if (niz[indeks] && !niz[indeks]->brisati && niz[indeks]->first == k)
            return indeks;
        else
            i++;
    } while (niz[indeks] != nullptr && i != max);
    return -1;
}
//dodavanje elementa
template <typename K, typename V>
int HashMapa<K, V>::addHashelement(const K& k) {
    int j, i = 0;
    do {
        j = hash(k, i);
        if (niz[j] == nullptr || niz[j]->brisati) {
            if(niz[j] != nullptr) delete niz[j];

            niz[j] = new Pair<K, V>(k, V());
            brEl++;
            return j;
        }
        else
            i++;
    } while (i < max);
    return -1;
}
//operator dodjele
template <typename K, typename V>
HashMapa<K, V>& HashMapa<K, V>::operator=(const HashMapa& rhs) {
    if (this == &rhs) return *this;
    obrisi();
    delete[] niz;
    brEl = rhs.brEl;
    max = rhs.max;
    niz = new Pair<K, V>*[max] {};
    krajNiza = niz + max;
    h = rhs.h;
    for (int i = 0; i < rhs.max; i++) if (rhs.niz[i]) niz[i] = new Pair<K, V>(*rhs.niz[i]);
    return *this;
}
//operator dodjele
template <typename K, typename V>
HashMapa<K, V>& HashMapa<K, V>::operator=(HashMapa&& rhs) {
    if (this == &rhs) return *this;
    obrisi();
    delete[] niz;
    brEl = rhs.brEl;
    max = rhs.max;
    niz = rhs.niz;
    krajNiza = rhs.krajNiza;
    h = rhs.h;
    rhs.niz = rhs.krajNiza = nullptr;
    return *this;
}
//funkcija za prosirivanje prostoja
template <typename K, typename V>
void HashMapa<K, V>::prosiri() {
    Pair<K, V> **noviNiz = new Pair<K, V>*[brEl * 2]{};
    Pair<K, V> **it, **it2;
    for (it = niz, it2 = noviNiz; it != krajNiza; it++)
        if (*it != nullptr)
            (*it2++) = new Pair<K, V>(**it++);
    int brEl2(brEl * 2);
    obrisi();
    delete[] niz;
    max = brEl = brEl2;
    krajNiza = it2;
    niz = noviNiz;
}
//pronalazi kljuc u nizu kljuceva (u slucaju da nema elementa dodaje ga)
template <typename K, typename V>
V& HashMapa<K, V>::operator[](const K& kljuc) {
    if (!h) throw "funkcija za hashiranje nije definisana";
    int indeks(searchHashelement(kljuc));
    if (indeks == -1) return niz[addHashelement(kljuc)]->second;
    return niz[indeks]->second;
}
//pronalazi kljuc u nizu kljuceva (u slucaju da ga nema vraca default vrijednost)
template <typename K, typename V>
V HashMapa<K, V>::operator[](const K& kljuc) const {
    if (!h) throw "funkcija za hashiranje nije definisana";
    int indeks(searchHashelement(kljuc));
    if (indeks == -1) return V();
    return niz[indeks]->second;
}
//funkcija koja brise element sa datim kljucem
template <typename K, typename V>
void HashMapa<K, V>::obrisi(const K& kljuc) {
    if (!ObrisiHash(kljuc)) throw "Element sa tim kljucem nije pronadjen u mapi";
}
//funkcija koja brise sve elemente u nizu
template <typename K, typename V>
void HashMapa<K, V>::obrisi() {
    for (Pair<K, V> **it = niz; it != krajNiza; it++) {delete *it; *it = nullptr;}
    brEl = 0;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
