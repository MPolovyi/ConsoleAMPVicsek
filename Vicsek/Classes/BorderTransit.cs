using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vicsek.Interfaces;

namespace Vicsek.Classes
{
    class BorderTransit : Border
    {
        public BorderTransit(List<PairDouble> _corners) : base(_corners)
        {

        }

        public override void Interract(IParticle _particle, int _index)
        {
         
            PairDouble newPos = new PairDouble(0,0);
            if (_index == 0)
            {
                newPos = _particle.CoordinatesInDouble +  new PairDouble(m_SizeX, 0);
            }
            if (_index == 1)
            {
                newPos = _particle.CoordinatesInDouble - new PairDouble(0, m_SizeY);
            }
            if (_index == 2)
            {
                newPos = _particle.CoordinatesInDouble - new PairDouble(m_SizeX, 0);
            }
            if (_index==3)
            {
                newPos = _particle.CoordinatesInDouble + new PairDouble(0, m_SizeY);
            }
            _particle.UpdCoordinates(newPos + _particle.SpeedInDouble);
        }
    }
}
