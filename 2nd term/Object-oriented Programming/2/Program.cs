using System;
using System.Collections.Generic;

class IntStream
{
    private int n;
    public IntStream()
    {
        n=0;
    }
    public int next()
    {
        return n++;
    }
    public bool eos()
    {
        return false;
    }
    void reset()
    {
        n=0;
    }
}

class PrimeStream : IntStream
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
            //Console.WriteLine("{0}", i);
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

    public new int next()
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
    public new bool eos()
    {
        return end;
    }
    void reset()
    {
        primes.Clear();
        primes.Add(2);
        end=false;
    }
}

class RandomStream : IntStream
{   
    Random r;

    public RandomStream()
    {
        r=new Random();
    }
    public new int next()
    {
        return r.Next();
    }
    public new bool eos()
    {
        return false;
    }
    void reset()
    {
        r=new Random();
    }
}

class RandomWordStream
{
    PrimeStream ps;
    RandomStream rs;
    public RandomWordStream()
    {
        ps=new PrimeStream();
        rs=new RandomStream();
    }
    public string next()
    {
        string s="";
        int prime=ps.next();
        while(s.Length<prime)
            s+=rs.next();
        return s.Substring(0, prime);
    }
}

class Program
{
    static void Main(string[] args)
    {
        RandomWordStream r=new RandomWordStream();
        for(int i=0; i<3; i++)
            Console.WriteLine("{0}\n", r.next());
        Console.WriteLine("Hello World! {0}", int.MaxValue);

    }
}
