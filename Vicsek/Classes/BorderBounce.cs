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
            var ptPos = _particle.CoordinatesInDouble;
            var ptSpeed = _particle.SpeedInDouble;
            var ptPosNext = ptSpeed + ptPos;

            var ptBordFirst = Borders[_index].First;
            var ptBordSecond = Borders[_index].Second;

            var ang = Miscelaneous.GetDegreeBetveen(ptPos, ptPosNext, (Pair<double>)ptBordFirst, (Pair<double>)ptBordSecond);

            var intersectionPt = Miscelaneous.IntersectionPoint(ptPos, ptPosNext, (Pair<double>)ptBordFirst,
                                                                (Pair<double>)ptBordSecond);

            if (ang > 90)
            {
                ang = -2 * (180 - ang);
            }
            else
            {
                ang = -2 * ang;
            }

            _particle.UpdCoordinates(Miscelaneous.Rotate(intersectionPt, ptPosNext, ang).Second);

            _particle.UpdSpeed(Miscelaneous.Rotate(ptSpeed, ang));
        }
    }
}
