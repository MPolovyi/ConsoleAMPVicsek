using System;
using System.Collections.Generic;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    public struct PairDouble
    {
        public double First { get; set; }
        public double Second { get; set; }

        public List<double> Components
        {
            get { return new List<double> {First, Second}; }
        }

        public PairDouble(double _f, double _s)
            : this()
        {
            First = _f;
            Second = _s;
        }

        public bool IsNaN()
        {
            //TODO: mb add checks in other operations as well, for example, add check on NaN to result of "/" opartion
            return double.IsNaN(First) || double.IsNaN(Second);
        }


        public static PairDouble operator +(PairDouble A, PairDouble B)
        {
            return new PairDouble(A.First + B.First, A.Second + B.Second);
        }

        public static PairDouble operator -(PairDouble A, PairDouble B)
        {
            return new PairDouble(A.First - B.First, A.Second - B.Second);
        }

        public static PairDouble operator /(PairDouble A, double B)
        {
            return new PairDouble(A.First / B, A.Second / B);
        }

        public static PairDouble operator *(PairDouble A, dynamic B)
        {
            return new PairDouble(A.First * B, A.Second * B);
        }

        public double ABS()
        {
            return Math.Sqrt((First*First + Second*Second));
        }

        public void Normalize()
        {
            this = this/(ABS());
        }
    }

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

        public PairDouble ToDouble()
        {
            return new PairDouble(m_X, m_X);
        }
    }

    public static partial class Miscelaneous
    {


        public static bool Intersect(PairDouble _a, PairDouble _b, PairDouble _c, PairDouble _d)
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
        
        public static PairDouble IntersectionPoint(PairDouble _a, PairDouble _b, PairDouble _c, PairDouble _d)
        {
            const double eps = 1E-9;
            double xA1 = _a.First, xB1 = _b.First, yA1 = _a.Second, yB1 = _b.Second;
            double xA2 = _c.First, xB2 = _d.First, yA2 = _c.Second, yB2 = _d.Second;
            double c1 = yA1*xB1 - xA1*yB1, c2 = yA2*xB2 - xA2*yB2;

            double det = Det(xB1 - xA1, -(yB1 - yA1), xB2 - xA2, -(yB2 - yA2));

            if (Math.Abs(det - 0) > eps)
            {
                double detX = Det(xB1 - xA1, c1, xB2 - xA2, c2);
                double detY = Det(c1, -(yB1 - yA1), c2, -(yB2 - yA2));

                double X = detX/det, Y = detY/det;

                return new PairDouble(X, Y);
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

        public static int GetDegreeBetveen(PairDouble _a, PairDouble _b, PairDouble _c, PairDouble _d)
        {
            return GetDegreeBetveen(_b - _a, _d - _c);
        }

        public static int GetDegreeBetveen(PairDouble _a, PairDouble _b)
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
        public static PairDouble Rotate(PairDouble _A, PairDouble _B, double _degree)
        {
            PairDouble toRotate = _B - _A;
            var rotated = Rotate(toRotate, _degree);

            return rotated + _A;
        }

        public static PairDouble Rotate(PairDouble _A, double _degree)
        {

            var cos = Math.Cos(_degree*Math.PI/180);
            var sin = Math.Sin(_degree*Math.PI/180);

            var xn = _A.First*cos - _A.Second*sin;
            var yn = _A.First*sin + _A.Second*cos;

            return new PairDouble(xn, yn);
            
        }

        public static void Swap<T>(ref T _a, ref T _b)
        {
            var c = _b;
            _b = _a;
            _a = c;
        }
    }
}


