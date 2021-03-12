#include <iostream>
using namespace std;
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

template <typename Tip>
class NizLista : public Lista<Tip> {
protected:
    int kapacitet;
    int duzina;
    int tekuci;
    Tip** L;
public:
    NizLista(const NizLista &lista) {
        kapacitet = lista.kapacitet;
        duzina = lista.duzina;
        tekuci = lista.tekuci;
        L = new Tip*[kapacitet];
        for (int i = 0; i < duzina; i++) {
            L[i] = new Tip(*lista.L[i]);
        }
    }
    NizLista &operator =(NizLista &lista){
        if(&lista == this) return *this;
        for (int i = 0; i < this->brojElemenata() ; i++) {
            delete L[i];
        }
        delete [] L;
        L = new Tip*[lista.kapacitet];
        this->duzina = lista.duzina;
        this->kapacitet = lista.kapacitet;
        this->tekuci = lista.tekuci;
        for (int i = 0; i < lista.duzina; i++) {
            L[i] = new Tip(*lista.L[i]);
        }
        return *this;
    }
    explicit NizLista(int size=100) {
        kapacitet = size;
        duzina = tekuci = 0;
        L = new Tip*[kapacitet];
    }
    ~NizLista() {
        for (int i = 0; i < this->brojElemenata() ; i++) {
            delete L[i];
        }
        delete [] L;
    }
    void obrisi();
    void dodajIspred(const Tip& el);
    void dodajIza(const Tip& el);
    int brojElemenata() const {
        return duzina;
    }
    bool prethodni() {
        if(tekuci == 0) return false;
        tekuci--;
        return true;
    }
    bool sljedeci() {
        if(tekuci == duzina-1) return false;
        tekuci++;
        return true;
    }
    void pocetak() {tekuci = 0;}
    void kraj() {tekuci = duzina-1;}
    Tip& trenutni() { return *L[tekuci]; }
    Tip& trenutni() const { return *L[tekuci]; }
    Tip& operator [](int broj) { return *L[broj];}
    Tip const& operator [](int broj) const {return *L[broj];}
};

template<typename Tip>
void NizLista<Tip>::dodajIspred(const Tip& el) {
    if(duzina<=kapacitet) {
        if(duzina == 0) {
            duzina++;
            L[0] = new Tip(el);
            tekuci = 0;
        }else {
            if(duzina == kapacitet) {
                Tip **nova = new Tip*[kapacitet * 2];
                for (int i = 0; i < kapacitet; i++) {
                    nova[i] = new Tip(*L[i]);
                }
                for (int i = 0; i < brojElemenata(); i++) {
                    delete L[i];
                }
                delete[] L;
                kapacitet=kapacitet*2;
                L = nova;
                nova = nullptr;
            }
            for (int i = duzina; i > tekuci; i--) {
                L[i] = new Tip(*L[i-1]);
            }
            L[tekuci] = new Tip(el);
            duzina++;
            tekuci++;
        }
    }
}

template<typename Tip>
void NizLista<Tip>::dodajIza(const Tip& el) {
    if(duzina<=kapacitet) {
        if(duzina == 0) {
            duzina++;
            L[0] = new Tip(el);
            tekuci = 0;
        }
        else {
            if(duzina == kapacitet) {
                Tip **nova = new Tip*[kapacitet * 2];
                for (int i = 0; i < kapacitet; i++) {
                    nova[i] = new Tip(*L[i]);
                }
                for (int i = 0; i < brojElemenata(); i++) {
                    delete L[i];
                }
                delete[] L;
                kapacitet=kapacitet*2;
                L = nova;
                nova = nullptr;
            }
            for (int i = duzina; i > tekuci+1; i--) {
                L[i] = new Tip(*L[i-1]);
            }
            L[tekuci+1] = new Tip(el);
            duzina++;
        }
    }
}

template<typename Tip>
void NizLista<Tip>::obrisi() {
    if(duzina != 0) {
        if (tekuci == duzina - 1) {
            if (tekuci != 0) {delete L[tekuci] ;tekuci--;};
            duzina--;
        } else {
            for (int i = tekuci; i <= duzina - 2; i++) {
                delete L[i];
                L[i] = new Tip(*L[i + 1]);
            }
            duzina--;
        }
    }
}

