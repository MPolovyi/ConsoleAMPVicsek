using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vicsek.Interfaces
{
    interface IDrawer
    {
        void Draw(IParticle _particle);
        void Draw(IParticleBox _box);
    }
}
