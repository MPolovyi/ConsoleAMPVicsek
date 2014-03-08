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
        
        public double Check(IParticle _particle)
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
                    return i;
                }
            }
            return double.NaN;
        }

        public virtual void Interract(IParticle _particle, int _index)
        {
            var ptPos = _particle.CoordinatesInDouble;
            var ptSpeed = _particle.SpeedInDouble;
            var ptPosNext = ptSpeed + ptPos;

            var ptBordFirst = Borders[_index].First;
            var ptBordSecond = Borders[_index].Second;

            var ang = Miscelaneous.GetDegreeBetveen(ptPos, ptPosNext, (Pair<double>) ptBordFirst, (Pair<double>) ptBordSecond);

            var intersectionPt = Miscelaneous.IntersectionPoint(ptPos, ptPosNext, (Pair<double>) ptBordFirst,
                                                                (Pair<double>) ptBordSecond);

            if (ang > 90)
            {
                ang = -2*(180 - ang);
            }
            else
            {
                ang = -2*ang;
            }

            _particle.UpdPosition(Miscelaneous.Rotate(intersectionPt, ptPosNext, ang).Second);

            _particle.UpdSpeed(Miscelaneous.Rotate(ptSpeed, ang));
        }
    }
}
