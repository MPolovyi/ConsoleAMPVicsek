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
        public List<IPair<IPair<double>>> Borders { get; private set; }

        public Border(List<IPair<double>> _corners)
        {
            Borders = new List<IPair<IPair<double>>>(_corners.Count);
            for (int i = 0; i < _corners.Count-1; i++)
            {
                Borders.Add(new Pair<IPair<double>>(_corners[i], _corners[i+1]));
            }
            Borders.Add(new Pair<IPair<double>>(_corners.Last(), _corners.First()));
        }



        public bool Check(IParticle _particle)
        {
            for (int i = 0; i < Borders.Count; i++)
            {
                var pt1 = _particle.CoordinatesInDouble;
                var pt2 = _particle.SpeedInDouble + pt1;

                var pt3 = Borders[i].First;
                var pt4 = Borders[i].Second;

                var isIntersection = Miscelaneous.Intersect(pt1, pt2, (Pair<double>)pt3, (Pair<double>)pt4);

                if (isIntersection)
                {
                    (_particle).UpdSpeed();
                    return true;
                }
            }
            return false;
        }
    }
}
