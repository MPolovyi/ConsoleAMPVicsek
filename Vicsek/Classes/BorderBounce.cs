using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    class BorderBounce : Border
    {
        public BorderBounce(List<IPair<double>> _corners) : base(_corners)
        {

        }

        public override void Interract(IParticle _particle, int _index)
        {
            
        }
    }
}
