using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    public class Particle : IParticle
    {
        private PairDouble m_Coordinates;

        private PairDouble m_CoordinatesNew;

        protected PairDouble m_Speed;
        protected IEnumerable<IParticle> m_Nearest;
        protected virtual IDrawer Drawer { get; private set; }

        
        public Particle(double _x, double _y, IDrawer _drawer)
        {
            Drawer = _drawer;
            m_CoordinatesNew = m_Coordinates = new PairDouble(_x, _y);
            
            Random k = new Random((int) DateTime.Now.ToBinary());
            Random r = new Random(k.GetHashCode());
            m_Speed = new PairDouble(Miscelaneous.ParticleSpeed * (0.5 - r.NextDouble()), Miscelaneous.ParticleSpeed * (0.5 - r.NextDouble()));
            m_Speed.Normalize();
        }

        
        public virtual void NextStep(IEnumerable<IParticle> _particles, IBorder _border)
        {
            GetNearests(_particles);
            Interract();

            int? check = _border.Check(this);
            
            if (check!=null)
            {
                _border.Interract(this, (int) check);
            }
            else
            {
                
                m_CoordinatesNew = m_Coordinates + m_Speed;
            }
        }
        
        public virtual IEnumerable<IParticle> GetNearests(IEnumerable<IParticle> _particles)
        {
            var particles = _particles as IList<IParticle> ?? _particles.ToList();
            var nearest = new List<IParticle>(particles.Count());

            Parallel.ForEach(particles, _particle =>
                {
                    if (CalkDistance(_particle) < Miscelaneous.InterractionR)
                    {
                        nearest.Add(_particle);
                    }
                });

            m_Nearest = nearest;
            return nearest;
        }

        public virtual double CalkDistance(IParticle _particle)
        {
            double dist = (CoordinatesInDouble - _particle.CoordinatesInDouble).ABS();
            return dist;
        }

        public virtual void Interract()
        {
            var cumulativeSpeed = new PairDouble(0, 0);

            foreach (var particle in m_Nearest)
            {
                cumulativeSpeed = cumulativeSpeed + particle.SpeedInDouble;
            }
            PairDouble averSpd = cumulativeSpeed / m_Nearest.Count();

            averSpd.Normalize();

            if (averSpd.IsNaN())
            {
                averSpd = new PairDouble(0, 0);
            }

            m_Speed = averSpd * Miscelaneous.ParticleSpeed;
            AddNoize(Miscelaneous.Noize);
        }

        public void AddNoize(double _maxNoize)
        {
            Random r = new Random((int) DateTime.Now.Ticks);
            var rnd = 0.5-r.NextDouble();
            UpdSpeed(Miscelaneous.Rotate(SpeedInDouble, _maxNoize * rnd));
        }

        public void Draw(PictureBox _bmp)
        {
            var graph = _bmp.CreateGraphics();
            var rect = new Rectangle(CoordinatesInPoint.X - 10, CoordinatesInPoint.Y - 10, 20, 20);

            graph.FillEllipse(new SolidBrush(Color.Red), rect);
            graph.DrawLine(new Pen(Color.Blue,2), CoordinatesInPoint, SpeedInPoint);
        }

        public void Draw()
        {
            Drawer.Draw(this);
        }

        public PairDouble CoordinatesInDouble { get { return m_Coordinates; } }
        public PairDouble SpeedInDouble { get { return m_Speed; } }

        public Point CoordinatesInPoint
        {
            get
            {
                var x = (int) Math.Floor(CoordinatesInDouble.First);
                var y = (int) Math.Floor(CoordinatesInDouble.Second);
                return new Point(x, y);
            }
        }

        public Point SpeedInPoint
        {
            get
            {
                var x = (int)Math.Floor(CoordinatesInDouble.First);
                var y = (int)Math.Floor(CoordinatesInDouble.Second);

                var vx = (int)Math.Floor(Miscelaneous.SpeedDrawMultiplayer * SpeedInDouble.First);
                var vy = (int)Math.Floor(Miscelaneous.SpeedDrawMultiplayer * SpeedInDouble.Second);

                return new Point(x+vx, y+vy);
            }
        }

        public void RevertSpeed()
        {
            m_Speed.First = -m_Speed.First;
            m_Speed.Second = -m_Speed.Second;
        }

        public void UpdSpeed(PairDouble _newSpd)
        {
            m_Speed = _newSpd;
        }

        public void UpdCoordinates(PairDouble _newPos)
        {
            m_CoordinatesNew = _newPos;
        }

        public void Move()
        {
            m_Coordinates = m_CoordinatesNew;
        }

        public override string ToString()
        {
            return CoordinatesInPoint.ToString();
        }
    }
}
