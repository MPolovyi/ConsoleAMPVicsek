using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    class ParticleInteresting : Particle
    {
        public ParticleInteresting(double _x, double _y, IDrawer _drawer) : base(_x, _y, _drawer)
        {
        }


        public override void Interract()
        {
            var cumulativeSpeed = new PairDouble(0, 0);

            foreach (var particle in m_Nearest)
            {
                cumulativeSpeed = cumulativeSpeed + particle.SpeedInDouble;
            }
            PairDouble averSpd = cumulativeSpeed / -m_Nearest.Count();

            averSpd.Normalize();

            if (averSpd.IsNaN())
            {
                averSpd = new PairDouble(0,0);
            }

            m_Speed = averSpd * Miscelaneous.ParticleSpeed;
            AddNoize(Miscelaneous.Noize);
        }
    }
}
