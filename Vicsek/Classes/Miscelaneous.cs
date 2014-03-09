using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    public struct Pair<T> : IPair<T>
    {
        public T First { get; set; }
        public T Second { get; set; }

        public IEnumerable<T> Components
        {
            get { return new List<T> {First, Second}; }
        }

        public Pair(T _f, T _s)
            : this()
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

        public static Pair<T> operator -(Pair<T> A, Pair<T> B)
        {
            dynamic a1 = A.First;
            dynamic a2 = A.Second;

            dynamic b1 = B.First;
            dynamic b2 = B.Second;

            return new Pair<T>(a1 - b1, a2 - b2);
        }

        public static Pair<T> operator /(Pair<T> A, dynamic B)
        {
            dynamic a1 = A.First;
            dynamic a2 = A.Second;
            return new Pair<T>(a1/B, a2/B);
        }

        public static Pair<T> operator *(Pair<T> A, dynamic B)
        {
            dynamic a1 = A.First;
            dynamic a2 = A.Second;
            return new Pair<T>(a1 * B, a2 * B);
        }

        public double ABS()
        {
            dynamic first = First;
            dynamic second = Second;

            return Math.Sqrt((double) (first*first + second*second));
        }

        public void Normalize()
        {
            this = this/(ABS());
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

    public static class Miscelaneous
    {
        public static int ParticleSpeed = 10;
        public static int SpeedDrawMultiplayer = 2;

        public static bool Intersect(Pair<double> _a, Pair<double> _b, Pair<double> _c, Pair<double> _d)
        {
            try
            {
                var ip = IntersectionPoint(_a, _b, _c, _d);
                double xA1 = _a.First, xB1 = _b.First, yA1 = _a.Second, yB1 = _b.Second;
                double xA2 = _c.First, xB2 = _d.First, yA2 = _c.Second, yB2 = _d.Second;
                var X = ip.First;
                var Y = ip.Second;
                return (Between(xA1, xB1, X) && Between(xA2, xB2, X) && Between(yA1, yB1, Y) && Between(yA2, yB2, Y));
            }
            catch (Exception)
            {
                return false;
            }
        }
        
        public static Pair<double> IntersectionPoint(Pair<double> _a, Pair<double> _b, Pair<double> _c, Pair<double> _d)
        {
            double eps = 1E-9;
            double xA1 = _a.First, xB1 = _b.First, yA1 = _a.Second, yB1 = _b.Second;
            double xA2 = _c.First, xB2 = _d.First, yA2 = _c.Second, yB2 = _d.Second;
            double c1 = yA1*xB1 - xA1*yB1, c2 = yA2*xB2 - xA2*yB2;

            double det = Det(xB1 - xA1, -(yB1 - yA1), xB2 - xA2, -(yB2 - yA2));

            if (Math.Abs(det - 0) > eps)
            {
                double detX = Det(xB1 - xA1, c1, xB2 - xA2, c2);
                double detY = Det(c1, -(yB1 - yA1), c2, -(yB2 - yA2));

                double X = detX/det, Y = detY/det;

                return new Pair<double>(X, Y);
            }
            throw new Exception("I don't know where interception is because lines are parallels");
        }

        public static bool Between(double _a, double _b, double _c)
        {
            double eps = 1E-9;
            return Math.Min(_a, _b) <= _c + eps && _c <= Math.Max(_a, _b) + eps;
        }

        public static double Det(double _a, double _b, double _c, double _d)
        {
            return _a * _d - _b * _c;
        }

        public static int GetDegreeBetveen(Pair<double> _a, Pair<double> _b, Pair<double> _c, Pair<double> _d)
        {
            return GetDegreeBetveen(_b - _a, _d - _c);
        }

        public static int GetDegreeBetveen(Pair<double> _a, Pair<double> _b)
        {
            var scalarAB = _a.First*_b.First + _a.Second*_b.Second;

            var ab = _a.ABS()*_b.ABS();

            return (int) (180*Math.Acos(scalarAB/ab)/Math.PI);
        }

        /// <summary>
        /// Rotates vector begins on A on degree in degrees
        /// </summary>
        /// <param name="_A"></param>
        /// <param name="_B"></param>
        /// <returns></returns>
        public static Pair<Pair<double>> Rotate(Pair<double> _A, Pair<double> _B, int _degree)
        {
            Pair<double> ToRotate = new Pair<double>(0,0);

            ToRotate = _B - _A;
            var Rotated = Rotate(ToRotate, _degree);
            
            Rotated = Rotated + _A;
            return new Pair<Pair<double>>(_A, Rotated);
        }
        
        public static Pair<double> Rotate(Pair<double> _A, int _degree)
        {

            var cos = Math.Cos(_degree*Math.PI/180);
            var sin = Math.Sin(_degree*Math.PI/180);

            var xn = _A.First*cos - _A.Second*sin;
            var yn = _A.First*sin + _A.Second*cos;

            return new Pair<double>(xn, yn);
            
        }

        public static void Swap<T>(ref T _a, ref T _b)
        {
            var c = _b;
            _b = _a;
            _a = c;
        }
    }
}


