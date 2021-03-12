#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <functional>


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
class HashMapaLan : public Mapa<K, V> {
    static const int POCETNA = 6000;
    int brElem;
    typename list<pair<K, V>>::iterator Trazi(const K& kljuc);
    typename list<pair<K, V>>::const_iterator Trazi(const K& kljuc) const;
    vector<list<pair<K, V>>> mapa;
    function<unsigned int(const K&, unsigned int)> h;
    int Velicina() const { return mapa.size(); }
public:
    HashMapaLan() : brElem(0), mapa(POCETNA, list<pair<K, V>>()) {}
    V& operator[](const K& kljuc);
    V operator[](const K& kljuc) const;
    int brojElemenata() const { return brElem; }
    void obrisi(const K& kljuc);
    void obrisi() {
        for(auto& l : mapa)
            l.clear();
        brElem = 0;
    }
    void definisiHashFunkciju(function<unsigned int(const K&, unsigned int)> h) { this->h = h; }
};

template <typename K, typename V>
typename list<pair<K, V>>::iterator HashMapaLan<K, V>::Trazi(const K& kljuc) {
    if (!h) throw "funkcija za hashiranje nije definisana";
    int hes(h(kljuc, Velicina()));
    auto it(mapa[hes].begin());
    while (it != mapa[hes].end() && kljuc > it->first) it++;
    return it;
}

template <typename K, typename V>
typename list<pair<K, V>>::const_iterator HashMapaLan<K, V>::Trazi(const K& kljuc) const {
    if (!h) throw "funkcija za hashiranje nije definisana";
    int hes(h(kljuc, Velicina()));
    auto it(mapa[hes].begin());
    while (it != mapa[hes].end() && kljuc > it->first) it++;
    return it;
}

template <typename K, typename V>
V& HashMapaLan<K, V>::operator[](const K& kljuc) {
    auto it(Trazi(kljuc));
    int hes(h(kljuc, Velicina()));
    if (it == mapa[hes].end() || it->first != kljuc) {
        it = mapa[hes].insert(it, pair<K, V>(kljuc, V()));
        brElem++;
    }
    return it->second;
}

template <typename K, typename V>
V HashMapaLan<K, V>::operator[](const K& kljuc) const {
    auto it(Trazi(kljuc));
    int hes(h(kljuc, Velicina()));
    if (it == mapa[hes].end() || it->first != kljuc)
        return V();
    return it->second;
}

template <typename K, typename V>
void HashMapaLan<K, V>::obrisi(const K& kljuc) {
    auto it(Trazi(kljuc));
    int hes(h(kljuc, Velicina()));
    if (it == mapa[hes].end() || it->first != kljuc)
        throw "Kljuc nije pronadjen";
    auto itp(it++);
    mapa[hes].erase(itp, it);
    brElem--;

}

//Zbog cikličnih zavisnosti klasa Grana, Cvor i GranaIterator, koristimo forward deklaracije
template <typename T>
class Cvor;
template <typename T>
class Grana;
template <typename T>
class GranaIterator;
//Klasa Usmejereni graf sa svim potrebim funkcijama
template <typename T>
class UsmjereniGraf {
protected:
    UsmjereniGraf() {}
public:
    UsmjereniGraf(int) {}
    virtual ~UsmjereniGraf() {};

