using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vicsek.Interfaces
{
    public interface IBorder
    {
        List<IPair<IPair<double>>> Borders { get; }

        bool Check(IParticle _particle);
    }
}
