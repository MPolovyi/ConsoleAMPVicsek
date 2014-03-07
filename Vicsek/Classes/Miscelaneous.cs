using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    struct Pair<T> : IPair<T>
    {
        public T First { get; set; }
        public T Second { get; set; }

        public IEnumerable<T> Components
        {
            get { return new List<T>{First, Second}; }
        }

        public Pair(T _f, T _s) : this()
        {
            First = _f;
            Second = _s;
        }

        public static Pair<T> operator +(Pair<T> A, Pair<T> B)
        {
            dynamic a1 = A.First;
            dynamic a2 = A.Second;

            dynamic b1 = B.First;
            dynamic b2 = B.Second;

            return new Pair<T>(a1 + b1, a2 + b2);
        }

        public static Pair<T> operator -(Pair<T> A, Pair<T> B )
        {
            dynamic a1 = A.First;
            dynamic a2 = A.Second;

            dynamic b1 = B.First;
            dynamic b2 = B.Second;

            return new Pair<T>(a1 - b1, a2 - b2);
        }

        public static Pair<T> operator /(Pair<T> A, dynamic B)
        {
            //if (typeof(B)!=typeof(int) || typeof(B)!=typeof(double))
            //{
            //    throw new NotImplementedException();
            //}
            //else
            //{
                dynamic a1 = A.First;
                dynamic a2 = A.Second;
                return new Pair<T>(a1/B, a2/B);
            //}
        }

        public double ABS()
        {
            dynamic first = First;
            dynamic second = Second;

            return Math.Sqrt((double) (first*first + second*second));
        }

        public double Normalize()
        {
            throw new NotImplementedException();
        }

        
    }

    internal class Position
    {
        private double m_X;
        private double m_Y;

        public Position(double _x, double _y)
        {
            m_X = _x;
            m_Y = _y;
        }

        public double GetX()
        {
            return m_X;
        }

        public double GetY()
        {
            return m_Y;
        }

        public Pair<double> ToDouble()
        {
            return new Pair<double>(m_X, m_X);
        }
    }

    static class Miscelaneous
    {
        //public static Pair<T> Average<T>(this IEnumerable<Pair<T>> _pairs)
        //{
            
        //}

        //public static IEnumerable<Pair<T>> GetSpeeds<T>(this IEnumerable<IParticle> _particles)
        //{
        //    var Speeds = _particles.Select(particle => particle.Speed<T>()).ToList();
        //}
    }


}