    virtual void dodajGranu(int, int, float = 0) = 0;
    virtual float dajTezinuGrane(int, int) const = 0;
    virtual bool postojiGrana(int, int) const = 0;
    virtual void obrisiGranu(int, int) = 0;
    virtual void postaviTezinuGrane(int, int, float = 0) = 0;

    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int) = 0;

    virtual Grana<T> dajGranu(int, int) = 0;
    virtual const Grana<T> dajGranu(int, int) const = 0;
    virtual Cvor<T> dajCvor(int) = 0;
    virtual const Cvor<T> dajCvor(int) const = 0;

    virtual void postaviOznakuCvora(int, T) = 0;
    virtual T dajOznakuCvora(int) const = 0;
    virtual void postaviOznakuGrane(int, int, T) = 0;
    virtual T dajOznakuGrane(int, int) const = 0;

    const GranaIterator<T> dajGranePocetak() const { return ++GranaIterator<T>(const_cast<UsmjereniGraf*>(this), 0, -1); }
    GranaIterator<T> dajGraneKraj() { return GranaIterator<T>(this, dajBrojCvorova(), 0); }
    GranaIterator<T> dajGranePocetak() { return ++GranaIterator<T>(this, 0, -1); }
    const GranaIterator<T> dajGraneKraj() const {return GranaIterator<T>(const_cast<UsmjereniGraf*>(this), dajBrojCvorova(), 0); }

};
//Klasa Grana koji koristimo u klasi Usmjereni Graf
template <typename T>
class Grana {
    UsmjereniGraf<T>* graf;
    int startni, krajnji;
public:
    Grana(UsmjereniGraf<T>* graf, int startni, int krajnji) : graf(graf), startni(startni), krajnji(krajnji) {}
    Cvor<T> dajPolazniCvor() { return graf->dajCvor(startni); }
    float dajTezinu() const { return graf->dajTezinuGrane(startni, krajnji); }
    void postaviTezinu(float tezina) { graf->postaviTezinuGrane(startni, krajnji, tezina); }
    const Cvor<T> dajPolazniCvor() const { return graf->dajCvor(startni); }
    Cvor<T> dajDolazniCvor() { return graf->dajCvor(krajnji); }
    const Cvor<T> dajDolazniCvor() const { return graf->dajCvor(krajnji); }
    T dajOznaku() const { return graf->dajOznakuGrane(startni, krajnji); }
    void postaviOznaku(T oznaka) { graf->postaviOznakuGrane(startni, krajnji, oznaka); }
};

template <typename T>
class GranaIterator {
    UsmjereniGraf<T>* graf;
    int i, j;
    void inkrementuj(int &i, int &j) { j + 1 >= graf->dajBrojCvorova() ? j = 0, i++ : j++; }
public:
    GranaIterator(UsmjereniGraf<T>* graf, int i, int j) : graf(graf), i(i), j(j) {}
    Grana<T> operator*() { return graf->dajGranu(i, j); }
    const Grana<T> operator*() const { return graf->dajGranu(i, j); }
    GranaIterator& operator++();
    GranaIterator operator++(int);
    bool operator==(const GranaIterator &iter) const { return i == iter.i && j == iter.j && graf == iter.graf; }
    bool operator!=(const GranaIterator &iter) const { return !(*this == iter); }
};
//Klasa cvor koju koristimo u klasi Usmjereni Graf sa svim potrebnim funkcijama
template <typename T>
class Cvor {
    UsmjereniGraf<T>* graf;
    int redniBr;
public:
    Cvor(UsmjereniGraf<T>* graf, int redniBr) : graf(graf),redniBr(redniBr) {}
    T dajOznaku() const { return graf->dajOznakuCvora(redniBr); }
    void postaviOznaku(T oznaka) { graf->postaviOznakuCvora(redniBr, oznaka); }
    int dajRedniBroj() const { return redniBr; }
};

template <typename T>
GranaIterator<T> GranaIterator<T>::operator++(int) {
    auto kopija(*this);
    ++(*this);
    return kopija;
}

template <typename T>
GranaIterator<T>& GranaIterator<T>::operator++() {
    do
        inkrementuj(i, j);
    while (i < graf->dajBrojCvorova() && !graf->postojiGrana(i, j));
    return *this;
}
template <typename T>
class MatricaGraf : public UsmjereniGraf<T> {
    const float BESKON = std::numeric_limits<float>::infinity();
    vector<vector<float>> m;
    vector<T> oznakeCvorova;
    vector<vector<T>> oznakeGrana;

    void validirajGranu(int, int) const;
    void validirajCvor(int) const;
    void postojanjeGrane(int, int) const;
public:
    MatricaGraf(int brojCvorova);

    int dajBrojCvorova() const { return m.size(); }
    void postaviBrojCvorova(int);

    Grana<T> dajGranu(int, int);
    const Grana<T> dajGranu(int, int) const;
    Cvor<T> dajCvor(int);
    const Cvor<T> dajCvor(int) const;

    void postaviOznakuCvora(int, T);
    T dajOznakuCvora(int) const;
    void postaviOznakuGrane(int, int, T);
    T dajOznakuGrane(int, int) const;

