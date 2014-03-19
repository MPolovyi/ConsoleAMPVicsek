using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    internal class ParticleFactoryInteresting : ParticleFactoryStandart
    {
        public ParticleFactoryInteresting(double _X, double _Y, IDrawer _drawer)
            : base(_X, _Y, _drawer)
        {

        }

        public override IEnumerable<IParticle> PopulateRandomDistributed(int _count)
        {

            List<ParticleInteresting> particles = new List<ParticleInteresting>(_count);
            Random r = new Random((int) DateTime.Now.ToBinary());

            for (int i = 0; i < _count; i++)
            {
                var d1 = r.NextDouble();
                var d2 = r.NextDouble();
                particles.Add(new ParticleInteresting(SizeX*d1, SizeY*d2, Drawer));
            }
            return particles;
        }

    }
}
