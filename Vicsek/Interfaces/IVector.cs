using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vicsek.Interfaces
{
    interface IVector<T>
    {
        IEnumerable<T> Components { get; }
        double ABS();
    }
}
