using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Classes;

namespace Vicsek.Interfaces
{
    public interface IBorder
    {
        List<IPair<PairDouble>> Borders { get; }

        int? Check(IParticle _particle);
        void Interract(IParticle _particle, int _index);
    }
}