    void dodajGranu(int, int, float = 0);
    void obrisiGranu(int, int);
    void postaviTezinuGrane(int, int, float = 0);
    float dajTezinuGrane(int, int) const;
    bool postojiGrana(int, int) const;
};
//Funkcija validiranja postojanja grane
template <typename T>
void MatricaGraf<T>::postojanjeGrane(int i, int j) const {
    validirajGranu(i, j);
    if (!postojiGrana(i, j))
        throw "Grana ne postoji";
}
//funkcija validiranja cvora
template <typename T>
void MatricaGraf<T>::validirajCvor(int indeks) const {
    if (indeks < 0 || indeks >= dajBrojCvorova())
        throw "Neispravan index";
}
//funkcija validiranja grane
template <typename T>
void MatricaGraf<T>::validirajGranu(int i, int j) const {
    validirajCvor(i);
    validirajCvor(j);
}
//Funkcija koja prikazuje graf preko matrice susjedstva
template <typename T>
MatricaGraf<T>::MatricaGraf(int brojCvorova) {
    if (brojCvorova < 0) throw "Broj cvorova ne moze biti manji od 0";
    m = vector<vector<float>>(brojCvorova, vector<float>(brojCvorova, BESKON));
    oznakeCvorova = vector<T>(brojCvorova);
    oznakeGrana = vector<vector<T>>(brojCvorova, vector<T>(brojCvorova));
}
//Funkcija za dodavanje nove grane
template <typename T>
void MatricaGraf<T>::dodajGranu(int i, int j, float w) {
    validirajGranu(i, j);
    m[i][j] = w;
}
//funkcija za brisanje grane
template <typename T>
void MatricaGraf<T>::obrisiGranu(int i, int j) {
    postojanjeGrane(i, j);
    m[i][j] = BESKON;
}
//Funkcija postvaljanja tezine grane za datu granu
template <typename T>
void MatricaGraf<T>::postaviTezinuGrane(int i, int j, float w) {
    postojanjeGrane(i, j);
    m[i][j] = w;
}
//Funkcija za postavljanje broja cvorova
template <typename T>
void MatricaGraf<T>::postaviBrojCvorova(int noviBrojCvorova) {
    if (noviBrojCvorova < dajBrojCvorova()) throw "Novi broj cvorova ne moze biti manji od trenutnog";
    m.resize(noviBrojCvorova);
    for (auto& vf : m) vf.resize(noviBrojCvorova, BESKON);
    oznakeCvorova.resize(noviBrojCvorova);
    oznakeGrana.resize(noviBrojCvorova);
    for (auto& vt : oznakeGrana) vt.resize(noviBrojCvorova);
}
//Funkcija koja vraca tezinu date grane
template <typename T>
float MatricaGraf<T>::dajTezinuGrane(int i, int j) const {
    postojanjeGrane(i, j);
    return m[i][j];
}
//Funkcija koja vraca oznaku za dati cvor
template <typename T>
T MatricaGraf<T>::dajOznakuCvora(int i) const {
    validirajCvor(i);
    return oznakeCvorova[i];
}
//Funkcija koja postavlja oznaku za datu granu
template <typename T>
void MatricaGraf<T>::postaviOznakuGrane(int i, int j, T oznaka) {
    postojanjeGrane(i, j);
    oznakeGrana[i][j] = oznaka;

}
//Funkcija koja vraca oznaku grane
template <typename T>
T MatricaGraf<T>::dajOznakuGrane(int i, int j) const {
    postojanjeGrane(i, j);
    return oznakeGrana[i][j];
}
//Funkcija koja provjerava postojanje grane
template <typename T>
bool MatricaGraf<T>::postojiGrana(int i, int j) const {
    validirajGranu(i, j);
    return m[i][j] != BESKON;
}
//Funkcija koja postavlja oznaku za dati cvor
template <typename T>
void MatricaGraf<T>::postaviOznakuCvora(int i, T oznaka) {
    validirajCvor(i);
    oznakeCvorova[i] = oznaka;
}
//Funkcija koja vraca granu
template <typename T>
Grana<T> MatricaGraf<T>::dajGranu(int i, int j) {
    postojanjeGrane(i, j);
    return Grana<T>(this, i, j);
}
//Funkija koja vraca cvor
template <typename T>
Cvor<T> MatricaGraf<T>::dajCvor(int i) {
    validirajCvor(i);
    return Cvor<T>(this, i);
}
//Funkija koja vraca cvor (za konstantne grafove)
template <typename T>
const Cvor<T> MatricaGraf<T>::dajCvor(int i) const {
    validirajCvor(i);
    return Cvor<T>(const_cast<MatricaGraf*>(this), i);
}
//Funkcija koja vraca granu (za konstantne grafove)
template <typename T>
const Grana<T> MatricaGraf<T>::dajGranu(int i, int j) const {
    postojanjeGrane(i, j);
    return Grana<T>(const_cast<MatricaGraf*>(this), i, j);
}

