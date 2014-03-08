using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vicsek.Interfaces
{
    public interface IPair<T> : IVector<T>
    {
        T First { get; set; }
        T Second { get; set; }
    }
}
