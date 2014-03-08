using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    public class Particle : IParticle
    {
        private Pair<double> m_Coordinates;
        private Pair<double> m_Speed;
        private IEnumerable<IParticle> m_Nearest;
        protected virtual IDrawer Drawer { get; private set; }

        private double m_InterractionRadius;

        public Particle(double _x, double _y, IDrawer _drawer)
        {
            Drawer = _drawer;
            m_Coordinates = new Pair<double>(_x, _y);
            m_InterractionRadius = 5;

            Random k = new Random((int) DateTime.Now.ToBinary());
            Random r = new Random(k.GetHashCode());
            m_Speed = new Pair<double>(10 * (1 - r.NextDouble()), 10 * (1 - r.NextDouble()));
        }

        
        public virtual void NextStep(IEnumerable<IParticle> _particles, IBorder _border)
        {
            GetNearests(_particles);
            Interract();
            
            m_Coordinates = m_Coordinates + m_Speed;
        }
        
        public virtual IEnumerable<IParticle> GetNearests(IEnumerable<IParticle> _particles)
        {
            var nearest = new List<IParticle>();
            foreach (var particle in _particles)
            {
                if (CalkDistance(particle) < m_InterractionRadius)
                {
                    nearest.Add(particle);
                }
            }
            m_Nearest = nearest;
            return nearest;
        }

        public double CalkDistance(IParticle _particle)
        {
            var cd1 = CoordinatesInDouble;
            var cd2 = _particle.CoordinatesInDouble;
            double dist = (CoordinatesInDouble - _particle.CoordinatesInDouble).ABS();
            return dist;
        }

        public virtual void Interract()
        {
            var averSpd = new Pair<double>(0, 0);
            var cumulativeSpeed = new Pair<double>(0, 0);

            foreach (var particle in m_Nearest)
            {
                cumulativeSpeed = cumulativeSpeed + particle.SpeedInDouble;
            }
            averSpd = cumulativeSpeed / (double)m_Nearest.Count();

            //if (Math.Abs(averSpd.ABS() - 1) > 0.1)
            //{
            //    averSpd.Normalize();
            //}

            m_Speed = averSpd;
        }

        public void AddNoize(double _noize)
        {
            throw new NotImplementedException();
        }

        public void Draw(PictureBox _bmp)
        {
            var graph = _bmp.CreateGraphics();
            var rect = new Rectangle(CoordinatesInPoint.X - 3, CoordinatesInPoint.Y - 3, 6, 6);

            graph.DrawEllipse(new Pen(Color.Black, 2), rect);
            graph.DrawLine(new Pen(Color.Blue,2), CoordinatesInPoint, SpeedInPoint);
        }

        public void Draw()
        {
            Drawer.Draw(this);
        }

        public Pair<double> CoordinatesInDouble { get { return m_Coordinates; } }
        public Pair<double> SpeedInDouble { get { return m_Speed; } }

        public Point CoordinatesInPoint
        {
            get
            {
                int x = (int) Math.Floor(CoordinatesInDouble.First);
                int y = (int) Math.Floor(CoordinatesInDouble.Second);
                return new Point(x, y);
            }
        }

        public Point SpeedInPoint
        {
            get
            {
                int x = (int)Math.Floor(CoordinatesInDouble.First);
                int y = (int)Math.Floor(CoordinatesInDouble.Second);

                int vx = (int)Math.Floor(2 * SpeedInDouble.First);
                int vy = (int)Math.Floor(2 * SpeedInDouble.Second);

                return new Point(x+vx, y+vy);
            }
        }

        public void UpdSpeed()
        {
            
            m_Speed.First = -m_Speed.First;
            m_Speed.Second = -m_Speed.Second;
        }
    }
}
