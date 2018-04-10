//Maciej Malicki 300669
//Zadania 5.2. i 5.4.

package com.company;

import java.util.HashMap;
import java.util.Map;

abstract class Wyrazenie
{
    static Map<String, Double> zmienne;
    abstract public double oblicz();
    abstract public Wyrazenie pochodna();
    public void dodajZmienna(String zmienna, double wartosc)
    {
        zmienne.put(zmienna, wartosc);
    }
    public Wyrazenie()
    {
        if(zmienne==null)
            zmienne=new HashMap<String, Double>();
    }

}

class Dodaj extends Wyrazenie
{
    Wyrazenie lewe, prawe;
    Dodaj(Wyrazenie skladnik1, Wyrazenie skladnik2)
    {
        this.lewe=skladnik1; this.prawe=skladnik2;
    }
    public double oblicz()
    {
        return lewe.oblicz()+prawe.oblicz();
    }
    public Wyrazenie pochodna()
    {
        return new Dodaj(lewe.pochodna(), prawe.pochodna());
    }
    public String toString()
    {
        return "("+lewe.toString()+"+"+prawe.toString()+")";
    }
}

class Odejmij extends Wyrazenie
{
    Wyrazenie lewe, prawe;
    Odejmij(Wyrazenie odjemna, Wyrazenie odjemnik)
    {
        this.lewe=odjemna; this.prawe=odjemnik;
    }
    public double oblicz()
    {
        return lewe.oblicz()-prawe.oblicz();
    }
    public Wyrazenie pochodna()
    {
        return new Odejmij(lewe.pochodna(), prawe.pochodna());
    }
    public String toString()
    {
        return "("+lewe.toString()+"-"+prawe.toString()+")";
    }
}

class Pomnoz extends Wyrazenie
{
    Wyrazenie lewe, prawe;
    Pomnoz(Wyrazenie skladnik1, Wyrazenie skladnik2)
    {
        this.lewe=skladnik1; this.prawe=skladnik2;
    }
    public double oblicz()
    {
        return lewe.oblicz()*prawe.oblicz();
    }
    public Wyrazenie pochodna()
    {
        return new Dodaj(new Pomnoz(lewe.pochodna(), prawe), new Pomnoz(lewe, prawe.pochodna()));
    }
    public String toString()
    {
        return "("+lewe.toString()+"*"+prawe.toString()+")";
    }
}


class Podziel extends Wyrazenie
{
    Wyrazenie lewe, prawe;
    Podziel(Wyrazenie dzielna, Wyrazenie dzielnik)
    {
        this.lewe=dzielna; this.prawe=dzielnik;
    }
    public double oblicz()
    {
        return lewe.oblicz()/prawe.oblicz();
    }
    public Wyrazenie pochodna()
    {
        return new Podziel(new Odejmij(new Pomnoz(lewe.pochodna(), prawe), new Pomnoz(lewe, prawe.pochodna())), new Pomnoz(prawe, prawe));
    }
    public String toString()
    {
        return "("+lewe.toString()+"/"+prawe.toString()+")";
    }
}

class Zmienna extends Wyrazenie
{
    String zmienna;
    public String toString()
    {
        return zmienna;
    }
    public double oblicz()
    {
        return zmienne.get(zmienna);
    }
    public Wyrazenie pochodna()
    {
        return new Stala(1.0);
    }
    Zmienna(String zmienna)
    {
        this.zmienna=zmienna;
    }
    Zmienna(String zmienna, double wartosc)
    {
        this.zmienna=zmienna;
        zmienne.put(zmienna, wartosc);
        System.out.println(this.oblicz());
    }
}
class Stala extends Wyrazenie
{
    double stala;
    public Stala(double stala)
    {
        this.stala=stala;
    }
    public String toString()
    {
        return Double.toString(stala);
    }
    public Wyrazenie pochodna()
    {
        return new Stala(0.0);
    }
    public double oblicz()
    {
        return stala;
    }
}

public class Main
{

    public static void main(String[] args)
    {
	// write your code here
        System.out.println("Wyrażenie i pochodna:");
        System.out.println("Hello World!");
        Wyrazenie nowe_wyr=new Dodaj(new Stala(2.0), new Pomnoz(new Stala(2.0), new Stala(2.0)));
        System.out.println(nowe_wyr.oblicz());
        System.out.println(nowe_wyr.toString());

        System.out.println("Wyrażenie i pochodna:");
        nowe_wyr=new Dodaj(new Zmienna("x", 5.0), new Podziel(nowe_wyr, new Zmienna("x")));
        System.out.println(nowe_wyr.toString());
        System.out.println(nowe_wyr.oblicz());

        System.out.println("Wyrażenie i pochodna:");
        nowe_wyr=new Pomnoz(new Pomnoz(new Zmienna("x"), new Zmienna("x")), new Zmienna("x"));
        System.out.println(nowe_wyr.toString());
        System.out.println(nowe_wyr.oblicz());
        nowe_wyr=nowe_wyr.pochodna();
        System.out.println(nowe_wyr.toString());
        System.out.println(nowe_wyr.oblicz());


        System.out.println("Wyrażenie i pochodna:");
        nowe_wyr=new Podziel(new Stala(1.0), new Pomnoz(new Zmienna("x"), new Zmienna("x")));
        System.out.println(nowe_wyr.toString());
        System.out.println(nowe_wyr.oblicz());
        nowe_wyr=nowe_wyr.pochodna();
        System.out.println(nowe_wyr.toString());
        System.out.println(nowe_wyr.oblicz());
        //zgadza się, bo pochodna 1/x^2 to -2/x^3
    }
}
