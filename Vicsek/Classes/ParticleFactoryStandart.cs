using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    class ParticleFactoryStandart : IParticleFactory2D
    {
        public double SizeX { get; set; }
        public double SizeY { get; set; }

        private IDrawer Drawer { get; set; }

        public ParticleFactoryStandart(double _X, double _Y, IDrawer _drawer)
        {
            Drawer = _drawer;
            SizeX = _X;
            SizeY = _Y;
        }
        
        public IEnumerable<IParticle> PopulateRandomDistributed(int _count)
        {
            List<Particle> particles = new List<Particle>(_count);
            Random r = new Random((int) DateTime.Now.ToBinary());
            
            for (int i = 0; i < _count; i++)
            {
                particles.Add(new Particle(SizeX*r.NextDouble(), SizeY*r.NextDouble(), Drawer));
            }
            return particles;
        }
    }
}
