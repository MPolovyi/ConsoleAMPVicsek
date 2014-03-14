using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    class Border : IBorder
    {
        public List<IPair<PairDouble>> Borders { get; private set; }

        public Border(List<PairDouble> _corners)
        {
            Borders = new List<IPair<PairDouble>>(_corners.Count);
            for (int i = 0; i < _corners.Count-1; i++)
            {
                Borders.Add(new Pair<PairDouble>(_corners[i], _corners[i+1]));
            }
            Borders.Add(new Pair<PairDouble>(_corners.Last(), _corners.First()));
        }
        
        public int? Check(IParticle _particle)
        {
            for (int i = 0; i < Borders.Count; i++)
            {
                var pt1 = _particle.CoordinatesInDouble;
                var pt2 = _particle.SpeedInDouble + pt1;

                var pt3 = Borders[i].First;
                var pt4 = Borders[i].Second;

                var isIntersection = Miscelaneous.Intersect(pt1, pt2, (PairDouble)pt3, (PairDouble)pt4);

                if (isIntersection)
                {
                    return i;
                }
            }
            return null;
        }

        public virtual void Interract(IParticle _particle, int _index)
        {

        }
    }
}