template <typename T>
class ListaGraf : public UsmjereniGraf<T> {
    struct Element {
        int cvor;
        float tezina;
        T oznaka;
        Element(int cvor, float tezina = 0, T oznaka = T()) :
                cvor(cvor), tezina(tezina), oznaka(oznaka) {}
    };

    vector<list<Element>> listaSusjedstva;
    vector<T> oznakeCvorova;

    void validirajCvor(int) const;
    void validirajGranu(int, int) const;
    void validirajPostojanjeGrane(int, int) const;

    typename list<Element>::iterator pronadjiElement(int i, int j);
    typename list<Element>::const_iterator pronadjiElement(int i, int j) const;
public:
    void ispisi() {
        for (int i = 0; i < listaSusjedstva.size(); i++) {
            cout << i << ": ";
            for (Element e : listaSusjedstva[i])
                cout << e.cvor << " ";
            cout << endl;
        }
    }
public:
    ListaGraf(int brojCvorova);

    int dajBrojCvorova() const { return listaSusjedstva.size(); }
    void postaviBrojCvorova(int);

    void postaviOznakuCvora(int, T);
    T dajOznakuCvora(int) const;
    void postaviOznakuGrane(int, int, T);
    T dajOznakuGrane(int, int) const;

    void dodajGranu(int, int, float = 0);
    void obrisiGranu(int, int);
    void postaviTezinuGrane(int, int, float = 0);
    float dajTezinuGrane(int, int) const;
    bool postojiGrana(int, int) const;

    Grana<T> dajGranu(int, int);
    const Grana<T> dajGranu(int, int) const;
    Cvor<T> dajCvor(int);
    const Cvor<T> dajCvor(int) const;
};


template <typename T>
typename list<typename ListaGraf<T>::Element>::iterator ListaGraf<T>::pronadjiElement(int i, int j) {
    typename list<Element>::iterator it;
    for (it = listaSusjedstva[i].begin(); it !=
                                          listaSusjedstva[i].end(); it++) {
        if (j == it->cvor)
            return it;
        else if (j < it->cvor)
            return listaSusjedstva[i].end();
    }
    return it;
}

template <typename T>
typename list<typename ListaGraf<T>::Element>::const_iterator ListaGraf<T>::pronadjiElement(int i, int j) const {
    typename list<Element>::const_iterator it;
    for (it = listaSusjedstva[i].begin(); it !=
                                          listaSusjedstva[i].end(); it++) {
        if (j == it->cvor)
            return it;
        else if (j < it->cvor)
            return listaSusjedstva[i].end();
    }
    return it;
}

template <typename T>
void ListaGraf<T>::validirajCvor(int indeks) const {
    if (indeks < 0 || indeks >= dajBrojCvorova())
        throw "Neispravan broj cvora";
}

template <typename T>
void ListaGraf<T>::validirajGranu(int i, int j) const {
    validirajCvor(i);
    validirajCvor(j);
}

template <typename T>
void ListaGraf<T>::validirajPostojanjeGrane(int i, int j) const {
    validirajGranu(i, j);
    if (!postojiGrana(i, j))
        throw "Grana ne postoji";
}

template <typename T>
ListaGraf<T>::ListaGraf(int brojCvorova) {
    if (brojCvorova < 0)
        throw "Broj cvorova ne moze biti negativan";
    listaSusjedstva = vector<list<Element>>(brojCvorova, list<Element>());
    oznakeCvorova = vector<T>(brojCvorova);
}

template <typename T>
void ListaGraf<T>::postaviBrojCvorova(int noviBrojCvorova) {
    if (noviBrojCvorova < dajBrojCvorova())
        throw "Novi broj cvorova ne moze biti manji od trenutnog";
    listaSusjedstva.resize(noviBrojCvorova);
    oznakeCvorova.resize(noviBrojCvorova);
}