template<typename Tip>
class JednostrukaLista: public Lista<Tip> {
private:
    struct Cvor {
        Tip element;
        Cvor* sljedeci;
        Cvor( const Tip &element1, Cvor *sljedeci1): element(element1) , sljedeci(sljedeci1) {}
    };
    Cvor *pocetni;
    Cvor *krajnji;
    Cvor *tekuci;
public:
    JednostrukaLista() : pocetni(0),krajnji(0),tekuci(0) {}
    JednostrukaLista(const JednostrukaLista &jdlista);
    JednostrukaLista &operator = (const JednostrukaLista &jdlista);
    ~JednostrukaLista();
    void pocetak() { tekuci = pocetni;}
    void kraj() { tekuci = krajnji; }
    bool prethodni();
    bool sljedeci();
    void obrisi() {
        if(krajnji == pocetni) { delete pocetni; pocetni= nullptr; krajnji= nullptr; tekuci= nullptr;}
        else if(tekuci == krajnji){
            Cvor* p(krajnji);
            prethodni(); tekuci->sljedeci= nullptr;
            delete p; p= nullptr;
            krajnji=tekuci;
        }
        else if(tekuci == pocetni){
            Cvor* p(pocetni);
            sljedeci(); p->sljedeci= nullptr;
            delete p; p= nullptr;
            pocetni=tekuci;
        }
        else {
            Cvor* p(pocetni);
            Cvor* q(pocetni);
            q=tekuci->sljedeci;
            for (auto i = pocetni; i != tekuci; i=i->sljedeci) {
                if(i->sljedeci == tekuci) {p=i; break;}
            }
            tekuci->sljedeci = nullptr;
            delete tekuci;
            p->sljedeci = q;
            tekuci = q;
        }
    }
    Tip& trenutni();
    Tip& trenutni() const;
    void dodajIspred(const Tip& el) {
        if(pocetni == nullptr) {
            Cvor* p = new Cvor(el,0);
            pocetni = p;
            krajnji = p;
            tekuci = p;
            p = nullptr;
        }else if(pocetni == krajnji){
            Cvor* novi = new Cvor(el,tekuci);
            pocetni = novi;
            novi = nullptr;
        }else {
            Cvor* p = (pocetni);
            while (p->sljedeci != tekuci)
                p = p->sljedeci;
            Cvor* novi= new Cvor(el, tekuci);
            p->sljedeci = novi;
            p = nullptr;
            novi = nullptr;
        }
    }
    void dodajIza(const Tip& el) {
        if(pocetni == nullptr) {
            Cvor* p = new Cvor(el,0);
            pocetni = p;
            krajnji = p;
            tekuci = p;
            p = nullptr;
        }else if(pocetni == krajnji){
            Cvor* novi = new Cvor(el,0);
            tekuci->sljedeci = novi;
            krajnji=novi;
            novi = nullptr;
        }else {
            Cvor* p = tekuci->sljedeci;
            Cvor* novi= new Cvor(el, p);
            tekuci->sljedeci = novi;
            p = pocetni;
            while(p->sljedeci != nullptr) p=p->sljedeci;
            krajnji = p;
            p = nullptr;
            novi = nullptr;
        }
    }
    int brojElemenata() const {
        int i(1);
        if(pocetni == krajnji && pocetni== nullptr) return 0;
        else if(pocetni == krajnji && pocetni!=nullptr ) return i;
        else{
            Cvor* p(pocetni);
            while(p->sljedeci != 0) {i++; p=p->sljedeci;}
        }
        return i;
    }
    Tip& operator [](int broj);
    Tip const& operator [](int broj) const;
};

template<typename Tip>
Tip& JednostrukaLista<Tip>::operator[](int broj) {
    if(broj<0 || broj>(this->brojElemenata()-1)) throw ("Neispravan index");
    int j = 0;
    Cvor* p(pocetni);
    for (; p->sljedeci != nullptr ; p=p->sljedeci)
        if(j++ == broj) break;
    return p->element;
}

template<typename Tip>
Tip const& JednostrukaLista<Tip>::operator[](int broj) const {
    if(broj<0 || broj>(this->brojElemenata()-1)) throw ("Neispravan index");
    int j = 0;
    Cvor* p(pocetni);
    for (; p->sljedeci != nullptr ; p=p->sljedeci)
        if(j++ == broj) break;
    return p->element;
}
template<typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista &jdlista) : pocetni(0), krajnji(0) {
    Cvor *p(jdlista.pocetni);
    while (p!=0) {
        dodajIza(p->element);
        if(this->brojElemenata() > 1) sljedeci();
        p=p->sljedeci;
    }
}

template<typename Tip>
JednostrukaLista<Tip>::~JednostrukaLista() {
    int brelem = this->brojElemenata();
    if(brelem != 0) {
        Cvor *p(pocetni);
        Cvor *pIduci(pocetni->sljedeci);
        for (int i = 0; i < brelem - 1; i++) {
            delete p;
            p = pIduci;
            pIduci = pIduci->sljedeci;
        }
        delete krajnji;
        krajnji = nullptr;
        pocetni = nullptr;
        tekuci = nullptr;
    }
}

template<typename Tip>
JednostrukaLista<Tip> &JednostrukaLista<Tip>::operator = (const JednostrukaLista &jdlista) {
    if(&jdlista==this) return *this;
    Cvor *p(jdlista.pocetni);
    while (p!=0) {
        dodajIza(p->element);
        if(this->brojElemenata() > 1) sljedeci();
        p=p->sljedeci;
    }
    return *this;
}

template<typename Tip>
bool JednostrukaLista<Tip>::prethodni() {
    if(tekuci == pocetni) return false;
    Cvor* p(pocetni);
    while (p->sljedeci != tekuci) p=p->sljedeci;
    tekuci = p;
    return true;
}

template<typename Tip>
bool JednostrukaLista<Tip>::sljedeci() {
    if(tekuci == krajnji) return false;
    tekuci = tekuci->sljedeci;
    return true;
}

template<typename Tip>
Tip& JednostrukaLista<Tip>::trenutni() {
    if (pocetni==0) throw "Prazno!\n";
    else return tekuci->element;
}

template<typename Tip>
Tip& JednostrukaLista<Tip>::trenutni() const{
    if (pocetni==0) throw "Prazno!\n";
    else return tekuci->element;
}

int main() {
    JednostrukaLista<int> lista;
    for (int i(1); i<=5; i++)
        lista.dodajIza(i);
    std::cout << lista.brojElemenata() << " ";
    for (int i(1); i<=5; i++)
        lista.obrisi();
    std::cout << lista.brojElemenata();
    return 0;
}
