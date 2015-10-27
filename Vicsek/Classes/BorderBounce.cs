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
        public BorderBounce(List<PairDouble> _corners) : base(_corners)
        {

        }

        public override void Interract(IParticle _particle, int _index)
        {
            var ptPos = _particle.CoordinatesInDouble;
            var ptSpeed = _particle.SpeedInDouble;
            var ptPosNext = ptSpeed + ptPos;

            var ptBordFirst = Borders[_index].First;
            var ptBordSecond = Borders[_index].Second;

            var ang = Miscelaneous.GetDegreeBetveen(ptPos, ptPosNext, ptBordFirst, ptBordSecond);
            var intersectionPt = Miscelaneous.IntersectionPoint(ptPos, ptPosNext, ptBordFirst, ptBordSecond);

            PairDouble newPos;
            if (_index == 0)
            {
                newPos = _particle.CoordinatesInDouble + new PairDouble(m_SizeX, 0);
                _particle.UpdCoordinates(newPos + _particle.SpeedInDouble);
            }
            if (_index == 1)
            {
                ang = -2 * ang;

                _particle.UpdCoordinates(Miscelaneous.Rotate(intersectionPt, ptPosNext, ang));
                _particle.UpdSpeed(Miscelaneous.Rotate(ptSpeed, ang));
            }
            if (_index == 2)
            {
                newPos = _particle.CoordinatesInDouble - new PairDouble(m_SizeX, 0);
                _particle.UpdCoordinates(newPos + _particle.SpeedInDouble);
            }
            if (_index == 3)
            {
                if (ang > 90)
                {
                    ang = 2 * (180 - ang);
                }
                else
                {
                    ang = -2 * ang;
                }
                _particle.UpdCoordinates(Miscelaneous.Rotate(intersectionPt, ptPosNext, ang));

                _particle.UpdSpeed(Miscelaneous.Rotate(ptSpeed, ang));

            }
            
        }
    }
}
