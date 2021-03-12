#include <iostream>
#include <utility>
#include <string>
using namespace std;
template <typename Kljuc, typename Vrijednost>
class Mapa{
private:
public:
    Mapa() = default;
    virtual ~Mapa() = default;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const Kljuc& kljuc) = 0;
    virtual Vrijednost& operator [](const Kljuc& broj) = 0;
    virtual const Vrijednost operator [](const Kljuc&broj) const = 0;
};

template <typename Kljuc, typename Vrijednost>
class NizMapa : public Mapa<Kljuc,Vrijednost>{
private:
    int brojelemenata;
    int kapacitet;
    std::pair<Kljuc,Vrijednost>** niz;
public:
    explicit NizMapa(int size = 10000) {
        brojelemenata = 0;
        kapacitet = size;
        niz = new std::pair<Kljuc,Vrijednost>*[kapacitet];
        for (int i = 0; i < kapacitet; ++i) {
            niz[i] = nullptr;
        }
    }
    NizMapa(const NizMapa<Kljuc,Vrijednost>& mapa){
        brojelemenata = mapa.brojelemenata;
        kapacitet = mapa.kapacitet;
        niz = new std::pair<Kljuc,Vrijednost>*[kapacitet];
        int i(0); int j(0);
        while (i < brojelemenata && j < kapacitet) {
            if(mapa.niz[j] == nullptr) { j++; continue; }
            niz[j] = new std::pair<Kljuc,Vrijednost>(*mapa.niz[j]);
            i++;
            j++;
        }
    }
    int brojElemenata() const override { return brojelemenata; }
    ~NizMapa() override {
        obrisi();
        delete []niz;
        niz = nullptr;
    }
    std::pair<Kljuc,Vrijednost>& dodaj(std::pair<Kljuc,Vrijednost> element) {
        int i(0);
        int j(0);
        if(brojelemenata == kapacitet) {
            NizMapa<Kljuc, Vrijednost> pomocni(kapacitet*2);
            int l(0);
            int k(0);
            while (l < brojelemenata && k < kapacitet) {
                if(niz[k] == nullptr) {k++;continue;}
                pomocni.niz[k] = niz[k];
                pomocni.brojelemenata++;
                l++;
                k++;
            }
            pomocni.niz[brojelemenata] = new std::pair<Kljuc,Vrijednost>(element);
            pomocni.brojelemenata++;
            obrisi();
            delete [] niz; niz = nullptr;
            *this = pomocni;
            return *niz[brojelemenata-1];
        }else {
            //nadji prvo prazno mjesto i dodaj element
            while (i <= brojelemenata) {
                if (*niz == nullptr || niz[i] == nullptr) {
                    //dodaj element
                    niz[i] = new std::pair<Kljuc,Vrijednost>(element);
                    brojelemenata++;
                    break;
                }
                i++;
            }
        }
        return *niz[i];
    }
    void obrisi() override {
        int i(0); int j(0);
        while(i < brojelemenata && j < kapacitet){
            if(niz[j] == nullptr) {j++;continue;}
            delete niz[j];
            niz[j] = nullptr;
            j++;
            i++;
        }
        brojelemenata = 0;
    }
    void obrisi(const Kljuc& kljuc) override{
        int i(0);
        int j(0);
        while (i < brojelemenata && j<kapacitet) {
            if (niz[j] == nullptr) {j++;continue;}
            if (niz[j]->first == kljuc) { delete niz[j]; niz[j] = nullptr ;break; }
            i++;
            j++;
        }
        if(i == brojelemenata ) throw ("Ne postoji element sa tim kljucem u mapi");
        brojelemenata--;
    }
    Vrijednost& operator [](const Kljuc& kljuc) override {
        int i(0);//Provjeri postoji li u mapi
        int j(0);
        while (i < brojelemenata && j<kapacitet) {
            if (niz[j] == nullptr) {j++;continue;}
            if (niz[j]->first == kljuc) return niz[j]->second;
            i++;
            j++;
        }//Ako ne postoji kreiraj par sa tim kljucem i default vrijednosti i ubaci ga u mapu
        std::pair <Kljuc,Vrijednost> noviElement;
        Vrijednost v{};
        noviElement = std::make_pair(kljuc,v);
        return dodaj(noviElement).second;
    }
    const Vrijednost operator [](const Kljuc& kljuc) const override {
        int i(0);//Provjerava da li se nalazi u mapi
        int j(0);
        while (i < brojelemenata && j<kapacitet) {
            if (niz[j] == nullptr) {j++;continue;}
            if (niz[j]->first == kljuc) return niz[j]->second;
            i++;
            j++;
        }
        //ako ne vrati default vrijednost vrijednosti para
        Vrijednost v{};
        return v;
    }
    NizMapa<Kljuc,Vrijednost>& operator =(const NizMapa<Kljuc,Vrijednost>& mapa){
        if(this == &mapa) return *this;
        if(kapacitet <= mapa.kapacitet){
            obrisi();
            int i(0); int j(0);
            while(i < mapa.brojelemenata && j < mapa.kapacitet){
                if(mapa.niz[j] == nullptr) continue;
                dodaj(*mapa.niz[j]);
                i++;
                j++;
            }
        }else{
            NizMapa<Kljuc, Vrijednost> pomocni(mapa.kapacitet);
            int j(0);
            int k(0);
            while (j < brojelemenata && k < mapa.kapacitet) {
                if(mapa.niz[k] == nullptr) {k++;continue;}
                pomocni.niz[k] = new std::pair<Kljuc,Vrijednost>(mapa.niz[k]->first,mapa.niz[k]->second);
                pomocni.brojelemenata++;
                j++;
                k++;
            }
            obrisi();
            delete [] niz; niz = nullptr;
            niz = pomocni.niz;
            pomocni.niz = nullptr;
            kapacitet = mapa.kapacitet;
        }
        return *this;
    };
};