template <typename T>
void ListaGraf<T>::dodajGranu(int i, int j, float w) {
    validirajGranu(i, j);
    typename list<Element>::iterator it;
    for (it = listaSusjedstva[i].begin();
         it != listaSusjedstva[i].end() && j > it->cvor;
         it++);
    listaSusjedstva[i].insert(it, Element(j, w));
}

template <typename T>
void ListaGraf<T>::obrisiGranu(int i, int j) {
    validirajPostojanjeGrane(i, j);
    listaSusjedstva[i].erase(pronadjiElement(i, j));
}

template <typename T>
void ListaGraf<T>::postaviTezinuGrane(int i, int j, float w) {
    validirajPostojanjeGrane(i, j);
    pronadjiElement(i, j)->tezina = w;
}

template <typename T>
void ListaGraf<T>::postaviOznakuCvora(int i, T oznaka) {
    validirajCvor(i);
    oznakeCvorova[i] = oznaka;
}

template <typename T>
T ListaGraf<T>::dajOznakuCvora(int i) const {
    validirajCvor(i);
    return oznakeCvorova[i];
}
template <typename T>
float ListaGraf<T>::dajTezinuGrane(int i, int j) const {
    validirajPostojanjeGrane(i, j);
    return pronadjiElement(i, j)->tezina;
}

template <typename T>
bool ListaGraf<T>::postojiGrana(int i, int j) const {
    validirajGranu(i, j);
    return pronadjiElement(i, j) != listaSusjedstva[i].end();
}

template <typename T>
void ListaGraf<T>::postaviOznakuGrane(int i, int j, T oznaka) {
    validirajPostojanjeGrane(i, j);
    pronadjiElement(i, j)->oznaka = oznaka;

}

template <typename T>
T ListaGraf<T>::dajOznakuGrane(int i, int j) const {
    validirajPostojanjeGrane(i, j);
    return pronadjiElement(i, j)->oznaka;
}

template <typename T>
Grana<T> ListaGraf<T>::dajGranu(int i, int j) {
    validirajPostojanjeGrane(i, j);
    return Grana<T>(this, i, j);
}
template <typename T>
const Grana<T> ListaGraf<T>::dajGranu(int i, int j) const {
    validirajPostojanjeGrane(i, j);
    return Grana<T>(const_cast<ListaGraf*>(this), i, j);
}
//funkcija koja vraca cvor za listuGraf
template <typename T>
Cvor<T> ListaGraf<T>::dajCvor(int i) {
    validirajCvor(i);
    return Cvor<T>(this, i);
}
//funkcija koja vraca cvor za listuGraf(const)
template <typename T>
const Cvor<T> ListaGraf<T>::dajCvor(int i) const {
    validirajCvor(i);
    return Cvor<T>(const_cast<ListaGraf*>(this), i);
}
//feunkcija za dfs pretragu za sve klase
template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *graf, vector<Cvor<TipOznake> > &dfs_obilazak,
         Cvor<TipOznake> cvor) {
    cvor.postaviOznaku(1);
    dfs_obilazak.push_back(cvor);
    for (GranaIterator<TipOznake> it = graf->dajGranePocetak(); it != graf->dajGraneKraj(); ++it) {
        Cvor<TipOznake> susjedni = (*it).dajDolazniCvor();
        if ((*it).dajPolazniCvor().dajRedniBroj() == cvor.dajRedniBroj() &&
            susjedni.dajOznaku() != 1)
            dfs(graf, dfs_obilazak, susjedni);
    }
}
//funkcija za bfs pretragu za sve klase
template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *graf, vector<Cvor<TipOznake> > &bfs_obilazak,
         Cvor<TipOznake> pocetni_cvor) {
    pocetni_cvor.postaviOznaku(1);
    bfs_obilazak.push_back(pocetni_cvor);
    queue<Cvor<TipOznake>> red;
    red.push(pocetni_cvor);
    while (!red.empty()) {
        Cvor<TipOznake> c = red.front();
        red.pop();
        for (GranaIterator<TipOznake> it = graf->dajGranePocetak(); it != graf->dajGraneKraj(); ++it) {
            Cvor<TipOznake> susjedni = (*it).dajDolazniCvor();
            if ((*it).dajPolazniCvor().dajRedniBroj() == c.dajRedniBroj() &&
                susjedni.dajOznaku() != 1) {
                susjedni.postaviOznaku(1);
                bfs_obilazak.push_back(susjedni);
                red.push(susjedni);
            }
        }

    }

}


int main() {
}