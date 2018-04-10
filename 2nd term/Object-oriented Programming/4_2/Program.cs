﻿using System;
using System.Collections.Generic;
using System.Collections;


class PrimeStream : IEnumerator
{
    List<int> primes;
    bool end;
    public static bool IsPrime(int number)
    {
        if(number==2)
            return true;
        if (number % 2 == 0)
            return false;
        for (int i = 3; i*i <= number; i+=2)
        {
            if (number % i == 0)
                return false;
        }
        return true;
    }
    
    public PrimeStream()
    {
        primes=new List<int>();
        primes.Add(2);
        end=false;
    }

    public int next()
    {
        for(int i=primes[primes.Count-1]+1;true ;i++)
        {
        //Console.WriteLine(i); 
            if(i==int.MaxValue)
            {
                end=true;
                return primes[primes.Count-1];
            }
            if(IsPrime(i))
            {
                primes.Add(i);
                break;
            }
        }
        return primes[primes.Count-2];
    }
    public bool eos()
    {
        return end;
    }
    public void Reset()
    {
        primes.Clear();
        primes.Add(2);
        end=false;
    }
    public bool MoveNext()
    {
        if(!eos())
            next();
        return !eos();
    }

    public object Current
    {
        get
        {
         return primes[primes.Count - 1];
        }
    }
}

class PrimeCollection : IEnumerable
{
    PrimeStream ps;
    public PrimeCollection()
    {
        ps=new PrimeStream();
    }


    public IEnumerator GetEnumerator()
    {
        return (IEnumerator) new PrimeStream();
    }
}

class Program
{
    static void Main(string[] args)
    {
        PrimeCollection pc=new PrimeCollection();
        foreach(int p in pc)
            System.Console.WriteLine(p);
    }
}