template <typename Kljuc, typename Vrijednost>
bool testFunkcija1(NizMapa<Kljuc,Vrijednost> mapa){
    NizMapa<Kljuc,Vrijednost> mapa2(mapa);
    NizMapa<Kljuc,Vrijednost> mapa3 = mapa;
    mapa["Sarajevo"] = "BiH";
    mapa["Tuzla"] = "BiH";
    mapa["Zenica"] = "BiH";
    mapa["Mostar"] = "BiH";
    mapa2["Sarajevo"] = "BiH";
    mapa2["Tuzla"] = "BiH";
    mapa2["Zenica"] = "BiH";
    mapa3["Sarajevo"] = "BiH";
    mapa3["Tuzla"] = "BiH";
    mapa.obrisi("Tuzla");
    return (mapa.brojElemenata() == 3 && mapa2.brojElemenata() == 3 && mapa3.brojElemenata() == 2);
}

template <typename Kljuc, typename Vrijednost>
bool testFunkcija2(NizMapa<Kljuc,Vrijednost> mapa){
    mapa["Sarajevo"] = "BiH";
    mapa["Tuzla"] = "BiH";
    mapa["Zenica"] = "BiH";
    mapa["Mostar"] = "BiH";
    mapa.obrisi("Tuzla");
    mapa.obrisi("Sarajevo");
    mapa.obrisi("Zenica");
    mapa.obrisi("Mostar");
    mapa["Berlin"] = "Njemacka";
    return (mapa.brojElemenata() == 1 && mapa["Berlin"].compare("Njemacka") == 0);
}

template <typename Kljuc, typename Vrijednost>
bool testFunkcija3(NizMapa<Kljuc,Vrijednost> mapa){
    mapa[1] = 5;
    mapa[2] = 1;
    mapa[3] = 2;
    mapa[4] = 3;
    mapa.obrisi();
    mapa[2] = 1;
    mapa[3] = 2;
    mapa[4] = 3;
    return (mapa.brojElemenata() == 3 && (mapa[2] + mapa[3] + mapa[4]) == 6);
}

template <typename Kljuc, typename Vrijednost>
bool testFunkcija4(NizMapa<Kljuc,Vrijednost> mapa){
    NizMapa<Kljuc,Vrijednost> mapa2(mapa);
    mapa[1] = 5;
    mapa[2] = 1;
    mapa[3] = 2;
    mapa[4] = 3;
    mapa.obrisi();
    mapa2[2] = 1;
    mapa2[3] = 2;
    mapa2[4] = 3;
    return (mapa2.brojElemenata() == 3 && mapa.brojElemenata()==0);

}

template <typename Kljuc, typename Vrijednost>
bool testFunkcija5(NizMapa<Kljuc,Vrijednost> mapa){
    NizMapa<Kljuc,Vrijednost> mapa2(mapa);
    mapa[1] = "Nesto";
    mapa[2] = "Nesto drugo";
    mapa2[1] = "Nesto";
    mapa2[3] = "Nesto drugo";
    return (mapa[2].compare(mapa2[3]) == 0);
}

int main() {
    NizMapa<std::string,std::string> mapa;
    NizMapa<int,int> mapa2;
    NizMapa<int,std::string> mapa3;

    if(testFunkcija1(mapa)) cout<<"Test uspjesan \n";
    else cout<<"Test nije uspjesan \n";

    if(testFunkcija2(mapa)) cout<<"Test uspjesan \n";
    else cout<<"Test nije uspjesan \n";

    if(testFunkcija3(mapa2)) cout<<"Test uspjesan \n";
    else cout<<"Test nije uspjesan \n";

    if(testFunkcija4(mapa2)) cout<<"Test uspjesan \n";
    else cout<<"Test nije uspjesan \n";

    if(testFunkcija5(mapa3)) cout<<"Test uspjesan \n";
    else cout<<"Test nije uspjesan \n";
    return 0;
}
