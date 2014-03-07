using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vicsek.Interfaces
{
    interface IParticleFactory2D
    {
        IEnumerable<IParticle> PopulateRandomDistributed(int _count);

        double SizeX { get; set; }
        double SizeY { get; set; }
    }
}
